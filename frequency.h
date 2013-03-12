#ifndef FREQUENCY_H
#define FREQUENCY_H
#include "rtos/core.h"
#include "timer0.h"
#include "timer2.h"
#include "proto_uso/channels.h"
#include "preferences.h"
#include "watchdog.h"
//----------------------------------------------------
void Frequency_Init(void);//инициализация частотных каналов
unsigned char Frequency_Measure_Process(void);//циклический процесс измерения частоты
unsigned char Sec_Task(void);//секундная задача для определения нулевой частоты
void INT0_ISR(void);//обработчик внешнего прерывания 0
void INT1_ISR(void);//обработчик внешнего прерывания 1
void INT2_ISR(void);//обработчик внешнего прерывания 2-использует внешний вход таймера T0
void Timer2_ISR(void);//обработчик прерывания счетного таймера частоты
//----------------------------------------------------
extern struct Channel xdata channels[CHANNEL_NUMBER];
//----------------------------------------------------
#endif