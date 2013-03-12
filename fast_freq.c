#include "fast_freq.h"

#define FREQ_CHANNELS		4
#define FAST_FREQ_CHNL		2 //высокоскоростной канал
#define FAST_FREQ_CHNL_MID	3 //высокоскоростной усредненный канал

#define FAST_FREQ_CHNL_NUM		10
#define FAST_FREQ_CHNL_MID_NUM	11

# define CLI_EXT  EX1=0; EX0=0; ET0=0;
# define STI_EXT  EX1=1; EX0=1; ET0=1;

sbit LED1=P3^7;
//-------------------------------------------------------- debug
unsigned char volatile idata  timer_busy=0;	   //флаг занятости таймера
unsigned long volatile idata counter_32; //если таймер переполнится, инкрементируем этот счетчик
unsigned long volatile idata freq_buffer[FREQ_CHANNELS]={0};
unsigned int volatile  idata channel_counter[3]={0};//дискреный счетчик канала

unsigned char volatile idata measure_state=0;//автомат состояний для оптимизации процесса вычисления(уменьшение нагрузки на процессор)
unsigned char volatile sec_10_counter=0;


#define TIMER_FREE 0  //таймер свободен для захвата
#define BUSY_CHN1  1
#define BUSY_CHN2  2
#define BUSY_CHN3  3

#define FRQ_CHNL_1 0
#define FRQ_CHNL_2 1
#define FRQ_CHNL_3 2
//--------------------------------------------------------
void Frequency_Init(void) //инициализация частотных каналов
{
	Timer0_Initialize();
	Timer2_Initialize();
	TIC_Init();//точный секундный интервал
	EX1=1;//включаем внешние прерывания
	EX0=1;
	IT0=1;
	IT1=1;
	return;
}
//--------------------------------------------------------
unsigned char Frequency_Measure_Process(void) //using 2//циклический процесс измерения частоты
{
	if(counter_32>0xC00000  && timer_busy)//если импульс долго не приходит, сбрасываем
	{
		TR2=0;
		timer_busy=TIMER_FREE;
		TH2=TL2=0;
		freq_buffer[timer_busy-1]=0xFFFFFFFF;
	} 

	switch(measure_state)
	{
		case 0:
		{
			channels[8].channel_data=((F_OSC)/ (freq_buffer[FRQ_CHNL_1]));
			measure_state=1;
		}
		break;

		case 1:
		{
			channels[9].channel_data=(F_OSC)/ (freq_buffer[FRQ_CHNL_2]);
			measure_state=1;
		}
		break;

	/*	case 2:
		{
			channels[10].channel_data=((F_OSC_12)/ (freq_buffer[FRQ_CHNL_3]));
			measure_state=0;
		}
		break; */
	}
//LED1=!LED1;		
//	setTimer(FREQ_TASK, 0, 50);
	return 0;
}
//--------------------------------------------------------
void INT0_ISR(void) interrupt 0 //using 3//обработчик внешнего прерывания 0
{
	CLI_EXT
	channel_counter[FRQ_CHNL_1]++;
	if(timer_busy==BUSY_CHN1) //если был занят этим расчетом
	{	

		TR2=0;
		// код
		freq_buffer[FRQ_CHNL_1]=(unsigned int)TH2<<8;
		freq_buffer[FRQ_CHNL_1]+=(TL2+counter_32);
		timer_busy=TIMER_FREE;
		STI_EXT
		return;	
	}
	if( timer_busy==TIMER_FREE)
	{
		//код
		TH2=TL2=0;
		TR2=1;//запускаем таймер
		counter_32=0;
		timer_busy=BUSY_CHN1;
		STI_EXT
		return;
	} 
	STI_EXT
	return;
}
//--------------------------------------------------------
void INT1_ISR(void) interrupt 2 //using 3//обработчик внешнего прерывания 1
{
	CLI_EXT
	channel_counter[FRQ_CHNL_2]++;
	if(timer_busy==BUSY_CHN2) //если был занят этим расчетом
	{
		TR2=0;
		// код
		freq_buffer[FRQ_CHNL_2]=(unsigned int)TH2<<8;
		freq_buffer[FRQ_CHNL_2]+=(TL2+counter_32);
		timer_busy=TIMER_FREE;
		STI_EXT
		return;	
	}
	if( timer_busy==TIMER_FREE)
	{
		//код
		TH2=TL2=0;
		TR2=1;//запускаем таймер
		counter_32=0;
		timer_busy=BUSY_CHN2;
		STI_EXT
		return;
	}
	STI_EXT
	
	return;
}
//--------------------------------------------------------
void INT2_ISR(void) interrupt 1 //using 3//обработчик внешнего прерывания 2-использует внешний вход таймера T0
{
	CLI_EXT
	freq_buffer[FAST_FREQ_CHNL]+=0xFFFF;//если 
	TH0=TL0=0x0;	
	STI_EXT
	
	return;
}
//--------------------------------------------------------
void Timer2_ISR(void) interrupt 5 //using 3//обработчик прерывания счетного таймера частоты
{
	TF2 = 0;
	TH2=TL2=0;
	counter_32+=0x10000;	//добавляем 17 разрядов
	return;
}
//------------------------------------------------------------
unsigned char Sec_Task(void) //using 2//секундная задача для определения нулевой частоты
{	
	unsigned char  i=0;
	for(i=0;i<FREQ_CHANNELS;i++)
	{
		if(!channel_counter[i])
		{
			freq_buffer[i]=0xFFFFFFFF;
		}
		channel_counter[i]=0;
	} 
//	WDT_Clear();
	//--------------------
//	setTimer(SEC_TASK, 0, 200);

	return 0;
}
//----------для точного измерения секундного интервала применим TIC
void TIC_ISR(void) interrupt 10 	//using 1   //прерывания таймера в 1 секунду
{
	channels[FAST_FREQ_CHNL_NUM].channel_data =	freq_buffer[FAST_FREQ_CHNL]+(((unsigned int)TH0<<8)|(unsigned int)TL0);
	freq_buffer[FAST_FREQ_CHNL]+=channels[FAST_FREQ_CHNL_NUM].channel_data;

	if(sec_10_counter<10)
	{
		 sec_10_counter++;
	}
	else
	{
		channels[FAST_FREQ_CHNL_MID_NUM].channel_data=freq_buffer[FAST_FREQ_CHNL_MID]/10;//каждые 10 секунд  передаем усредненное значение 	в буфер канала
		freq_buffer[FAST_FREQ_CHNL_MID]=0;		

		sec_10_counter=0;
	}
	TIMECON&=0x0;
	return;
}