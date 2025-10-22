typedef enum {
    EMPTY,
    START,
    END,
    OBSTACLE,
    PATH,
} celltype;

struct cell {
    int row;
    int col;
    celltype type;
    int weight;
};

struct matrix {
    int height;
    int width;
    // puntero al primer elemento de un bloque contiguo de h * w celdas
    // Para una matriz 2x4 en memoria:
    // (1, 1) | (1, 2) | (2, 1) | (2, 2) | (3, 1) | ...
    struct cell *cells;
};

struct matrix* create_matrix(int h, int w);

void destroy_matrix(struct matrix *m);

struct cell* get_cell(struct matrix *m, int row, int col);
