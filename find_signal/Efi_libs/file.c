#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Headers/file.h"


FILE* open_file(char* filename, char* mode)
{
	FILE* file;

	if ((file = fopen(filename, mode)) == NULL)
        printf("Error opening file '%s'.\r\n", filename);
	else
    {
        printf("File '%s' is succesful opened\r\n", filename);
		return file;
    }
	return NULL;
	
}

uint64_t filesize(char* filename)
{
	uint64_t size = 0;

	FILE* file;

	if ((file = open_file(filename, "r")) != NULL)
	{
		fseek(file, 0, SEEK_END);
		size = ftell(file);
		fclose(file);
	}
	else
		exit(0);
	return size;
}
