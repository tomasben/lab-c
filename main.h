typedef enum {
    EMPTY,
    START,
    END,
    OBSTACLE,
    PATH,
} celltype;

struct cell {
    int x;
    int y;
    celltype type;
    int weight;
};

struct grid {
    int height;
    int width;
    // puntero al primer elemento de un bloque contiguo de h * w celdas
    // Para una matriz 2x4 en memoria:
    // (1, 1) | (1, 2) | (2, 1) | (2, 2) | (3, 1) | ...
    struct cell *cells;
};

struct grid* create_grid(int h, int w);

void destroy_grid(struct grid *g);

struct cell* get_cell(struct grid *g, int h, int w);
