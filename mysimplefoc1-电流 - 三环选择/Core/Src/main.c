/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "MyProject.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
char message[64]="";
int fputc(int ch,FILE *f)
{ 
    HAL_UART_Transmit(&huart1,(unsigned char *)&ch,1,1);
    return ch;
}
float target=0;
float target_v=15;
float target_c=0.3;
int choose=2;
float pid_p=0;
float pid_i=0;
float pid_d=0;
uint8_t  RecieveBuffer[1]={0};//????????
//HAL_UART_Receive_IT(&huart1, (uint8_t *)RecieveBuffer, 1);
uint8_t  CO2Buffer[12];//????????
uint8_t  RxLine=0; //????????
uint8_t errorBuffer[]="\r\nerror\r\n"; //????
/**
 * @brief ???????
 * @param  ???
 * @retval void
 * @author smart_mode
 * @Time 2021?11?21?
*/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	UNUSED(huart);
	RxLine++;
	CO2Buffer[RxLine-1]=RecieveBuffer[0];
	if (RxLine>12) //?????????,?????????????
	{
		RxLine=0;
		memset(CO2Buffer,0,sizeof(CO2Buffer)); 
		HAL_UART_Transmit_IT(&huart1, (uint8_t *)errorBuffer,sizeof(errorBuffer));
	}
	if (RecieveBuffer[0]==0x0A)  //???????,????????
	{
		switch(CO2Buffer[0])
		{
			case 'P':
				//printf("Hello World!\r\n");
			pid_p=atof((const char *)(CO2Buffer+1));
			//PID_current_d.P=pid_p;
			P_angle.P=pid_p;
			//PID_velocity.P=pid_p;
				printf("RX=%.4f\r\n", pid_p);
				break;
			case 'I':
				//printf("Hello World!\r\n");
			pid_i=atof((const char *)(CO2Buffer+1));
			//PID_current_d.I=pid_i;
			P_angle.I=pid_i;
			//PID_velocity.I=pid_i;
				printf("RX=%.4f\r\n", pid_i);
				break;
			case 'D':
				//printf("Hello World!\r\n");
			pid_d=atof((const char *)(CO2Buffer+1));
			//PID_current_q.D=pid_d;
			P_angle.D=pid_d;
				printf("RX=%.4f\r\n", pid_d);
				break;
			case 'V':   //T6.28
				target_v=atof((const char *)(CO2Buffer+1));
				printf("RX=%.4f\r\n", target_v);
				break;
			case 'C':   //T6.28
				target_c=atof((const char *)(CO2Buffer+1));
				printf("RX=%.4f\r\n", target_c);
				break;
			case 'T':   //T6.28
				choose=2;
				target=atof((const char *)(CO2Buffer+1));
				printf("RX=%.4f\r\n", target);
				break;
			case 't':   //T6.28//no current
				choose=3;
				target=atof((const char *)(CO2Buffer+1));
				printf("RX=%.4f\r\n", target);
				break;
		}
		//HAL_UART_Transmit_IT(&huart1, (uint8_t *)CO2Buffer,RxLine);  //??????????,????????
		RxLine=0;	
	}
/*switch(RecieveBuffer[0])
		{
			case 'H':
				printf("Hello World!\r\n");
				break;
			case 'T':   //T6.28
				target=atof((const char *)(RecieveBuffer+1));
				printf("RX=%.4f\r\n", target);
				break;
		}*/
	RecieveBuffer[0]=0;
	HAL_UART_Receive_IT(&huart1, (uint8_t *)RecieveBuffer, 1);
	
	//??????????
}

void systick_CountMode(void)
{
	SysTick->LOAD = 0xFFFFFF-1;      //set reload register
  SysTick->VAL  = 0;
  SysTick->CTRL = SysTick_CTRL_ENABLE_Msk; //Enable SysTick Timer
}
/*
uint8_t AdcConvEnd=0;
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    if(hadc==&hadc1)
    {
        AdcConvEnd=1;
    }
}*/
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_ADC1_Init();
  MX_I2C1_Init();
  MX_I2C2_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
	 //HAL_TIM_Base_Start(&htim2);
HAL_UART_Receive_IT(&huart1, (uint8_t *)RecieveBuffer, 1);
//HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);
HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_3);
HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_4);
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);

	 /*uint16_t ADC_Value[18]={0}; 
		uint16_t value1=0;
		uint16_t value2=0;
	HAL_ADCEx_Calibration_Start(&hadc1);
	 HAL_ADC_Start_DMA(&hadc1, (uint32_t *)ADC_Value, 18);*/
	 InlineCurrentSense_Init(); //ADC??????????
//int i=0;
//float a1=0;
HAL_Delay(1000);
//printf("ADC7 : %.5f \r\n",_readADCVoltageInline(7));
/*for(i=0; i<1000; i++)
	{
		a1 += _readADCVoltageInline(7);
		
		HAL_Delay(1);
	}
	a1 = a1/1000;
	printf("ADC7 : %.5f \r\n",a1);*/
Current_calibrateOffsets();
	InlineCurrentSense(0.01,50,7,8,NOT_SET);    //SimpleMotor//??????,????,A?,B?,C?
	
	LPF_init();                //LPF?????
	PID_init();                //PID?????
	
	voltage_power_supply=12;   //V ????
	pole_pairs=7;              //?????,??????,???????????????
	voltage_sensor_align=2;    //V alignSensor() use it,?????????????0.5-1,????????????2-3
	voltage_limit=6;           //V,???????????,??????12/1.732=6.9
	velocity_limit=target_v;         //rad/s ???????????,?????????????
	current_limit=target_c;          //A,foc_current?dc_current??????,???0?????????????
	torque_controller=Type_foc_current;  //Type_dc_current;//  Type_foc_current;  //Type_voltage;
	controller=Type_angle;  //Type_torque;  //Type_velocity;  //Type_angle; 
	PID_current_d.P=20;   //0.6 //60   //???PI??,???? PID_init() ?????????
	PID_current_d.I=0.3;    //0  //40     //???I???????,???0??P?????
	//PID_current_d.limit = 0.5;
	PID_current_q.P=60;
	PID_current_q.I=1;
	//PID_current_q.limit = 0.5;
	PID_velocity.P=0.05;        //0.05, ???PI??,??P????????
	PID_velocity.I=0.001;
	P_angle.P=2;
	P_angle.D=0.1;//?????,??P??
	PID_velocity.output_ramp=40;//20 //??????,??????????0
	LPF_velocity.Tf=0.005;  //0.0001    //Tf?????,????????,???????;?????????????,Tf?????????
	target=0;
	
	Motor_init();
	Motor_initFOC(0,UNKNOWN);  //(2.2,CW);(0,UNKNOWN);  //?????????????,???????????????????
  printf("Motor ready.\r\n");
	
	systick_CountMode();   //?????delay_us()?delay_ms()??
  int aa=0;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
		
		if(aa>10){aa=0;printf("%.3f , %.6f , %.4f , %.4f , %.2f\r\n",target,voltageq,-getVelocity(),-getAngle(),P_angle.P);}
		//if(aa>10){aa=0;printf("%.3f , %.3f\r\n",(value1*3.3f/4679),(value2*3.3f/4679));}
		if(choose==3)
			{
				torque_controller=Type_voltage;
				PID_velocity.P=0.1;        //0.05, ???PI??,??P????????
	      PID_velocity.I=0.01;
				PID_velocity.limit=voltage_limit;
			}else{
				torque_controller=Type_foc_current;
				PID_velocity.P=0.05;        //0.05, ???PI??,??P????????
	      PID_velocity.I=0.001;
				PID_velocity.limit=target_c;
			}
		P_angle.limit=target_v;        
	//PID_velocity.limit=target_c; 
		move(target);
		loopFOC();
		aa++;
		
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV8;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
