#include "proto_uso.h"
#include <intrins.h>
#include "led_process.h"
#include "skd.h"
#include "keyboard.h"
//-----------------------------------------------------------------------------------
  unsigned char code Crc8Table[256]={0x00, 0xBC, 0x01, 0xBD, 0x02, 0xBE, 0x03, 0xBF, 
 									 0x04, 0xB8, 0x05, 0xB9, 0x06, 0xBA, 0x07, 0xBB, 
									 0x08, 0xB4, 0x09, 0xB5, 0x0A, 0xB6, 0x0B, 0xB7, 
									 0x0C, 0xB0, 0x0D, 0xB1, 0x0E, 0xB2, 0x0F, 0xB3, 
									 0x10, 0xAC, 0x11, 0xAD, 0x12, 0xAE, 0x13, 0xAF, 
									 0x14, 0xA8, 0x15, 0xA9, 0x16, 0xAA, 0x17, 0xAB, 
									 0x18, 0xA4, 0x19, 0xA5, 0x1A, 0xA6, 0x1B, 0xA7, 
									 0x1C, 0xA0, 0x1D, 0xA1, 0x1E, 0xA2, 0x1F, 0xA3, 
									 0x20, 0x9C, 0x21, 0x9D, 0x22, 0x9E, 0x23, 0x9F, 
									 0x24, 0x98, 0x25, 0x99, 0x26, 0x9A, 0x27, 0x9B, 
									 0x28, 0x94, 0x29, 0x95, 0x2A, 0x96, 0x2B, 0x97, 
									 0x2C, 0x90, 0x2D, 0x91, 0x2E, 0x92, 0x2F, 0x93, 
									 0x30, 0x8C, 0x31, 0x8D, 0x32, 0x8E, 0x33, 0x8F, 
									 0x34, 0x88, 0x35, 0x89, 0x36, 0x8A, 0x37, 0x8B, 
									 0x38, 0x84, 0x39, 0x85, 0x3A, 0x86, 0x3B, 0x87, 
									 0x3C, 0x80, 0x3D, 0x81, 0x3E, 0x82, 0x3F, 0x83, 
									 0x40, 0xFC, 0x41, 0xFD, 0x42, 0xFE, 0x43, 0xFF, 
									 0x44, 0xF8, 0x45, 0xF9, 0x46, 0xFA, 0x47, 0xFB, 
									 0x48, 0xF4, 0x49, 0xF5, 0x4A, 0xF6, 0x4B, 0xF7, 
									 0x4C, 0xF0, 0x4D, 0xF1, 0x4E, 0xF2, 0x4F, 0xF3, 
									 0x50, 0xEC, 0x51, 0xED, 0x52, 0xEE, 0x53, 0xEF, 
									 0x54, 0xE8, 0x55, 0xE9, 0x56, 0xEA, 0x57, 0xEB, 
									 0x58, 0xE4, 0x59, 0xE5, 0x5A, 0xE6, 0x5B, 0xE7, 
									 0x5C, 0xE0, 0x5D, 0xE1, 0x5E, 0xE2, 0x5F, 0xE3, 
									 0x60, 0xDC, 0x61, 0xDD, 0x62, 0xDE, 0x63, 0xDF, 
									 0x64, 0xD8, 0x65, 0xD9, 0x66, 0xDA, 0x67, 0xDB, 
									 0x68, 0xD4, 0x69, 0xD5, 0x6A, 0xD6, 0x6B, 0xD7, 
									 0x6C, 0xD0, 0x6D, 0xD1, 0x6E, 0xD2, 0x6F, 0xD3, 
									 0x70, 0xCC, 0x71, 0xCD, 0x72, 0xCE, 0x73, 0xCF, 
									 0x74, 0xC8, 0x75, 0xC9, 0x76, 0xCA, 0x77, 0xCB, 
									 0x78, 0xC4, 0x79, 0xC5, 0x7A, 0xC6, 0x7B, 0xC7, 
									 0x7C, 0xC0, 0x7D, 0xC1, 0x7E, 0xC2, 0x7F, 0xC3};
sbit DE_RE=P3^5;

//-----------------------------------------------------------------------------------

volatile unsigned char xdata ADRESS_DEV=0x1;

extern volatile unsigned char  SHOW_VOLTAGE;
//--------------------------------global variable------------------------------------

volatile unsigned char xdata	RECIEVED=0;//принято
volatile unsigned char xdata    recieve_count;//счетчик приемного буфера
volatile unsigned char xdata	transf_count;//счетчик передаваемых байтов	   
volatile unsigned char xdata	buf_len;//длина передаваемого буфера

volatile unsigned char xdata    CUT_OUT_NULL=0;//флаг-вырезаем 0 после 0xD7
volatile unsigned char xdata    frame_len=0;//длина кадра, которую вытаскиваем из шестого байта кадра
//--------------------------------------------------------------------
volatile unsigned char xdata    RecieveBuf[MAX_LENGTH_REC_BUF]={0} ; //буфер принимаемых данных
volatile unsigned char xdata    TransferBuf[MAX_LENGTH_REC_BUF]={0xD7};
//--------------------------------------------------------------------
volatile unsigned char xdata    STATE_BYTE=0xC0;//байт состояния устройства
volatile unsigned char xdata    symbol=0xFF;//принятый символ

volatile struct pt pt_proto;
//-----------------------------------------------------------------------------------
#pragma OT(0,Speed)
void UART_ISR(void) interrupt 4 //using 1
{	
	EA=0;	//запрет прерывания
	
	if(RI)
	{
	//	RI=0; 
//----------------------обрабатываем возможные ошибки длины кадра-------------
		if(recieve_count>MAX_LENGTH_REC_BUF)	//если посылка слишком длинная
		{
			PT_RESTART_OUT(pt_proto);  //внепроцессный рестарт
			return;
		} 


		symbol=SBUF;
		switch(symbol)
		{
			case (char)(0xD7):
			{
				RecieveBuf[recieve_count]=symbol;
				recieve_count++;
				CUT_OUT_NULL=1;		 
			}
			break;

			case (char)(0x29):
			{
				if(CUT_OUT_NULL==1)
				{
					RecieveBuf[0]=0x0;
					RecieveBuf[1]=0xD7;
					RecieveBuf[2]=0x29;
					recieve_count=0x3;	 	
				}
				else
				{
					RecieveBuf[recieve_count]=symbol;
					recieve_count++;	
				}
				CUT_OUT_NULL=0;
			}
			break;

			case (char)(0x0):
			{
 				if(CUT_OUT_NULL==1)	  //если после 0xD7-пропускаем
				{
					CUT_OUT_NULL=0;		
				}
				else
				{
					RecieveBuf[recieve_count]=symbol;
					recieve_count++;	
				}
			}
			break;

			default :
			{
				RecieveBuf[recieve_count]=symbol;
				recieve_count++;
				CUT_OUT_NULL=0;
				
			}
		}

	   if(recieve_count>6)
	   {
	   		  if(recieve_count==6+frame_len)	  // принимаем указанное в frame_len число байт данные, 6 значит что обмен идет с компом, надо ставаить 5 чтобы обмениваться с устройствами
   			  {
					RECIEVED=1;//буфер принят
			  		ES=0;
			  		REN=0;  //recieve disable -запрещаем принимать в буфер	
				   	CUT_OUT_NULL=0;  			  			
			  }	  
	   }
	   else
	   {
			   if(recieve_count==6)
			   {     
		        	frame_len=RecieveBuf[recieve_count-1];  // получаем длину данных после заголовка					 
			   }	   		
	   }
	   RI=0;										
	}
//----------------------------передача----------------------------------------------------------------
	if(TI)
	{
		//TI=0;
		 
		if((transf_count<buf_len) || (CUT_OUT_NULL==0x1))
		{
			if(transf_count<3)//передаем заголовок
			{
				SBUF=TransferBuf[transf_count];	
				transf_count++;		
			}
			else   //тело...   подставляем 0 после 0xD7
			{
					if(CUT_OUT_NULL==0)
					{
						if(TransferBuf[transf_count]==(unsigned char)0xD7)//проверим, это  ,0xD7 или другое
						{			
							CUT_OUT_NULL=0x1;
							//buf_len++;//увеличиваем буфер на '0'	
						}
						SBUF=TransferBuf[transf_count];	
						transf_count++;		
					}
					else
					{
						SBUF=(unsigned char)0x0;
						CUT_OUT_NULL=0;							
					}						
			}				
		}
		else
		{
			transf_count=0;		//обнуляем счетчик
			CUT_OUT_NULL=0;
			PT_RESTART_OUT(pt_proto);  //внепроцессный рестарт			
		}
		TI=0;					   
	}			
	EA=1;
	return;
}
//------------------------------------------------------------------------------
#pragma OT(6,Speed)
void Protocol_Init(void) //using 0
{
	TI=0;
	RI=0;
	
	recieve_count=0x0;//счетчик буфера приема
	transf_count=0x0;//счетчик передаваемых байтов
	buf_len=0x0;//длина передаваемого буфера
	DE_RE=0;//линия на прием
	CUT_OUT_NULL=0;
	STATE_BYTE=0xC0;
	PT_INIT(&pt_proto);

	return;
}
//-----------------------------------------------------------------------------
unsigned char Send_Info(void) //using 0    //посылка данных с устройства
{
	    unsigned char   idata i=0;
	   									
	   //заголовок кадра---
	   TransferBuf[0]=0x00;
	   TransferBuf[1]=0xD7;
	   TransferBuf[2]=0x29;
	   //------------------
	   TransferBuf[3]=ADRESS_DEV;  // адрес узла
	   TransferBuf[4]=GET_DEV_INFO_RESP;  // код операции
	   TransferBuf[5]=25;//длина оставшейся части кадра

	   ((float*)(&TransferBuf[6]))[0]=skd.SKD_Set.SKD_Settings.calibr_high; //верхнее калибровочное значение
	   ((float*)(&TransferBuf[10]))[0]=skd.SKD_Set.SKD_Settings.calibr_low; //нижнее калибровочное значение
	   ((float*)(&TransferBuf[14]))[0]=skd.SKD_Set.SKD_Settings.diap_high; //верхняя граница
	   ((float*)(&TransferBuf[18]))[0]=skd.SKD_Set.SKD_Settings.diap_low; //нижняя граница

	   TransferBuf[22]= skd.SKD_Set.SKD_Settings.adc_diap;
	   ((unsigned int*)(&TransferBuf[23]))[0]=(skd.SKD_Set.SKD_Settings.indicate_time*10); //время индикации, мс

	   for(i=0;i<5;i++)
	   {
	   		TransferBuf[25+i]=skd.SKD_Set.SKD_Settings.dev_id[i];	
	   }
	   

	   TransferBuf[30]=CRC_Check(&TransferBuf[1],29); // подсчет контрольной суммы

	return 31; 
}
//-----------------------------------------------------------------------------
unsigned char  Channel_Set_Parameters(void) //using 0 //Установить параметры 
{
	    unsigned char   idata i=0;
	   									
	   (skd.SKD_Set.SKD_Settings.calibr_high)=((float*)(&RecieveBuf[6]))[0]; //верхнее калибровочное значение
	   (skd.SKD_Set.SKD_Settings.calibr_low)=((float*)(&RecieveBuf[10]))[0]; //нижнее калибровочное значение
	   (skd.SKD_Set.SKD_Settings.diap_high)=((float*)(&RecieveBuf[14]))[0]; //верхняя граница
	   (skd.SKD_Set.SKD_Settings.diap_low)=((float*)(&RecieveBuf[18]))[0]; //нижняя граница

	   skd.SKD_Set.SKD_Settings.adc_diap=RecieveBuf[22]&0x7;
	   (skd.SKD_Set.SKD_Settings.indicate_time)=((unsigned int*)(&RecieveBuf[23]))[0]/10; //время индикации, мс


	   if(skd.SKD_Set.SKD_Settings.diap_high>=100.0 || skd.SKD_Set.SKD_Settings.diap_low<-100.0) //передвигаем десятичную точку
	   {
	   		LED_SetPoint(INDICATOR_1,2);
	   }
	   else
	   {
	   		LED_SetPoint(INDICATOR_1,3);
	   }

	   if(SHOW_VOLTAGE)//если показываем напряжение то точка на третьем знакоместе
	   {
	   		LED_SetPoint(INDICATOR_1,4);
	   }

	   for(i=0;i<5;i++)	  //копируем ID
	   {
	   		skd.SKD_Set.SKD_Settings.dev_id[i]=RecieveBuf[25+i];	
	   }

	   Store_Channels_Data();//сохраняем настройки в EEPROM
	   return Request_Error(FR_SUCCESFUL);
}
//-----------------------------------------------------------------------------
unsigned char Channel_Set_Reset_State_Flags(void) //using 0 //	Установка/Сброс флагов состояния 
{
	STATE_BYTE=0x40;
	return	Request_Error(FR_SUCCESFUL);//ошибки нет, подтверждение
}
//-----------------------------------------------------------------------------
unsigned char Request_Error(unsigned char error_code) //using 0 //	Ошибочный запрос/ответ;
{
	TransferBuf[0]=0x00;TransferBuf[1]=0xD7;TransferBuf[2]=0x29;
    TransferBuf[3]=ADRESS_DEV;  // адрес узла
    TransferBuf[7]=RecieveBuf[4]; // код сбойной команды
    TransferBuf[4]=0xFF;  // код операции

	TransferBuf[6]=STATE_BYTE; // байт статуса узла
    TransferBuf[8]=error_code;	 
    TransferBuf[5]=0x04;	  // длина данных
    TransferBuf[9]=CRC_Check(TransferBuf,9);
	return 10;
}
//--------------------------------------------------------------------------------------
#pragma OT(0,Speed) 
PT_THREAD(ProtoProcess(struct pt *pt))
 {

 static unsigned char  CRC=0x0;
 static struct pt pt_handle_thread;
  PT_BEGIN(pt);

  while(1) 
  {
  //----------restart------------
		recieve_count=0x0;//??
		REN=1;//recieve enqble
		DE_RE=0;//линия на прием
		ES=1;
		RI=0;
		TI=0;
  //-----------------------------
	   PT_WAIT_UNTIL(pt,RECIEVED); //ждем команды на старт
	   RECIEVED=0;
		
		if(RecieveBuf[3]!=ADRESS_DEV)//если адрес совпал	  
		{
			PT_RESTART(pt);//если адрес не сошeлся-перезапустим протокол			
		}	
				
	    CRC=RecieveBuf[recieve_count-1];
				
		if(CRC_Check(&RecieveBuf,(recieve_count-CRC_LEN))!=CRC)
		{		
			PT_RESTART(pt);//если CRC не сошлось-перезапустим протокол	 
		}
		PT_YIELD(pt);//дадим другим процессам время
  //-----------------------------	

	   //обработка буфера
		//---------------------------------------
		  	if(RecieveBuf[4]==GET_DEV_INFO_REQ)
			{
				buf_len=Send_Info();	
			}
			//-----------------------------------
			else if (RecieveBuf[4]==CHANNEL_SET_PARAMETERS_REQ)
			{
				buf_len=Channel_Set_Parameters();
			}
		//------------------------------------------
			
//------------------------------------------
	
		
		if(buf_len==0)//если в буфере пусто
		{
			PT_RESTART(pt);//перезапустим протокол	
		}
		else
		{
			DE_RE=1; //переключаем RS485 в режим передачи
							
			REN=0;	//запрет приема-только передача
			transf_count=0;
			CUT_OUT_NULL=0;
			ES=1; //включим прерывание уарт	

			SBUF=TransferBuf[transf_count];//передача байта, остальным займется автомат
			transf_count++;//инкрементируем счетчик переданных
			
			PT_DELAY(pt,50);		
		}
  //-----------------------------
  }

 PT_END(pt);
}
//-----------------------CRC------------------------------------------------------------
#pragma OT(6,Speed)
  unsigned char CRC_Check( unsigned char xdata *Spool_pr,unsigned char Count_pr ) 
 {
     unsigned char crc = 0x0;

     while (Count_pr--)
         crc = Crc8Table[crc ^ *Spool_pr++];

     return crc;
 }
//-----------------------------------------------------------------------------------------------

