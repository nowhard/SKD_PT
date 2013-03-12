#ifndef DAC_H
#define DAC_H
#include <ADuC845.h>
#include "preferences.h"
#include "pt/pt.h"
//------------------------------------------------------------
#define REF_VOLTAGE 2.5
#define DAC_MAX_CURRENT 15 //максимальный ток на уровне 15 мА
//------------------------------------------------------------
void DAC_Init(void);//инициализация ЦАП
void DAC_Set_Value(unsigned int val); //установить напряжение на ЦАП
PT_THREAD(DAC_Process(struct pt *pt));
//------------------------------------------------------------


#endif