#include <stdio.h>

#include "matrix.h"
#include "cli.h"
#include "path.h"

#define MATRIX_H_LIMIT 100
#define MATRIX_W_LIMIT 100

int main(void)
{
    int mat_h, mat_w = 0;

    while (1)
    {
        clear();
        printf("--- Definición de la matriz ---\n");
        printf("\nAlto de la matriz: "); scanf("%i", &mat_h);
        printf("\nLargo de la matriz: "); scanf("%i", &mat_w);

        if ((mat_h > 0 && mat_h <= MATRIX_H_LIMIT)
            && (mat_w > 0 && mat_w <= MATRIX_W_LIMIT))
        {
            break;
        }

        printf("El mapa puede tener un alto máximo de %i y %i de ancho.\n", MATRIX_H_LIMIT, MATRIX_W_LIMIT);
        printf("Ingrese las dimensiones nuevamente.\n");
        printf("Presione ENTER para continuar.\n");
        scanf("%i", &mat_h);
    }

    struct matrix *map = create_matrix(mat_h, mat_w);

    struct cell *start, *end;
    start = get_cell(map, 2, 4);
    if (start != NULL)
    {
        start->type = START;
    }

    end = get_cell(map, 20, 23);
    if (end != NULL)
    {
        end->type = END;
    }

    // set_range(map, OBSTACLE);
    dijkstra(map, start, end);
    print_map(map);
}
