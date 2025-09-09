

#ifndef MYPROJECT_H
#define MYPROJECT_H

/* Includes ------------------------------------------------------------------*/

#include "stm32f1xx_hal.h"
#include "usart.h"

#include "tim.h"
#include "gpio.h"

#include "foc_utils.h" 
#include "FOCMotor.h" 
#include "BLDCmotor.h" 

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define M1_Enable    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);          //高电平使能
#define M1_Disable   HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);        //低电平解除

#endif

