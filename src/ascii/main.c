#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifdef _WIN32
    #include <windows.h>
    #define WINDOWS_CP_UTF8 65001
#endif

#include "../../include/main.h"
#include "../../include/matrix.h"
#include "../../include/path.h"
#include "../../include/cli.h"
#include "../../include/utils.h"

#define MATRIX_H_LIMIT 100
#define MATRIX_W_LIMIT 80

int main(void)
{
    set_windows_codepage();

    const char *overview =
    "                                                              \n"
    "    +---+---+---+    | Dimensiones: %ix%i                     \n"
    "    |---|---|---|    | Permitir desplazamientos diagonales? %s\n"
    "    |---|---|---|    | Punto de inicio: (%i, %i)              \n"
    "    +---+---+---+    | Punto de fín: (%i, %i)                 \n";

    const char *options =
    "\n  OPCIONES:                         "
    "\n    [1] Definir obstáculos          "
    "\n    [2] Modificar costos de celdas  "
    "\n    [3] Previsualizar radar         "
    "\n    [4] Calcular camino óptimo      "
    "\n    [5] Cargar mapas de muestra     ";

    struct matrix *map = define_map();
    struct cell *start = define_start(map);
    struct cell *end = define_end(map, start);

    int input = 0;
    int cont = 1;
    int result;

    while (cont)
    {
        clear();
        printf(overview, map->height, map->width,
            map->allow_diag_moves ? "Si" : "No", start->col + 1,
            start->row + 1, end->col + 1, end->row + 1);
        printf(options);

        printf("\n\n  > ");
        scanf(" %i", &input);

        switch (input)
        {
            case 1:
                define_obstacles(map);
                break;
            case 2:
                define_areas(map);
                break;
            case 3:
                clear();
                print_map(map, 1, 0);

                printf("\n  Presione ENTER para retroceder");

                int c;
                while ((c = getchar()) != '\n' && c != EOF);

                getchar();
                break;
            case 4:
                result = dijkstra(map, start, end);
                if (result)
                {
                    clear();
                    print_map(map, 0, 1);
                    cont = 0;
                }
                else
                {
                    printf("\n  Error: no existe un camino hacia el objetivo.\n");
                    printf("\n  Presione ENTER para volver");

                    int c;
                    while ((c = getchar()) != '\n' && c != EOF);
                    getchar();
                }
                break;
            case 5:
                load_maps(&map, &start, &end);
                break;
        }
    }
}

struct matrix* define_map()
{
    const char *header =
    "                                 \n"
    "           +-----+-----+         \n"
    "           |     |     |         \n"
    "           |-----|-----|         \n"
    "           |     |     |         \n"
    "           +-----+-----+         \n"
    "  -------------------------------\n"
    "  ----- Definición del radar ----\n"
    "  -------------------------------\n";

    const char *info =
    "\n  * Para un radar visualmente cuadrado, el largo"
    "\n  debe ser el doble de su alto. (Ej: 25x50)\n   ";

    clear();
    printf(header);
    int mat_h, mat_w = 0;

    printf("\n  (1/4) Ingrese las dimensiones del radar \n");
    printf(info);

    while (1)
    {
        printf("\n  Alto: "); scanf("%i", &mat_h);
        printf("  Largo: "); scanf("%i", &mat_w);

        if ((mat_h > 0 && mat_h <= MATRIX_H_LIMIT)
            && (mat_w > 0 && mat_w <= MATRIX_W_LIMIT))
        {
            break;
        }

        printf("\n  Error: el radar puede tener un máximo de %ix%i celdas.",
            MATRIX_H_LIMIT, MATRIX_W_LIMIT);
        printf("\n  Ingrese las dimensiones nuevamente.\n");
    }

    char input;
    int allow_diag = 0;
    printf("\n  (2/4) Desea permitir movimientos diagonales? [s/n]\n");
    while (1)
        {
            printf("\n  > ");
            if (scanf(" %c", &input) != 1)
            {
                printf("\n\n  Error de lectura.\n");
                continue;
            }

            if (input == 's')
            {
                allow_diag = 1;
                break;
            }
            else if (input == 'n')
            {
                break;
            }

            printf("\n  Error: ingrese el caracter 's' o 'n'.\n");
        }

    return create_matrix(mat_h, mat_w, allow_diag);
}

struct cell* define_start(struct matrix *m)
{
    printf("\n  (3/4) Seleccione el punto de partida\n");

    struct vertex *point = get_vertices(m, 1);
    if (point == NULL) return NULL;

    struct cell *start = get_cell(m, point->row - 1, point->col - 1);
    if (start != NULL) start->type = START;

    free(point);

    return start;
}

struct cell* define_end(struct matrix *m, struct cell *start)
{
    printf("\n  (4/4) Seleccione el punto de fin\n");

    struct vertex *point = {0};
    while (1)
    {
        point = get_vertices(m, 1);
        if (point == NULL) return NULL;

        if (point->row != (start->row+1) || point->col != (start->col+1)) break;

        free(point);
        printf("\n  Error: el punto de fin e inicio no pueden coincidir.\n");
    }

    struct cell *end = get_cell(m, point->row - 1, point->col - 1);
    if (end != NULL) end->type = END;

    free(point);

    return end;
}

void define_obstacles(struct matrix *m)
{
    const char *header =
    "                                 \n"
    "           +-----------+         \n"
    "           |#####***>**|         \n"
    "           |##*****####|         \n"
    "           |****#######|         \n"
    "           +-----------+         \n"
    "  -------------------------------\n"
    "  --- Definición de obstáculos --\n"
    "  -------------------------------\n";

    const char *options =
    "\n  OPCIONES:                                      "
    "\n   [1] Definir obstáculos                        "
    "\n   [2] Restaurar celdas                          "
    "\n   [3] Retroceder ↩︎                              ";

    const char *info =
    "\n  * Defina dos puntos en la misma fila o columna para crear  "
    "\n  una línea de obstáculos, o seleccione dos puntos diferentes"
    "\n  para marcar el área comprendida entre estos.\n             ";

    int input = 0;
    int cont = 1;
    while (cont)
    {
        clear();
        printf(header);
        printf(options);
        printf("\n\n  > ");
        scanf(" %i", &input);

        switch (input)
        {
            case 1:
            {
                printf(info);
                struct vertex *points = get_vertices(m, 2);
                struct vertex p1 = points[0],  p2 = points[1];

                int min_row = (p2.row > p1.row) ? p1.row : p2.row;
                int max_row = (p2.row > p1.row) ? p2.row : p1.row;
                int min_col = (p2.col > p1.col) ? p1.col : p2.col;
                int max_col = (p2.col > p1.col) ? p2.col : p1.col;

                for (int i = min_row; i <= max_row; i++)
                {
                    for (int j = min_col; j <= max_col; j++)
                    {
                        set_cell_type(m, i - 1, j - 1, OBSTACLE);
                    }
                }

                break;
            }
            case 2:
            {
                struct vertex *points = get_vertices(m, 2);
                struct vertex p1 = points[0],  p2 = points[1];

                int min_row = (p2.row > p1.row) ? p1.row : p2.row;
                int max_row = (p2.row > p1.row) ? p2.row : p1.row;
                int min_col = (p2.col > p1.col) ? p1.col : p2.col;
                int max_col = (p2.col > p1.col) ? p2.col : p1.col;

                for (int i = min_row; i <= max_row; i++)
                {
                    for (int j = min_col; j <= max_col; j++)
                    {
                        set_cell_type(m, i - 1, j - 1, EMPTY);
                    }
                }

                break;
            }
            case 3:
            {
                cont = 0;
                break;
            }
        }
    }
}

void define_areas(struct matrix *m)
{
    const char *header =
    "                                 \n"
    "           +-----------+         \n"
    "           |##%%%%&&&&&|         \n"
    "           |####%%%%&&&|         \n"
    "           |######%%%%&|         \n"
    "           +-----------+         \n"
    "  -------------------------------\n"
    "  ----- Asignación de pesos -----\n"
    "  -------------------------------\n";

    const char *info =
    "\n  * El peso máximo actual de una celda es de %i                 "
    "\n    y el peso mínimo es de %i.\n                                "
    "\n  * Defina dos puntos en la misma fila o columna para modificar "
    "\n  el peso de una línea de celdas, o seleccione dos puntos       "
    "\n  diferentes para modificar el área comprendida entre estos.\n  ";

    clear();
    printf(header);
    printf(info, m->max_weight, m->min_weight);

    struct vertex *points = get_vertices(m, 2);
    struct vertex p1 = points[0],  p2 = points[1];

    int new_weight = 1;
    printf("\n  Ingrese nuevo peso: ");
    scanf(" %i", &new_weight);

    int min_row = (p2.row > p1.row) ? p1.row : p2.row;
    int max_row = (p2.row > p1.row) ? p2.row : p1.row;
    int min_col = (p2.col > p1.col) ? p1.col : p2.col;
    int max_col = (p2.col > p1.col) ? p2.col : p1.col;

    for (int i = min_row; i <= max_row; i++)
    {
        for (int j = min_col; j <= max_col; j++)
        {
            set_cell_weight(m, i - 1, j - 1, new_weight);
        }
    }
}

void load_maps(struct matrix **m, struct cell **start, struct cell **end)
{
    const char *options =
    "                           \n"
    "  [1] Camino entre icebers \n"
    "  [2] Hielos flotantes     \n";

    int input;
    printf(options);

    struct matrix *new_map = NULL;
    while (1)
    {
        printf("\n  > ");
        scanf(" %i", &input);

        if (input == 1)
            new_map = load_map("mapas/entre icebergs.txt", &(*start), &(*end));
        else
            new_map = load_map("mapas/hielos flotantes.txt", &(*start), &(*end));

        if (new_map)
        {
            destroy_matrix(*m);
            *m = new_map;
            break;
        }
    }
}

void set_windows_codepage()
{
    #ifdef _WIN32
        if (SetConsoleOutputCP(WINDOWS_CP_UTF8))
            printf("Console codepage set to UTF-8 on Windows.\n");
        else
            fprintf(stderr, "Warning: failed to set console codepage to UTF-8.\n");
    #endif
}
