#ifndef PROTO_USO_H
#define PROTO_USO_H

#include <ADuC845.h>
#include "adc.h"
#include <string.h>

#define HI(x) ((x)>>8)
#define LO(x) ((x)&0xFF)

#include "pt/pt.h"
//#define ADRESS_DEV 			0x1 //����� ����������
//#define CHANNEL_NUMBER	8//���������� �������


//--------------------------------------------------------------------------------
#define MAX_LENGTH_REC_BUF 	128 //������������ ����� ������������ �����
#define MIN_LENGTH_REC_BUF	5 //����������� ����� ������������ �����

#define MAX_LENGTH_TR_BUF  	128 //������������ ����� ������������� �����
#define CRC_LEN				1 //����� ���� CRC
//-------------------------��� ��������-------------------------------------------
#define  GET_DEV_INFO_REQ 				0x1 //�������� ���������� �� ����������	(��� �������)
#define  GET_DEV_INFO_RESP				0x2	//�������� ���������� �� ����������	(��� ������)

#define  NODE_FULL_INIT_REQ				0x3	//������ ������������� ����
#define  NODE_FULL_INIT_RESP			//������ ������������� ����-��� ������

#define  CHANNEL_LIST_INIT_REQ			0x5 //������������� ������ ������� ���� (��� ������ ������);
#define  CHANNEL_LIST_INIT_RESP			//������������� ������ ������� ���� (��� ������ ������); ��� ������

#define  CHANNEL_GET_DATA_REQ			0x7// ������ ������ �� �������, �������� ���������� ���������;
#define  CHANNEL_GET_DATA_RESP			0x8// ������ ������ �� �������, �������� ���������� ���������;

#define  CHANNEL_SET_PARAMETERS_REQ  	0x9	//���������� ��������� �� �������, �������� ���������� ���������;	
#define  CHANNEL_SET_PARAMETERS_RESP  	//���������� ��������� �� �������, �������� ���������� ���������; ��� ������

#define  CHANNEL_SET_ORDER_QUERY_REQ   	0xA//������ ������������������ ������;
#define  CHANNEL_SET_ORDER_QUERY_RESP 	//������ ������������������ ������; ��� ������

#define  CHANNEL_GET_DATA_ORDER_REQ		0xC	//������ ������ �� �������, �������� ������������������ ������;
#define  CHANNEL_GET_DATA_ORDER_RESP	0xD	//������ ������ �� �������, �������� ������������������ ������;

#define  CHANNEL_SET_STATE_REQ		   	0xE//���������� ��������� �� �������, �������� ���������� ���������;
#define  CHANNEL_SET_STATE_RESP			   //���������� ��������� �� �������, �������� ���������� ���������; ��� ������

#define CHANNEL_GET_DATA_ORDER_M2_REQ	0x10 //������ ������ �� �������, �������� ������������������ ������;
#define CHANNEL_GET_DATA_ORDER_M2_RESP	0x11//������ ������ �� �������, �������� ������������������ ������;

#define CHANNEL_SET_RESET_STATE_FLAGS_REQ  0x12//���������/����� ������ ��������� 
#define CHANNEL_SET_RESET_STATE_FLAGS_RESP //���������/����� ������ ��������� -��� ������

#define  CHANNEL_ALL_GET_DATA_REQ		0x14 //������ ���������� �� ���� ������� ���� (����������� �����);
#define  CHANNEL_ALL_GET_DATA_RESP		0x15 //������ ���������� �� ���� ������� ���� (����������� �����);

#define  CHANNEL_SET_DISCRET_OUT_REQ	0x16//���������� �������� ������
#define  CHANNEL_SET_DISCRET_OUT_RESP	0x17

#define  TIMER_SET_TIME_REQ				0x18//��������� ��������� ����� ��������� �������
#define  TIMER_SET_TIME_RESP			0x19

#define  TIMER_GET_TIME_REQ				0x1A//��������� ��������� ����� ��������� �������
#define  TIMER_GET_TIME_RESP			0x1B

#define  MEMORY_WRITE_BUF_REQ			0x1C// �������� ����� � I2C ������
#define  MEMORY_WRITE_BUF_RESP			0x1D

#define  MEMORY_READ_BUF_REQ			0x1E// ������� ����� �� I2C ������
#define  MEMORY_READ_BUF_RESP			0x1F

#define  CHANNEL_SET_ADDRESS_DESC		0xCD //���������� ����� ����� ����������, ���, ��������, ������ �������� � �����������

#define  CHANNEL_SET_CALIBRATE			0xCA//���������� ������� ��� ������ ����� ������������ ����������

#define  REQUEST_ERROR					0xFF//��������� ������/�����;

//-------------------------���� ������� ��������-------------------------------------------
#define	FR_SUCCESFUL 								0x0//��� ������ (������������ ��� �������������)  
#define	FR_UNATTENDED_CHANNEL 						0x1//� ������� ����� �����, �� ������������� ������������� �������;
#define	FR_SET_UNATTENDED_CHANNEL					0x2//� �������� ������������������ ������ ����� �����, �� ������������� ��-����������� ������� (�������� � ����� �� �������� ������� 0Ah);
#define	FR_ORDER_NOT_SET							0x3//������������������ ������ �� ������ (�������� � ����� �� �������� ������� 0Ch);
#define	FR_CHNL_NOT_IN_ORDER_REQ					0x4//� ������� ������������ �����, �� �������� � ������������������ ������ (�������� � ����� �� �������� ������� 0Ch);
#define	FR_FALSE_TYPE_CHNL_DATA						0x5//���������� ���������� �� ������ � ������� ���� ������ ���������� �� ����� (�������� � ����� �� �������� ������� 09h ��� 0Eh);
#define	FR_COMMAND_NOT_EXIST						0x6//�������������� �������;
#define	FR_CHNL_TYPE_NOT_EXIST						0x7//�������������� ���/����������� ������;
#define	FR_COMMAND_NOT_SUPPORT						0x8//������� �� ��������������;
#define	FR_COMMAND_STRUCT_ERROR						0x9//������ � ��������� �������;
#define	FR_FRAME_LENGTH_ERROR						0xA//�������������� ����� ����� � ���������� �������;
#define	FR_CHANNEL_NUM_TOO_MUCH						0xC//������� ����� ������� � �����.
#define	FR_CHNL_TYPE_ERROR							0xD//�������������� ���� ������
#define	FR_CHNL_NOT_EXIST							0xF//������������� �����
//--------------------------------���������---------------------------
void UART_ISR(void); //���������� ���������� ����
//void TIC_ISR(void);//���������� ���������� TIC ��� ��������� ��������� �������� 

void Protocol_Init(void); //������������� ���������

unsigned char Send_Info(void);     //������� ���������� �� ����������
//unsigned char Node_Full_Init(void);//������ ������������� ����
unsigned char Channel_List_Init(void);//������������� ������ ������� ���� (��� ������ ������);
//unsigned char Channel_Get_Data(void);//������ ������ �� �������, �������� ���������� ���������;
unsigned char Channel_Set_Parameters(void);//���������� ��������� �� �������, �������� ���������� ���������;
unsigned char Channel_Set_Order_Query(void);//������ ������������������ ������;
unsigned char Channel_Get_Data_Order(void);//������ ������ �� �������, �������� ������������������ ������;
unsigned char Channel_Set_State(void);//���������� ��������� �� �������, �������� ���������� ���������;
unsigned char Channel_Get_Data_Order_M2(void);//������ ������ �� �������, �������� ������������������ ������;
unsigned char Channel_Set_Reset_State_Flags(void);//	���������/����� ������ ��������� 
unsigned char Channel_All_Get_Data(void);//������ ���������� �� ���� ������� ���� (����������� �����);

unsigned char Request_Error(unsigned char error_code);//	��������� ������/�����;

PT_THREAD(ProtoProcess(struct pt *pt));

static unsigned char  CRC_Check( unsigned char xdata *Spool,unsigned char Count);//������ CRC

//--------------------------------------------------------------------

//---------------------------------------------------------------------
#endif