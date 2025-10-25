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
        printf("Intentando indexar elemento fuera de los límites %i - %i\n", row, col);
        return NULL;
    }

    // Fórmula para obtener elemento a_ij de una matriz 2D en memoria contigua
    // índice = (i * longitud) + j
    return &m->cells[row * m->width + col];
}

void set_cell_weight(struct matrix *m)
{
    int max_h = m->height;
    int max_w = m->width;
    int new_w = 1;
    struct vertex point = {0};

    while (1)
    {
        printf("\n  Coordenada en el eje X: ");
        scanf(" %i", &point.col);
        printf("  Coordenada en el eje Y: ");
        scanf(" %i", &point.row);

        printf("\n  Nuevo peso: "); scanf(" %i", &new_w);

        if ((point.row > 0 && point.row <= max_h)
            && (point.col > 0 && point.col <= max_w))
        {
            break;
        }

        printf("\n  Error: coordenada fuera de los límites.");
        printf("\n  Por favor ingrese un par entre (0, 0) y (%i, %i).\n", max_w, max_h);
    }

    if (new_w > m->max_weight) m->max_weight = new_w;

    struct cell *cell = get_cell(m, point.row, point.col);
    if (cell != NULL)
    {
        cell->weight = new_w;
    }
}

void set_range_weight(struct matrix *m)
{
    int max_h = m->height;
    int max_w = m->width;
    int new_w = 1;
    struct vertex p1 = {0}, p2 = {0};
    struct vertex *points[] = {&p1, &p2};

    printf("\n  🛈  Defina dos puntos en la misma fila o columna para modificar"
           "\n  el peso de la línea de celdas, o seleccione dos puntos diferentes"
           "\n  para modificar el área comprendida entre estos.\n");

    int should_continue = 1;
    while (should_continue)
    {
        for (int i = 0; i < 2; i++)
        {
            int x_pos, y_pos;
            struct vertex *current = points[i];

            printf("\n  Coordenada X del extremo #%i: ", i + 1);
            scanf(" %i", &x_pos);

            printf("  Coordenada Y del extremo #%i: ", i + 1);
            scanf(" %i", &y_pos);

            current->col = x_pos;
            current->row = y_pos;
        }

        if ((p1.row > 0 && p1.row <= max_h) && (p1.col > 0 && p1.col <= max_w)
            && (p2.row > 0 && p2.row <= max_h) && (p2.col > 0 && p2.col <= max_w))
        {
            break;
        }

        printf("\n  Error: coordenadas fuera de los límites.");
        printf("\n  Por favor ingrese pares entre (0, 0) y (%i, %i).", max_w, max_h);
    }

    printf("\n  Nuevo peso: "); scanf(" %i", &new_w);

    int min_row = (p2.row > p1.row) ? p1.row : p2.row;
    int max_row = (p2.row > p1.row) ? p2.row : p1.row;
    int min_col = (p2.col > p1.col) ? p1.col : p2.col;
    int max_col = (p2.col > p1.col) ? p2.col : p1.col;

    if (new_w > m->max_weight) m->max_weight = new_w;

    struct cell *cell;
    for (int i = min_row; i <= max_row; i++)
    {
        for (int j = min_col; j <= max_col; j++)
        {
            cell = get_cell(m, i, j);
            if (cell != NULL)
            {
                cell->weight = new_w;
            }
        }
    }
}

void set_cell_type(struct matrix *m, celltype new_t)
{
    int max_h = m->height;
    int max_w = m->width;
    struct vertex point = {0};

    while (1)
    {
        printf("\n  Coordenada en el eje X: ");
        scanf(" %i", &point.col);
        printf("  Coordenada en el eje Y: ");
        scanf(" %i", &point.row);

        if ((point.row > 0 && point.row <= max_h)
            && (point.col > 0 && point.col <= max_w))
        {
            break;
        }

        printf("\n  Error: coordenada fuera de los límites.");
        printf("\n  Por favor ingrese un par entre (0, 0) y (%i, %i).\n", max_w, max_h);
    }

    struct cell *cell = get_cell(m, point.row, point.col);
    if (cell != NULL)
    {
        if (cell->type == START || cell->type == END) return;
        cell->type = new_t;
    }
}

void set_range_type(struct matrix *m, celltype new_t)
{
    int max_h = m->height;
    int max_w = m->width;
    struct vertex p1 = {0}, p2 = {0};
    struct vertex *points[] = {&p1, &p2};

    printf("\n  🛈  Defina dos puntos en la misma fila o columna para crear"
           "\n  una línea de obstáculos, o seleccione dos puntos diferentes"
           "\n  para marcar el área comprendida entre estos.\n");

    int should_continue = 1;
    while (should_continue)
    {
        for (int i = 0; i < 2; i++)
        {
            int x_pos, y_pos;
            struct vertex *current = points[i];

            printf("\n  Coordenada X del extremo #%i: ", i + 1);
            scanf(" %i", &x_pos);

            printf("  Coordenada Y del extremo #%i: ", i + 1);
            scanf(" %i", &y_pos);

            current->col = x_pos;
            current->row = y_pos;
        }

        if ((p1.row > 0 && p1.row <= max_h) && (p1.col > 0 && p1.col <= max_w)
            && (p2.row > 0 && p2.row <= max_h) && (p2.col > 0 && p2.col <= max_w))
        {
            break;
        }

        printf("\n  Error: coordenadas fuera de los límites.");
        printf("\n  Por favor ingrese pares entre (0, 0) y (%i, %i).", max_w, max_h);
    }

    int min_row = (p2.row > p1.row) ? p1.row : p2.row;
    int max_row = (p2.row > p1.row) ? p2.row : p1.row;
    int min_col = (p2.col > p1.col) ? p1.col : p2.col;
    int max_col = (p2.col > p1.col) ? p2.col : p1.col;

    struct cell *cell;
    for (int i = min_row; i <= max_row; i++)
    {
        for (int j = min_col; j <= max_col; j++)
        {
            cell = get_cell(m, i, j);
            if (cell != NULL)
            {
                if (cell->type == START || cell->type == END) return;
                cell->type = new_t;
            }
        }
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

void destroy_matrix(struct matrix *m)
{
    if (m != NULL)
    {
        free(m->cells);
        free(m);
    }
}
