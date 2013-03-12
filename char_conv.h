#ifndef CHAR_CONV_H
#define CHAR_CONV_H

unsigned int Sym_4_To_Int(unsigned char * pucFrame);//перевод 4х символов в массиве в целое	int
unsigned char Sym_2_To_Char(unsigned char * pucFrame);//перевод 2х символов в массиве в целое char
unsigned long Sym_8_To_Long(unsigned char * pucFrame);//перевод 8ми символов в массиве в целоe long

void Int_To_Sym_4(unsigned int val, unsigned char* buf[]);//перевод целого значения int в массив из 4х символов
void Char_To_Sym_2(unsigned char val, unsigned char* buf[]);//перевод целого значения char в массив из 2х символов
void Long_To_Sym_8(unsigned long val, unsigned char* buf[]);//перевод целого значения long в массив из 8ми символов

#endif