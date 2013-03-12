#ifndef CHANNELS_H
#define CHANNELS_H
#include "eeprom/eeprom.h"
#include "preferences.h"
//---------------------------------------
#define CHANNEL_NUMBER	11//количество каналов
//---------------------------------------
struct Channel
{
	unsigned char number ;	  	// номер канала 
	
	union 
	{
		struct
		{
			unsigned char type;		  // тип канала
			unsigned char modific;	  // модификатор канала
			unsigned char state_byte_1;	// байт состояния канала
			unsigned char state_byte_2; 
		}set;
	
		unsigned long serialize;//сериализованная часть структуры
	} settings;
	
	unsigned long channel_data;		  // значение канала
	unsigned long channel_data_calibrate;//значение канала калиброванное

	union
	{
		struct
		{
			unsigned char calibrate;//флаг калиброванности канала
			float K;//
			float C;//y=kx+c		
		} cal;
		unsigned long serialize[3];//сериализованная часть структуры
	} calibrate;
	
} ;
//---------------------------------------
void ChannelsInit(void);//инициализация структуры каналов
void Store_Channels_Data(void);//Сохраним данные каналов в ППЗУ
void Restore_Channels_Data(void);//Восстановим данные каналов из ППЗУ

//---------------------------------------
#endif