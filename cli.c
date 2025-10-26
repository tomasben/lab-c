#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "matrix.h"

#define SOLID_BLOCK "█"
#define DARK_SHADE "▓"
#define MEDIUM_SHADE "▒"
#define LIGHT_SHADE "░"

#define TARGET "◎"
#define CROSS "✗"
#define DOT "•"
#define INVALID "�"

#define BORDER_HORIZONTAL "━"
#define BORDER_VERTICAL "┃"
#define BORDER_UPPER_LEFT "┏"
#define BORDER_UPPER_RIGHT "┓"
#define BORDER_LOWER_LEFT "┗"
#define BORDER_LOWER_RIGHT "┛"

#define LEFT_PADDING "  "

void print_cell(struct matrix *m, struct cell *c)
{
    float ratio;
    switch (c->type)
    {
        case EMPTY:
            ratio = c->weight / m->max_weight;

            if (ratio <= 0.25)
                printf("%c", ' ');
            else if (ratio <= 0.50)
                printf("%s", LIGHT_SHADE);
            else if (ratio <= 0.75)
                printf("%s", MEDIUM_SHADE);
            else
                printf("%s", DARK_SHADE);

            break;
        case START:
            printf("%s", TARGET);
            break;
        case END:
            printf("%s", CROSS);
            break;
        case OBSTACLE:
            printf("%s", SOLID_BLOCK);
            break;
        case PATH:
            printf("%s", DOT);
            break;
        default:
            printf("%s", INVALID);
    }
}

void print_map(struct matrix *m)
{
    const char *references =
    "  Referencias:                                                      \n"
    "   [█] bloque sólido: representa obstáculos  ┃ [•] pasos del camino \n"
    "   [▓] sombreado alto: celdas de peso mayor  ┃ [◎] punto de inicio  \n"
    "   [▒] sombreado medio: celdas de peso medio ┃ [✗] punto de fín     \n"
    "   [░] sombreado bajo: celdas de peso menor  ┃                      \n";

    int i, j;

    printf("\n");
    printf("%s%s", LEFT_PADDING, BORDER_UPPER_LEFT);
    for (i = 0; i < m->width; i++)
    {
        printf("%s", BORDER_HORIZONTAL);
    }
    printf("%s\n", BORDER_UPPER_RIGHT);

    for (i = 0; i < m->height; i++)
    {
        printf("%s%s", LEFT_PADDING, BORDER_VERTICAL);
        for (j = 0; j < m->width; j++)
        {
            print_cell(m, get_cell(m, i, j));
        }
        printf("%s\n", BORDER_VERTICAL);
    }

    printf("%s%s", LEFT_PADDING, BORDER_LOWER_LEFT);
    for (i = 0; i < m->width; i++)
    {
        printf("%s", BORDER_HORIZONTAL);
    }
    printf("%s\n", BORDER_LOWER_RIGHT);

    printf("\n%s", references);
}

void clear()
{
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}
