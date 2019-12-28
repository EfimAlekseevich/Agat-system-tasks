#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include "Headers/dsp.h"


uint64_t get_absolute_correlation(Discrete_signal * long_s, Discrete_signal * short_s, uint32_t shift)
{
	int64_t corr = 0;

	for (uint32_t i = 0; i < short_s->len; i++)
		corr += (int64_t)short_s->sequence[i] * long_s->sequence[i+shift];

	return corr;
}


void get_absolute_correlations(Discrete_signal * s1, Discrete_signal * s2, uint16_t * correlations)
{
	Discrete_signal *long_s = s1, *short_s = s2;

	if (s1->len < s2->len)
	{
		long_s = s2;
		short_s = s1;
	}

	uint32_t shifts = long_s->len - short_s->len + 1;

	for (uint32_t shift = 0; shift < shifts; shift++)
		correlations[shift] = get_absolute_correlation(long_s, short_s, shift) / short_s->len;
}
