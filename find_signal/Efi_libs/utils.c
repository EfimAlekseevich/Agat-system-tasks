#include <stdint.h>
#include <stdio.h>
#include <Windows.h>


void show_progress(
	char *progress_str, 
	uint8_t visible_slots, 
	uint8_t progress_slots,
	uint8_t ofset,
	float progress_persents)
{
	sprintf(&progress_str[55], "%.2f %%", progress_persents);
	while (visible_slots < progress_slots)
		progress_str[ofset + visible_slots++] = '#';
	printf("\r%s", progress_str);
}


void finish_progress(
	char *progress_str,
	uint8_t max_slots,
	uint8_t ofset)
{	
	sprintf(progress_str,"%s : %-52s", "Finish", "100 %");
	printf("\r%s", progress_str);
}
