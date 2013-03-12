#include <ADuC847.h>
//#include "ToBCD.h"
#define N_LED 4
//----------------------------------------------------------
sbit CS=P0^5;
sbit WR_DATA=P3^7;
sbit CLK=P3^6;
//----------------------------------------------------------


//----------------------------------------------------------
void InitLED(void);
void WriteDataSPI(unsigned int spi_data);
void PrintLED(unsigned char bcd_buf[], unsigned char n_led);
void PrintLEDInteger(unsigned long int num);
extern void ToBCD(unsigned  int num,unsigned char bcd_buf[]);
void delay(unsigned int time);
void SetBrightness(unsigned char brghtnss);
//----------------------------------------------------------