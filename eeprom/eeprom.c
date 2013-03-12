#include "eeprom.h"
 sbit LED=P3^7;
 //----------------------------------------------------------------------------------
unsigned int  EEPROM_Write(void *buffer,unsigned int len,unsigned int addr) //using 0//записать буфер по адресу
{
//небезопасная
  unsigned int i=0;
  unsigned char *buf;
						   
  if (addr+len>= EEMEM_SIZE) //если адрес выходит за границы
  	return (0UL);
//LED=1;
	buf=buffer;

   for(i=0;i<len;i++)
   {	   
	   EDATA1=buf[i*4]; 
	   EDATA2=buf[i*4+1]; 
	   EDATA3=buf[i*4+2]; 
	   EDATA4=buf[i*4+3]; 
	  
	   EADR=addr;

	   ECON=EE_ERASE_PAGE;
   	   ECON=EE_WRITE_PAGE;
	  
	   addr++;
   }
//LED=0;
   return i;
}
//----------------------------------------------------------------------------------
unsigned int EEPROM_Read(void *buffer,unsigned int len,unsigned int addr) //using 0//прочитать несколько страниц с адреса
{
//небезопасная

  unsigned int  i=0;
  unsigned char *buf;
//LED=1;
  if (addr+len>= EEMEM_SIZE) //если адрес выходит за границы
  	return (0UL);

   buf=buffer;
  	
   for(i=0;i<len;i++)
   {
   	   EADR=addr;
   	   ECON=EE_READ_PAGE;
	   
	   buf[i*4]  =EDATA1; 
	   buf[i*4+1]=EDATA2; 
	   buf[i*4+2]=EDATA3; 
	   buf[i*4+3]=EDATA4; 
	   addr++;
   }  
//LED=0;
   return i;
}

//----------------------------------------------------------------------------------
/*void  EEPROM_Erase(void) //using 0//
{
	ECON = EE_ERASE_ALL;
} */