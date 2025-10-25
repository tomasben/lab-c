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
    int max_weight;
    int allow_diag_moves;
    // Puntero al 1° elemento de un bloque contiguo de celdas
    // (0, 0) | (0, 1) | (0, 2) | (0, 3) | (1, 0) | (1, 1) | ...
    struct cell *cells;
};

struct cell* get_cell(struct matrix *m, int row, int col);

void set_cell_weight(struct matrix *m);

void set_range_weight(struct matrix *m);

void set_cell_type(struct matrix *m, celltype new_t);

void set_range_type(struct matrix *m, celltype new_t);

struct matrix* create_matrix(int h, int w, int allow_diag);

void destroy_matrix(struct matrix *m);
