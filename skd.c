#include "skd.h"

//-----------------------------------
volatile struct SKD xdata skd ;
//-----------------------------------
void ChannelsInit(void) //using 0//инициализация структуры каналов
{	
//	Restore_Channels_Data();
	//добавить приведение даннных к значению по умолчанию в случае ошибочных значений
	skd.SKD_Set.SKD_Settings.calibr_low=CALIBR_LOW;
    skd.SKD_Set.SKD_Settings.calibr_high=CALIBR_HIGH;//задаем явно

	skd.SKD_Set.SKD_Settings.diap_low=DIAP_LOW;
    skd.SKD_Set.SKD_Settings.diap_high=DIAP_HIGH;//задаем явно

	skd.SKD_Set.SKD_Settings.indicate_time=INDICATE_TIME;
    skd.SKD_Set.SKD_Settings.adc_diap=0x7;
	return;
}
//-----------------------------------
void Store_Channels_Data(void) //using 0//Сохраним данные каналов в ППЗУ
{

		EEPROM_Write(&skd.SKD_Set.serialize,8,SETTINGS_ADDR);

	return;
}
//-----------------------------------
void Restore_Channels_Data(void) //using 0//Восстановим данные каналов из ППЗУ
{

		EEPROM_Read(&skd.SKD_Set.serialize,8,SETTINGS_ADDR);

	return;
}
