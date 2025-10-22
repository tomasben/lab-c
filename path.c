#include <stdlib.h>
#include <stdio.h>

#include "main.h"

#define INFINITY 999999

struct vertex {
    int row;
    int col;
};

struct vertex* get_neighbours(struct matrix *m, struct cell *current, int *neighbour_count)
{
    int count = 0;
    struct vertex candidates[4];

    if (current->row > 0) // Arriba
    {
        candidates[count].row = current->row - 1;
        candidates[count].col = current->col;
        count++;
    }

    if (current->row < (m->height - 1)) // Abajo
    {
        candidates[count].row = current->row + 1;
        candidates[count].col = current->col;
        count++;
    }

    if (current->col < (m->width - 1)) // Derecha
    {
        candidates[count].row = current->row;
        candidates[count].col = current->col + 1;
        count++;
    }

    if (current->col > 0) // Izquierda
    {
        candidates[count].row = current->row;
        candidates[count].col = current->col - 1;
        count++;
    }

    *neighbour_count = count;

    struct vertex *neighbours = (struct vertex *)malloc(count * sizeof(struct vertex));
    if (neighbours == NULL)
    {
        printf("Error al almacenar memoria para vértices vecinos");
        return NULL;
    }

    for (int i = 0; i < count; i++)
    {
        neighbours[i] = candidates[i];
    }

    return neighbours;
}

void dijkstra(struct matrix *m, struct cell *start, struct cell *target)
{
    int i, j;
    int visited[m->height][m->width];
    int distance[m->height][m->width];
    struct cell *previous[m->height][m->width];

    for (i = 0; i < m->height; i++)
    {
        for (j = 0; j < m->width; j++)
        {
            visited[i][j] = 0;
            distance[i][j] = INFINITY;
        }
    }

    distance[start->row][start->col] = 0;

    int iter = 0;
    while (1)
    {
        int min_dist = INFINITY;
        struct cell *current = NULL;

        // Obtenemos el nodo mas cercano
        // En la primera iteración será el nodo de inicio porque su distancia es 0
        for (i = 0; i < m->height; i++)
        {
            for (j = 0; j < m->width; j++)
            {
                if (!visited[i][j] && distance[i][j] < min_dist)
                {
                    min_dist = distance[i][j];
                    current = get_cell(m, i, j);
                }
            }
        }

        // Si no quedan nodos por visitar o llegamos al objetivo, salimos
        if (current == NULL || current == target)
        {
            break;
        }
        printf("\n[iter %i] elemento elegido: A%i%i\n", iter, current->row, current->col);

        visited[current->row][current->col] = 1;

        // Obtenemos todas las celdas vecinas al nodo actual y actualizamos la
        // distancia hasta estos sumando a su distancia actual el recorrido hecho
        int neighbour_count = 0;
        struct vertex *neighbours = get_neighbours(m, current, &neighbour_count);

        for (i = 0; i < neighbour_count; i++)
        {
            struct vertex nth_neighbour = neighbours[i];
            struct cell *neighbour_cell = get_cell(m, nth_neighbour.row, nth_neighbour.col);

            if (neighbour_cell && !visited[nth_neighbour.row][nth_neighbour.col])
            {
                int new_dist = distance[current->row][current->col] + neighbour_cell->weight;

                if (new_dist < distance[nth_neighbour.row][nth_neighbour.col])
                {
                    distance[nth_neighbour.row][nth_neighbour.col] = new_dist;
                    previous[nth_neighbour.row][nth_neighbour.col] = current;
                }
            }
        }

        for (i = 0; i < m->height; i++)
        {
            for (j = 0; j < m->width; j++)
            {
                printf(
                    "A%i%i visitado: %i & distancia: %i\n",
                    i, j, visited[i][j], distance[i][j]);
            }
        }

        free(neighbours);
        iter++;
    }

    struct cell *current = target;
    while (current != start)
    {
        current = previous[current->row][current->col];
        if (current != NULL)
        {
            current->type = PATH;
        }
    }
}
