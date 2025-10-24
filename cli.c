#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"

#define ARROW "▶"
#define TARGET "◎"
#define SQUARE "■"
#define DOT "•"

#define BORDER_HORIZONTAL "═"
#define BORDER_VERTICAL "║"
#define BORDER_UPPER_LEFT "╔"
#define BORDER_UPPER_RIGHT "╗"
#define BORDER_LOWER_LEFT "╚"
#define BORDER_LOWER_RIGHT "╝"

#define LEFT_PADDING "    "

void print_cell(struct cell *c)
{
    switch (c->type)
    {
        case EMPTY:
            printf("%c", ' ');
            break;
        case START:
            printf("%c", '+');
            break;
        case END:
            printf("%s", TARGET);
            break;
        case OBSTACLE:
            printf("%s", SQUARE);
            break;
        case PATH:
            printf("%s", DOT);
            break;
        default:
            printf("%s", "!");
    }
}

void print_map(struct matrix *m)
{
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
            print_cell(get_cell(m, i, j));
        }
        printf("%s\n", BORDER_VERTICAL);
    }

    printf("%s%s", LEFT_PADDING, BORDER_LOWER_LEFT);
    for (i = 0; i < m->width; i++)
    {
        printf("%s", BORDER_HORIZONTAL);
    }
    printf("%s\n", BORDER_LOWER_RIGHT);
}

void clear()
{
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}
