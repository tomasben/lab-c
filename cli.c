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

const char* get_cell_char(struct matrix *m, int row, int col)
{
    float ratio;
    struct cell *cell = get_cell(m, row, col);

    switch (cell->type)
    {
        case EMPTY:
            ratio = cell->weight / m->max_weight;

            if (ratio <= 0.25)
                return " ";
            else if (ratio <= 0.50)
                return LIGHT_SHADE;
            else if (ratio <= 0.75)
                return MEDIUM_SHADE;
            else
                return " ";
        case START:
            return TARGET;
        case END:
            return CROSS;
        case OBSTACLE:
            return SOLID_BLOCK;
        case PATH:
            return DOT;
        default:
            return INVALID;
    }
}

void print_map(struct matrix *m, int label_lines, int print_references)
{
    const char *references =
    "  Referencias:                                                      \n"
    "   [█] bloque sólido: representa obstáculos  ┃ [•] pasos del camino \n"
    "   [▓] sombreado alto: celdas de peso mayor  ┃ [◎] punto de inicio  \n"
    "   [▒] sombreado medio: celdas de peso medio ┃ [✗] punto de fín     \n"
    "   [░] sombreado bajo: celdas de peso menor  ┃                      \n";

    size_t buffer_size = ((m->width + 4) * 4) * (m->height + 2) + strlen(references);
    if (label_lines) buffer_size += (2 * 4) * (m->height + 2) + ((m->width + 6) * 4);

    char *buffer = malloc(buffer_size);
    if (buffer == NULL) return;

    char *ptr = buffer;

    ptr += sprintf(ptr, "\n");
    if (label_lines)
    {
        ptr += sprintf(ptr, "    0  ");

        for (int i = 1; i <= m->width; i++)
        {
            if (i % 5 == 0)
                ptr += sprintf(ptr, "%i", i % 10);
            else
                ptr += sprintf(ptr, "%s", DOT);
        }
        ptr += sprintf(ptr, "\n");
    }

    ptr += sprintf(ptr, "%s", LEFT_PADDING);
    if (label_lines) ptr += sprintf(ptr, "    ");
    ptr += sprintf(ptr, "%s", BORDER_UPPER_LEFT);

    for (int i = 0; i < m->width; i++)
    {
        ptr += sprintf(ptr, "%s", BORDER_HORIZONTAL);
    }
    ptr += sprintf(ptr, "%s\n", BORDER_UPPER_RIGHT);

    for (int i = 0; i < m->height; i++)
    {
        ptr += sprintf(ptr, "%s", LEFT_PADDING);
        if (label_lines)
        {
            if ((i+1) % 5 == 0)
                ptr += sprintf(ptr, "%3d ", (i+1));
            else
                ptr += sprintf(ptr, "  %s ", DOT);
        }
        ptr += sprintf(ptr, "%s", BORDER_VERTICAL);

        for (int j = 0; j < m->width; j++)
        {
            ptr += sprintf(ptr, "%s", get_cell_char(m, i, j));
        }
        ptr += sprintf(ptr, "%s\n", BORDER_VERTICAL);
    }

    ptr += sprintf(ptr, "%s", LEFT_PADDING);
    if (label_lines) ptr += sprintf(ptr, "    ");
    ptr += sprintf(ptr, "%s", BORDER_LOWER_LEFT);

    for (int i = 0; i < m->width; i++)
    {
        ptr += sprintf(ptr, "%s", BORDER_HORIZONTAL);
    }
    ptr += sprintf(ptr, "%s\n", BORDER_LOWER_RIGHT);

    if (print_references) ptr += sprintf(ptr, "\n%s", references);

    fwrite(buffer, 1, ptr - buffer, stdout);
    fflush(stdout);

    free(buffer);
}

void clear()
{
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}
