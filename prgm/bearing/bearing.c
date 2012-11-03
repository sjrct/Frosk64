#include <expanse.h>

static pid_t expsys;

const void (*funcs[])(pid_t) = {
	create_api_expanse,
	remove_expanse
};


int main() {
	int func;
	while(!(expsys = getexpsys()));
	
	if(!register_em()) {
		// print "other em running"
		return 1;
	}
	

	while(1) {
		while(!receive(expsys, EXPANSE_SYSTEM_PORT, &id, sizeof(int)));
		funcs[num](id);
		id = poll();
	}
	return 0;
}

int send_func(pid_t pid, int port, int func) {
	send(pid, port, &func, sizeof(int));
}

bool register_em() {
	bool res;
	send_func(expsys, EXPANSE_SYSTEM_PORT, ES_REGISTER_EM);
	while(!receive(expsys, EXPANSE_SYSTEM_PORT, &res, sizeof(bool));
	return res;
}

// from handle event?
void resize_expanse(expanse e, int new_width, int new_height) {
	e.api_win.width = new_width;
	e.api_win.height = new_height;
	set_expanse_geometry(&e);
	update_expanse(e);
	update_decoration_buffer(e);
}

// from handle event?
void move_expanse(expanse e, int new_x, int new_y) {
	e.x = new_x;
	e.y = new_y;
	update_expanse(e);
}
