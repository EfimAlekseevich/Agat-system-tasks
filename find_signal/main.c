/*!
\file Основной файл
Данный файл содержит в себе решение задачи (читайте ТЗ).
*/


#include <stdio.h>
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


/*!
Вычисляет нормированный коэффициет коррляции
\param[in] buffer Указатель на буффер с данными
\param[in] sync Указатель на синхросигнал
\param[in] len Длинна буфера
\param[in] shift Сдвиг, необходим если len(sync) < len(buffer)
\param[in] sqrt_auto_corr_sync Значение корня из автокорреляции синхросигнала
\return double Значение корреляции
\throw len - длинна sync, len(sync) <= len(buffer)
*/

double get_coef_corr(const int16_t * buffer, const int16_t * sync, uint32_t len, uint32_t shift, double sqrt_auto_corr_sync)
{
    double coef = (double)get_absolute_correlation(buffer, sync, len, shift) /
                  sqrt(get_absolute_correlation(buffer, buffer, len, shift)) / sqrt_auto_corr_sync;
    return coef;
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


void delete_last_position(FILE * out_file)
{
    fseek(out_file, -15, SEEK_END);
}


/*! ТЗ:
 * В файле real_rec.txt запись реального эфирного сигнала.
 * Нужно найти в нем все позиции начала кадровой синхронизации (шаблон поиска в файле pattern.c)
 * и выдать результат в текстовый файл (одна строка - одна позиция).
 * Позицию выводить в формате времени с точностью до десятых долей миллисекунд.
 * Частота дискретизации файла - 24 кГц, 16 бит на отсчет.
 * Обработка ВЕДЕТСЯ БУФЕРАМИ ПО 5 МС (не понимаю почему нужно именно так???). (ГРУСТНЫЙ СМАЙЛИК)
 * 1 / 24 000 = 41,(6) мкс.
 * 5 мс >>---> 5 мс / 41,(6) мкс ~ 120 отсчётов.
*/


/*!
Начальная основная функция
\return 0 при успешном завершении
*/

int main()
{
    const float sensivity_deviation  = 0.2;  // Максимально допустимое отклонение.

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
    double auto_corr = get_absolute_correlation(sync, sync, sync_len, 0);

    FILE * data_file = open_file("real_rec.txt", "r"); if (data_file == NULL) exit(1);
    FILE * out_file = open_file("out.txt", "w"); if (out_file == NULL) fclose(data_file), exit(1);

    int16_t * buffer = (int16_t *)malloc(BUFFER_LEN*(sizeof (int16_t)));
    uint32_t filling;
    uint32_t shift;
    uint32_t sample = 0, point;
    double corr;




    while(filling = fill_buffer(data_file, buffer, BUFFER_LEN))
    {
        point = 0;
        uint32_t shifts = BUFFER_LEN - sync_len + 1;

        for (shift=0; shift<shifts; shift++)
        {
            corr = (double)get_absolute_correlation(buffer, sync, sync_len, shift) / auto_corr;
            if (corr > 1-sensivity_deviation && corr < 1+sensivity_deviation)
            {
               //printf("%d       %d \n", sample, point);
               write_position(out_file, sample * BUFFER_LEN + point);
            }
            point++;
        }
        sample++;
    }

    fclose(data_file);
    fclose(out_file);
    free(buffer);
    printf("Succesful exit");
    return 0;
}
