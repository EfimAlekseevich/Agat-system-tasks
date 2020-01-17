#include <stdio.h>
#include <stdint.h>

#include "Headers/str.h"


void print_string(char* string)
{
	uint32_t i = 0;
	while (string[i] != '\0')
		printf("%c", string[i++]);
}


uint8_t is_letter(char symbol)
{
	if (is_capital(symbol) || is_lowercase(symbol))
		return 1;
	return 0;
}

uint8_t is_capital(char symbol)
{
	if (symbol > 64 && symbol < 91)
		return 1;
	return 0;
}


uint8_t is_lowercase(char symbol)
{
	if (symbol > 96 && symbol < 123)
		return 1;
	return 0;
}


void upper(char* string)
{
	uint32_t i = 0;
	while (string[i] != '\0')
	{
		if (is_lowercase(string[i]))
			string[i] -= 32;
		i++;
	}
}
