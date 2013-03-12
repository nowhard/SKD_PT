#include "dac.h"
#include "skd.h"
#define I04_mA 	0x333
#define I20_mA	0xFFF

#define	CODE_MIN	0x0
#define CODE_MAX	100//Hz

#define DEAD_ZONE_1	-0x100
#define DEAD_ZONE_2	 0x100

extern volatile struct SKD xdata skd ;

sbit KEY_1=P2^1;//ключи направления вращения двигателя
sbit KEY_2=P2^2;
//-------------------------------------------------------------------------------
void DAC_Init(void)//инициализация ЦАП
{

	DACCON=0x3;//dac enable  Vref (AINCOM)
	return;
}
//-------------------------------------------------------------------------------
void DAC_Set_Value(unsigned int val) //?????????? ?????????? ?? ???
{
	DACH=(val>>8)&0xF;
	DACL=val&0xFF;
	return;
}
//------------------------------------------------------------------------------
PT_THREAD(DAC_Process(struct pt *pt))
 {
   
 static volatile unsigned long temp_current;
 PT_BEGIN(pt);
  while(1) 
  {
  		PT_DELAY(pt,10);

		if(skd.joystick>=DEAD_ZONE_1 && skd.joystick<=DEAD_ZONE_2)//мертвая зона джойстика
		{
			 DAC_Set_Value(I04_mA);
			 KEY_1=0;
			 KEY_2=0;
			 PT_RESTART(pt);
		}
		else//иначе включаем ключи
		{
			if(skd.joystick<DEAD_ZONE_1)	//вращаем назад
			{
				KEY_1=1;
				KEY_2=0;
			}
			else  //вперед
			{
			   	KEY_1=0;
				KEY_2=1;
			}
		}

		temp_current=skd.joystick;
		temp_current=((temp_current-((unsigned long)CODE_MIN<<8))*((unsigned long)I20_mA-(unsigned long)I04_mA))/(((unsigned long)CODE_MAX-(unsigned long)CODE_MIN)<<8)+(unsigned long)I04_mA;

	   	if(temp_current>=I20_mA)
		{
			temp_current=I20_mA;
		}
	
		if(temp_current<=I04_mA)
		{
			temp_current=I04_mA;	
		}
		
		DAC_Set_Value((unsigned int)temp_current);
  }

  PT_END(pt);
 }