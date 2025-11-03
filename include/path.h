struct vertex {
    int row;
    int col;
    int is_diagonal;
};

struct node {
    struct cell *value;
    struct node *next;
};

int dijkstra(struct matrix *m, struct cell *start, struct cell *target);
