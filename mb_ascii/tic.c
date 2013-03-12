#include "tic.h"

//-----------------------------------
void TIC_Init(void) //using 0//инициализация таймера
{
	  TIMECON&=0x0;
	  INTVAL=0x1;
	  IEIP2 |= 0x04;
	  return;
}
//-----------------------------------
/*void TIC_Start(void) //using 0//запуск таймера
{
	  TIMECON&=0x0;
	  INTVAL=0x1;
	 TIMECON=0x1B;//00011011 -один период, прерывание 1 секунда
	 return;	
}*/
//-----------------------------------
/*void TIC_Stop(void) //using 0//останов таймера
{
	  TIMECON&=0x0; 
	  return;	
}*/
//-----------------------------------
