#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "Efi_libs/Headers/std.h"
#include "Efi_libs/Headers/file.h"
#include "Efi_libs/Headers/dsp.h"


void left_shift(int16_t * array, uint32_t len)
{
    uint32_t i;
    for (i=0; i<len-1; i++)
        array[i] = array[i+1];
}


void primary_load(FILE * data_file, int16_t * buffer, uint32_t len)
{
    char line[5];
    uint32_t i;
    for (i=1; i<len; i++)
    {
        fgets(line, 5, data_file);
        buffer[i] = atoi(line);
    }
}


int main()
{
    int16_t sync[] = {
      -170,  -233,  -281,  -309,  -318,  -312,  -300,  -290,  -287,  -290,  -294,  -291,  -278,  -256,
      -235,  -228,  -246,  -290,  -351,  -406,  -424,  -384,  -276,  -115,    64,   218,   305,   301,
       209,    56,  -109,  -234,  -281,  -234,  -109,    56,   208,   301,   305,   219,    64,  -119,
      -288,  -406,  -455,  -438,  -375,  -295,  -226,  -190,  -190,  -222,  -271,  -319,  -353,  -363,
      -342,  -288,  -203,   -90,    39,   170,   285,   369,   411,   409,   366,   287,   182,    62,
       -63,  -179,  -275,  -343,  -378,  -379,  -347,  -287,  -200,   -93,    29,   157,   277,   374,
       433,   440,   386,   275,   124,   -39,  -181,  -271,  -291,  -237,  -125,    20,   167,   289,
       366,   392,   374,   329,   279,   242,   228,   236,   256,   277,   291,   294,   290,   287,
       290,   300,   312,   318,   309,   281,   233,   170};

    uint32_t sync_len = sizeof (sync) / sizeof (int16_t);
    int32_t auto_corr = get_absolute_correlation(sync, sync, sync_len, 0) / sync_len;
    const float sensivity = 0.9;

    char * filename = "data_file.txt";
    FILE * data_file = open_file(filename, "r");
    if (data_file == NULL)
    {
        printf("ERROR --> Exit");
        fclose(data_file);
        exit(0);
    }
    else
    {
        printf("Start finding\r\n");
        int16_t * buffer = (int16_t *)malloc(sync_len*(sizeof (int16_t)));
        char line[5];
        uint32_t position = 0;

        primary_load(data_file, buffer, sync_len);

        printf("Primary loading is completed.");

        while(feof(data_file) == 0)
        {
            left_shift(buffer, sync_len);
            fgets(line, 5, data_file);

            printf("%s\n",line);

            buffer[sync_len-1] = atoi(line);
            if (get_absolute_correlation(buffer, sync, sync_len, 0) / auto_corr > sensivity)
                printf("%d  ", position);

            position++;
        }

        fclose(data_file);
        free(buffer);
    }
    printf("Exit");
}
