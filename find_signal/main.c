/*!
\file Основной файл
 * ТЗ:
 * В файле real_rec.txt запись реального эфирного сигнала.
 * Нужно найти в нем все позиции начала кадровой синхронизации.
 * и выдать результат в текстовый файл (одна строка - одна позиция).
 * Позицию выводить в формате времени с точностью до десятых долей миллисекунд.
 * Частота дискретизации файла - 24 кГц, 16 бит на отсчет.
 * Обработка ВЕДЕТСЯ БУФЕРАМИ ПО 5 МС (не понимаю почему нужно именно так???).
 * 1 / 24 000 = 41,(6) мкс.
 * 5 мс >>---> 5 мс / 41,(6) мкс ~ 120 отсчётов.
*/


#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#include "Efi_libs/Headers/std.h"
#include "Efi_libs/Headers/file.h"
#include "Efi_libs/Headers/dsp.h"

#define SAMPLING_FREQUENCY 24000
#define RESOLUTION_TYPE int16_t
#define BUFFER_LEN 120


/*!
Заполняет буффер данными из файла
\param[in] data_file Указатель на файл с данными
\param[out] buffer Буфер для загрузки данных
\param[in] len Длинна буфера
\return Количество записанных элементов в буффер
*/

uint32_t fill_buffer(FILE * data_file, int16_t * buffer, uint32_t buffer_len)
{
    uint32_t i = 0;
    while (i < buffer_len && feof(data_file) == 0)
            fscanf(data_file, "%hd", &buffer[i++]);
    return i;
}


double position_to_time(uint32_t position)
{
    return (double)position / SAMPLING_FREQUENCY;
}


void write_position(FILE * out_file, uint32_t position)
{
    double time = position_to_time(position);
    fprintf(out_file,"%.6f \n", time);
}


void get_combined_buf(int16_t * previous_buf, int16_t * actual_buf, int16_t * combined_buf)
{
    memcpy(combined_buf, previous_buf, BUFFER_LEN*sizeof (int16_t));
    memcpy(combined_buf+BUFFER_LEN, actual_buf, (BUFFER_LEN-1)*sizeof (int16_t));
}


uint16_t get_best_match_index(int32_t * correlations, uint16_t len, int32_t auto_corr)
{
    uint16_t i, index = 0;
    int32_t min_diff = abs(correlations[index] - auto_corr);

    for (i=1; i<len; i++)
        if (abs(correlations[i] - auto_corr) < min_diff)
        {
            index = i;
            min_diff = abs(correlations[i] - auto_corr);
        }

    return index;
}


/*!
Начальная основная функция
\return 0 при успешном завершении
*/

int main()
{
    const float sensivity_deviation  = 0.025;  // Максимально допустимое отклонение.

    const int16_t sync[] = {
      -170,  -233,  -281,  -309,  -318,  -312,  -300,  -290,  -287,  -290,  -294,  -291,  -278,  -256,
      -235,  -228,  -246,  -290,  -351,  -406,  -424,  -384,  -276,  -115,    64,   218,   305,   301,
       209,    56,  -109,  -234,  -281,  -234,  -109,    56,   208,   301,   305,   219,    64,  -119,
      -288,  -406,  -455,  -438,  -375,  -295,  -226,  -190,  -190,  -222,  -271,  -319,  -353,  -363,
      -342,  -288,  -203,   -90,    39,   170,   285,   369,   411,   409,   366,   287,   182,    62,
       -63,  -179,  -275,  -343,  -378,  -379,  -347,  -287,  -200,   -93,    29,   157,   277,   374,
       433,   440,   386,   275,   124,   -39,  -181,  -271,  -291,  -237,  -125,    20,   167,   289,
       366,   392,   374,   329,   279,   242,   228,   236,   256,   277,   291,   294,   290,   287,
       290,   300,   312,   318,   309,   281,   233,   170}; // Синхронизирующий сигнал

    uint32_t sync_len = sizeof (sync) / sizeof (int16_t);
    int32_t sync_auto_corr = get_absolute_correlation(sync, sync, sync_len, 0);

    FILE * data_file = open_file("real_rec.txt", "r"); if (data_file == NULL) exit(1);
    FILE * out_file = open_file("out.txt", "w"); if (out_file == NULL) fclose(data_file), exit(1);

    uint16_t combined_buf_len = 2*BUFFER_LEN - 1;

    int32_t * correlations = (int32_t *)malloc((BUFFER_LEN-1)*(sizeof (int16_t)));
    int16_t * actual_buf = (int16_t *)malloc(BUFFER_LEN*(sizeof (int16_t)));
    int16_t * previous_buf = (int16_t *)malloc(BUFFER_LEN*(sizeof (int16_t)));
    int16_t * combined_buf = (int16_t *)malloc(combined_buf_len*(sizeof (int16_t)));

    double best_match;
    uint16_t filling;
    uint16_t sample = 0, index;


    fill_buffer(data_file, previous_buf, BUFFER_LEN);

    while((filling = fill_buffer(data_file, actual_buf, BUFFER_LEN)) != 0)
    {
        get_combined_buf(previous_buf, actual_buf, combined_buf);
        get_absolute_correlations(combined_buf, sync, combined_buf_len-BUFFER_LEN+filling, sync_len, correlations);

        index = get_best_match_index(correlations, BUFFER_LEN-1, sync_auto_corr);
        best_match = (double)correlations[index] / sync_auto_corr;

        if (fabs(1-best_match) < sensivity_deviation)
            write_position(out_file, BUFFER_LEN*sample+index);

        sample++;
        swap_ptr(&actual_buf, &previous_buf);
    }

    fclose(data_file);
    fclose(out_file);
    free(correlations);
    free(actual_buf);
    free(previous_buf);

    printf("Succesful exit");
    return 0;
}
