#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

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
	file_size = filesize("", args[1]);
	printf("Size of file '%s' is %llu bytes\n", args[1], file_size);
}


void task_3(int num_args, char* args[])
{
	FILE* readable_file, stat_file;

}


int main(int num_args, char* args[])
{
	//task_1();
	task_2(num_args, args);

	return 1;
}