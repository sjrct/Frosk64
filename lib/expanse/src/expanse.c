#include <expanse.h>
#include <stdlib.h>

expanse_list * receive_exp_list(pid_t id, int port) {
	int count;
	expanse_list * list = NULL;
	expanse_list * itr;
	while(!receive(id, port, &count, sizeof(int)));
	for(; count > 0; count--) {
		if(list == NULL) {
			list = malloc(sizeof(expanse_list));
			itr = list;
		} else {
			itr->next = malloc(sizeof(expanse_list));
			itr = itr->next;
		}
		while(!receive(id, port, &itr->exp, sizeof(expanse)));
	}
	return list;
}
void send_exp_list(pid_t id, int port, expanse_list * list) {
	int count = 0;
	expanse_list * itr;
	for(itr = list; itr != NULL; itr = itr->next){
		count++;
	}
	send(id, port, &count, sizeof(int));
	for(itr = list; itr != NULL; itr = itr->next) {
		send(id, port, &itr->exp, sizeof(expanse));
	}
}
void free_exp_list(expanse_list * list) {
	if(list != NULL) {
		free_exp_list(list->next);
		free(list);
	}
}
