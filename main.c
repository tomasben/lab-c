#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"
#include "cli.h"
#include "path.h"

/*
 * @brief Devuelve un puntero a una celda (x, y)
 *
 * @param m Puntero a la matriz donde buscar la celda
 * @param row Coordenada en eje Y comenzando desde el 0
 * @param col Coordenada en eje X comenzando desde el 0
 * @return Puntero a la celda especificada o NULL si fuera de los límites
 */
struct cell* get_cell(struct matrix *m, int row, int col)
{
    if ((row < 0 || row >= m->height) || (col < 0 || col >= m->width))
    {
        printf("Intentando indexar elemento fuera de los límites a%i%i\n", row, col);
        return NULL;
    }

    // Fórmula para obtener elemento a_ij de una matriz 2D en memoria contigua
    // índice = (i * longitud) + j
    return &m->cells[row * m->width + col];
}


struct matrix* create_matrix(int h, int w)
{
    if (h <= 0 || w <= 0) return NULL;

    struct matrix *m = malloc(sizeof(struct matrix));
    if (m == NULL) return NULL;

    m->height = h;
    m->width = w;

    size_t total_cells = (size_t)h * w;
    m->cells = malloc(total_cells * sizeof(struct cell));
    if (m->cells == NULL)
    {
        printf("No se pudo asignar las celdas del mapa. Abortando...\n");
        free(m);
        return NULL;
    }

    int i, j;
    for (i = 0; i < h; i++)
    {
        for (j = 0; j < w; j++)
        {
            struct cell *c = get_cell(m, i, j);
            c->row = i;
            c->col = j;
            c->type = EMPTY;
            c->weight = 1;
        }
    }

    return m;
}

void destroy_matrix(struct matrix *m)
{
    if (m != NULL)
    {
        free(m->cells);
        free(m);
    }
}

int main(void)
{
    struct matrix *map = create_matrix(25, 50);

    struct cell *start, *end;
    start = get_cell(map, 23, 48);
    if (start != NULL)
    {
        start->type = START;
    }

    end = get_cell(map, 2, 4);
    if (end != NULL)
    {
        end->type = END;
    }

    dijkstra(map, start, end);
    print_map(map);
}
