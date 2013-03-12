#ifndef I2C_SLAVE_H
#define	I2C_SLAVE_H
#include <ADuC845.h>
#include "proto_uso/channels.h"
//-----------------------------------
extern struct Channel xdata channels[CHANNEL_NUMBER];
//--------------ведомый I2C---------
void I2C_Slave_Init(void);//инициализация узла i2c slave
void I2C_ISR(void);//прерывание I2C
//void I2C_
//----------------------------------

#endif