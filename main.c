#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "main.h"
#include "matrix.h"
#include "path.h"
#include "cli.h"
#include "utils.h"

#define MATRIX_H_LIMIT 100
#define MATRIX_W_LIMIT 150

/*
 * TODO:
 * (*) Mostrar mensaje de error cuando dijkstra no encuentre un camino
 * (*) Manejo de números negativos o grandes en inputs
 * (*) Mejorar mostrado de matriz en pantalla usando buffers [B]
 * (*) Añadir opción para enumerar filas y columnas al mostrar matriz [B]
 * (*) Agregar static a variables donde se pueda y unsigned int
 */

int main(void)
{
    const char *overview =
    "                                                              \n"
    "    ┏━━━┳━━━┳━━━┓    ┃ Dimensiones: %i✕%i                     \n"
    "    ┣━━━╋━━━╋━━━┫    ┃ Permitir desplazamientos diagonales? %s\n"
    "    ┣━━━╋━━━╋━━━┫    ┃ Punto de inicio: (%i, %i)              \n"
    "    ┗━━━┻━━━┻━━━┛    ┃ Punto de fín: (%i, %i)                 \n";

    const char *options =
    "\n  OPCIONES:                         "
    "\n    [1] Definir celdas de obstáculos"
    "\n    [2] Modificar costos de celdas  "
    "\n    [3] Previsualizar mapa          "
    "\n    [4] Calcular camino óptimo      ";

    struct matrix *map = define_map();
    struct cell *start = define_start(map);
    struct cell *end = define_end(map, start);

    int input = 0;
    int cont = 1;
    while (cont)
    {
        clear();
        printf(overview, map->height, map->width,
            map->allow_diag_moves ? "Si" : "No", start->col + 1,
            start->row + 1, end->col + 1, end->row + 1);
        printf(options);
        printf("\n\n  ▶ "); scanf(" %i", &input);

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

                printf("\n  Presione ENTER para retroceder ↩︎ ");

                int c;
                while ((c = getchar()) != '\n' && c != EOF) {
                    /* Consume characters */
                }
                getchar();
                break;
            case 4:
                cont = 0;
                break;
        }
    }

    dijkstra(map, start, end);
    clear();
    print_map(map, 0, 1);
}

struct matrix* define_map()
{
    const char *header =
    "                                 \n"
    "           ┏━━━┳━━━┳━━━┓         \n"
    "           ┣━━━╋━━━╋━━━┫         \n"
    "           ┣━━━╋━━━╋━━━┫         \n"
    "           ┗━━━┻━━━┻━━━┛         \n"
    "  ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n"
    "  ━━━━━ Definición del mapa ━━━━━\n"
    "  ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";

    const char *info =
    "\n  🛈  Para una matriz visualmente cuadrada,"
    "\n  el largo debe ser el doble de su alto.\n ";

    clear();
    printf(header);
    int mat_h, mat_w = 0;

    printf("\n  (1/4) Ingrese las dimensiones del mapa \n");
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

        printf("\n  Error: el mapa puede tener un máximo de %i✕%i celdas.",
            MATRIX_H_LIMIT, MATRIX_W_LIMIT);
        printf("\n  Ingrese las dimensiones nuevamente.\n");
    }

    char input;
    int allow_diag = 0;
    printf("\n  (2/4) Desea permitir movimientos diagonales? [s/n]\n");
    while (1)
        {
            printf("\n  ▶ ");
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
    printf("\n  (4/4) Seleccione el punto de fín\n");

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
    "           ┏━━━━━━━━━━━┓         \n"
    "           ┃█████•••▶••┃         \n"
    "           ┃██•••••████┃         \n"
    "           ┃••••███████┃         \n"
    "           ┗━━━━━━━━━━━┛         \n"
    "  ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n"
    "  ━━━ Definición de obstáculos ━━\n"
    "  ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";

    const char *options =
    "\n  OPCIONES:                                      "
    "\n   [1] Definir una celda como obstáculo          "
    "\n   [2] Definir un rango de celdas como obstáculos"
    "\n   [3] Retroceder ↩︎                              ";

    const char *info =
    "\n  🛈  Defina dos puntos en la misma fila o columna para crear"
    "\n  una línea de obstáculos, o seleccione dos puntos diferentes"
    "\n  para marcar el área comprendida entre estos.\n             ";

    int input = 0;
    int cont = 1;
    while (cont)
    {
        clear();
        printf(header);
        printf(options);
        printf("\n\n  ▶ ");
        scanf(" %i", &input);

        switch (input)
        {
            case 1:
            {
                struct vertex *points = get_vertices(m, 1);
                if (points == NULL) break;

                struct vertex point = points[0];
                set_cell_type(m, point.row - 1, point.col - 1, OBSTACLE);

                free(points);
                break;
            }
            case 2:
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
                        set_cell_type(m, i, j, OBSTACLE);
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
    "           ┏━━━━━━━━━━━┓         \n"
    "           ┃▓▓▒▒▒▒░░░░░┃         \n"
    "           ┃▓▓▓▓▒▒▒▒░░░┃         \n"
    "           ┃▓▓▓▓▓▓▒▒▒▒░┃         \n"
    "           ┗━━━━━━━━━━━┛         \n"
    "  ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n"
    "  ━━━━━ Asignación de pesos ━━━━━\n"
    "  ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";

    const char *options =
    "\n  OPCIONES:                                   "
    "\n   [1] Modificar el peso de una celda         "
    "\n   [2] Modificar el peso de un rango de celdas"
    "\n   [3] Retroceder ↩︎                           ";

    const char *info = "\n  🛈  El peso máximo actual de una celda es de %i\n";

    int input = 0;
    int cont = 1;
    while (cont)
    {
        clear();
        printf(header);
        printf(info, m->max_weight);
        printf(options);
        printf("\n\n  ▶ ");
        scanf(" %i", &input);

        switch (input)
        {
            case 1:
            {
                struct vertex *points = get_vertices(m, 1);
                if (points == NULL) break;

                int new_weight = 1;
                printf("\n  Ingrese nuevo peso: ");
                scanf(" %i", &new_weight);

                struct vertex point = points[0];
                set_cell_weight(m, point.row - 1, point.col - 1, new_weight);

                free(points);
                break;
            }
            case 2:
            {
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
                        printf("Setting cell (%i, %i) to weight of %i\n", j, i, new_weight);
                        set_cell_weight(m, i, j, new_weight);
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
