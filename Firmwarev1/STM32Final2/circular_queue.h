#include "stm32f10x.h"

// circular queue struct where we don't need to consider overwriting
struct circular_buffer{
	uint16_t [5][ items;
	int8_t back;
	uint8_t size;
};

void enqueue(struct circular_buffer * pt, uint16_t * x, uint8_t length);

uint16_t * back(struct circular_buffer * pt);
