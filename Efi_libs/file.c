#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "Headers/file.h"

#define MAX_LEN_ARGS 1024 //8191


FILE* open_file(char* path, char* filename, char* mode)
{
	char* path_name[MAX_LEN_ARGS];
	strcpy(path_name, path);
	strcat(path_name, filename);

	FILE* file;

	if ((file = fopen(path_name, mode)) == NULL)
		printf("Error opening file '%s'.", path_name);
	else
		return file;
	return NULL;
	
}

uint64_t filesize(char* path, char* filename)
{
	uint64_t size = 0;

	FILE* file;
	if (file = open_file(path, filename, "r") != NULL)
	{
		fseek(file, 0, SEEK_END);
		size = ftell(file);
		fclose(file);
	}
	else
		exit(0);
	return size;
}