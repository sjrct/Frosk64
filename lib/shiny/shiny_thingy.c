void draw(shiny_thingy * thingy) {
	switch (thingy->type) {
		case CONTAINER:
			draw(SHINY_CONTAINER(thingy), thingy->loc, thingy->size);
			break;
		case BUFFER:
			draw(SHINY_BUFFER(thingy), thingy->loc, thingy->size);
			break;
	}
}
