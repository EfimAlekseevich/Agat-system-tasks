#pragma once

#include <stdint.h>


void show_progress(
	char* progress_str,
	uint8_t visible_slots,
	uint8_t progress_slots,
	uint8_t ofset,
	float progress_persents);

void finish_progress(
	char* progress_str,
	uint8_t max_slots,
	uint8_t ofset);