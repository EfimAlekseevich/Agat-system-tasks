#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include "Headers/dsp.h"


float cross_correlation(int16_t* seq_1, int16_t* seq_2, uint32_t len)
{
	int64_t cross_corr = 0, auto_corr_1 = 0, auto_corr_2 = 0;
	uint32_t i = 0;

	while (i++ < len)
	{
		cross_corr += (int64_t)seq_1[i] * seq_2[i];
		auto_corr_1 += (int64_t)seq_1[i] * seq_1[i];
		auto_corr_2 += (int64_t)seq_2[i] * seq_2[i];
	}

	//printf("%lld  %lld  %lld  ", cross_corr, auto_corr_1, auto_corr_2);
	float norm_cross_correlation = (double)cross_corr / sqrt(auto_corr_1) / sqrt(auto_corr_2);

	return norm_cross_correlation;
}
