#ifndef THERMOCOMP_H
#define	THERMOCOMP_H
#include "adc.h"
#include "eeprom/eeprom.h"

//-----------------------------------------------------------------------------
/*#define TABLE_LEN 	100
#define MIN_TEMP	0
#define MAX_TEMP	100
#define STEP_TEMP	1

#define TABLE_EEPROM_ADDR	0x20 */

#define FRST_PNT_THM_ADDR	0x10
#define FRST_PNT_VLT_ADDR	0x11

#define SCND_PNT_THM_ADDR	0x12
#define	SCND_PNT_VLT_ADDR	0x13
//-----------------------------------------------------------------------------
static float first_thm, second_thm;
static float first_vlt,second_vlt;
static float thm_coeff;
//-----------------------------------------------------------------------------
//void FillTermTable(void);//заполнение таблицы термокомпенсации
//void StoreTermTable(void);//сохранение таблицы термокомпенсации	в EEPROM
//void RestoreTermTable(void);//загрузка таблицы термокомпенсации из EEPROM
//long TermCompensate(long temp,long pressure);//функция табличной термокомпенсации

void SetFirstThmPoint(float temp, float	voltage); //установка первой точки термокомпенсации
void SetSecondThmPoint(float temp, float voltage); //установка второй точки термокомпенсации

void SetCompensate(unsigned char point);//установка точек компенсации
void RestoreCompensate(void);//восстановление термокомпенсационных коэфициентов после включения

float Compensate(float voltage,float temp);//функция компенсации 



//-----------------------------------------------------------------------------
//static long  volatile TermTable[100];//таблица термокомпенсации в ОЗУ
/*структура таблицы*/
//TermTable[0] ->coeff 20 C
//TermTable[1] ->coeff 21 C
//TermTable[2] ->coeff 22 C
//..........................
//..........................
//------------------------------------------------------------------------------
//static char table_counter=0;
#endif