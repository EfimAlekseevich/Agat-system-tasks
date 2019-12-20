#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "Headers/std.h"


int64_t randint(int64_t min, int64_t max)
{
	int64_t range = abs(max - min);
	double rand_koef = rand() / (double)RAND_MAX;
	int64_t random = rand_koef * range + min;
	return random;
}


void print_array(uint8_t* array, uint32_t len)
{
	for (uint32_t i = 0; i < len; i++)
		printf("%d ", array[i]);
	printf("\n");
}


uint32_t sum(uint32_t a, uint32_t b)
{
	printf("%d %d  ", a, b);
	return a + b;
}
