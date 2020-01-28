#include <stdint.h>
#include <math.h>

#include "Headers/dsp.h"
#include "Headers/std.h"


int64_t get_absolute_correlation(const int16_t * long_s, const int16_t * short_s, uint32_t short_len, uint32_t shift)
{
    int64_t corr = 0;

    for (uint64_t i = 0; i < short_len; i++)
        corr += (int64_t)short_s[i] * long_s[i+shift];

	return corr;
}


void get_absolute_correlations(const int16_t * long_s, const int16_t * short_s, uint32_t long_len, uint32_t short_len, int32_t * correlations)
{
	uint32_t shifts = long_len - short_len + 1;

	for (uint32_t shift = 0; shift < shifts; shift++)
        correlations[shift] = (int64_t)get_absolute_correlation(long_s, short_s, short_len, shift) / short_len;
}


void get_convolution( const int16_t * long_s, const int16_t * short_s, uint32_t long_len, uint32_t short_len, int32_t * convolution)
{
	reverse(short_s, short_len);
	get_absolute_correlations(long_s, short_s, long_len, short_len, convolution);
}


double get_normal_correlation_coefficient( const int16_t * long_s, const int16_t * short_s, uint32_t short_len, uint32_t shift)
{
    double coef = (double)get_absolute_correlation(long_s, short_s, short_len, shift) /
                  sqrt(get_absolute_correlation(long_s, long_s, short_len, shift)) /
                  sqrt(get_absolute_correlation(short_s, short_s, short_len, 0));
    return coef;
}
