#include "led.h"
//---------------------------------------------------------

unsigned char brightness;
unsigned char led_buf[8];
//---------------------------------------------------------
void InitLED(void)
{
	brightness=0x5;
	WriteDataSPI(0xC01); //shutdown no
	//WriteDataSPI(0x900); //decode no
	WriteDataSPI(0x9FF); //decode all
	WriteDataSPI(0xF00); //test no
	WriteDataSPI(0xA00+brightness); //intensity
	WriteDataSPI(0xB05); //scan limit 5	 	
}
//---------------------------------------------------------
void WriteDataSPI(unsigned int spi_data)
{
	unsigned char i;
	CS=0;

	for(i=0;i<16;i++)
	{
		WR_DATA=spi_data &(0x8000>>i); //-->> write bit
		delay(1);	
		CLK=1;	 //clock
		delay(1);	
		CLK=0;
	}	
	delay(1);
	CS=1;	 //write in display
	delay(1);
	CS=0;
}
//---------------------------------------------------------
void PrintLED(unsigned char bcd_buf[], unsigned char n_led)
{
	unsigned char i;

	 for(i=n_led;i>0;i--)
	 {
	 	WriteDataSPI(0x100*i+bcd_buf[n_led-i]); 	
	 }
}
//---------------------------------------------------------
/*void ToBCD(unsigned long int num,unsigned char bcd_buf[])
{
	unsigned char i=0;
	for(i=0;i<8;i++)
		 bcd_buf[i]=0;
	i=0;
	while(num>0)
	{	
		bcd_buf[i]=num%10;
		num=num/10;
		i++;
	}
	#pragma asm
		dec bcd_buf?346;
		dec bcd_buf?346+01H;
		dec bcd_buf?346+02h;
		dec bcd_buf?346+03h;
		dec bcd_buf?346+04h;
;//---------------------------------
LOOP1:
		inc bcd_buf?346+04h;
		clr C

		mov A,num?345;
		subb A,#LOW(10000)
		mov	 num?345,A

		mov A,num?345+01h;
		subb A,#HIGH(10000)
		mov	 num?345+01h,A

		jnc LOOP1
;//---------------------------------
LOOP2:
		inc bcd_buf?346+03h;
		clr C

		mov A,num?345;
		subb A,#LOW(1000)
		mov	 num?345,A

		mov A,num?345+01h;
		subb A,#HIGH(1000)
		mov	 num?345+01h,A
																			   
		jnc LOOP2
;//---------------------------------
LOOP3:
		inc bcd_buf?346+02h;
		clr C

		mov A,num?345;
		subb A,#LOW(100)
		mov	 num?345,A

		mov A,num?345+01h;
		subb A,#HIGH(100)
		mov	 num?345+01h,A

		jnc LOOP3
;//---------------------------------
LOOP4:
		inc bcd_buf?346+01h;
		clr C

		mov A,num?345;
		subb A,#LOW(10)
		mov	 num?345,A

		mov A,num?345+01h;
		subb A,#HIGH(10)
		mov	 num?345+01h,A

		jnc LOOP4
;//---------------------------------
		mov  bcd_buf?346,A ;//? ????? ? ???????? ????????? ????????? ??????

	#pragma endasm	
}	*/
//---------------------------------------------------------
void PrintLEDInteger(unsigned long int num)
{
	ToBCD(num,&led_buf);
	PrintLED(&led_buf,N_LED);
}
//---------------------------------------------------------
void SetBrightness(unsigned char brghtnss)
{
	WriteDataSPI(0xA00+brghtnss); //intensity
}
//---------------------------------------------------------
void delay(unsigned int time)
{
	while(time)
		time--;
}