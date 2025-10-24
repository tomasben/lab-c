#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"
#include "cli.h"
#include "path.h"

/*
 * @brief Devuelve un puntero a un elemento a_(ij) de la matriz
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
        printf("Intentando indexar elemento fuera de los límites a%i%i\n", row, col);
        return NULL;
    }

    // Fórmula para obtener elemento a_ij de una matriz 2D en memoria contigua
    // índice = (i * longitud) + j
    return &m->cells[row * m->width + col];
}

void set_range(struct matrix *m, celltype new_t)
{
    struct vertex p1 = {0}, p2 = {0};
    struct vertex *points[] = {&p1, &p2};

    for (int i = 0; i < 2; i++)
    {
        struct vertex *current_p = points[i];
        int x_pos, y_pos;

        clear();
        printf("--- Definición del extremo %d ---\n", i + 1);
        printf("Ingrese las coordenadas (x, y):\n");

        printf("\nPosición en eje X: ");
        if (scanf("%d", &x_pos) != 1) {
            printf("Error de entrada. Terminando.\n");
            return;
        }

        current_p->col = x_pos;

        printf("\nPosición en eje Y: ");
        if (scanf("%d", &y_pos) != 1) {
            printf("Error de entrada. Terminando.\n");
            return;
        }

        current_p->row = y_pos;
    }

    if (p1.row > p2.row)
    {
        printf("Error: la coordenada y del primer extremo no puede ser mayor que la del segundo. Terminando.");
        return;
    }
    else if (p1.col > p2.col)
    {
        printf("Error: la coordenada x del primer extremo no puede ser mayor que la del segundo. Terminando.");
        return;
    }

    struct cell *cell;
    for (int i = p1.row; i <= p2.row; i++)
    {
        for (int j = p1.col; j <= p2.col; j++)
        {
            cell = get_cell(m, i, j);
            if (cell != NULL)
            {
                cell->type = new_t;
            }
        }
    }
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
