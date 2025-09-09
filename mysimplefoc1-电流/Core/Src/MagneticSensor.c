#include "MyProject.h"



unsigned char write_reg(unsigned char reg, unsigned char value)
{
	return HAL_I2C_Mem_Write(&hi2c1, Slave_Addr, reg, I2C_MEMADD_SIZE_8BIT, &value, 1, 50);
}


unsigned char write_regs(unsigned char reg, unsigned char *value, unsigned char len)
{
	return HAL_I2C_Mem_Write(&hi2c1, Slave_Addr, reg, I2C_MEMADD_SIZE_8BIT, value, len, 50);
}

unsigned char read_reg(unsigned char reg, unsigned char* buf, unsigned short len)
{
	return HAL_I2C_Mem_Read(&hi2c1, Slave_Addr, reg, I2C_MEMADD_SIZE_8BIT, buf, len, 50);
}



float GetAngle_NoTrack(void)
{   float angle_d;
	int16_t in_angle;
    uint8_t temp[2]={0,0};
		read_reg( Angle_Hight_Register_Addr, temp, 2);
    //printf("%d  %d\n",temp[0],temp[1]);
    in_angle = ((int16_t)temp[0] <<8) | (temp[1]);
    angle_d = (float)in_angle * cpr / 4096;
		//printf("angle=%.2f\n",angle_d);

		return angle_d;
	//angle_d????,???0-6.28	
}

float full_rotations = 0.0;
float Last_Angle = 0.0;
float getAngle(void)
{
	float D_Angle = 0.0;
	float Angle = GetAngle_NoTrack();
	D_Angle = Angle - Last_Angle;
	
	if( fabs(D_Angle) > (0.8f*2*PI) )
	{
		full_rotations = full_rotations + ((D_Angle > 0) ? -1 :1);
	}
	
	Last_Angle = Angle;
	
	return (full_rotations * 2 * PI + Last_Angle);
}

float Last_Vel_ts = 0.0;
float angle_prev = 0.0;
float getVelocity(void)
{
	float dt = 0.0;
	float Vel_ts = SysTick -> VAL;
	if(Vel_ts < Last_Vel_ts) dt = (Last_Vel_ts - Vel_ts)/9*1e-6f;
	else dt = (0xFFFFFF - Vel_ts + Last_Vel_ts)/9*1e-6f;
	
	if(dt == 0 || dt > 0.5) dt = 1e-3;
	
	float Vel_Angle = getAngle();
	
	float dv = Vel_Angle - angle_prev;

	float velocity = (Vel_Angle - angle_prev)/dt;

	Last_Vel_ts = Vel_ts;
  angle_prev = Vel_Angle;
  
	
  return velocity;
}