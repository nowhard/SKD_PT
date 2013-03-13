#include <ADuC845.h>


#include "timer3.h"
#include "preferences.h"
#include "eeprom/eeprom.h"
#include "timer1.h"
#include "adc.h"
#include "watchdog.h"
#include "proto_uso/proto_uso.h"

#include "pt/pt.h"
#include "keyboard.h"

#include "led_process.h"

#include "calibrate/calibrate.h"
#include "skd.h"
#include "intrins.h"
//unsigned char code mas[8182]={0};
//unsigned char xdata mas2[512]={0};

extern volatile unsigned char  SHOW_VOLTAGE;
sbit BUTTON1=P3^2;

struct  pt pt1, pt2,pt3,pt_key,pt_dac,pt_beep,pt_blink;

extern  struct SKD xdata skd ;
extern  struct pt pt_proto;

//-----------------------------------------
static PT_THREAD(Display_Out_Process(struct pt *pt));
//---------------------------------------



void main(void) //using 0
{			   
	
	EA = 0;
		
	PLLCON&=PLLCON_VAL;//настройка частоты процессора

	if(!BUTTON1)
	{
		SHOW_VOLTAGE=1;
	}
	
	Timer1_Initialize(); //таймер шедулера 200√ц
	
	ADC_Initialize();

	ChannelsInit();
	UART_Init();

	WDT_Init(WDT_250);//включить сторожевой таймер



	EA=1;
	PT_INIT(&pt1);
	PT_INIT(&pt3);
	PT_INIT(&pt_key);
	PT_INIT(&pt_blink);

 	if(!SHOW_VOLTAGE)
	{
	   if(skd.SKD_Set.SKD_Settings.diap_high>=100.0 || skd.SKD_Set.SKD_Settings.diap_low<-100.0) //передвигаем дес€тичную точку
	   {
	   		LED_SetPoint(INDICATOR_1,2);
	   }
	   else
	   {
	   		LED_SetPoint(INDICATOR_1,3);
	   }
		LED_Set_Brightness(INDICATOR_1,0);
		//LED_Out_Float(INDICATOR_1,0.0);
	}
	else
	{
		LED_SetPoint(INDICATOR_1,4);
		LED_Set_Brightness(INDICATOR_1,0);
		LED_Out_Float(INDICATOR_1,0.0);	
	}
	Protocol_Init();
	while(1)
	{		
		LED_Process(&pt3);
		Display_Out_Process(&pt1);
		Keyboard_Process(&pt_key);
		ProtoProcess(&pt_proto);
		LED_BlinkTask(&pt_blink);
		WDT_Clear();
	}
}
//-----------------------------------------------------------------------------
 void Timer1_Interrupt(void) interrupt 3  //using 1
{
	pt1.pt_time++;
	pt3.pt_time++;
	pt_key.pt_time++;
	pt_blink.pt_time++;
	pt_proto.pt_time++;

	TH1	= TH1_VAL; ///1000 Hz; 
	TL1 = TL1_VAL;//
}
//-----------------------------------------------------------------------------
static PT_THREAD(Display_Out_Process(struct pt *pt))
 {
   static float val=0.0;
   PT_BEGIN(pt);

  while(1) {

	PT_DELAY(pt,skd.SKD_Set.SKD_Settings.indicate_time);

 	Meaning_Process();
	if(!SHOW_VOLTAGE)
	{
		   val=GetCalibrateVal(skd.line_sensor);

		   if((val<skd.SKD_Set.SKD_Settings.diap_low)||(val>skd.SKD_Set.SKD_Settings.diap_high))
		   {
		   		LED_SetBlink(INDICATOR_1,1);
				LED_Set_Brightness(INDICATOR_1,0xF);
				if(val<skd.SKD_Set.SKD_Settings.diap_low)
				{
					val=skd.SKD_Set.SKD_Settings.diap_low;
				}	

				if(val>skd.SKD_Set.SKD_Settings.diap_high)
				{
					val=skd.SKD_Set.SKD_Settings.diap_high;
				}
		   }
		   else
		   {
		   		LED_SetBlink(INDICATOR_1,0);
		   }
		   
		   LED_Out_Float(INDICATOR_1,val);
	}
	else
	{
	  	LED_Out_Float(INDICATOR_1,GetVoltage());
	}
	
   }
   PT_END(pt);
 }
 //--------------------------
