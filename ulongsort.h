#ifndef ULONGSORT_H
#define ULONGSORT_H
#include "adc.h"
//#include "proto_uso/channels.h"
#include "pt/pt.h"
#include "calibrate/calibrate.h"
#include <string.h>
#include <math.h>

//#define COPY_STATE	0x1//состояние копирования массива
//#define SORT_STATE	0x2//состояние сортировки массива
//#define MID_STATE	0x3//возврат среднего значения 
//#define WAIT_STATE	0x4//состояние ожидания
//#define FIND_NEW_MEASURE	0x5//найдем массив с новыми измерениями
//void ulongsort_process(void);//сортирует массив и выдает медианное значение для усреднения
 PT_THREAD(ulongsort_process(struct pt *pt));
extern struct ADC_Channels xdata adc_channels[];

#endif