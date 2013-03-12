#include "proto_uso.h"
#include "calibrate\calibrate.h"
#include <intrins.h>

extern bit SHOW_VOLTAGE;

sbit DE_RE=P3^5;
// sbit LED=P2^6;
sbit LED=P0^6;
//-----------------------------------------------------------------------------------
//volatile unsigned char xdata DEV_NAME[DEVICE_NAME_LENGTH_SYM] ="<<uUSO_2>>"; //имя устройства
//volatile unsigned char xdata NOTICE[DEVICE_DESC_MAX_LENGTH_SYM]="<-- GEOSPHERA_2011 -->";//примечание 	
//volatile unsigned char xdata VERSION[DEVICE_VER_LENGTH_SYM] ="\x30\x30\x30\x30\x31";	// версия программы ПЗУ	не больше 5 байт

volatile unsigned char idata ADRESS_DEV=0x1;

//volatile unsigned char xdata dev_desc_len=20;//длина описания устройства
//--------------------------------global variable------------------------------------
//volatile unsigned char idata	PROTO_STATE;//счетчик состояний
volatile unsigned char idata	RECIEVED=0;//принято
volatile unsigned char idata    recieve_count;//счетчик приемного буфера
volatile unsigned char idata	transf_count;//счетчик передаваемых байтов	   
volatile unsigned char idata	buf_len;//длина передаваемого буфера

//------------------------флаги ошибок--------------------------------
//volatile unsigned char idata CRC_ERR;	//ошибка сrc
//volatile unsigned char idata COMMAND_ERR;//неподдерживаемая команда

//volatile unsigned char idata TIMEOUT;//таймаут 

volatile unsigned char idata  CUT_OUT_NULL;//флаг-вырезаем 0 после 0xD7
volatile unsigned char idata frame_len=0;//длина кадра, которую вытаскиваем из шестого байта кадра
//--------------------------------------------------------------------
volatile unsigned char xdata  RecieveBuf[MAX_LENGTH_REC_BUF]={0} ; //буфер принимаемых данных
volatile unsigned char xdata 			*TransferBuf;
//static unsigned char /*data*/ volatile  TransferBuf[MAX_LENGTH_TR_BUF] ; //буфер передаваемых данных
extern volatile struct SKD xdata skd ;

struct pt pt_proto;
//--------------------------------------------------------------------
//volatile unsigned char xdata  STATE_BYTE=0xC0;//байт состояния устройства
//-----------------------------------------------------------------------------------
union //объединение для конвертирования char->long
{
	float result_float;
	unsigned char result_char[4];
}
sym_8_to_float;
//-----------------------------------------------------------------------------------

void UART_ISR(void) interrupt 4 //using 1
{	
  unsigned char idata symbol=0xFF;//принятый символ

	EA=0;	//запрет прерывания
	
	if(RI)
	{
		RI=0; 
//----------------------обрабатываем возможные ошибки длины кадра-------------
		if(recieve_count>MAX_LENGTH_REC_BUF)	//если посылка слишком длинная
		{
//			PROTO_STATE=PROTO_RESTART;//делаем рестарт протокола
			PT_RESTART_OUT(pt_proto);  //внепроцессный рестарт

			return;
		} 



		symbol=SBUF;
		
//--------------------------начало кадра...проверка до длины кадра--------
	    if(recieve_count<6)
		{
				switch(recieve_count)	 
				{
					case  0:   //первый символ 0
					{
	 				 	 if(symbol!=0x00)
						 {
						 	//PROTO_STATE=PROTO_RESTART;
							PT_RESTART_OUT(pt_proto);
							EA=1;
							return;								 	
						 }	 
					}
					break;
	
					case 1:	 //второй символ 0xD7
					{
						 if(symbol!=0xD7)
						 {
						 	//PROTO_STATE=PROTO_RESTART;
							PT_RESTART_OUT(pt_proto);
							EA=1;
							return;
						 }		 
					}
					break;
	
					case 2:	 //	третий символ 0x29
					{
					 	 if(symbol!=0x29)
						 {
						 	//PROTO_STATE=PROTO_RESTART;
							PT_RESTART_OUT(pt_proto);
							EA=1;
							return;	
						 }	 
					}
					break;

					case 3:	//если адрес не совпал, то сбросим//NEW
					{
						if(symbol!=ADRESS_DEV)//если адрес совпал	  
						{
							//PROTO_STATE=PROTO_RESTART;		//иначе ждем новой посылки
							PT_RESTART_OUT(pt_proto);		
						}
					}
					break;
	
					default:  //
					{					
					}
					break;
				}

			RecieveBuf[recieve_count]=symbol;//сохраняем принятый символ в буфер
			recieve_count++;//счетчик буфера

			if(recieve_count==6)
			{
				frame_len=RecieveBuf[recieve_count-1]; //получим оставшуюся длину
			}
		}
//---------------------------------------------------------
		else  //отсюда знаем длину кадра и удаляем нули после 0xD7
		{							
			switch(symbol)//проверим, это 0x0 ,0xD7 или другое
			{
				case 0xD7:
				{
					CUT_OUT_NULL=1;	//
					RecieveBuf[recieve_count]=symbol;
					recieve_count++;
						
				}
				break;

				case 0x0:
				{
					if(!CUT_OUT_NULL)  //если вырезать 0x0 не надо, то не вырезаем
					{
						RecieveBuf[recieve_count]=symbol;
						recieve_count++;								
					}		
					else //иначе в буфер не кладем, сбросим флаг
					{
						CUT_OUT_NULL=0;			
					}	
				}
				break;

				default:  //другие кладем в буфер
				{			
					CUT_OUT_NULL=0;
					RecieveBuf[recieve_count]=symbol;
					recieve_count++;
				}
				break;
			}

			if(recieve_count>=frame_len+6)//если приняты  все байты
			{
				//PROTO_STATE=PROTO_CRC_CHECK;//PROTO_ADDR_CHECK;
				RECIEVED=1;

			  	ES=0;
			  	REN=0;  //recieve disable -запрещаем принимать в буфер		
			}
		}										
	}
//----------------------------передача----------------------------------------------------------------
	if(TI)
	{
		TI=0;
		 
		if(transf_count<buf_len)
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
			//PROTO_STATE=PROTO_RESTART;
			PT_RESTART_OUT(pt_proto);
			CUT_OUT_NULL=0;
			DE_RE=0;//линия на прием
			
		}					   
	}			
	EA=1;
	return;
}
 
//------------------------------------------------------------------------------
void Protocol_Init(void) //using 0
{
	TI=0;
	RI=0;
	//------------------------флаги ошибок--------------------------------
	
//	CRC_ERR=0x0;	//ошибка crc
//	COMMAND_ERR=0x0;//неподдерживаемая команда
	
	TransferBuf=&RecieveBuf[0];	 //буфер ответа =буфер запроса

//	PROTO_STATE=PROTO_RESTART;//счетчик состояний
	recieve_count=0x0;//счетчик буфера приема
	transf_count=0x0;//счетчик передаваемых байтов
	buf_len=0x0;//длина передаваемого буфера
	DE_RE=0;//линия на прием
	CUT_OUT_NULL=0;
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
	   ((unsigned int*)(&TransferBuf[23]))[0]=(skd.SKD_Set.SKD_Settings.indicate_time); //время индикации, мс

	   for(i=0;i<5;i++)
	   {
	   		TransferBuf[25+i]=skd.SKD_Set.SKD_Settings.dev_id[i];	
	   }
	   

	   TransferBuf[30]=CRC_Check(&TransferBuf[1],29); // подсчет контрольной суммы
	   LED=0;
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

	   skd.SKD_Set.SKD_Settings.adc_diap=RecieveBuf[22];
	   (skd.SKD_Set.SKD_Settings.indicate_time)=((unsigned int*)(&TransferBuf[23]))[0]; //время индикации, мс

	   //setTimer(ADC_MEAN_TASK, 0, skd.SKD_Set.SKD_Settings.indicate_time>>3);

	   if(skd.SKD_Set.SKD_Settings.diap_high>=100.0 || skd.SKD_Set.SKD_Settings.diap_low<-100.0) //передвигаем десятичную точку
	   {
	   		LED_SetPoint(4);
	   }
	   else
	   {
	   		LED_SetPoint(3);
	   }

	   if(SHOW_VOLTAGE)//если показываем напряжение то точка на третьем знакоместе
	   {
	   		LED_SetPoint(3);
	   }

	   for(i=0;i<5;i++)	  //копируем ID
	   {
	   		skd.SKD_Set.SKD_Settings.dev_id[i]=RecieveBuf[25+i];	
	   }
	   
	   Store_Channels_Data();//сохраняем настройки в EEPROM
	   return Request_Error(FR_SUCCESFUL);
}
//-----------------------------------------------------------------------------
unsigned char Request_Error(unsigned char error_code) //using 0 //	Ошибочный запрос/ответ;
{
	TransferBuf[0]=0x00;TransferBuf[1]=0xD7;TransferBuf[2]=0x29;
    TransferBuf[3]=ADRESS_DEV;  // адрес узла
    TransferBuf[7]=RecieveBuf[4]; // код сбойной команды
    TransferBuf[4]=0xFF;  // код операции

	TransferBuf[6]=0;//STATE_BYTE; // байт статуса узла
    TransferBuf[8]=error_code;	 
    TransferBuf[5]=0x04;	  // длина данных
    TransferBuf[9]=CRC_Check(TransferBuf,9);
	return 10;
}
//-----------------------------------------------------------------------------
void ProtoBufHandling(void) //using 0 //процесс обработки принятого запроса
{
  switch(RecieveBuf[4])
  {
//---------------------------------------
  	case GET_DEV_INFO_REQ:
	{
		buf_len=Send_Info();	
	}
	break;

	//-----------------------------------
	case CHANNEL_SET_PARAMETERS_REQ:
	{
			
		buf_len=Channel_Set_Parameters();

	}
	break;
	//-----------------------------------
    default:
	{
//       COMMAND_ERR=0x1;//несуществующая команда
	   buf_len=Request_Error(FR_COMMAND_NOT_EXIST);
//	   PROTO_STATE=PROTO_ERR_HANDLING;//на обработчик ошибки
    }								   
  }

  return;
}
//-----------------------------------------------------------------------------------
PT_THREAD(ProtoProcess(struct pt *pt))
 {
 unsigned char i=0;
 unsigned char  CRC=0x0;
  PT_BEGIN(pt);

  while(1) 
  {
  //----------restart------------
		recieve_count=0x0;//??
		REN=1;//recieve enqble
		DE_RE=0;//линия на прием
		ES=1;
  //-----------------------------
	   PT_WAIT_UNTIL(pt,RECIEVED); //ждем команды на старт
	   RECIEVED=0;
			
	   CRC=RecieveBuf[recieve_count-1];
				
		if(CRC_Check(&RecieveBuf,(recieve_count-CRC_LEN))!=CRC)
		{
			PT_RESTART(pt);//если CRC не сошлось-перезапустим протокол	 
		}
		PT_YIELD(pt);//дадим другим процессам время
  //-----------------------------
  		ProtoBufHandling();//процедура обработки сообщения	
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
			SBUF=TransferBuf[transf_count];//передача байта, остальным займется автомат
			transf_count++;//инкрементируем счетчик переданных
			
			PT_RESTART(pt);//перезапустим протокол	
			
			ES=1; //включим прерывание уарт		
		}

  //-----------------------------
  }

 PT_END(pt);
}
//-----------------------------------------------------------------------------------
//void ProtoProcess(void) //главный процесс 
//{
//	switch(PROTO_STATE)
//	{
////------------------------------------------
//		case(PROTO_RESTART):
//		{
//			recieve_count=0x0;//??
//		//	transf_count=0x0;
//
//
//			PROTO_STATE=PROTO_WAIT;//переходим в состояние ожидания 
//
//			REN=1;//recieve enqble
//			DE_RE=0;//линия на прием
//			ES=1;	//прерывание uart	
//		}
//		break;
////------------------------------------------
//		case(PROTO_WAIT)://ожидание приема
//		{		
//			//пусто
//			_nop_();
//		}
//		break;
////------------------------------------------
//
////------------------------------------------
//		case(PROTO_CRC_CHECK):// проверка CRC
//		{		 	
//			  
//					
//			CRC=RecieveBuf[recieve_count-1];
//					
//			if(CRC_Check(&RecieveBuf,(recieve_count-CRC_LEN))==CRC)
//			{
//				PROTO_STATE=PROTO_BUF_HANDLING;	 
//			}
//			else
//			{		
////				CRC_ERR=0x1;//несовпадение CRC
//	   			PROTO_STATE=PROTO_ERR_HANDLING;//на обработчик ошибки								
//			} 	
//		}
//		break;
////------------------------------------------
//		case(PROTO_BUF_HANDLING):// обработка принятого буфера
//		{			
//				ProtoBufHandling();//процедура обработки сообщения	
//				if(buf_len)//если в буфере что то есть
//					PROTO_STATE=PROTO_BUF_TRANSFER;//после обработки передаем сообщение	
//				else
//					PROTO_STATE=PROTO_RESTART; //иначе рестарт протокола		
//		}
//		break;
////------------------------------------------
//		case(PROTO_BUF_TRANSFER):// передача буфера
//		{
//				DE_RE=1; //переключаем RS485 в режим передачи
//								
//				REN=0;	//запрет приема-только передача
//				transf_count=0;
//				CUT_OUT_NULL=0;
//				SBUF=TransferBuf[transf_count];//передача байта, остальным займется автомат
//				transf_count++;//инкрементируем счетчик переданных
//				PROTO_STATE=PROTO_WAIT;//забросили данные в автомат и ждем
//				
//				ES=1; //включим прерывание уарт	
//		}
//		break;
////------------------------------------------
//		case(PROTO_ERR_HANDLING):// обработка ошибок // можно не реализовывать
//		{
//				//---обработка ошибок----
////				if(CRC_ERR)
//				{
//					//написать обработку ошибок
//
////					CRC_ERR=0x0;	//ошибка lrc
//				}
//
////				if(COMMAND_ERR)
//				{
//
////					COMMAND_ERR=0x0;//неподдерживаемая команда	
//				}
//				//-----------------------
//				
//				
//				PROTO_STATE=PROTO_RESTART;		//иначе рестарт и ждем новой посылки			
//		}
//		break;
////------------------------------------------
//		default:
//		{
//			PROTO_STATE==PROTO_RESTART;
//		}
//		break;
//	}
//
//	return;
//}
//-----------------------CRC------------------------------------------------------------
static unsigned char  CRC_Check( unsigned char xdata *Spool_pr,unsigned char Count_pr )  //проверить
{
    unsigned char   idata CRC = 0;
	 	unsigned char xdata *Spool; 
	 	unsigned char idata Count ;
	
	Spool=Spool_pr;
	Count=Count_pr;

  		while(Count!=0x0)
        {
	        CRC = CRC ^ (*Spool++);//
	        // циклический сдвиг вправо
	        CRC = ((CRC & 0x01) ? (unsigned char)0x80: (unsigned char)0x00) | (unsigned char)(CRC >> 1);
	        // инверсия битов с 2 по 5, если бит 7 равен 1
	        if (CRC & (unsigned char)0x80) CRC = CRC ^ (unsigned char)0x3C;
			Count--;
        }
    return CRC;
}

//-----------------------------------------------------------------------------------------------

