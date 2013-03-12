#include <ADuC845.h>


#include "timer3.h"
#include "preferences.h"
#include "eeprom/eeprom.h"
#include "rtos/timer1.h"
#include "adc.h"
#include "dac.h"
#include "watchdog.h"

#include "pt/pt.h"
#include "keyboard.h"
#include "ulongsort.h"
#include "led_process.h"

#include "calibrate/calibrate.h"
#include "skd.h"

extern volatile unsigned char  SHOW_VOLTAGE;
sbit BUTTON1=P3^2;

volatile struct pt pt1, pt2,pt3,pt_key,pt_dac,pt_beep,pt_blink;

extern volatile struct SKD xdata skd ;


//-----------------------------------------
static PT_THREAD(Display_Out_Process(struct pt *pt));
static PT_THREAD(MEAN_TSK(struct pt *pt));
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

//	WDT_Init(WDT_2000);//включить сторожевой таймер



	EA=1;
	PT_INIT(&pt1);
    PT_INIT(&pt2);
	PT_INIT(&pt3);
	PT_INIT(&pt_key);
	PT_INIT(&pt_blink);

 	if(!SHOW_VOLTAGE)
	{
		LED_SetPoint(INDICATOR_1,2);
		LED_Set_Brightness(INDICATOR_1,0);
		LED_Out_Float(INDICATOR_1,0.0);
	}
	else
	{
		LED_SetPoint(INDICATOR_1,3);
		LED_Set_Brightness(INDICATOR_1,0);
		LED_Out_Float(INDICATOR_1,0.0);	
	}

	while(1)
	{	
		MEAN_TSK(&pt2);
		LED_BlinkTask(&pt_blink);
		LED_Process(&pt3);
		Display_Out_Process(&pt1);
		Keyboard_Process(&pt_key);
	}
}
//-----------------------------------------------------------------------------
 void Timer1_Interrupt(void) interrupt 3  //using 1
{
	pt1.pt_time++;
	pt2.pt_time++;
	pt3.pt_time++;
	pt_key.pt_time++;
	pt_blink.pt_time++;

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
	LED_Set_Brightness(INDICATOR_1,skd.brightness>>4);
 	
	if(!SHOW_VOLTAGE)
	{
		   val=GetCalibrateVal(skd.line_sensor);

		   if((val<skd.SKD_Set.SKD_Settings.diap_low)||(val>skd.SKD_Set.SKD_Settings.diap_high))
		   {
		   		LED_SetBlink(INDICATOR_1,1);
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
 static PT_THREAD(MEAN_TSK(struct pt *pt))
 {
   PT_BEGIN(pt);

  while(1) 
  {
		PT_DELAY(pt,10);
		Meaning_Process();
   }
   PT_END(pt);
 }