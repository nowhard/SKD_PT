#include "i2c.h"
//-------------------------------------------------------------
volatile struct I2C_STATE  xdata i2c_st;//структура состояний
volatile unsigned char xdata delay_counter=0;//счетчик задержки
volatile unsigned char xdata DEV_ADDR;
volatile unsigned char  xdata transfer_byte=0;
volatile unsigned char xdata recieve_byte=0;
volatile unsigned char xdata recieve_byte_counter=0;
volatile unsigned char xdata transfer_byte_counter=0;
volatile unsigned char xdata bit_counter=0;
volatile unsigned char xdata byte_num=0;//количество байт в массиве
volatile unsigned char xdata byte_num2=0;//количество байт в массиве(при вторичном старте)
volatile unsigned char *i2c_buf;//
volatile unsigned char *i2c_buf2;//иногда используется при двойном старте
volatile unsigned char xdata rec_tr_flag=0;//передача или прием
volatile unsigned char xdata second_start=0;
volatile unsigned char xdata repeated_strt_read=0;
volatile unsigned char xdata data_transfered_flag=0,byte_transfered_flag=0;

volatile unsigned char xdata operation_status=0;//статус операции-0-выполнено, 1 не выполнено

volatile unsigned char xdata op_flag=0;

volatile unsigned char i2c_mas[10];

sbit LED1=P3^7;
//-------------------------------------------------------------

void I2C_Init(void)
{
	I2CM=1;//ведущий
	i2c_st.GLOBAL_STATE=I2C_RESTART;

	setHandler(I2C_IS_WRITE_MSG , &I2C_Is_Write);
	setHandler(I2C_IS_READ_MSG , &I2C_Is_Read);

	return;
}
//-------------------------------------------------------------
void I2C_Write_Buf(unsigned char addr,unsigned char *buf,unsigned char buf_len) //запись буфера в шину
{
	//---------обнулим автоматы-------------
	 i2c_st.START_STATE=0;
	 i2c_st.STOP_STATE=0;
	 i2c_st.RESTART_STATE=0;
	 i2c_st.SET_ACK_STATE=0;
	 i2c_st.GET_ACK_STATE=0;
	 i2c_st.SET_NACK_STATE=0;
	 i2c_st.TRANSFER_BYTE_STATE=0;
	 i2c_st.RECIEVE_BYTE_STATE=0;
	//-----------------------
	
	rec_tr_flag=0;//на передачу
	byte_num=buf_len;
	i2c_buf=buf;
	transfer_byte=(addr<<1)&0xFE;//первым байтом передаем адрес
	i2c_st.GLOBAL_STATE=I2C_START;
	return;	
}
//-------------------------------------------------------------
void I2C_Read_Buf(unsigned char addr,unsigned char *buf,unsigned char buf_len)  //чтение из шины в буфер
{

	//----------обнулим автоматы------------
	 i2c_st.START_STATE=0;
	 i2c_st.STOP_STATE=0;
	 i2c_st.RESTART_STATE=0;
	 i2c_st.SET_ACK_STATE=0;
	 i2c_st.GET_ACK_STATE=0;
	 i2c_st.SET_NACK_STATE=0;
	 i2c_st.TRANSFER_BYTE_STATE=0;
	 i2c_st.RECIEVE_BYTE_STATE=0;
	//-----------------------
  	rec_tr_flag=1;//на прием
	byte_num=buf_len;
	i2c_buf=buf;
	transfer_byte=(addr<<1)&0xFE|1;//0x80;//первым байтом передаем адрес
	i2c_st.GLOBAL_STATE=I2C_START;
	return;
}
//-------------------------------------------------------------
void I2C_Repeat_Start_Read(unsigned char addr,unsigned char *par_buf,unsigned char par_buf_len,unsigned char *buf,unsigned char buf_len)//запрос с двумя стартами
{
	//----------обнулим автоматы------------
	 i2c_st.START_STATE=0;
	 i2c_st.STOP_STATE=0;
	 i2c_st.RESTART_STATE=0;
	 i2c_st.SET_ACK_STATE=0;
	 i2c_st.GET_ACK_STATE=0;
	 i2c_st.SET_NACK_STATE=0;
	 i2c_st.TRANSFER_BYTE_STATE=0;
	 i2c_st.RECIEVE_BYTE_STATE=0;
	//-----------------------

	repeated_strt_read=1;	//с повтором

	DEV_ADDR=(addr<<1)&0xFE;
	transfer_byte=DEV_ADDR;//первым байтом передаем адрес

	rec_tr_flag=0;//на передачу
	
	byte_num=par_buf_len;
	byte_num2=buf_len;
	
	i2c_buf=par_buf;
	i2c_buf2=buf;

	i2c_st.GLOBAL_STATE=I2C_START;
	return;
}
//-------------------------------------------------------------
void I2C_Process(void)	//процесс автомата состояний
{
	switch(i2c_st.GLOBAL_STATE)
	{
//-------------------------------------------------------------
		case I2C_START:	   //full
		{
			MDE=1;	//software output enable
			//puts("I2C_START\n");
			switch(i2c_st.START_STATE) //разделим старт на несколько состояний
			{
//--------------
				case 0:
				{
					MDO=1;	   // START
					MCO=1;	   //clock out

					//transfer_byte= DEV_ADDR;//	первым передадим адрес

					i2c_st.LAST_GLOBAL_STATE=i2c_st.GLOBAL_STATE;//сохраняем состояние
					delay_counter=1;
					i2c_st.GLOBAL_STATE=I2C_DELAY;
					i2c_st.START_STATE=1;//
				}
				break;
//--------------
				case 1:
				{
					MDO=0;


					//transfer_byte= DEV_ADDR;//	первым передадим адрес

					i2c_st.LAST_GLOBAL_STATE=i2c_st.GLOBAL_STATE;//сохраняем состояние
					delay_counter=1;
					i2c_st.GLOBAL_STATE=I2C_DELAY;
					i2c_st.START_STATE=2;//
				}
				break;
//--------------
				case 2:
				{
					
					MCO=0;	   //------
					i2c_st.START_STATE=0;//
					//if(!second_start) //повторный старт
					{
						i2c_st.GLOBAL_STATE=I2C_TRANSFER_BYTE;
						 //second_start=1;
					}
				/*	else
					{
						i2c_st.GLOBAL_STATE=I2C_RECIEVE_BYTE;
						//second_start=1;	
					}  */
					//debug
					i2c_st.LAST_GLOBAL_STATE=i2c_st.GLOBAL_STATE;//сохраняем состояние
					delay_counter=1;
					i2c_st.GLOBAL_STATE=I2C_DELAY;
					//debug
				}
				break;
//--------------				
			}
		}
		break;
 //-------------------------------------------------------------
		case I2C_STOP:
		{
		//	//puts("I2C_IN_STOP\n") ;
		
				MDE=1;	//software output enable
				//puts("I2C_STOP\n");
				switch(i2c_st.STOP_STATE) //разделим старт на несколько состояний
				{
	//--------------
					case 0:
					{
						MDO=0;	  // STOP  ???
						MCO=0;
	
						i2c_st.LAST_GLOBAL_STATE=i2c_st.GLOBAL_STATE;//сохраняем состояние
						delay_counter=1;
						i2c_st.GLOBAL_STATE=I2C_DELAY;
						i2c_st.STOP_STATE=1;//
					}
					break;
	//--------------
						case 1:
					{

						MCO=1;
						i2c_st.LAST_GLOBAL_STATE=i2c_st.GLOBAL_STATE;//сохраняем состояние
						delay_counter=1;
						i2c_st.GLOBAL_STATE=I2C_DELAY;
						i2c_st.STOP_STATE=2;//
					}
					break;
	//--------------
					case 2:
					{	
						
						
						MDO=1;
								///???
							  //-----	   //------
						i2c_st.STOP_STATE=0;//
						i2c_st.GLOBAL_STATE=I2C_RESTART;//реинициализация всех переменных
						//послать сообщение
						if(!operation_status)//если операция была успешной
						{
							if(!rec_tr_flag)//если передаем, то посылаем сообщение, что передано 
							{
								sendMessage(I2C_IS_WRITE_MSG,0);
							}
							else //если принимаем, посылаем сообщение, что принято
							{
								sendMessage(I2C_IS_READ_MSG,0);
							}
						}
					}
					break;
	//--------------
				}
		}
		break;
//-------------------------------------------------------------
		case I2C_REPEAT_START:
		{
		   		//puts("I2C_REPEAT_START\n");
			  	repeated_strt_read=0;
				second_start=1;

				rec_tr_flag=1;//на прием
				byte_num=byte_num2;
				i2c_buf=i2c_buf2;
				transfer_byte=DEV_ADDR|1;//0x80;//первым байтом передаем адрес
				i2c_st.GLOBAL_STATE=I2C_START;

				recieve_byte_counter=0;
			 	transfer_byte_counter=0;
			 	bit_counter=0;
		}
		break;
//-------------------------------------------------------------
		case I2C_SET_ACK:  // full
		{
			MDE=1;	//software output enable
		//	//puts("I2C_SET_ACK\n");
			//--------------
			switch(i2c_st.SET_ACK_STATE) //разделим  на несколько состояний
			{
				case 0:
				{
				//	MDE=1; // отправка положительной квитанции
					MDO=0;
					MCO=1;	  // 		

					i2c_st.LAST_GLOBAL_STATE=i2c_st.GLOBAL_STATE;//сохраняем состояние
					delay_counter=5;
					i2c_st.GLOBAL_STATE=I2C_DELAY;
					i2c_st.SET_ACK_STATE=1;//
				}
				break;
//--------------
				case 1:
				{
					
					MCO=0;
					MDO=1;	  
					i2c_st.SET_ACK_STATE=0;//

					//if(rec_tr_flag)	//если прием
					//{
					i2c_st.GLOBAL_STATE=I2C_RECIEVE_BYTE;// следующий байт
					//}

					//debug
					i2c_st.LAST_GLOBAL_STATE=i2c_st.GLOBAL_STATE;//сохраняем состояние
					delay_counter=5;
					i2c_st.GLOBAL_STATE=I2C_DELAY;
					//debug
				}
				break;
			}
		}
		break;
//-------------------------------------------------------------
		case I2C_SET_NACK:	  //full
		{
			MDE=1;	//software output enable
		//	//puts("I2C_SET_NACK\n");
//--------------
			switch(i2c_st.SET_NACK_STATE) //разделим старт на несколько состояний
			{
				case 0:
				{
				//	MDE=1; // отправка jnhbw квитанции
					MDO=1;	  // 		
					MCO=1;
					i2c_st.LAST_GLOBAL_STATE=i2c_st.GLOBAL_STATE;//сохраняем состояние
					delay_counter=1;
					i2c_st.GLOBAL_STATE=I2C_DELAY;
					i2c_st.SET_NACK_STATE=1;//
				}
				break;
//--------------
				case 1:
				{
					MCO=0;
					MDO=0;
						  
					i2c_st.SET_NACK_STATE=0;//

					//if(rec_tr_flag)	//если прием
					//{
					i2c_st.GLOBAL_STATE=I2C_STOP;//останов
					//}

					//debug
					i2c_st.LAST_GLOBAL_STATE=i2c_st.GLOBAL_STATE;//сохраняем состояние
					delay_counter=1;
					i2c_st.GLOBAL_STATE=I2C_DELAY;
					//debug
				}
				break;
			}	
		}
		break;
//-------------------------------------------------------------
		case I2C_TRANSFER_BYTE:	 //передаем побитно байт  //full
		{
			MDE=1;
			//puts("I2C_TRANSFER_BYTE\n");
		   	switch(i2c_st.TRANSFER_BYTE_STATE) //разделим старт на несколько состояний
			{
//--------------
				case 0:
				{	
					//puts("I2C_TRANSFER_BYTE1\n");
										
					if(bit_counter>=8)	//байт передан, берем следующий, а если закончились все, переходим к завершению
					{
					//	byte_transfered_flag=1;//данные переданы
						bit_counter=0;
						//if(rec_tr_flag)
						//{				
						transfer_byte=i2c_buf[transfer_byte_counter];
						transfer_byte_counter++;
						i2c_st.GLOBAL_STATE=I2C_GET_ACK;
						//byte_transfered_flag=0;
						//debug
						i2c_st.LAST_GLOBAL_STATE=i2c_st.GLOBAL_STATE;//сохраняем состояние
						delay_counter=1;
						i2c_st.GLOBAL_STATE=I2C_DELAY;
						//debug
						//}
					}
					else
					{
						transfer_byte=transfer_byte<<1;//>>1;	 //???
						MDO=CY;
						
						i2c_st.LAST_GLOBAL_STATE=i2c_st.GLOBAL_STATE;//сохраняем состояние
						delay_counter=1;
						i2c_st.GLOBAL_STATE=I2C_DELAY;
	
						bit_counter++;
	
						i2c_st.TRANSFER_BYTE_STATE=1;//
	
	
							MCO=1;//clock вверх	
					}
				}
				break;
//--------------
				case 1:
				{

			  //debug
				   //debug
				//puts("I2C_TRANSFER_BYTE2\n");

						if(MCO)	  // slave прижал тактирование к земле, ждем
						{
							MCO=0;
							i2c_st.TRANSFER_BYTE_STATE=0;
						}
										//debug
					i2c_st.LAST_GLOBAL_STATE=i2c_st.GLOBAL_STATE;//сохраняем состояние
					delay_counter=1;
					i2c_st.GLOBAL_STATE=I2C_DELAY;
					//debug
				}
				break;
//--------------
			}
		}
		break;
//-------------------------------------------------------------
		case I2C_RECIEVE_BYTE:	 //состояние приема байта	//full
		{
		   	MDE=0;
			//puts("I2C_RECIEVE_BYTE\n");
			switch(i2c_st.RECIEVE_BYTE_STATE) //разделим старт на несколько состояний
			{
//--------------
				case 0:	 //	 поднимаем clock и ждем
				{	
					MCO=1;
					i2c_st.LAST_GLOBAL_STATE=i2c_st.GLOBAL_STATE;//сохраняем состояние
					delay_counter=1;
					i2c_st.GLOBAL_STATE=I2C_DELAY;
					i2c_st.RECIEVE_BYTE_STATE=1;
				}
				break;
//--------------

				case 1:
				{
					
					MCO=0;//
					recieve_byte=(recieve_byte<<1)|MDI;
					bit_counter++;
					i2c_st.RECIEVE_BYTE_STATE=0;
					if(bit_counter>=8)	//получил байт-передай ACK!
					{
						 i2c_buf[recieve_byte_counter]=recieve_byte;
						 recieve_byte_counter++;
						 bit_counter=0;
						 if(recieve_byte_counter>=byte_num)
						 {
						 	 i2c_st.GLOBAL_STATE=I2C_SET_NACK;	//если все байты приняты -отправим NACK
						 }
						 else
						 {										// иначе ACKуем
						 	 i2c_st.GLOBAL_STATE=I2C_SET_ACK;
						 }
						//debug
						i2c_st.LAST_GLOBAL_STATE=i2c_st.GLOBAL_STATE;//сохраняем состояние
						delay_counter=1;
						i2c_st.GLOBAL_STATE=I2C_DELAY;
						//debug
					}
				}
				break;
//--------------
			}
		}
		break;
//-------------------------------------------------------------
		case I2C_DELAY:	 //состояние задержки //full
		{
			//puts("I2C_DELAY\n");
			delay_counter--;
			if(!delay_counter)
			{
				i2c_st.GLOBAL_STATE=i2c_st.LAST_GLOBAL_STATE ;
			}
		}
		break;
//-------------------------------------------------------------
		case I2C_GET_ACK:  //full  ???
		{
		   	MDE=0;		   // получение квитанции
			//puts("I2C_GET_ACK\n");
			switch(i2c_st.GET_ACK_STATE) //разделим  на несколько состояний
			{
//--------------
				case 0:
				{
					MCO=1;
					i2c_st.LAST_GLOBAL_STATE=i2c_st.GLOBAL_STATE;//сохраняем состояние
					delay_counter=1;
					i2c_st.GLOBAL_STATE=I2C_DELAY;
					i2c_st.GET_ACK_STATE=1;//
					
				}
				break;
//--------------
				case 1:
				{
					MCO=0;
					if(!MDI) //ACK
					{
						//printf("SLAVE: ACK , %u bit\n",(unsigned int)bit_counter);
						if(!rec_tr_flag)//если передаем буфер
						{
							if(transfer_byte_counter>byte_num)//если все данные переданы, идем на стоп
							{
								if(!repeated_strt_read)
								{
									i2c_st.GLOBAL_STATE= I2C_STOP;
									
								}
								else
								{
									i2c_st.GLOBAL_STATE=I2C_REPEAT_START;
								} 
								//data_transfered_flag=1;//все данные переданы	
							}
							else//если не все-идем на передачу
							{
							//printf("symbols %u = %u \n",(unsigned int)transfer_byte_counter,(unsigned int)byte_num) ;
								i2c_st.GLOBAL_STATE= I2C_TRANSFER_BYTE;
							}	
						}
						else//если принимаем буфер
						{
							  bit_counter=0;
							  i2c_st.GLOBAL_STATE= I2C_RECIEVE_BYTE; //идем на прием байтов
						}						

						MDE=1;
					}
					else  //NACK
					{
						//printf("SLAVE: NACK , %u bit\n",(unsigned int)bit_counter);
						i2c_st.GLOBAL_STATE=I2C_STOP;
						operation_status=1;//операция завершена неуспешно	
					}
					i2c_st.GET_ACK_STATE=0;//

					//debug
					i2c_st.LAST_GLOBAL_STATE=i2c_st.GLOBAL_STATE;//сохраняем состояние
					delay_counter=10;
					i2c_st.GLOBAL_STATE=I2C_DELAY;
					//debug
				}
				break;
//--------------					 	
			}
		}
		break;
//-------------------------------------------------------------
		case I2C_WAIT_FOR_IO:  //full
		{
			//пусто
		}
		break;
//-------------------------------------------------------------
		case I2C_RESTART: //рестарт мастера
		{
			//реинициализация переменных...
			 //transfer_byte=0;
			 //puts("I2C_RESTART\n");
			 recieve_byte=0;
			 recieve_byte_counter=0;
			 transfer_byte_counter=0;
			 bit_counter=0;
			 second_start=0;
			 data_transfered_flag=0;
			 byte_transfered_flag=0;
			 repeated_strt_read=0;
			 //i2c_st.LAST_GLOBAL_STATE=I2C_WAIT_FOR_IO;

			 i2c_st.START_STATE=0;
			 i2c_st.STOP_STATE=0;
			 i2c_st.RESTART_STATE=0;
			 i2c_st.SET_ACK_STATE=0;
			 i2c_st.GET_ACK_STATE=0;
			 i2c_st.SET_NACK_STATE=0;
			 i2c_st.TRANSFER_BYTE_STATE=0;
			 i2c_st.RECIEVE_BYTE_STATE=0;
			//--------------------------------------
			operation_status=0;
			i2c_st.GLOBAL_STATE=I2C_WAIT_FOR_IO;//пойдем на ожидание
		}
		break;
//-------------------------------------------------------------
	   default:
	   {
	   		i2c_st.GLOBAL_STATE=I2C_RESTART;
	   }
	   break;
	}

return;
}
//---------------------------------------------------------------------------------------------
void I2C_Is_Write(void)//если запись прошла успешно
{
   //op_flag++;	
   //puts("Write succesfull\n");

return;
}
//---------------------------------------------------------------------------------------------
void I2C_Is_Read(void)	//если чтение прошло успешно
{
 	channels[11].channel_data=(unsigned long)(i2c_mas[0])|(unsigned long)(i2c_mas[1])<<8|(unsigned long)(i2c_mas[2])<<16|(unsigned long)(i2c_mas[3])<<24;
	return;
}
//---------------------------------------------------------------------------------------------