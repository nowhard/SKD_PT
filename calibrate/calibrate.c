#include "calibrate.h"
extern  struct SKD xdata skd ;
//------------------------------------------------------------
void Calibrate(unsigned char point) //общая функция калибровки
{
	switch(point)
	{
		case 1:
		{
			skd.SKD_Set.SKD_Settings.adc_calibr_low=skd.line_sensor;	
			EEPROM_Write(&skd.SKD_Set.SKD_Settings.adc_calibr_low,1,SETTINGS_ADDR);
			EEPROM_Write(&skd.SKD_Set.SKD_Settings.calibr_low,1,SETTINGS_ADDR+1);
		}
		break;

		case 2:
		{
			skd.SKD_Set.SKD_Settings.adc_calibr_high=skd.line_sensor;
			EEPROM_Write(&skd.SKD_Set.SKD_Settings.adc_calibr_high,1,SETTINGS_ADDR+2);
			EEPROM_Write(&skd.SKD_Set.SKD_Settings.calibr_high,1,SETTINGS_ADDR+3);
		}
		break;

		default:
		{
			return;
		}
	}
}
//------------------------------------------------------------
float GetCalibrateVal(long ADC_Code)   //преобразование значения АЦП в калиброванное значение	 ??? проверить && debug
{
  	float   val=0;//плотность

	val=(((float)((float)(ADC_Code-skd.SKD_Set.SKD_Settings.adc_calibr_low)/(float)(skd.SKD_Set.SKD_Settings.adc_calibr_high-skd.SKD_Set.SKD_Settings.adc_calibr_low)))*(float)(skd.SKD_Set.SKD_Settings.calibr_high-skd.SKD_Set.SKD_Settings.calibr_low))+skd.SKD_Set.SKD_Settings.calibr_low; //формула калибровки по 2м точкам
	return val;
}
//---------------------------------------------------------------
