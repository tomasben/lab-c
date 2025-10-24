#include <stdlib.h>
#include <stdio.h>

#include "matrix.h"
#include "path.h"

#define INFINITY 999999

struct cell* get_neighbours(struct matrix *m, struct cell *current, int *neighbour_count)
{
    int allow_diagonal_moves = 1;
    int count = 0;
    struct vertex candidates[8];

    int dy[8] = {-1, 1, 0,  0, -1, -1,  1, 1};
    int dx[8] = { 0, 0, 1, -1, -1,  1, -1, 1};
    int limit = allow_diagonal_moves ? 8 : 4;

    struct vertex temp = {0};
    for (int i = 0; i < limit; i++)
    {
        temp.row = current->row + dy[i];
        temp.col = current->col + dx[i];

        if ((temp.row >= 0 && temp.row < m->height)
            && (temp.col >= 0 && temp.col < m->width))
        {
            candidates[count] = temp;
            count++;
        }
    }

    *neighbour_count = count;

    struct cell *neighbours = (struct cell *)malloc(count * sizeof(struct cell));
    if (neighbours == NULL)
    {
        printf("Error al almacenar memoria para vértices vecinos");
        return NULL;
    }

    for (int i = 0; i < count; i++)
    {
        neighbours[i] = *get_cell(m, candidates[i].row, candidates[i].col);
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
        struct cell *current, *temp = NULL;

        // Obtenemos el nodo mas cercano
        // En la primera iteración será el nodo de inicio porque su distancia es 0
        for (i = 0; i < m->height; i++)
        {
            for (j = 0; j < m->width; j++)
            {
                temp = get_cell(m, i, j);
                if (!visited[i][j] && distance[i][j] < min_dist
                    && temp->type != OBSTACLE)
                {
                    min_dist = distance[i][j];
                    current = temp;
                }
            }
        }

        // Si no quedan nodos por visitar o llegamos al objetivo, salimos
        if (current == NULL || current == target)
        {
            break;
        }
        // printf("\n[iter %i] elemento elegido: A%i%i\n", iter, current->row, current->col);

        visited[current->row][current->col] = 1;

        // Obtenemos todas las celdas vecinas al nodo actual y actualizamos la
        // distancia hasta estos sumando a su distancia actual el recorrido hecho
        int neighbour_count = 0;
        struct cell *neighbours = get_neighbours(m, current, &neighbour_count);

        for (i = 0; i < neighbour_count; i++)
        {
            struct cell *nth_neighbour = &neighbours[i];
            if (nth_neighbour && !visited[nth_neighbour->row][nth_neighbour->col])
            {
                int new_dist = distance[current->row][current->col] + nth_neighbour->weight;

                if (new_dist < distance[nth_neighbour->row][nth_neighbour->col])
                {
                    distance[nth_neighbour->row][nth_neighbour->col] = new_dist;
                    previous[nth_neighbour->row][nth_neighbour->col] = current;
                }
            }
        }

        // for (i = 0; i < m->height; i++)
        // {
        //     for (j = 0; j < m->width; j++)
        //     {
        //         printf(
        //             "A%i%i visitado: %i & distancia: %i\n",
        //             i, j, visited[i][j], distance[i][j]);
        //     }
        // }

        free(neighbours);
        iter++;
    }

    // Reconstruimos el camino hasta el inicio
    struct cell *current = previous[target->row][target->col];
    while (current != NULL && current != start)
    {
        if (current != NULL)
        {
            current->type = PATH;
        }
        current = previous[current->row][current->col];
    }
}
