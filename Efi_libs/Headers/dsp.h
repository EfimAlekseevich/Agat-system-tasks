#pragma once


#include <stdint.h>


typedef struct
{
	uint32_t len;
	int16_t * sequence;
	uint8_t sizeof_type;

} Discrete_signal;


uint64_t get_absolute_correlation(Discrete_signal * long_s, Discrete_signal * short_s, uint32_t shift);
void get_absolute_correlations(Discrete_signal * s1, Discrete_signal * s2, uint16_t * correlations);
void get_convolution(Discrete_signal* s1, Discrete_signal* s2, uint16_t* convolution);
