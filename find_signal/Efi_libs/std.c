#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "Headers/std.h"




int64_t randint(int64_t min, int64_t max)
{
	int64_t range = abs(max - min);
	double rand_koef = (double)rand() / RAND_MAX;
	int64_t random = rand_koef * range + min;
	return random;
}


void print_array(int16_t * array, uint32_t len)
{
	for (uint32_t i = 0; i < len; i++)
		printf("%d ", array[i]);
	printf("\n");
}


uint32_t max_index(int32_t * array, uint32_t len)
{
	uint32_t index = 0, max_index = 0;

	while (++index < len)
		if (array[index] > array[max_index])
			max_index = index;
		
	return max_index;
}


int16_t * reverse(int16_t * array, uint32_t len)
{
	int16_t temp;

	for (uint32_t i = 0; i < len / 2; i++)
	{
		temp = array[i];
		array[i] = array[len - i - 1];
		array[len - i - 1] = temp;
	}

	return array;
}


void swap_ptr(void ** ptr_1, void ** ptr_2)
{
    void ** temp = *ptr_1;
    *ptr_1 = *ptr_2;
    *ptr_2 = temp;
}
