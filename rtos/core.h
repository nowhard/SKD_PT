#ifndef CORE_H
#define CORE_H
#include <ADuC845.h>                 // SFR declarations

#include "timer0.h"
#include "preferences.h"

#define maxHandlers 	8
#define maxMessages 	8
#define maxTimers  		4




#define ADC_MEAN_TASK		1
#define BLINK_TASK	    2
#define SEC_TASK	    3
#define CYCLE_TASK		4

#define I2C_IS_READ_MSG		5
#define I2C_IS_WRITE_MSG	6



typedef unsigned char msg_num; // тип сообытия - мне пока хватает одного байта
typedef int msg_par; // тип параметра события
typedef unsigned char (*handler)(msg_par); // описание функции-обработчика
//---------прототипы-------------------------------------------------------
void setHandler(msg_num msg, handler hnd);
//void killHandler(msg_num msg, handler hnd);
void sendMessage(msg_num msg, msg_par par);
void dispatchMessage(void);
void setTimer(msg_num msg, msg_par par, unsigned int time);
void killTimer(msg_num msg);
//void dispatchTimer(void);
void Timer1_Interrupt(void);//прерывание 1мсек

#endif