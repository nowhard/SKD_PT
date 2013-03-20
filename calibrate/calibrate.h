#ifndef CALIBRATE_H
#define CALIBRATE_H
#include "eeprom/eeprom.h"
#include "adc.h"
#include "skd.h"
//функции калибровки по двум точкам
//------------------------------------------------------------

void Calibrate(unsigned char point); //общая функция калибровки
float GetCalibrateVal(long ADC_Code);   //преобразование значения АЦП в значение плотности в кг\м^3
//------------------------------------------------------------
#endif