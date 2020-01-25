#pragma once


#include <stdint.h>


int32_t get_absolute_correlation(const int16_t* long_s, const int16_t* short_s, uint32_t short_len, uint32_t shift);
void get_absolute_correlations(const int16_t* long_s, const int16_t* short_s, uint32_t long_len, uint32_t short_len, int32_t * correlations);
void get_convolution(const int16_t* long_s, const int16_t* short_s, uint32_t long_len, uint32_t short_len, int32_t * convolution);
double get_normal_correlation_coefficient(const int16_t * long_s, const int16_t * short_s, uint32_t short_len, uint32_t shift);
