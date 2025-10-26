#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"
#include "path.h"

struct vertex* get_vertices(struct matrix *m, int amount)
{
    int max_h = m->height;
    int max_w = m->width;

    struct vertex *vertices = malloc(amount * sizeof(struct vertex));
    if (vertices == NULL) return NULL;

    int row, col = 0;
    for (int i = 0; i < amount; i++)
    {
        int cont = 1;

        while (cont)
        {
            struct vertex *current = &vertices[i];

            if (amount > 1) printf("\n  Para el punto %i", i + 1);
            printf("\n  Coordenada en X: "); scanf("%i", &col);
            printf("  Coordenada en Y: "); scanf("%i", &row);

            if ((row > 0 && row <= max_h) && (col > 0 && col <= max_w))
            {
                current->row = row;
                current->col = col;
                break;
            }

            printf("\n  Error: coordenada fuera de los límites.");
            printf("\n  Por favor ingrese un par entre (1, 1) y (%i, %i).\n", max_w, max_h);
        }
    }

    return vertices;
}
