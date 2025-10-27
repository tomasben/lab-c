#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"
#include "cli.h"
#include "path.h"

/*
 * @brief Devuelve un puntero a un elemento a[ij] de la matriz
 *
 * @param m Puntero a la matriz donde buscar la celda
 * @param row Coordenada en eje Y comenzando desde el 0
 * @param col Coordenada en eje X comenzando desde el 0
 * @return Puntero a la celda especificada o NULL si está fuera de los límites
 */
struct cell* get_cell(struct matrix *m, int row, int col)
{
    if ((row < 0 || row >= m->height) || (col < 0 || col >= m->width))
    {
        printf("Intentando indexar elemento fuera de los límites %i - %i\n", row, col);
        return NULL;
    }

    // Fórmula para obtener elemento a_ij de una matriz 2D en memoria contigua
    // índice = (i * longitud) + j
    return &m->cells[row * m->width + col];
}

void set_cell_weight(struct matrix *m, int row, int col, int new_weight)
{
    struct cell *cell = get_cell(m, row, col);
    if (cell != NULL)
    {
        if (cell->type == START) return;
        if (new_weight > m->max_weight) m->max_weight = new_weight;
        cell->weight = new_weight;
    }
}

void set_cell_type(struct matrix *m, int row, int col, celltype new_type)
{
    struct cell *cell = get_cell(m, row, col);
    if (cell != NULL)
    {
        if (cell->type == START || cell->type == END) return;
        cell->type = new_type;
    }
}

struct matrix* create_matrix(int h, int w, int allow_diag)
{
    if (h <= 0 || w <= 0) return NULL;

    struct matrix *m = malloc(sizeof(struct matrix));
    if (m == NULL) return NULL;

    m->height = h;
    m->width = w;
    m->max_weight = 1;
    m->allow_diag_moves = allow_diag;

    size_t total_cells = (size_t)h * w;
    m->cells = malloc(total_cells * sizeof(struct cell));
    if (m->cells == NULL)
    {
        printf("No se pudo asignar las celdas del mapa. Abortando.\n");
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
