struct vertex {
    int row;
    int col;
    int is_diagonal;
};

int dijkstra(struct matrix *m, struct cell *start, struct cell *target);
