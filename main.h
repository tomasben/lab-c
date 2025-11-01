struct matrix* define_map();

struct cell* define_start(struct matrix *m);

struct cell* define_end(struct matrix *m, struct cell *start);

void define_obstacles(struct matrix *m);

void define_areas(struct matrix *m);

void set_windows_codepage();
