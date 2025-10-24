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
    // (0, 0) | (0, 1) | (0, 2) | (0, 3) | (1, 0) | (1, 1) | ...
    struct cell *cells;
};

struct cell* get_cell(struct matrix *m, int row, int col);

void set_range(struct matrix *m, celltype new_t);

struct matrix* create_matrix(int h, int w);

void destroy_matrix(struct matrix *m);
