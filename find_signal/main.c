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
#define MIN_GAIN 50



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


/*!
Вторую половину массива переносим в начало, а на её место записываем новые значения
\param[in] data_file Указатель на файл с данными
\param[out] buffer Буфер для загрузки данных
\param[in] half_buffer_len Половина от длинны буфера
\return Количество записанных элементов в буффер
*/

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
            printf("Warning. Line number %d.\r\n", line);
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
      170, 233, 281, 309, 318, 312, 300, 290, 287, 290, 294, 291, 277, 256, 236, 228, 242, 279,
        329, 374, 392, 366, 289, 167, 20, -125, -237, -291, -271, -181, -39, 124, 275, 386, 440,
        433, 374, 277, 157, 29, -93, -200, -287, -347, -379, -378, -343, -275, -179, -63, 62, 182,
        287, 366, 409, 411, 369, 285, 170, 39, -90, -203, -288, -342, -363, -353, -319, -271, -222,
        -190, -190, -226, -295, -375, -438, -455, -406, -288, -119, 64, 219, 305, 301, 208, 56, -109,
        -234, -281, -234, -109, 56, 209, 301, 305, 218, 64, -115, -276, -384, -424, -406, -351, -290,
        -246, -228, -235, -256, -278, -291, -294, -290, -287, -290, -300, -312, -318, -309, -281, -233,
        -170
    }; // Синхронизирующий сигнал

    uint32_t sync_len = sizeof (sync) / sizeof (int16_t);
    int32_t sync_auto_corr = get_absolute_correlation(sync, sync, sync_len, 0) / sync_len;


    FILE * data_file = open_file("real_rec.txt", "r");
    if (data_file == NULL) exit(1);

    FILE * out_file = open_file("out.txt", "w");
    if (out_file == NULL) fclose(data_file), exit(1);


    int32_t * correlations = (int32_t *)malloc(BUFFER_LEN*(sizeof (int32_t)));
    int16_t * buffer = (int16_t *)malloc(2*BUFFER_LEN*(sizeof (int16_t)));

    uint32_t filling;
    uint32_t index;
    uint32_t sample = 0;

    prepare_buffer(data_file, buffer, BUFFER_LEN); // Заполняем первую часть массива данными, чтобы случайные числа не совпали с шаблоном

    while(feof(data_file) == 0)
    {
        filling = prepare_buffer(data_file, buffer, BUFFER_LEN); // вторую половину массива переносим в начало, и записываем туда новые значения
        get_absolute_correlations(buffer, sync, 2*BUFFER_LEN-1, sync_len, correlations); //находим корреляции
        index = max_index(correlations, --filling); // индекс элемента
        if (correlations[index] / sync_auto_corr > MIN_GAIN)
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
