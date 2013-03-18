#ifndef LED_PROCESS_H
#define LED_PROCESS_H
#include <ADuC845.h>
#include <math.h>
#include "skd.h"
#include "pt/pt.h"

#define INDICATOR_NUM	1

sbit CS_1=P2^3;
//sbit CS_2=P2^4;
//sbit CS_3=P2^5;
//sbit CS_4=P2^6;
			   //каналы spi bit-bang
sbit WR_DATA=P3^7;
sbit CLK=P3^6;

//#define LED_DIG_NUM	5

#define INDICATOR_1	0
#define INDICATOR_2	1
#define INDICATOR_3	2
#define INDICATOR_4	3

//-------------------------------------------
PT_THREAD(LED_Process(struct pt *pt));
//void LED_Out_Buf(unsigned int out_buf[], unsigned char out_buf_len);//вывод буфера в SPI bit-bang

//void LED_Out_Integer(unsigned long num);//вывод целочисленного значения
void LED_Out_Float(unsigned char display_num,float num);//вывод значения с плавающей точкой

void LED_Set_Digit_Num(unsigned char display_num,unsigned char num);//установить количество отображаемых чисел
void LED_Set_Brightness(unsigned char display_num,unsigned char bright);//установить яркость индикаторов
void LED_Set_Decode(unsigned char display_num,unsigned char decode);//установить яркость индикаторов
void LED_SetPoint(unsigned char display_num,unsigned char pnt);//на какое знакоместо ставим точку, начиная слева
void LED_SetBlink(unsigned char display_num,unsigned char blink);//мигание
PT_THREAD(LED_BlinkTask(struct pt *pt));
 void SetBarGraph(unsigned char val,unsigned char point_1,unsigned char point_2, unsigned char invert);
extern struct SKD xdata skd;  //структура СКД

#endif