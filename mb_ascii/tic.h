#ifndef TIC_H
#define TIC_H
#include <ADuC845.h>

//-----------------------------------
//таймер, отмер€ющий секундный интервал дл€ таймаута modbus ascii
//-----------------------------------
void TIC_Init(void);//инициализаци€ таймера
void TIC_Start(void);//запуск таймера
void TIC_Stop(void);//останов таймера
//void TIC_ISR(void);
//-----------------------------------

#endif