struct container_thingy;

shiny_thingy * create_container(int width, int height);

void add_thingy(container_thingy *, shiny_thingy *);
void remove_thingy(container_thingy *, shiny_thingy *);

void draw(container_thingy *, shiny_loc, shiny_size);
