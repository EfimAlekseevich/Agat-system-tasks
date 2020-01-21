#pragma once

#include <stdint.h>


int64_t randint(int64_t min, int64_t max);
void print_array(int16_t* array, uint32_t len);
uint32_t max_index(int32_t* array, uint32_t len);
int16_t * reverse(int16_t* array, uint32_t len);
void swap_ptr(void ** ptr_1, void ** ptr_2);
