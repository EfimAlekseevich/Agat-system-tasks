#pragma once


#include <stdint.h>


int64_t get_absolute_correlation(int16_t* long_s, int16_t* short_s, uint32_t short_len, uint32_t shift);
void get_absolute_correlations(int16_t* long_s, int16_t* short_s, uint32_t long_len, uint32_t short_len, int32_t * correlations);
void get_convolution(int16_t* long_s, int16_t* short_s, uint32_t long_len, uint32_t short_len, int32_t * convolution);
