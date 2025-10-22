#include <stdio.h>

#include "main.h"

#define SOLID_SHADE "█"
#define DARK_SHADE "▒"
#define LIGHT_SHADE "░"
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

#define VERTICAL "|"
#define HORIZONTAL "-"
#define CROSS "+"

#define LEFT_PADDING "    "

void print_cell(struct cell *c)
{
    switch (c->type)
    {
        case EMPTY:
            printf("%s", LIGHT_SHADE);
            break;
        case START:
            printf("%s", ARROW);
            break;
        case END:
            printf("%s", TARGET);
            break;
        case OBSTACLE:
            printf("%s", DARK_SHADE);
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
