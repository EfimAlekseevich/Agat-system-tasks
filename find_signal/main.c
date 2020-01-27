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
#define MAX_DEVIATION 0.0001



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
    while (i < buffer_len && feof(data_file) == 0)  // пока не дошли до конца буффера или файла
            fscanf(data_file, "%hd", &buffer[i++]); // считывает чило типа int16_t в буффер
    return i;
}


uint32_t prepare_buffer(FILE * data_file, int16_t * buffer, uint32_t half_buffer_len)
{
    uint32_t i;
    for (i=0; i < half_buffer_len; i++)
        buffer[i] = buffer[i+half_buffer_len];
    return fill_buffer(data_file, buffer+half_buffer_len, half_buffer_len);
}


/*!
Преобразует номер отсчёта в соответствуещее ему время
\param[in] position номер отсчёта
\return Момент времени в секндах
*/

double position_to_time(uint32_t position)
{
    return (double)position / SAMPLING_FREQUENCY;
}


/*!
Записывает в файл время отсчёта в секундах до 6 знака после точки
\param[out] out_file Файл в который записывается момент времени
\param[in] position Отсчёт, который необходимо преобразовать занести в файл
\return Ничего
*/

void write_position(FILE * out_file, uint32_t position)
{
    double time = position_to_time(position);
    fprintf(out_file,"%.6f\n", time);
}


double diff_normilize(int16_t value, int16_t denominator)
{
    return fabs(1.0 - (double)value/denominator);
}


/*!
Находит индекс элемента с лучшим совпадением значения с автокорреляцией синхропоследовательностью
\param[in] correlations Массив корреляций
\param[in] len Длина массива корреляций
\param[in] auto_corr Значение автокореляции синхропоследовательности
\return Индекс элемента с лучшим совпадением
*/

uint32_t search_match_index(const int16_t * correlations, uint32_t len, int16_t sync_auto_corr)
{
    double diff, min_diff = MAX_DEVIATION;
    uint32_t i, index = len;

    for (i=0; i<len; i++)
    {
        diff = diff_normilize(correlations[i], sync_auto_corr);
        if (diff < min_diff)
        {
            min_diff = diff;
            index = i;
        }
    }
    return index;
}


void check_result(uint32_t sync_len)
{
    FILE * data_file = open_file("out.txt", "r");
    if (data_file == NULL) exit(1);

    uint32_t line = 0;
    float previous_time, actual_time, diff, sync_time = (float)sync_len / SAMPLING_FREQUENCY;
    fscanf(data_file, "%f", &previous_time);


    while (fscanf(data_file, "%f", &actual_time) != EOF)
    {
        if ((actual_time - previous_time) < sync_time)
        {
            diff = actual_time - previous_time;
            printf("\r\n %f \r\n %f \r\n Difference %f so less then SAMPLING_PERIOD %.4f\r\n",
                   previous_time, actual_time, diff, sync_time);
            printf("Warning. Line number %d. You need reduce MAX_DEVIATION(%lf).\r\n", line, MAX_DEVIATION);
            break;
        }
        previous_time = actual_time;
        line++;
    }
    fclose(data_file);
}

/*!
Начальная основная функция
\return 0 при успешном завершении
*/

int main()
{

    int16_t sync[] = {
      -170, -233, -281, -309, -318, -312, -300, -290, -287, -290, -294, -291, -278, -256, -235,
      -228, -246, -290, -351, -406, -424, -384, -276, -115, 64, 218, 305, 301, 209, 56, -109, -234,
      -281, -234, -109, 56, 208, 301, 305, 219, 64, -119, -288, -406, -455, -438, -375, -295, -226,
      -190, -190, -222, -271, -319, -353, -363, -342, -288, -203, -90, 39, 170, 285, 369, 411, 409,
       366, 287, 182, 62, -63, -179, -275, -343, -378, -379, -347, -287, -200, -93, 29, 157, 277,
       374, 433, 440, 386, 275, 124, -39, -181, -271, -291, -237, -125, 20, 167, 289, 366, 392, 374,
       329, 279, 242, 228, 236, 256, 277, 291, 294, 290, 287, 290, 300, 312, 318, 309, 281, 233, 170
    }; // Синхронизирующий сигнал

    uint32_t sync_len = sizeof (sync) / sizeof (int16_t);
    int16_t sync_auto_corr = get_absolute_correlation(sync, sync, sync_len, 0) / sync_len;

    FILE * data_file = open_file("real_rec.txt", "r");
    if (data_file == NULL) exit(1);

    FILE * out_file = open_file("out.txt", "w");
    if (out_file == NULL) fclose(data_file), exit(1);


    int16_t * correlations = (int16_t *)malloc(BUFFER_LEN*(sizeof (int16_t)));
    int16_t * buffer = (int16_t *)malloc(2*BUFFER_LEN*(sizeof (int16_t)));

    uint32_t filling;
    uint32_t index;
    uint32_t sample = 0;

    prepare_buffer(data_file, buffer, BUFFER_LEN); // Заполняем первую часть массива данными, чтобы случайные числа не совпали с шаблоном

    while(feof(data_file) == 0)
    {
        filling = prepare_buffer(data_file, buffer, BUFFER_LEN); // вторую половину массива переносим в начала, и записываем туда новые значения
        get_absolute_correlations(buffer, sync, 2*BUFFER_LEN-1, sync_len, correlations); //находим корреляции

        index = search_match_index(correlations, --filling, sync_auto_corr); // индекс элемента
        if (index != filling)
            write_position(out_file, BUFFER_LEN*sample+index); // Запись в файл отсчёта в секундах

        sample++; // Переход к следующему семплу.
    }
    // Закрываем файлы
    fclose(data_file);
    fclose(out_file);
    // Освобождаем память
    free(buffer);
    free(correlations);
    // Выводим инф-цию об успешном завершении программы
    printf("Succesful writing in out.txt\r\n");
    check_result(sync_len); // Проверка результата
    printf("\r\nGOOD\r\n");
    return 0;
}
