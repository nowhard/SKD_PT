#include "timer1.h"

//---------------------------------------------------------
void Timer1_Initialize(void) //initialize delay timer1
{
	TMOD &= 0xF; // 0000 1111
	TMOD |= 0x10; //16bit 

	TH1	= TH1_VAL; ///1000 Hz; 
	TL1 = TL1_VAL;//
	

	ET1=1;
	TR1=1;
	return;
}
