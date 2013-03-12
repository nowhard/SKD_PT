#ifndef PWM_H
#define PWM_H

#include <ADuC845.h> 

//---------режимы шим----------------
#define PWM_DISABLE			0x0
#define PWM_SINGLE			0x10
#define	PWM_TWIN_8_BIT		0x20
#define	PWM_TWIN_16_BIT		0x30
#define PWM_DUAL_NRZ_16_BIT	0x40
#define PWM_DUAL_8_BIT		0x50
#define PWM_DUAL_RZ_16_BIT	0x60
#define PWM_RESET			0x70
//----------делители частоты---------
#define	CLOCK			0x0
#define	CLOCK_DIV_4		0x4
#define	CLOCK_DIV_16	0x8
#define	CLOCK_DIV_64	0xC
//-----------источник частоты---------
#define FXTAL_DIV_15	0//2.184kHz
#define	FXTAL			1//32.768 kHz
#define EXTERNAL_SOURCE	3//P2.7
#define FVCO			4//12.58MHz
//------------------------------------  
void PWM_Init(unsigned char mode,unsigned char clock_divider,unsigned char clock_source);//инициализация шим
void PWM_Set_Single(unsigned int period,unsigned int length);//установка периода и длительности импульса
void PWM_Set_8_Bit_1_Channel(unsigned char period,unsigned char length);//8битный шим, канал 1
void PWM_Set_8_Bit_2_Channel(unsigned char period,unsigned char length);//8битный шим, канал 2
//для остальных случаев требуется реализация

#endif