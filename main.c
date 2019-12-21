#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>


#include "Efi_libs/Headers/std.h"
#include "Efi_libs/Headers/str.h"
#include "Efi_libs/Headers/file.h"
#include "Efi_libs/Headers/utils.h"


void task_1()
{
	uint32_t max_len = 64;
	char* string = (char*)malloc(max_len);

	printf("Input string(<%d symbols):\n", max_len);
	fgets(string, max_len, stdin);

	upper(string);
	print_string(string);
	free(string);
}


void task_2(int num_args, char* args[])
{
	uint64_t file_size;
	file_size = filesize(args[1]);
	printf("Size of file '%s' is %llu bytes\n", args[1], file_size);
}



void task_3(int num_args, char* args[])
{

	#define BUFFER_SIZE 1024
	#define NUM_SYMBOLS 256

	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	FILE *readable_file, *stat_file;
	uint16_t i;

	char* buffer = (char*)malloc(BUFFER_SIZE + 1);
	uint32_t stats[NUM_SYMBOLS]; memset(stats, 0, NUM_SYMBOLS*4);
		
	uint64_t progress_bytes = 0;
	uint64_t file_size = filesize(args[1]);
	uint8_t progress_slots, visible_slots = 0, max_slots = 40;
	char progress[64]; memset(progress, ' ', 64);
	sprintf(progress, "Progress: ");

	readable_file = open_file(args[1], "r");
	stat_file = open_file("stat_file.txt", "w"); if (stat_file == NULL) exit(0);

	printf("File '%s' (%lld bytes) is open.\n", args[1], file_size);

	while (feof(readable_file) == 0)
	{
		fgets(buffer, BUFFER_SIZE, readable_file);
		i = 0;
		while (i < BUFFER_SIZE)
		{
			stats[(uint8_t)buffer[i++]]++;
			progress_bytes++;
			if (buffer[i] == '\0')
				break;
		}

		progress_slots = (float)progress_bytes * max_slots / file_size;
		show_progress(progress, visible_slots, progress_slots, 10, (float)progress_bytes * 100 / file_size);
	}

	finish_progress(progress, max_slots, 10);
	fclose(readable_file);
	free(buffer);

	for (i = 224; i < 256; i++)
		fprintf(stat_file, "%9c%c", (char)i, (char)(i - 32));
	fprintf(stat_file, "\n");
	for (i = 224; i < 256; i++)
		fprintf(stat_file, "%10ld", stats[i] + stats[i-32]);

	fclose(stat_file);
}


int main(int num_args, char* args[])
{
	//task_1();
	//task_2(num_args, args);
	task_3(num_args, args);
	return 1;
}