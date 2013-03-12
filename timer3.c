#include "timer3.h"
//------------------------------
void UART_Init(void) //using 0
{ 
    			//Configure UART
    T3CON = T3CON_VAL;

    T3FD = T3FD_VAL;
    SCON =0x52; //0x53;
//	ES=1;
//	TI=0;
//	RI=0;
//	PS=1; //наивысший приоритет у UART	возможно вызывает ошибки
	return;
}