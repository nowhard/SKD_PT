#include "timer0.h"


void Timer0_Initialize(void) //using 0
{
	TMOD &= 0xF0; // 1111 0000 очищаем
	TMOD |= 0x5; // таймер в режим счетчика 16 разрядов 

	TH0	= TH0_VAL; /// 
	TL0 = TL0_VAL;//
	

	ET0=1;
	TR0=1;
	return;
}


