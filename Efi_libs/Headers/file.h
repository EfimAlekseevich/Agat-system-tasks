#pragma once

#include <stdint.h>

FILE* open_file(char* path, char* filename, char* mode);
uint64_t filesize(char* path, char* filename);