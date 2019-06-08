#include "circular_queue.h"

void enqueue(struct circular_buffer * pt, uint16_t * x, uint8_t length) {
	pt->back = (pt->back + 1) % pt->size;
	for (int i = 0; i < length; i++) {
		pt->items[pt->back][i] = x[i];
	}
}

uint16_t * back(struct circular_buffer * pt) {
	return *(pt->items + pt->back);
}
