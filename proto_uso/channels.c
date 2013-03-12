#include "channels.h"


//-----------------------------------
volatile struct Channel xdata channels[CHANNEL_NUMBER] /*_at_ 0x20*/;
//-----------------------------------
void ChannelsInit(void) //using 0//инициализация структуры каналов
{
	
	Restore_Channels_Data();
	if(channels[0].settings.set.type!=0 || channels[1].settings.set.type!=0)
	{	
		channels[0].number=0;		  // номера каналов должны строго идти последовательно и начинаться с нуля
		channels[0].settings.set.type=0;
		channels[0].settings.set.modific=3;
		channels[0].settings.set.state_byte_1=0x40;
		channels[0].settings.set.state_byte_2=0x06;
		channels[0].channel_data=0;
		channels[0].channel_data_calibrate=0;
		channels[0].calibrate.cal.calibrate=0;
		// КАНАЛ 2 - фиксированый АЦП
		channels[1].number=1;
		channels[1].settings.set.type=0;
		channels[1].settings.set.modific=3;
		channels[1].settings.set.state_byte_1=0x40;
		channels[1].settings.set.state_byte_2=0x06;
		channels[1].channel_data=0;
		channels[1].channel_data_calibrate=0;
		channels[1].calibrate.cal.calibrate=0;
		// КАНАЛ 3 - фиксированый АЦП
		channels[2].number=2;		 
		channels[2].settings.set.type=0;
		channels[2].settings.set.modific=3;
		channels[2].settings.set.state_byte_1=0x40;
		channels[2].settings.set.state_byte_2=0x06;
		channels[2].channel_data=0;
		channels[2].channel_data_calibrate=0;
		channels[2].calibrate.cal.calibrate=0;
		// КАНАЛ 4 - фиксированый АЦП
		channels[3].number=3;		 
		channels[3].settings.set.type=0;
		channels[3].settings.set.modific=3;
		channels[3].settings.set.state_byte_1=0x40;
		channels[3].settings.set.state_byte_2=0x06;
		channels[3].channel_data=0;
		channels[3].channel_data_calibrate=0;
		channels[3].calibrate.cal.calibrate=0;
		// КАНАЛ 5 - фиксированый АЦП
		channels[4].number=4;		 
		channels[4].settings.set.type=0;
		channels[4].settings.set.modific=3;
		channels[4].settings.set.state_byte_1=0x40;
		channels[4].settings.set.state_byte_2=0x06;
		channels[4].channel_data=0;
		channels[4].channel_data_calibrate=0;
		channels[4].calibrate.cal.calibrate=0;
		// КАНАЛ 6 - фиксированый АЦП
		channels[5].number=5;		 
		channels[5].settings.set.type=0;
		channels[5].settings.set.modific=3;
		channels[5].settings.set.state_byte_1=0x40;
		channels[5].settings.set.state_byte_2=0x06;
		channels[5].channel_data=0;
		channels[5].channel_data_calibrate=0;
		channels[5].calibrate.cal.calibrate=0;
		// КАНАЛ 7 - фиксированый АЦП
		channels[6].number=6;		 
		channels[6].settings.set.type=0;
		channels[6].settings.set.modific=3;
		channels[6].settings.set.state_byte_1=0x40;
		channels[6].settings.set.state_byte_2=0x06;
		channels[6].channel_data=0;
		channels[6].channel_data_calibrate=0;
		channels[6].calibrate.cal.calibrate=0;
		// КАНАЛ 8 - фиксированый АЦП
		channels[7].number=7;		 
		channels[7].settings.set.type=0;
		channels[7].settings.set.modific=3;
		channels[7].settings.set.state_byte_1=0x40;
		channels[7].settings.set.state_byte_2=0x06;
		channels[7].channel_data=0;
		channels[7].channel_data_calibrate=0;
		channels[7].calibrate.cal.calibrate=0;
		// КАНАЛ 9 - Логический вход
		channels[8].number=8;		 
		channels[8].settings.set.type=2;
		channels[8].settings.set.modific=0;
		channels[8].settings.set.state_byte_1=0x40;
		channels[8].settings.set.state_byte_2=0x0A;
		channels[8].channel_data=0x0;
		channels[8].calibrate.cal.calibrate=0;
		// КАНАЛ 10 - Логический выход
		channels[9].number=9;		 
		channels[9].settings.set.type=2;
		channels[9].settings.set.modific=0;
		channels[9].settings.set.state_byte_1=0x40;
		channels[9].settings.set.state_byte_2=0x0A;
		channels[9].channel_data=0x0;
		channels[9].calibrate.cal.calibrate=0;
		// КАНАЛ 11 - Логический выход
		channels[10].number=10;		 
		channels[10].settings.set.type=2;
		channels[10].settings.set.modific=0;
		channels[10].settings.set.state_byte_1=0x40;
		channels[10].settings.set.state_byte_2=0x0A;
		channels[10].channel_data=0x0;
		channels[10].calibrate.cal.calibrate=0;
/*		// КАНАЛ 9 - Частотомер 0-256 Гц
		channels[8].number=8;		 
		channels[8].settings.set.type=2;
		channels[8].settings.set.modific=1;
		channels[8].settings.set.state_byte_1=0x40;
		channels[8].settings.set.state_byte_2=0x0A;
		channels[8].channel_data=0x0;
		channels[8].calibrate.cal.calibrate=0;
		// КАНАЛ 10 - Частотомер 0-256 Гц
		channels[9].number=9;		 
		channels[9].settings.set.type=2;
		channels[9].settings.set.modific=1;
		channels[9].settings.set.state_byte_1=0x40;
		channels[9].settings.set.state_byte_2=0x0A;
		channels[9].channel_data=0x0;
		channels[9].calibrate.cal.calibrate=0;
		// КАНАЛ 11 - Частотомер 0-256 Гц
		channels[10].number=10;		 
		channels[10].settings.set.type=2;
		channels[10].settings.set.modific=1;
		channels[10].settings.set.state_byte_1=0x40;
		channels[10].settings.set.state_byte_2=0x0A;
		channels[10].channel_data=0x1;
		channels[10].calibrate.cal.calibrate=0;
		// КАНАЛ 12 - ДОЛ I2C
		channels[11].number=11;		 
		channels[11].settings.set.type=1;
		channels[11].settings.set.modific=0;
		channels[11].settings.set.state_byte_1=0x40;
		channels[11].settings.set.state_byte_2=0x0A;
		channels[11].channel_data=0x0;
		channels[11].calibrate.cal.calibrate=0;
				// КАНАЛ 13 частота
		channels[12].number=12;		 
		channels[12].settings.set.type=2;
		channels[12].settings.set.modific=0;
		channels[12].settings.set.state_byte_1=0x40;
		channels[12].settings.set.state_byte_2=0x0A;
		channels[12].channel_data=0x0;

				// КАНАЛ 14 частота средняя
		channels[13].number=13;		 
		channels[13].settings.set.type=2;
		channels[13].settings.set.modific=0;
		channels[13].settings.set.state_byte_1=0x40;
		channels[13].settings.set.state_byte_2=0x0A;
		channels[13].channel_data=0x0;		*/
	}
//	channels[11].channel_data=0x80008000;
	return;
}
//-----------------------------------
void Store_Channels_Data(void) //using 0//Сохраним данные каналов в ППЗУ
{
	unsigned char i=0;
	for(i=0;i<CHANNEL_NUMBER;i++)
	{
		EEPROM_Write(&channels[i].settings.serialize,1,ADC_SETTINGS_ADDR+i);
	}
	return;
}
//-----------------------------------
void Restore_Channels_Data(void) //using 0//Восстановим данные каналов из ППЗУ
{
	unsigned char i=0;
	for(i=0;i<CHANNEL_NUMBER;i++)
	{
		EEPROM_Read(&channels[i].settings.serialize,1,ADC_SETTINGS_ADDR+i);
	}
	return;
}
