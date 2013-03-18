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


extern volatile unsigned char  SHOW_VOLTAGE;
sbit BUTTON1=P3^2;
sbit BUTTON2=P3^3;
volatile struct  pt data pt_display,pt_led,pt_key,pt_blink;

extern  struct SKD xdata skd ;
extern  struct pt pt_proto;

//-----------------------------------------
static PT_THREAD(Display_Out_Process(struct pt *pt));
//------------------------------------------------
volatile struct  pt ptest1,ptest2,ptest3,ptest4,ptest5,ptest6,ptest7,ptest8;
PT_THREAD(PTEST1(struct pt *pt))
{
   PT_BEGIN(pt);

  while(1) {
  PT_YIELD(pt);
  PT_YIELD(pt);
  PT_YIELD(pt);
  PT_YIELD(pt);
  PT_YIELD(pt);
  PT_YIELD(pt);
  PT_YIELD(pt);
  PT_YIELD(pt);
  }
  PT_END(pt);		
}
PT_THREAD(PTEST2(struct pt *pt))
{
   PT_BEGIN(pt);

  while(1) {
  PT_YIELD(pt);
  PT_YIELD(pt);
  PT_YIELD(pt);
  PT_YIELD(pt);
  PT_YIELD(pt);
  PT_YIELD(pt);
  PT_YIELD(pt);
  PT_YIELD(pt);
  }
  PT_END(pt);		
}
PT_THREAD(PTEST3(struct pt *pt))
{
   PT_BEGIN(pt);

  while(1) {
  PT_YIELD(pt);
  PT_YIELD(pt);
  PT_YIELD(pt);
  PT_YIELD(pt);
  PT_YIELD(pt);
  PT_YIELD(pt);
  PT_YIELD(pt);
  PT_YIELD(pt);
  }
  PT_END(pt);		
}
PT_THREAD(PTEST4(struct pt *pt))
{
   PT_BEGIN(pt);

  while(1) {
  PT_YIELD(pt);
  PT_YIELD(pt);
  PT_YIELD(pt);
  PT_YIELD(pt);
  PT_YIELD(pt);
  PT_YIELD(pt);
  PT_YIELD(pt);
  PT_YIELD(pt);
  }
  PT_END(pt);		
}
PT_THREAD(PTEST5(struct pt *pt))
{
   PT_BEGIN(pt);

  while(1) {
  PT_YIELD(pt);
  PT_YIELD(pt);
  PT_YIELD(pt);
  PT_YIELD(pt);
  PT_YIELD(pt);
  PT_YIELD(pt);
  PT_YIELD(pt);
  PT_YIELD(pt);
  }
  PT_END(pt);		
}
PT_THREAD(PTEST6(struct pt *pt))
{
   PT_BEGIN(pt);

  while(1) {
  PT_YIELD(pt);
  PT_YIELD(pt);
  PT_YIELD(pt);
  PT_YIELD(pt);
  PT_YIELD(pt);
  PT_YIELD(pt);
  PT_YIELD(pt);
  PT_YIELD(pt);
  }
  PT_END(pt);		
}
PT_THREAD(PTEST7(struct pt *pt))
{
   PT_BEGIN(pt);

  while(1) {
  PT_YIELD(pt);
  PT_YIELD(pt);
  PT_YIELD(pt);
  PT_YIELD(pt);
  PT_YIELD(pt);
  PT_YIELD(pt);
  PT_YIELD(pt);
  PT_YIELD(pt);
  }
  PT_END(pt);		
}
PT_THREAD(PTEST8(struct pt *pt))
{
   PT_BEGIN(pt);

  while(1) {
  PT_YIELD(pt);
  PT_YIELD(pt);
  PT_YIELD(pt);
  PT_YIELD(pt);
  PT_YIELD(pt);
  PT_YIELD(pt);
  PT_YIELD(pt);
  PT_YIELD(pt);
  }
  PT_END(pt);		
}

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

//	WDT_Init(WDT_250);//включить сторожевой таймер



	EA=1;
	PT_INIT(&pt_display);
	PT_INIT(&pt_led);
	PT_INIT(&pt_key);
	PT_INIT(&pt_blink);

	//----------------
	 PT_INIT(&ptest1);
	 PT_INIT(&ptest2);
	 PT_INIT(&ptest3);
	 PT_INIT(&ptest4);
	 PT_INIT(&ptest5);
	 PT_INIT(&ptest6);
	 PT_INIT(&ptest7);
	 PT_INIT(&ptest8);
	//----------------

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
		LED_Process(&pt_led);
	PTEST1(&ptest1);
		Display_Out_Process(&pt_display);






PTEST6(&ptest6);
PTEST7(&ptest7);
PTEST8(&ptest8);

		Keyboard_Process(&pt_key);
  PTEST2(&ptest2);
		ProtoProcess(&pt_proto);
  PTEST3(&ptest3);
		LED_BlinkTask(&pt_blink);
  PTEST4(&ptest4);
		if(SHOW_VOLTAGE)
		{
			BUTTON2^=1;	
		}
  PTEST5(&ptest5);
	//	WDT_Clear();
	}
}
//-----------------------------------------------------------------------------
 void Timer1_Interrupt(void) interrupt 3  //using 1
{
	EA=0;
	pt_display.pt_time++;
	pt_led.pt_time++;
	pt_key.pt_time++;
	pt_blink.pt_time++;
	pt_proto.pt_time++;

	TH1	= TH1_VAL; ///1000 Hz; 
	TL1 = TL1_VAL;//
	EA=1;
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
	  	val=GetVoltage();
		LED_Out_Float(INDICATOR_1,val);
	}
	PT_RESTART(pt);
   }
   PT_END(pt);
 }
 //--------------------------
