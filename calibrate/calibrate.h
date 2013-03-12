#ifndef CALIBRATE_H
#define CALIBRATE_H
#include "eeprom/eeprom.h"
#include "adc.h"
#include "skd.h"
//функции калибровки по двум точкам
//------------------------------------------------------------
//static void SetFirstPoint(long ADC_Code,float val);	 //установка первой точки -воздух плотность = 0
//static void SetSecondPoint(long ADC_Code,float val); //установка второй точки -вода	  плотность = 1

//float GetFirstDensityPoint(void);
//float GetSecondDensityPoint(void);

void Calibrate(unsigned char point); //общая функция калибровки
//void RestoreCalibrate(void);		 //восстановление точек калибровки из EEPROM
float GetCalibrateVal(long ADC_Code);   //преобразование значения АЦП в значение плотности в кг\м^3

//float GetCompensedDensity(float pressure,float temp);   //преобразование значения АЦП в значение плотности в кг\м^3

//------------------------------------------------------------
#endif