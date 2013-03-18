#include "led_process.h"
#include "pt/pt.h"
#include <intrins.h>
#include <string.h>
#include "skd.h"
#include "calibrate/calibrate.h"
//----------------------------------------------

volatile unsigned char  spi_bit_counter=0;//счетчик передаваемых битов при bit-bang
unsigned int   spi_data=0;//данные bit-bang

#define HEAD_LEN_7219	5
unsigned int   spi_buf[20]={0xC01,0x9FF,0xF00,0xA0E,0xB05,0x505,0x404,0x303,0x208,0x109,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
//unsigned char  spi_buf_length=0;
volatile unsigned char   spi_buf_counter=0;

//unsigned int code init_buf[8]={0x100,0x200,0x300,0x400,0x500,0x600,0x700,0x800};

volatile unsigned char  num_conv_counter=0;

unsigned long  integer_num=0;

extern  struct SKD xdata skd ;
//----------------------------------------------
volatile unsigned int indicator_brightness[INDICATOR_NUM]={0xA0F};//,0xA0F,0xA0F,0xA0F};
volatile unsigned int indicator_scan[INDICATOR_NUM]={0xB04};//,0xB07,0xB04,0xB04};
volatile unsigned char indicator_point[INDICATOR_NUM]={0x5};//,0x0,0x5,0x5};
volatile unsigned char indicator_blink[INDICATOR_NUM]={0x0};//,0x0,0x0,0x0};
volatile unsigned char indicator_blink_flag[INDICATOR_NUM]={0x1};//,0x0,0x0,0x0};
volatile unsigned int indicator_decode[INDICATOR_NUM]={0x9FF};//,0x900,0x9FF,0x9FF};

 long  indicator_buf[INDICATOR_NUM];//буфер значений индикаторов

 unsigned char current_indicator=INDICATOR_1;//индикатор, захвативший шину

//-------------protothreads---------------------
//unsigned char LED_START=0;


// unsigned char  code LED_BAR_STAMP_RED[4]   ={8 ,128,4 ,16};
// unsigned char  code LED_BAR_STAMP_GREEN[4] ={64,2  ,32,1 };
// unsigned char  code LED_BAR_STAMP_ORANGE[4]={72,130,36,17};
//----------------------------------------------
//struct led_bar_struct
//{		  
//	unsigned char val;
//	unsigned char point_1;
//	unsigned char point_2;
//    unsigned char invert;
//} volatile led_bar;
// #pragma OT(0,Speed) 
//----------------------------------------------
PT_THREAD(LED_Process(struct pt *pt))
 {
 static unsigned char  i=0;
  PT_BEGIN(pt);

  while(1) 
  {
   	    PT_DELAY(pt,2);
		if(current_indicator>=INDICATOR_NUM)
		{
			  current_indicator=INDICATOR_1;
		}
		//-----------------предварительная настройка----------
		 spi_buf[1]=indicator_decode[current_indicator];//устанавливаем decode mode
		 spi_buf[4]=indicator_scan[current_indicator];//установим scan_limit

		 if(!indicator_blink[current_indicator])
		 {
		 		LED_Set_Brightness(INDICATOR_1,skd.brightness>>4);//яркость по АЦП
		 }
		 spi_buf[3]=indicator_brightness[current_indicator];//установим яркость

		//----------------------------------------------------
		integer_num=fabs(indicator_buf[current_indicator]);	  //берем абсолютное
		PT_YIELD(pt);
		
		//------------------int to bcd------------------------
		if(indicator_decode[current_indicator]==0x9FF)	 //если декодирование включено
		{
			for(num_conv_counter=0;(num_conv_counter<=(indicator_scan[current_indicator]&0x7));num_conv_counter++) //переводим в bcd
			{
				spi_buf[num_conv_counter+5]=integer_num%10|(0x100*(((indicator_scan[current_indicator]&0x7)+1)-num_conv_counter));
				integer_num=integer_num/10;	
				PT_YIELD(pt);
			}
				//-----------------------------поставим точку-----------------------------------							
			if(indicator_point[current_indicator])
			{
				spi_buf[5+indicator_point[current_indicator]-1]|=	0x80;  //
				
				for(i=(indicator_scan[current_indicator]&0x7);i>((indicator_scan[current_indicator]&0x7)+indicator_point[current_indicator]-5);i--)
				{
				   if((spi_buf[5+i]&0xf)==0x0)
				   {
				   		spi_buf[5+i]|=0xf;	
				   }
				   else
				   {
				   		break;
				   }
					
				}
			}
			//----------------------------поставим минус-------------------------------------			
			if(indicator_buf[current_indicator]<0)  //по возможности автоматизировать подстройку debug
			{
				spi_buf[9]=(spi_buf[9]&0xFFF0)|0xA;//ставим минус, если число отрицательное
			}
			//-------------------------------------------------------------------------------
			if(indicator_blink[current_indicator])
			{
				if(indicator_blink_flag[current_indicator]==0)
				{
					for(i=0;i<((indicator_scan[current_indicator]&0x7)+1);i++)
					{
						 spi_buf[5+i]&=0xff00;
						 spi_buf[5+i]|=0x000f;
					}
				}
			}
			//-------------------------------------------------------------------------------
		}
//		else//SET BARGRAPH
//		{
//			 	memcpy(&spi_buf[5],&init_buf,8*sizeof(unsigned int));//обнулим буфер
//				PT_YIELD(pt);
//
//				spi_buf[(led_bar.point_1/4)+5]|=((led_bar.point_1/4+1)<<8)|LED_BAR_STAMP_ORANGE[led_bar.point_1%4];
//				spi_buf[(led_bar.point_2/4)+5]|=((led_bar.point_2/4+1)<<8)|LED_BAR_STAMP_RED[led_bar.point_2%4];
//				PT_YIELD(pt);
//		
//			
//				for(i=0;i<led_bar.val;i++)
//				{
//					PT_YIELD(pt);
//					if(i<led_bar.point_1)
//					{
//						spi_buf[(i/4)+5]|=((i/4+1)<<8)|LED_BAR_STAMP_GREEN[i%4];
//					}
//					else
//					{
//						if((i>=led_bar.point_1) && (i<led_bar.point_2))
//						{
//							spi_buf[(i/4)+5]|=((i/4+1)<<8)|LED_BAR_STAMP_ORANGE[i%4];
//						}
//						else
//						{
//							spi_buf[(i/4)+5]|=((i/4+1)<<8)|LED_BAR_STAMP_RED[i%4];	
//						}
//					}
//					
//				}
//		}


//				//-------------------------------------------------------------------------------			

				
			
				switch(current_indicator)
				{
					case INDICATOR_1:
					{
						CS_1=0;
					}
					break;

//					case INDICATOR_2:
//					{
//						CS_2=0;
//					}
//					break;
//
//					case INDICATOR_3:
//					{
//						CS_3=0;
//					}
//					break;
//
//					case INDICATOR_4:
//					{
//						CS_4=0;
//					}
//					break;

					default:
					{
						PT_RESTART(pt);
					}
				}

	
			//--------------передача--------------------------------------
			for(spi_buf_counter=0;spi_buf_counter<(HEAD_LEN_7219+((indicator_scan[current_indicator]&0x7)+1));spi_buf_counter++)
			{
				spi_data=spi_buf[spi_buf_counter];
				for(spi_bit_counter=0;spi_bit_counter<16;spi_bit_counter++)
				{
					//WR_DATA=spi_data&0x8000;
					if(spi_data&0x8000)	
					{
						WR_DATA=1;
					}
					else
					{
						WR_DATA=0;
					}
					spi_data=spi_data<<1;//
					//WR_DATA=CY;
					CLK=1;
					_nop_();
					_nop_();
					_nop_();	
					CLK=0;
				}
				PT_YIELD(pt);
			
				switch(current_indicator)
				{
					case INDICATOR_1:
					{
						CS_1=1;
					}
					break;

//					case INDICATOR_2:
//					{
//						CS_2=1;
//					}
//					break;
//
//					case INDICATOR_3:
//					{
//						CS_3=1;
//					}
//					break;
//
//					case INDICATOR_4:
//					{
//						CS_4=1;
//					}
//					break;

					default:
					{
						PT_RESTART(pt);
					}
				}


				PT_YIELD(pt);

				switch(current_indicator)
				{
					case INDICATOR_1:
					{
						CS_1=0;
					}
					break;

//					case INDICATOR_2:
//					{
//						CS_2=0;
//					}
//					break;
//
//					case INDICATOR_3:
//					{
//						CS_3=0;
//					}
//					break;
//
//					case INDICATOR_4:
//					{
//						CS_4=0;
//					}
//					break;

					default:
					{
						PT_RESTART(pt);
					}
				}

			}
			current_indicator++;

			//----------------------------------------------------
			PT_RESTART(pt);
  }
   PT_END(pt);

 }
////--------------------------------------------------------------------------------
void LED_Out_Float(unsigned char  display_num,float  num)//вывод значения с плавающей точкой  //не реализовано
{
 	static unsigned int pwr=1;
	static unsigned char i;

	pwr=1;

	for(i=0;i<(indicator_point[display_num]-1);i++) //возводим 10 в степень положения точки
	{
		pwr=pwr*10;
	}
	indicator_buf[display_num]=(long)(num*pwr);

	return;
}
//--------------------------------------------------------------------------------
void LED_Set_Digit_Num(unsigned char display_num,unsigned char num)//установить количество отображаемых чисел
{
	//(spi_buf[4]&0xfff0)|((num-1)&0x7);
	
	indicator_scan[display_num]=(num-1)&0x7;
	return;	
}
//--------------------------------------------------------------------------------
void LED_Set_Brightness(unsigned char display_num,unsigned char bright)//установить яркость индикаторов
{
	indicator_brightness[display_num]=((unsigned int)0xA<<8)|(bright&0xf);
	return;	
}
//---------------------------------------------------------------------------------
void LED_Set_Decode(unsigned char display_num,unsigned char decode)//установить яркость индикаторов
{
	//spi_buf[3]=((unsigned int)0xA<<8)|(bright&0xf);
	
	indicator_decode[display_num]=((unsigned int)0x9<<8)|(decode&0xf);
	return;	
}
//--------------------------------------------------------------------------------
void LED_SetPoint(unsigned char display_num,unsigned char pnt)//на какое знакоместо ставим точку, начиная слева
{
	//indicator_point[current_indicator]=(8-2)-pnt;
	indicator_point[display_num]=pnt;//(8-2)-pnt;
	return;
}
//--------------------------------------------------------------------------------
void LED_SetBlink(unsigned char display_num,unsigned char blink)//мигание
{
	indicator_blink[display_num]=blink;//
	return;
}
//----------------------------------------------------------------------------------

PT_THREAD(LED_BlinkTask(struct pt *pt))//процесс мигания
 {
 	static    unsigned char i=0;
  PT_BEGIN(pt);

    while(1) 
    {
	   PT_DELAY(pt,100);
	   for(i=0;i<INDICATOR_NUM;i++)
	   {
	   		indicator_blink_flag[i]^=1;	
	   }
	}
  PT_END(pt);
 }
 //----------------------------------------------------------------------------------
// void SetBarGraph(unsigned char val,unsigned char point_1,unsigned char point_2, unsigned char invert)
// {
// 	 // unsigned char i=0;
//	led_bar.val=val;
//	led_bar.point_1=point_1-1;
//	led_bar.point_2=point_2-1;
//
//	if(led_bar.val>32)
//	{
//		led_bar.val=32;
//	}
//
//	if(point_1==0)
//	{
//		led_bar.point_1=0;	
//	}
//
//	if(point_2==0)
//	{
//		led_bar.point_2=0;	
//	}
//
//	if(point_1>32)
//	{
//		led_bar.point_1=33;	
//	}
//
//	if(point_2>32)
//	{
//		led_bar.point_2=33;	
//	}
//
//
// }
 //----------------------------------------------------------------------------------