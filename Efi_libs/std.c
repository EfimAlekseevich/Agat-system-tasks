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


void print_array(uint16_t* array, uint64_t len)
{
	for (uint32_t i = 0; i < len; i++)
		printf("%d ", array[i]);
	printf("\n");
}


uint64_t max_index(int16_t* array, int64_t len)
{
	uint64_t index = 0, max_index = 0;

	while (++index < len)
		if (array[index] > array[max_index])
			max_index = index;
		
	return max_index;
}