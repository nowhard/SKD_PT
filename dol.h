#ifndef DOL_H
#define DOL_H
#include "rtos/core.h"
#include "proto_uso/channels.h"
#include "preferences.h"
#include "watchdog.h"
//----------------------------------------------------
void Dol_Init(void);//инициализация частотных каналов
//unsigned char Dol_Measure_Process(void);//циклический процесс измерения частоты
unsigned char Sec_Task(void);//секундная задача для определения нулевой частоты
void INT0_ISR(void);//обработчик внешнего прерывания 0
//----------------------------------------------------
extern struct Channel xdata channels[CHANNEL_NUMBER];
//----------------------------------------------------
#endif