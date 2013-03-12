#include "pwm.h"
//----------------------------------------------------------------
void PWM_Init(unsigned char mode,unsigned char clock_divider,unsigned char clock_source)//инициализация шим
{
	 PWMCON=(mode|clock_divider|clock_source);
	 return;
}
//----------------------------------------------------------------
void PWM_Set_Single(unsigned int period,unsigned int length)//установка периода и длительности импульса
{
	   PWM1L=period&0xFF;
	   PWM1H=(period>>8)&0xFF;

	   PWM0L=length&0xFF;
	   PWM0H=(length>>8)&0xFF;

	   return;
}	
//----------------------------------------------------------------
void PWM_Set_8_Bit_1_Channel(unsigned char period,unsigned char length)//8битный шим, канал 1
{
	  PWM1L=period;
	  PWM0L=length;
	  return;
}
//----------------------------------------------------------------
void PWM_Set_8_Bit_2_Channel(unsigned char period,unsigned char length)//8битный шим, канал 2
{
	  PWM1H=period;
	  PWM0H=length;
	  return;
}