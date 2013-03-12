#include "ulongsort.h"
#include "skd.h"
//volatile unsigned char sort_auto=WAIT_STATE;//счетчик автомата сортировки(кооперативная многозадачность)
volatile unsigned long mas_copy[SENSOR_QUEUE_LEN];
volatile unsigned char xdata mid_cycle_counter=0, sort_cycle_counter=0;
volatile unsigned long  temp;
//volatile unsigned char cycle_count=0;
//volatile unsigned char contin=1;

//unsigned char SORT=0;//флаг начала сортировки

extern volatile struct SKD xdata skd ;

//------------------------------------------------
 PT_THREAD(ulongsort_process(struct pt *pt))
 {
   static volatile  unsigned char i=0,j=0;
   
   PT_BEGIN(pt);

  while(1) 
  {

	   PT_DELAY(pt,10);
	
	  //-----------------копируем во временный массив-----------------------------
	   	memcpy(mas_copy,skd.ADC_SENSOR_UN,SENSOR_QUEUE_LEN*sizeof(unsigned long)); 
		
		PT_YIELD(pt);//отдаем другим процессам
	  //--------------сортировка пузырьком--------------------------------
		  for (i=SENSOR_QUEUE_LEN-1;i>0;i--)
		  {
		    for (j=0;j<i;j++)
		    {
		        if(mas_copy[j]>mas_copy[j+1])
		        {
		            temp=mas_copy[j];
		            mas_copy[j]=mas_copy[j+1];
		            mas_copy[j+1]=temp;
		        }
				PT_YIELD(pt);
		     }
		  }
	   //-----------находим медиану--------------------
	  	skd.line_sensor=(mas_copy[(SENSOR_QUEUE_LEN>>1)-1]+mas_copy[SENSOR_QUEUE_LEN>>1])>>1;


		PT_RESTART(pt);
	  //----------------------------------------------	
   }
   PT_END(pt);
 }

