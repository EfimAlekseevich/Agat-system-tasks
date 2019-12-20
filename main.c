#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>


#include "Efi_libs/Headers/std.h"
#include "Efi_libs/Headers/str.h"
#include "Efi_libs/Headers/file.h"


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
	// Util - Дополнительная фича, отображает прогресс считывания файла

	#define BUFFER_SIZE 1024
	#define NUM_SYMBOLS 256

	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	FILE *readable_file, *stat_file;
	uint16_t i;

	char* buffer = (char*)malloc(BUFFER_SIZE + 1);
	uint32_t* stats[NUM_SYMBOLS]; for (i = 0; i < NUM_SYMBOLS; stats[i++] = (uint32_t)0);
		
	// Util
	uint64_t progress_bytes = 0;
	uint64_t file_size = filesize(args[1]);
	uint8_t slots, num_slots = 50;
	// Util

	readable_file = open_file(args[1], "r");
	stat_file = open_file("stat_file.txt", "w");

	printf("File '%s' (%lld bytes) is open.\n", args[1], file_size);

	
	while (feof(readable_file) == 0)
	{
		fgets(buffer, BUFFER_SIZE, readable_file);

		i = 0;
		while (i < BUFFER_SIZE && buffer[i] != '\0')
		{
			stats[(uint8_t)buffer[i]]++;
			progress_bytes++;
			i++;
		}

		// Util
		printf("Progress: ");
		slots = progress_bytes * num_slots / file_size;
		for (uint8_t slot = 0; slot < slots ; slot++)
			printf("%c", (char)35);
		printf(" %f %%\r", ((float)progress_bytes * 100 / file_size));
		// Util
	}
	// Util
	printf("Finish  : ");
	for (uint8_t slot = 0; slot < num_slots; slot++)
		printf("%c", (char)35);
	printf(" 100 %%                                                                           \n\n");
	// Util

	//for (i = 0; i < NUM_SYMBOLS; printf("%c  ---  %p \n", (char)i, stats[i++]));
	fclose(readable_file);
	free(buffer);
	for (i = 224; i < 256; i++)
		fprintf(stat_file, "%9c%c", (char)i, (char)(i - 32));
	fprintf(stat_file, "\n");
	for (i = 224; i < 256; i++)
		fprintf(stat_file, "%10d", sum(stats[i], stats[i-32]));

	fclose(stat_file);
}


int main(int num_args, char* args[])
{
	//task_1();
	//task_2(num_args, args);
	task_3(num_args, args);
	return 1;
}