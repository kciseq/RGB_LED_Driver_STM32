/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Moje_Systick_Handle.h"
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
uint8_t znak, flaga, licznik=0, komunikat[20],dl_kom,flaga2, flaga_stopniowanie,
bufor_PWM1,bufor_PWM2,bufor_PWM3,licznik_stopniowania;
uint8_t bufor[15];
uint8_t zparsowany[3], sukces, zparsowany1, zparsowany2, zparsowany3;
uint8_t R,G,B,i,j;
unsigned int _licznik_10ms, _10ms_go;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void Moje_Systick_Handle()
{
	if (flaga_stopniowanie == 1)
		{
			_licznik_10ms++;
			if (_licznik_10ms >= 10)
			{
				_licznik_10ms = 0;
				_10ms_go = 1;
			}
		}
}

int parsowanie_RGB(uint8_t* bufor)
{
	uint8_t check[14];
	for(i=0;i<3;i++)
	{
		if(*bufor=='R')
		{
		bufor++;
		for(j=0;j<3;j++)
		{
			zparsowany[j]=*bufor-48;
			bufor++;
		}
		R=(zparsowany[0])*100+(zparsowany[1])*10+zparsowany[2];

		}
		if(*bufor=='G')
		{
				bufor++;
				for(j=0;j<3;j++)
		{
					zparsowany[j]=*bufor-48;
					bufor++;
		}
		G=zparsowany[0]*100+zparsowany[1]*10+zparsowany[2];

		}
		if(*bufor=='B')
		{
				bufor++;
				for(j=0;j<3;j++)
				{
					zparsowany[j]=*bufor-48;
					bufor++;
				}
				B=zparsowany[0]*100+zparsowany[1]*10+zparsowany[2];

				}
	}

	for (i=1; i<3; i++)
	{
		bufor++;
	}

	for (i = 13; i>0; i--)
	{
		check[i] = *bufor;
		bufor--;
	}


	if((check[0] == 'R' ) && (check[4] == 'G' ) && (check[8] == 'B'))
	{
	return 1; // 1 oznacza sukces parsowania
	}else{
	return 2; // 2 oznacza błąd parsowania
	}

}



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
  MX_TIM1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
  HAL_UART_Receive_IT(&huart2, &znak, 1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  if(flaga==1)
	  {
		  bufor[licznik]=znak;
		  licznik++;

		  flaga=0;
	  }

	  if(licznik==14){
		  licznik=0;
		  sukces = parsowanie_RGB(&bufor[0]);

		  }

	  if(sukces==1)
	  {
		  	  	  	sukces=0;
		  	  	  	// check if higher then 255
		  	  	  	if (R > 255)
		  	  	  	{
		  	  	  		R = 255;
		  	  	  	}
		  	  	  	if (G > 255)
		  	  	  	{
		  	  	  		G = 255;
		  	  	  	}
		  	  	  	if (B > 255)
		  	  	  	{
		  	  	  		B = 255;
		  	  	  	}
		  	  	  	// Now check if smaller then 0
		  	  	  	if (R < 0)
		  	  	  	{
		   	  	  		R = 0;
		  	 	  	}
		   	  	  	if (G < 0)
		  	  	  	{
		  	  	  		G = 0;
		  	  		}
		  	  		if (B < 0)
		  	  		{
		  	   	  		B = 0;
		  	   	  	}

		  	  	  	bufor_PWM1 = htim1.Instance->CCR1;
		  	  	  	bufor_PWM2 = htim1.Instance->CCR2;
		  	  	  	bufor_PWM3 = htim1.Instance->CCR3;
		  	  	  	flaga_stopniowanie = 1;
		  	  	  	  	 while(flaga_stopniowanie == 1)
		  	  	  	  	  	  {
		  	  	  	  	  	  if(_10ms_go == 1)
		  	  	  	  	  	  {
		  	  	  	  	  		  _10ms_go = 0;
	 			  	  	  	  	  htim1.Instance->CCR1+=(B-bufor_PWM1)/10;
	 			  	  	  	  	  htim1.Instance->CCR2+=(G-bufor_PWM2)/10;
	 			  	  	  	  	  htim1.Instance->CCR3+=(R-bufor_PWM3)/10;
	 			  	  	  	  	  licznik_stopniowania++;
		  	  	  	  	  	  }
		  	  	  	  	  	  	  if(licznik_stopniowania >=10)
		  	  	  	  	  	  	  {
		  	  	  	  	  	  		  htim1.Instance->CCR1=B;
		  	  	  	  	  	  		  htim1.Instance->CCR2=G;
		  	  	  	  	  	  		  htim1.Instance->CCR3=R;
		  	  	  	  	  	  		  licznik_stopniowania =0;
		  	  	  	  	  	  		  flaga_stopniowanie = 0;
		  	  	  	  	  	  		  _licznik_10ms = 0;
		  	  	  	  	  	  	  }
		  	  	  	  	  	  }
	  }else if(sukces == 2)
	  {
		  HAL_Delay(5); // to make sure that any further incorrect data will be omitted
		  licznik = 0;
		  for(i=0; i<15; i++)
		  {
			  bufor[i] = 0;
		  }
	  }


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
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART2){
		flaga=1;
		HAL_UART_Receive_IT(&huart2, &znak, 1);
	}
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
