void (*main_func)();

shiny_thingy * create_shiny_expanse(int width, int height);
void destroy_expanse(expanse_handle handle);
void draw_buffer(pixel_buffer buffer, shiny_loc loc);
void register_event_handler(shiny_thingy *, event_type, void (*event_handler)(shiny_thingy *, event))

