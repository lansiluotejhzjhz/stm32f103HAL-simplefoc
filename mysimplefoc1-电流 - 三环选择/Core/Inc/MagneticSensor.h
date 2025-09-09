#ifndef MAGNETICSENSOR_LIB_H
#define MAGNETICSENSOR_LIB_H

#include "i2c.h"

/******************************************************************************/
#define PI					3.14159265358979f
#define cpr (float)(2.0f*PI)
#define Slave_Addr                0x36<<1//?????
#define Write_Bit                 0	   //???	
#define Read_Bit                  1    //???
#define Angle_Hight_Register_Addr 0x0C //???????
#define Angle_Low_Register_Addr   0x0D //???????
extern I2C_HandleTypeDef hi2c1;
extern float angle_prev;
unsigned char read_reg(unsigned char reg, unsigned char* buf, unsigned short len);
unsigned char write_reg(unsigned char reg, unsigned char value);
float GetAngle_NoTrack(void);
float getAngle(void);
float getVelocity(void);
/******************************************************************************/

#endif
