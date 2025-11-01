const char *get_cell_char(struct matrix *m, int row, int col);

void print_map(struct matrix *m, int label_lines, int print_references);

void clear();

struct matrix* load_map(const char *filename, struct cell **start, struct cell **end);
