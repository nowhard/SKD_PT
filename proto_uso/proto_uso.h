#ifndef PROTO_USO_H
#define PROTO_USO_H

#include <ADuC845.h>

#include "adc.h"
#include "calibrate/calibrate.h"
#include "pt/pt.h"

#include <string.h>
#include "led_process.h"
#define HI(x) ((x)>>8)
#define LO(x) ((x)&0xFF)

//#define ADRESS_DEV 			0x1 //адрес устройства
//#define CHANNEL_NUMBER	8//количество каналов


//--------------------------------------------------------------------------------
#define MAX_LENGTH_REC_BUF 	128 //максимальная длина принимаемого кадра
#define MIN_LENGTH_REC_BUF	5 //минимальная длина принимаемого кадра

#define MAX_LENGTH_TR_BUF  	128 //максимальная длина передаваемого кадра
#define CRC_LEN				1 //длина поля CRC
//-------------------------код операции-------------------------------------------
#define  GET_DEV_INFO_REQ 				0x1 //получить информацию об устройстве	(код запроса)
#define  GET_DEV_INFO_RESP				0x2	//получить информацию об устройстве	(код ответа)

#define  NODE_FULL_INIT_REQ				0x3	//полная инициализация узла
#define  NODE_FULL_INIT_RESP			//полная инициализация узла-нет ответа

#define  CHANNEL_LIST_INIT_REQ			0x5 //Инициализация списка каналов узла (без потери данных);
#define  CHANNEL_LIST_INIT_RESP			//Инициализация списка каналов узла (без потери данных); нет ответа

#define  CHANNEL_GET_DATA_REQ			0x7// Выдать данные по каналам, согласно абсолютной нумерации;
#define  CHANNEL_GET_DATA_RESP			0x8// Выдать данные по каналам, согласно абсолютной нумерации;

#define  CHANNEL_SET_PARAMETERS_REQ  	0x9	//Установить параметры по каналам, согласно абсолютной нумерации;	
#define  CHANNEL_SET_PARAMETERS_RESP  	//Установить параметры по каналам, согласно абсолютной нумерации; нет ответа

#define  CHANNEL_SET_ORDER_QUERY_REQ   	0xA//Задать последовательность опроса;
#define  CHANNEL_SET_ORDER_QUERY_RESP 	//Задать последовательность опроса; нет ответа

#define  CHANNEL_GET_DATA_ORDER_REQ		0xC	//Выдать данные по каналам, согласно последовательности опроса;
#define  CHANNEL_GET_DATA_ORDER_RESP	0xD	//Выдать данные по каналам, согласно последовательности опроса;

#define  CHANNEL_SET_STATE_REQ		   	0xE//Установить состояния по каналам, согласно абсолютной нумерации;
#define  CHANNEL_SET_STATE_RESP			   //Установить состояния по каналам, согласно абсолютной нумерации; нет ответа

#define CHANNEL_GET_DATA_ORDER_M2_REQ	0x10 //Выдать данные по каналам, согласно последовательности опроса;
#define CHANNEL_GET_DATA_ORDER_M2_RESP	0x11//Выдать данные по каналам, согласно последовательности опроса;

#define CHANNEL_SET_RESET_STATE_FLAGS_REQ  0x12//Установка/Сброс флагов состояния 
#define CHANNEL_SET_RESET_STATE_FLAGS_RESP //Установка/Сброс флагов состояния -нет ответа

#define  CHANNEL_ALL_GET_DATA_REQ		0x14 //Выдать информацию по всем каналам узла (расширенный режим);
#define  CHANNEL_ALL_GET_DATA_RESP		0x15 //Выдать информацию по всем каналам узла (расширенный режим);

#define  CHANNEL_SET_ADDRESS_DESC		0xCD //установить новый адрес устройства, имя, описание, версию прошивки и комментарий

#define  CHANNEL_SET_CALIBRATE			0xCA//установить верхнюю или нижнюю точку двухточечной калибровки

#define  REQUEST_ERROR					0xFF//Ошибочный запрос/ответ;

//-------------------------коды сбойных ситуаций-------------------------------------------
#define	FR_SUCCESFUL 								0x0//Нет ошибки (используется для подтверждения)  
#define	FR_UNATTENDED_CHANNEL 						0x1//В запросе задан канал, не обслуживаемый измерительным модулем;
#define	FR_SET_UNATTENDED_CHANNEL					0x2//В заданной последовательности опроса задан канал, не обслуживаемый из-мерительным модулем (выдается в ответ на неверную команду 0Ah);
#define	FR_ORDER_NOT_SET							0x3//Последовательность опроса не задана (выдается в ответ на неверную команду 0Ch);
#define	FR_CHNL_NOT_IN_ORDER_REQ					0x4//В запросе присутствует канал, не заданный в последовательности опроса (выдается в ответ на неверную команду 0Ch);
#define	FR_FALSE_TYPE_CHNL_DATA						0x5//Задаваемая информация по каналу к данному типу канала относиться не может (выдается в ответ на неверную команду 09h или 0Eh);
#define	FR_COMMAND_NOT_EXIST						0x6//Несуществующая команда;
#define	FR_CHNL_TYPE_NOT_EXIST						0x7//Несуществующий тип/модификация канала;
#define	FR_COMMAND_NOT_SUPPORT						0x8//Команда не поддерживается;
#define	FR_COMMAND_STRUCT_ERROR						0x9//Ошибка в структуре команды;
#define	FR_FRAME_LENGTH_ERROR						0xA//Несоответствие длины кадра и количества каналов;
#define	FR_CHANNEL_NUM_TOO_MUCH						0xC//слишком много каналов в кадре.
#define	FR_CHNL_TYPE_ERROR							0xD//Несоответствие типа канала
#define	FR_CHNL_NOT_EXIST							0xF//Отсутствующий канал
//--------------------------состояния цифрового автомата--------------
#define PROTO_RESTART		0	//рестарт цикла и обнуление
#define PROTO_WAIT 			1	//ожидание данных
#define	PROTO_PLACE_BUF		2	//прием данных в буфер
#define	PROTO_ADDR_CHECK	3	//проверка адреса
#define	PROTO_CRC_CHECK		4	//проверка контрольной суммы	
#define	PROTO_BUF_HANDLING	5	//обработка команды
#define PROTO_BUF_TRANSFER	6	//передача ответа ведущему
#define	PROTO_ERR_HANDLING	7	//обработка ошибок
//--------------------------------------------------------------------
//--------------------------------прототипы---------------------------
void UART_ISR(void); //обработчик прерывания уарт
//void TIC_ISR(void);//обработчик прерывания TIC для измерения интервала таймаута 

void Protocol_Init(void); //инициализация протокола

unsigned char Send_Info(void);     //посылка информации об устройстве
unsigned char Node_Full_Init(void);//полная инициализация узла
unsigned char Channel_List_Init(void);//Инициализация списка каналов узла (без потери данных);
unsigned char Channel_Get_Data(void);//Выдать данные по каналам, согласно абсолютной нумерации;
unsigned char Channel_Set_Parameters(void);//Установить параметры по каналам, согласно абсолютной нумерации;
unsigned char Channel_Set_Order_Query(void);//Задать последовательность опроса;
unsigned char Channel_Get_Data_Order(void);//Выдать данные по каналам, согласно последовательности опроса;
unsigned char Channel_Set_State(void);//Установить состояния по каналам, согласно абсолютной нумерации;
unsigned char Channel_Get_Data_Order_M2(void);//Выдать данные по каналам, согласно последовательности опроса;
unsigned char Channel_Set_Reset_State_Flags(void);//	Установка/Сброс флагов состояния 
unsigned char Channel_All_Get_Data(void);//Выдать информацию по всем каналам узла (расширенный режим);
unsigned char Channel_Set_Address_Desc(void);//установить новый адрес устройства, имя, описание, версию прошивки и комментарий
unsigned char Channel_Set_Calibrate(void);//установить верхнюю или нижнюю точку калибровки
unsigned char Request_Error(unsigned char error_code);//	Ошибочный запрос/ответ;


void ProtoBufHandling(void); //процесс обработки принятого запроса
//void ProtoProcess(void);//главный процесс протокола 
PT_THREAD(ProtoProcess(struct pt *pt));
static unsigned char  CRC_Check( unsigned char xdata *Spool,unsigned char Count);//расчет CRC

void Store_Dev_Address_Desc(unsigned char addr,void* name,void* ver,void* desc,unsigned char desc_len);//сохранить в ППЗУ новый адрес устройства, имя, версию, описание
void Restore_Dev_Address_Desc(void);//восстановить из ппзу адрес и информацию об устройстве
 //------------------------------------------------------------------------------
// extern struct Channel xdata channels[CHANNEL_NUMBER];//обобщенная структура каналов
 //extern struct ADC_Channels xdata adc_channels[ADC_CHANNELS_NUM]; //каналы ацп
//--------------------------------------------------------------------
#endif