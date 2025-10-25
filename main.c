#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "main.h"
#include "matrix.h"
#include "cli.h"
#include "path.h"

#define MATRIX_H_LIMIT 100
#define MATRIX_W_LIMIT 150

/*
 * Todo:
 * (1) Manejo de números negativos o grandes en inputs
 * (2) Mejorar mostrado de matriz en pantalla usando buffers
 * (3) Añadir opción para enumerar filas y columnas al mostrar matriz
 */

int main(void)
{
    const char *overview =
    "                                                    \n"
    "    ┏━━━┳━━━┳━━━┓    ┃ Dimensiones: %i✕%i          \n"
    "    ┣━━━╋━━━╋━━━┫    ┃ Permitir mov. diagonales? %s\n"
    "    ┣━━━╋━━━╋━━━┫    ┃ Punto de inicio: (%i, %i)   \n"
    "    ┗━━━┻━━━┻━━━┛    ┃ Punto de fín: (%i, %i)      \n";

    struct matrix *map = define_matrix();
    struct cell *start = define_start(map);
    struct cell *end = define_end(map, start);

    int input = 0;
    int should_continue = 1;
    while (should_continue)
    {
        clear();
        printf(overview, map->height, map->width,
            map->allow_diag_moves ? "Si" : "No", start->row + 1,
            start->col + 1, end->row + 1, end->col + 1);

        printf("\n  OPCIONES:");
        printf("\n    [1] Definir celdas de obstáculos");
        printf("\n    [2] Modificar costos de celdas");
        // printf("\n    [3] Previsualizar mapa");
        printf("\n    [4] Calcular camino óptimo");
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
                break;
            case 4:
                should_continue = 0;
                break;
            default:
                continue;
        }
    }

    dijkstra(map, start, end);
    print_map(map);
}

struct matrix* define_matrix()
{
    const char *text_header =
    "                                 \n"
    "           ┏━━━┳━━━┳━━━┓         \n"
    "           ┣━━━╋━━━╋━━━┫         \n"
    "           ┣━━━╋━━━╋━━━┫         \n"
    "           ┗━━━┻━━━┻━━━┛         \n"
    "  ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n"
    "  ━━━━━ Definición del mapa ━━━━━\n"
    "  ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";

    clear();
    write(1, text_header, strlen(text_header));
    int mat_h, mat_w = 0;

    printf("\n  (1/4) Ingrese las dimensiones del mapa \n");
    printf("\n  🛈  Para una matriz visualmente cuadrada,"
           "\n  el largo debe ser el doble de su alto.\n");

    while (1)
    {
        printf("\n  Alto: "); scanf("%i", &mat_h);
        printf("  Largo: "); scanf("%i", &mat_w);

        if ((mat_h > 0 && mat_h <= MATRIX_H_LIMIT)
            && (mat_w > 0 && mat_w <= MATRIX_W_LIMIT))
        {
            break;
        }

        printf("\n  Error: el mapa puede tener un máximo de %i✕%i celdas.\n", MATRIX_H_LIMIT, MATRIX_W_LIMIT);
        printf("  Ingrese las dimensiones nuevamente.\n");
    }

    char input;
    int allow_diag = 0;
    printf("\n  (2/4) Desea permitir movimientos diagonales? [s/n]\n");
    while (1)
        {
            printf("\n  ▶ ");
            if (scanf(" %c", &input) != 1) {
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
    int max_h = m->height;
    int max_w = m->width;

    struct vertex point = {0};

    printf("\n  (3/4) Seleccione el punto de partida\n");
    while (1)
    {
        printf("\n  Coordenada en X: "); scanf("%i", &point.col);
        printf("  Coordenada en Y: "); scanf("%i", &point.row);

        if (point.row <= max_h && point.col <= max_w)
        {
            break;
        }

        printf("\n  Error: coordenada fuera de los límites.");
        printf("\n  Por favor ingrese un par entre (0, 0) y (%i, %i).\n", max_w, max_h);
    }

    struct cell *start = get_cell(m, point.row - 1, point.col - 1);
    if (start != NULL)
    {
        start->type = START;
    }

    return start;
}

struct cell* define_end(struct matrix *m, struct cell *start)
{
    int max_h = m->height;
    int max_w = m->width;
    int start_x = start->col + 1;
    int start_y = start->row + 1;

    struct vertex point = {0};

    printf("\n  (4/4) Seleccione el punto de fín\n");
    while (1)
    {
        printf("\n  Coordenada en X: "); scanf("%i", &point.col);
        printf("  Coordenada en Y: "); scanf("%i", &point.row);

        if ((point.row <= max_h && point.col <= max_w)
            && (point.row != start_y || point.col != start_x))
        {
            break;
        }

        printf("\n  Error: coordenada fuera de los límites o inválida.");
        printf("\n  Por favor ingrese un par entre (0, 0) y (%i, %i).", max_w, max_h);
        printf("\n  y diferente del punto de inicio (%i, %i).\n", start_x, start_y);
    }

    struct cell *end = get_cell(m, point.row - 1, point.col - 1);
    if (end != NULL)
    {
        end->type = END;
    }

    return end;
}

void define_obstacles(struct matrix *m)
{
    const char *text_header =
    "                                 \n"
    "           ┏━━━━━━━━━━━┓         \n"
    "           ┃█████•••▶••┃         \n"
    "           ┃██•••••████┃         \n"
    "           ┃••••███████┃         \n"
    "           ┗━━━━━━━━━━━┛         \n"
    "  ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n"
    "  ━━━ Definición de obstáculos ━━\n"
    "  ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";

    int input = 0;
    int should_continue = 1;
    while (should_continue)
    {
        clear();
        write(1, text_header, strlen(text_header));

        printf("\n  OPCIONES:");
        printf("\n   [1] Definir una celda como obstáculo");
        printf("\n   [2] Definir un rango de celdas como obstáculos");
        printf("\n   [3] Retroceder ↩︎");
        printf("\n\n  ▶ "); scanf(" %i", &input);

        switch (input)
        {
            case 1:
                set_cell_type(m, OBSTACLE);
                break;
            case 2:
                set_range_type(m, OBSTACLE);
                break;
            case 3:
                should_continue = 0;
                break;
            default:
                continue;
        }
    }
}

void define_areas(struct matrix *m)
{
    const char *text_header =
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

    int input = 0;
    int should_continue = 1;
    while (should_continue)
    {
        clear();
        write(1, text_header, strlen(text_header));
        printf("\n  🛈  El peso máximo actual de una celda es de %i\n",
            m->max_weight);

        write(1, options, strlen(options));
        printf("\n\n  ▶ "); scanf(" %i", &input);

        switch (input)
        {
            case 1:
                set_cell_weight(m);
                break;
            case 2:
                set_range_weight(m);
                break;
            case 3:
                should_continue = 0;
                break;
            default:
                continue;
        }
    }
}
