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
    float weight;
};

struct matrix {
    int height;
    int width;
    int max_weight;
    int min_weight;
    int allow_diag_moves;
    // Puntero al 1er elemento de un bloque contiguo de celdas
    // (0, 0) | (0, 1) | (0, 2) | (0, 3) | (1, 0) | ...
    struct cell *cells;
};

struct cell* get_cell(struct matrix *m, int row, int col);

void set_cell_type(struct matrix *m, int row, int col, celltype new_type);

void set_cell_weight(struct matrix *m, int row, int col, int new_weight);

struct matrix* create_matrix(int h, int w, int allow_diag);

void destroy_matrix(struct matrix *m);
