#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "matrix.h"
#include "path.h"

/*
 * Algoritmo de Dijkstra paso a paso:
 * (1) se crea un conjunto con todos los nodos sin visitar.
 * (2) se asigna a todos los nodos un peso que indica el costo para llegar
 *  hasta este desde el punto de partida. Este valor inicialmente es infinito
 *  y luego se lo reemplaza por la sumatoria de los costos de los nodos que
 *  lo para llegar hasta él.
 * (3) de los nodos no visitados, se selecciona el siguiente punto con el menor
 *  peso. Inicialmente este va a ser el punto inicial por ser el único con peso
 *  cero.
 * (4) para el nodo seleccionado, se obtiene todos los vecinos y se actualiza
 * su peso considerando el mentor entre su peso actual y el costo acumulado
 * para haber llegado hasta él.
 * (5) luego de realizar el paso anterior por cada nodo vecino, se marca el
 * actual como visitado y se repite desde el paso 5.
 * (6) una vez agotado el conjunto de los no visitados, cada nodo contendrá
 * como peso el menor costo posible para llegar hasta él y el nodo que le
 * precede, permitiendo así reconstruir el camino hasta el inicio.
 */

struct vertex* get_neighbours(struct matrix *m, struct cell *current,
    int *neighbour_count)
{
    int count = 0;
    struct vertex candidates[8];

    //            ↑  ↓  →   ←   ↖   ↗   ↙  ↘
    int dy[8] = {-1, 1, 0,  0, -1, -1,  1, 1};
    int dx[8] = { 0, 0, 1, -1, -1,  1, -1, 1};

    int limit = m->allow_diag_moves ? 8 : 4;

    struct vertex temp;
    for (int i = 0; i < limit; i++)
    {
        temp.row = current->row + dy[i];
        temp.col = current->col + dx[i];
        temp.is_diagonal = i > 3 ? 1 : 0;

        if ((temp.row >= 0 && temp.row < m->height)
            && (temp.col >= 0 && temp.col < m->width))
        {
            candidates[count] = temp;
            count++;
        }
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

int dijkstra(struct matrix *m, struct cell *start, struct cell *target)
{
    int i, j;
    int visited[m->height][m->width];
    float distance[m->height][m->width];
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

    while (1)
    {
        float min_dist = INFINITY;
        struct cell *current = NULL, *temp = NULL;

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

        if (current == NULL) break;

        visited[current->row][current->col] = 1;

        int neighbour_count = 0;
        struct vertex *neighbours = get_neighbours(m, current, &neighbour_count);

        for (i = 0; i < neighbour_count; i++)
        {
            struct cell *nth_neighbour = get_cell(m, neighbours[i].row, neighbours[i].col);
            if (nth_neighbour && !visited[nth_neighbour->row][nth_neighbour->col])
            {
                float added_cost = neighbours[i].is_diagonal ? sqrt(2) * nth_neighbour->weight : nth_neighbour->weight;
                float new_dist = distance[current->row][current->col] + added_cost;

                if (new_dist < distance[nth_neighbour->row][nth_neighbour->col])
                {
                    distance[nth_neighbour->row][nth_neighbour->col] = new_dist;
                    previous[nth_neighbour->row][nth_neighbour->col] = current;
                }
            }
        }

        free(neighbours);
    }

    struct cell *current = previous[target->row][target->col];
    if (current == NULL) return 0;
    else
    {
        while (current != NULL && current != start)
        {
            if (current != NULL) current->type = PATH;
            current = previous[current->row][current->col];
        }
    }

    return 1;
}
