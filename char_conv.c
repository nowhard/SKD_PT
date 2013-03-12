#include "char_conv.h"

//-----------------------------------------------------------------------------------------------
unsigned int Sym_4_To_Int(unsigned char * pucFrame)//перевод 4х символов в массиве в целое	int
{
	union
	{
		unsigned int result_int;
		unsigned char result_char[2];
	}
	sym_4_to_int;
	sym_4_to_int.result_char[0]=(unsigned char)MBCHAR2BIN(pucFrame[0])<<4 | (unsigned char)MBCHAR2BIN(pucFrame[1]);
	sym_4_to_int.result_char[1]=(unsigned char)MBCHAR2BIN(pucFrame[2])<<4 | (unsigned char)MBCHAR2BIN(pucFrame[3]);
	
	return sym_4_to_int.result_int;	
}
//-----------------------------------------------------------------------------------------------
unsigned char Sym_2_To_Char(unsigned char * pucFrame)//перевод 2х символов в массиве в целое char
{
	unsigned char result_char;

	result_char= (unsigned char)MBCHAR2BIN(pucFrame[0])<<4 | (unsigned char)MBCHAR2BIN(pucFrame[1]);
	return result_char; 
}
//-----------------------------------------------------------------------------------------------
void Int_To_Sym_4(unsigned int val, unsigned char* buf)//перевод целого значения int в массив из 4х символов
{
	union
	{
		unsigned int int_num;
		unsigned char result_char[2];
	}
	int_to_sym_4;
	
	int_to_sym_4.int_num=val;

	buf[1]=MBBIN2CHAR(int_to_sym_4.result_char[0]&0xF);
	buf[0]=MBBIN2CHAR((int_to_sym_4.result_char[0]>>4)&0xF);
	buf[3]=MBBIN2CHAR(int_to_sym_4.result_char[1]&0xF);
	buf[2]=MBBIN2CHAR((int_to_sym_4.result_char[1]>>4)&0xF);

	return;
}
//-----------------------------------------------------------------------------------------------
void Char_To_Sym_2(unsigned char val, unsigned char* buf)//перевод целого значения char в массив из 2х символов
{	
	buf[0]=(unsigned char)MBBIN2CHAR( (unsigned char)((val>>4)&0xF));
	buf[1]=(unsigned char)MBBIN2CHAR( (unsigned char)(val&0xF));	
}
//-----------------------------------------------------------------------------------------------
unsigned long Sym_8_To_Long(unsigned char * pucFrame)//перевод 8ми символов в массиве в целоe long
{
	union
	{
		unsigned long result_long;
		unsigned char result_char[4];
	}
	sym_8_to_long;

	sym_8_to_long.result_char[0]=(unsigned char)MBCHAR2BIN(pucFrame[0])<<4 | (unsigned char)MBCHAR2BIN(pucFrame[1]);
	sym_8_to_long.result_char[1]=(unsigned char)MBCHAR2BIN(pucFrame[2])<<4 | (unsigned char)MBCHAR2BIN(pucFrame[3]);
	sym_8_to_long.result_char[2]=(unsigned char)MBCHAR2BIN(pucFrame[4])<<4 | (unsigned char)MBCHAR2BIN(pucFrame[5]);
	sym_8_to_long.result_char[3]=(unsigned char)MBCHAR2BIN(pucFrame[6])<<4 | (unsigned char)MBCHAR2BIN(pucFrame[7]);	

	return sym_8_to_long.result_long;			
}
//-----------------------------------------------------------------------------------------------
void Long_To_Sym_8(unsigned long val, unsigned char* buf)//перевод целого значения long в массив из 8ми символов
{
   unsigned char i;

	for(i=0;i<8;i++)
	{
		buf[i]=MBBIN2CHAR( (unsigned char)((val&0xF0000000)>>28));
		val=val<<4;	
	}
}
//-----------------------------------------------------------------------------------------------