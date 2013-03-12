#ifndef SKD_H
#define SKD_H

#include "eeprom/eeprom.h"
#include "preferences.h"

#define SENSOR_QUEUE_LEN 8

#define CALIBR_LOW   0.0
#define CALIBR_HIGH 35.0

#define DIAP_LOW   0.0
#define DIAP_HIGH 40.0

#define INDICATE_TIME	50

//----------------структура настроек SKD
struct SKD
{
	union  
	{
		struct
		{

		   long adc_calibr_low;//нижнее значение ацп при калибровке
		   float calibr_low;//нижнее калибровочное значение
		   
		   long adc_calibr_high;//верхнее значение ацп при калибровке
		   float calibr_high;//верхнее калибровочное значение
		  
		   float diap_low;//нижняя граница диапазона
		   float diap_high;//верхняя граница диапазона
		   
		   unsigned char adc_diap;//диапазон вх. напряжения ацп
		   unsigned char dev_id[5];//id в формате xx.yyy
		   unsigned int indicate_time;//время смены индикации, ms


		}SKD_Settings;
	
		unsigned long serialize[8];//сериализованная часть структуры
	} SKD_Set;

   // long brightness_channel;   //канал ацп яркости

	union  //буфер усреднения для  канала
	{
				 long ADC_LONG	;
				 char ADC_CHAR[4];
	} ADC_SENSOR_UN[SENSOR_QUEUE_LEN];

	unsigned char adc_sensor_queue_counter;//счетчик кругового буфера сенсора ацп


	long line_sensor;
	unsigned char brightness;
	

};

//--------------------------------------
void SKDInit(void);//инициализация структуры каналов
void Store_Channels_Data(void);//Сохраним данные каналов в ППЗУ
void Restore_Channels_Data(void);//Восстановим данные каналов из ППЗУ
//--------------------------------------

#endif