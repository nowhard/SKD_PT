#ifndef EEPROM_H
#define EEPROM_H
#include <ADuC845.h>

//---------------------------------------
//запись,чтение и стирание внутреннего 
//пользовательского ППЗУ
//---------------------------------------
#define EE_READ_PAGE	0x1	 //команда чтения
#define EE_WRITE_PAGE	0x2	 //команда записи
#define EE_VERIFY_PAGE	0x4	 //проверка данных страницы
#define EE_ERASE_PAGE	0x5	 //очистить страницу
#define EE_ERASE_ALL	0x6  //очистить все
#define EE_READ_BYTE	0x81 //прочитать байт
#define EE_WRITE_BYTE 	0x82 // записать байт 

//---------------------------------------
#define EEMEM_SIZE 		1024 //размер памяти в страницах
//---------------------------------------
sfr16 EADR=0xC6;
//---------------------------------------
unsigned int EEPROM_Read(void *buffer,unsigned int len,unsigned int addr);//прочитать несколько страниц с адреса
unsigned int  EEPROM_Write(void *buffer,unsigned int len,unsigned int addr);//записать буфер по адресу
void 		  EEPROM_Erase(void);// 
#endif 