#pragma once

#include <stdint.h>
#include <stdio.h>

FILE * open_file(char* filename, char* mode);
uint64_t filesize(char* filename);
