#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"
#include "cli.h"

struct cell* get_cell(struct grid *g, int h, int w)
{
    if ((h <= 0 || h > g->height) || (w <= 0 || w > g->width))
    {
        printf("Intentando indexar celda fuera de los límites h%i - w%i", h, w);
        return NULL;
    }

    // Fórmula para obtener elemento a_ij de una matriz 2D en memoria contigua
    // índice = (i * longitud) + j
    return &g->cells[((h-1) * g->width) + (w-1)];
}


struct grid* create_grid(int h, int w)
{
    struct grid *g = malloc(sizeof(struct grid));
    if (g == NULL) return NULL;

    g->height = h;
    g->width = w;

    size_t total_cells = (size_t)h * w;
    g->cells = malloc(total_cells * sizeof(struct cell));
    if (g->cells == NULL)
    {
        printf("No se pudo asignar las celdas del mapa. Abortando...");
        free(g);
        return NULL;
    }

    int i, j;
    for (i = 1; i <= h; i++)
    {
        for (j = 1; j <= w; j++)
        {
            struct cell *c = get_cell(g, i, j);
            c->x = i;
            c->y = j;
            c->type = EMPTY;
            c->weight = 1;
        }
    }

    return g;
}

void destroy_grid(struct grid *g)
{
    if (g != NULL)
    {
        free(g->cells);
        free(g);
    }
}

int main(void)
{
    struct grid *map = create_grid(25, 50);

    struct cell *c;
    c = get_cell(map, 25, 50);
    if (c != NULL)
    {
        c->type = START;
    }

    c = get_cell(map, 2, 2);
    if (c != NULL)
    {
        c->type = END;
    }

    print_map(map);
}
