/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdlib.h> //vagy stdio.h kell?
#include <string.h>
#include <math.h>
#include "sensors.h"
#include "digoutputs.h"
#include "lcd2x16.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

enum States{
	firstNumber,
	clearORsetBrightness,
	secondNumber,
	overError,
	resultOnDisplay
};


/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */



/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
 ADC_HandleTypeDef hadc1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */


ADC_ChannelConfTypeDef ADC_Channels; //ADC-hez kell

AnalogValuesType analogValues; //Ebben taroljuk analog ertekeket
storedValuesType storedValues;
enum States mainstate = firstNumber;
int cntrPWM=0;
double PWMLVL=0.875; //lehet inkabb int
double result;

// Egyetlen bájtos adatfogadási buffer
char rxBuffer;
// Ide másoljuk a küldés alatt álló stringet. Az UART_SendString() használja.
#define TXBUFFERSIZE 255
char txBuffer[TXBUFFERSIZE];
uint8_t lastReceivedUartCommand = 0;

//Signal1: PB6, Signal2: PC14




/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_ADC1_Init(void);
/* USER CODE BEGIN PFP */

HAL_StatusTypeDef UART_Init();
HAL_StatusTypeDef UART_SendString(char *str);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *handle);

void UARTSettings(char* str);
static void LCD2x16_Init(void);
static void TimerandPWM_Init(void);


void GetAnalogValues(void); //ezutan hivjuk meg ketszer, kulonbozo signallal a setHalfOfArrayValues-t
void ADConversion(void);

void calcPWM(void);
void DelayWithPWM(int delay);
double power(double num, int pow);
double shortendoubleto16digits(double r);
void setLCDstringbydouble(double r);

//State Functions
void FirstNumberStateFunction(void);

//outputLCDType CommandTypeToOutputLCDType(commandType command, uint32_t vo_pwm, uint32_t enable, uint32_t a_pwm);
//outputLCDType ClearScreen(void);
	//numberToLCD
void ClearOrSetBrightnessStateFunction(void); //Here we simply set the brightness level, the current PWM values are decided at the end of the loop (pinek: 8 illetve 12)
void SecondNumberStateFunction(void);
void OverErrorStateFunction(void);
void ResultOnDisplayStateFunction(void);



/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/*      **PUBLIKUS VALTOZOK**       */
outputLCDType outputLCD; //biztos?
commandType command;
char LCDstring[20]; //16db karakter + \n + \0

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */


	//char msgUART[17] = {'\0','\0','\0','\0','\0','\0','\0','\0',   '\0','\0','\0','\0','\0','\0','\0','\0'   ,'\0'};; //UARThoz   //FONTOS ATGONNDOLI LEZARO 0-T
	//int darknessLvl = 0;



	storedValues.storedChar='0';
	storedValues.firstStoredNumber=0;
	storedValues.secondStoredNumber=0;
	storedValues.statusDot=0;

	strcpy(LCDstring," ");

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
  MX_USART2_UART_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */
  TimerandPWM_Init();
  LCD2x16_Init();
  UART_Init(); //?

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

	//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET); //LED

	ADConversion();

	HAL_Delay(1);
	setOutputPin(RS_PIN, 1); //For writing characters
	HAL_Delay(1);


	//WriteLCDChar(1, 0, 'F'); //TESZTKOD
	//HAL_Delay(2000);

	//strcpy(LCDstring,"first test\n"); //EZ MUKODIK!
	//strcat(LCDstring, "+");


	  //lekerdezni bemeneteket + aktualizalas sensors.c valtozojanak, itt kell csak hasznalni a signalt
	 switch(mainstate){
	 	 case firstNumber :
	 		 FirstNumberStateFunction();
	 		 setLCDstringbydoublewithNewline(storedValues.firstStoredNumber);
	 		 break;
	 	 case clearORsetBrightness :
	 		 ClearOrSetBrightnessStateFunction();
	 		 strcpy(LCDstring,"Clear?\n");
	 		 break;
	 	 case secondNumber :
	 		 SecondNumberStateFunction();
	 		 setLCDstringbydoublewithNewline(storedValues.secondStoredNumber);
	 		 break;
	 	 case overError :
	 		 OverErrorStateFunction();
	 		 strcpy(LCDstring,"OverError!\n");
	 		 break;
	 	 case resultOnDisplay:

	 		 ResultOnDisplayStateFunction();
	 		 setLCDstringbydoublewithNewline(shortendoubleto16digits(result));
	 		 break;
	 }

	UART_SendString(LCDstring);

	cntrPWM++;
	if (cntrPWM>31)
		cntrPWM=0;

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  } //end of while
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
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

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = ENABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;  //ATIRVA 1-RE        ATIRVA 1-RE        ATIRVA 1-RE        ATIRVA 1-RE        ATIRVA 1-RE        ATIRVA 1-RE        ATIRVA 1-RE        ATIRVA 1-RE
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  //               SAJAT KIKOMMENTELES                 SAJAT KIKOMMENTELES                SAJAT KIKOMMENTELES                SAJAT KIKOMMENTELES                SAJAT KIKOMMENTELES
  // /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  // */
  // sConfig.Channel = ADC_CHANNEL_6;
  // sConfig.Rank = 1;
  // sConfig.SamplingTime = ADC_SAMPLETIME_56CYCLES;
  // if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  // {
  //   Error_Handler();
  // }

  // /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  // */
  // sConfig.Channel = ADC_CHANNEL_1;
  // sConfig.Rank = 2;
  // if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  // {
  //   Error_Handler();
  // }

  // /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  // */
  // sConfig.Channel = ADC_CHANNEL_7;
  // sConfig.Rank = 3;
  // if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  // {
  //   Error_Handler();
  // }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  // sConfig.Channel = ADC_CHANNEL_8;
  // sConfig.Rank = 4;
  // if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  // {
  //   Error_Handler();
  // }

//  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
//  */
//  sConfig.Channel = ADC_CHANNEL_9;
//  sConfig.Rank = 5;
//  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
//  {
//    Error_Handler();
//  }
//
//  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
//  */
//  sConfig.Channel = ADC_CHANNEL_12;
//  sConfig.Rank = 6;
//  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
//  {
//    Error_Handler();
//  }
//
//  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
//  */
//  sConfig.Channel = ADC_CHANNEL_14;
//  sConfig.Rank = 7;
//  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
//  {
//    Error_Handler();
//  }
//
//  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
//  */
//  sConfig.Channel = ADC_CHANNEL_13;
//  sConfig.Rank = 8;
//  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
//  {
//    Error_Handler();
//  }
  /* USER CODE BEGIN ADC1_Init 2 */

  //KIKOMMENTELTEM GENERALT KODBOL A CONFIGURATION-T ES HELYETTE MAINBEN DOLGOZOK, ATIRTAM NUMBER OF CONVERSIONT 8-ROL 1-RE******************************************************************************************************************************************
  // EZ ALAPJAN:https://www.youtube.com/watch?v=5l-b6lsubBE											***********************************************************************************************************************************************************************

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_5|GPIO_PIN_6
                          |GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_12, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8
                          |GPIO_PIN_9, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);

  /*Configure GPIO pins : PC13 PC14 PC5 PC6
                           PC8 PC9 PC10 PC11
                           PC12 */
  GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_5|GPIO_PIN_6
                          |GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PB12 PB6 PB7 PB8
                           PB9 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8
                          |GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PD2 */
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

HAL_StatusTypeDef UART_Init(){
	MX_USART2_UART_Init();

	HAL_UART_Receive_IT(&huart2, (uint8_t*) &rxBuffer, 1 );

}
HAL_StatusTypeDef UART_SendString(char *str){
	while(huart2.gState!= HAL_UART_STATE_READY && huart2.gState!=  HAL_UART_STATE_BUSY_RX );

	strncpy(txBuffer,str,TXBUFFERSIZE);
	HAL_UART_Transmit_IT(&huart2,(uint8_t* )txBuffer,strlen(txBuffer));

}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *handle){
	if(handle == &huart2){
		lastReceivedUartCommand=(uint8_t) rxBuffer;
		HAL_UART_Receive_IT(handle,(uint8_t*) &rxBuffer,1);

	}
}

void UARTSettings(char* str){
 //
}

void GetAnalogValues(){


	ADC_ChannelConfTypeDef sConfig = {0};


	  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
	  */
	  sConfig.Channel = ADC_CHANNEL_6;
	  sConfig.Rank = 1;
	  sConfig.SamplingTime = ADC_SAMPLETIME_56CYCLES;
	  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	  {
	    Error_Handler();
	  }

	  HAL_ADC_Start(&hadc1);
	  HAL_ADC_PollForConversion(&hadc1, 10);
	  analogValues.irs12 = HAL_ADC_GetValue(&hadc1)*3.3/4096;
	  HAL_ADC_Stop(&hadc1);

	  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
	  */
	  sConfig.Channel = ADC_CHANNEL_1;
	  //sConfig.Rank = 2;
	  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	  {
	    Error_Handler();
	  }

	  HAL_ADC_Start(&hadc1);
	  HAL_ADC_PollForConversion(&hadc1, 10);
	  analogValues.irs3plus = HAL_ADC_GetValue(&hadc1)*3.3/4096;
	  HAL_ADC_Stop(&hadc1);

	  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
	  */
	  sConfig.Channel = ADC_CHANNEL_7;
	  //sConfig.Rank = 3;
	  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	  {
	    Error_Handler();
	  }

	  HAL_ADC_Start(&hadc1);
	  HAL_ADC_PollForConversion(&hadc1, 10);
	  analogValues.irs45 = HAL_ADC_GetValue(&hadc1)*3.3/4096;
	  HAL_ADC_Stop(&hadc1);

	  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
	  */
	  sConfig.Channel = ADC_CHANNEL_8;
	  //sConfig.Rank = 4;
	  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	  {
	    Error_Handler();
	  }

	  HAL_ADC_Start(&hadc1);
	  HAL_ADC_PollForConversion(&hadc1, 10);
	  analogValues.irs6minus = HAL_ADC_GetValue(&hadc1)*3.3/4096;
	  HAL_ADC_Stop(&hadc1);

	  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
	  */
	  sConfig.Channel = ADC_CHANNEL_9;
	  //sConfig.Rank = 5;
	  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	  {
	    Error_Handler();
	  }

	  HAL_ADC_Start(&hadc1);
	  HAL_ADC_PollForConversion(&hadc1, 10);
	  analogValues.irs78 = HAL_ADC_GetValue(&hadc1)*3.3/4096;
	  HAL_ADC_Stop(&hadc1);
	  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
	  */
	  sConfig.Channel = ADC_CHANNEL_12;
	  //sConfig.Rank = 6;
	  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	  {
	    Error_Handler();
	  }

	  HAL_ADC_Start(&hadc1);
	  HAL_ADC_PollForConversion(&hadc1, 10);
	  analogValues.irs9x = HAL_ADC_GetValue(&hadc1)*3.3/4096;
	  HAL_ADC_Stop(&hadc1);

	  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
	  */
	  sConfig.Channel = ADC_CHANNEL_14;
	  //sConfig.Rank = 7;
	  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	  {
	    Error_Handler();
	  }

	  HAL_ADC_Start(&hadc1);
	  HAL_ADC_PollForConversion(&hadc1, 10);
	  analogValues.irs0dot = HAL_ADC_GetValue(&hadc1)*3.3/4096;
	  HAL_ADC_Stop(&hadc1);

	  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
	  */
	  sConfig.Channel = ADC_CHANNEL_13;
	  //sConfig.Rank = 8;
	  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	  {
	    Error_Handler();
	  }

	  HAL_ADC_Start(&hadc1);
	  HAL_ADC_PollForConversion(&hadc1, 10);
	  analogValues.irsequaldivide = HAL_ADC_GetValue(&hadc1)*3.3/4096;
	  HAL_ADC_Stop(&hadc1);


} //End of GetAnalogValues() function


void ADConversion(void){
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET); //Signal2
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET); //Signal1
	HAL_Delay(10);

	GetAnalogValues();
	//setHalfOfArrayValues(analogValues, 0);
	setBitCalcArrayValues(analogValues);

	/*HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET); //Signal1
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_SET); //Signal2   //LAB NEM MEGY 3.3V-BA!!!
	HAL_Delay(10);

	GetAnalogValues();
	setHalfOfArrayValues(analogValues, 1);

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET); //Signal2 */
}


void LCD2x16_Init(void)
{


	setOutputPin(RW_PIN,0);
	setOutputPin(E_PIN, 0);
	setOutputPin(RS_PIN,0);

	setOutputPin(A_PWM_PIN,1);
	setOutputPin(VO_PWM_PIN,1);

	HAL_Delay(60); //From the data sheet, setup time is at least 40ms after VDD reaches 2.7V and further initialization delays occur

	//1st step: Required initialization
	//1st command: 8 bit bus, 1 line display, 5x8   0  0 | 0 0 1 1 0 0 X X (RS RW DB7-DB0)

	WriteLCDBinary(0, 0, 0x30); //1st function set
	HAL_Delay(10);
	WriteLCDBinary(0, 0, 0x30); //2nd function set
	HAL_Delay(1);
	WriteLCDBinary(0, 0, 0x30); //3rd function set
	HAL_Delay(1);
	WriteLCDBinary(0, 0, 0x38);	//8 bit interface, 2 rows, 5x8 character font
	HAL_Delay(1);
	WriteLCDBinary(0, 0, 0x08);	//Display OFF
	HAL_Delay(1);
	WriteLCDBinary(0, 0, 0x01);	//Clear display
	HAL_Delay(1);
	WriteLCDBinary(0, 0, 0x06); //Entry mode: cursor to the right, no shifting
	HAL_Delay(1); //lehet talan tobb varakozas
	//WriteLCDBinary(0, 0, 0x0C); //Display on, no cursor
	WriteLCDBinary(0, 0, 0x0F); //Display on with cursor


} //End of LCDInit() function

static void TimerandPWM_Init(void){
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET); //Signal1
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET); //Signal2

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
} //End of PWMInit() function


void calcPWM(void){
	int x = (int) 32*PWMLVL;
	int y = cntrPWM < x ? 1 : 0;
	setPWMPins(y, y);
}
void DelayWithPWM(int delay){
	int i=0;
	for (i=0; i< delay; i++){
		HAL_Delay(1);
		calcPWM();
		cntrPWM++;
		if (cntrPWM>31)
				cntrPWM=0;
	}
}

double power(double num, int pow){
	int i;
	double out=1;
	for(i=0;i<pow;i++){
		out=out*num;
	}
	return out;
}

double shortendoubleto16digits(double r){
	double x = r;
	double z;
	long long y;
	int k=0;
	int i;
	if(x==0)
		return 0;
	while (x<10000000000000000){
		x=x*10;
		k++;
	}
	x=x/10;  //Assuming given double wasn't bigger than 10^16-1 = 9999...99 (16 darab)
	y = (long long) x;
	x = y;
	z=x;
	//modf(x,&z);
	for(i=0;i<k-1;i++)
		z=z/10;
	return z;
}
void setLCDstringbydoublewithNewline(double r){
	char outer[16];
	snprintf(outer,16,"%f",r);
	strcpy(LCDstring,outer);
	strcat(LCDstring,"\n" );
}
char* doubletostring(double r){
	char copy[20];
	char outer[16];
	snprintf(outer,16,"%f",r);
	strcpy(copy,outer);
	return copy;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////

void FirstNumberStateFunction(void){

	switch(storedValues.storedChar){
		case '0':
			storedValues.firstStoredNumber= storedValues.statusDot<1? 2*storedValues.firstStoredNumber + 0 : storedValues.firstStoredNumber + 0 * power(0.5, storedValues.statusDot);
			if(storedValues.statusDot>0)
				storedValues.statusDot++;

			setLCDstringbydoublewithNewline(storedValues.firstStoredNumber);
			UART_SendString(LCDstring);
			WriteLCDChar(1, 0, '0');
			DelayWithPWM(1500);
			ADConversion();
			storedValues.storedChar='s';
			break;
		case '1':
			storedValues.firstStoredNumber= storedValues.statusDot<1? 2*storedValues.firstStoredNumber + 1 : storedValues.firstStoredNumber + 1 * power(0.5, storedValues.statusDot);
			if(storedValues.statusDot>0)
				storedValues.statusDot++;
			setLCDstringbydoublewithNewline(storedValues.firstStoredNumber);
			UART_SendString(LCDstring);
			WriteLCDChar(1, 0, '1');
			DelayWithPWM(1500);
			ADConversion();
			storedValues.storedChar='s';
			break;

		case 'p':
			ClearScreen();
			ReturnHome();
			storedValues.storedChar='s';
			break;
		case 'c':
			ClearScreen();
			ReturnHome();
			strcpy(LCDstring,"CLEARING\n");
			UART_SendString(LCDstring);
			storedValues.firstStoredNumber=0;
			storedValues.statusDot=0;
			storedValues.secondStoredNumber=0;
			storedValues.storedChar='s';
			DelayWithPWM(800);
			break;
		default: break;


	}  //storedChar kezeles pl. 'n'-re eloszor kis delay

	//megnezni a bemenetek kozul van-e ami le van nyomva
	if(sensorArray.irs0>0)
		storedValues.storedChar='0';
	if(sensorArray.irs1>0)
		storedValues.storedChar='1';


	if(sensorArray.irsdot>0){
		if(storedValues.statusDot==0){
			storedValues.statusDot=1;
			WriteLCDChar(1, 0, '.');
		}

	}

	if(sensorArray.irsequal>0){
		mainstate = clearORsetBrightness;
		storedValues.storedChar='=';
	}


	if(sensorArray.irsplus>0){
		mainstate = secondNumber;
		storedValues.storedChar='+';
		storedValues.statusDot=0;
	}

	if(sensorArray.irsminus>0){
		mainstate = secondNumber;
		storedValues.storedChar='-';
		storedValues.statusDot=0;
	}
	if(sensorArray.irsx>0){
		mainstate = secondNumber;
		storedValues.storedChar='x';
		storedValues.statusDot=0;
	}
	if(sensorArray.irsdivide>0){
		mainstate = secondNumber;
		storedValues.storedChar='/';
		storedValues.statusDot=0;
	}

	if(storedValues.firstStoredNumber*power(10, storedValues.statusDot)>power(10, 16)-1){
		mainstate = overError;
		storedValues.storedChar='A';
	}
}

void ClearOrSetBrightnessStateFunction(void){
	switch(storedValues.storedChar){
		case '=':
			ClearScreen();
			WriteLCDChar(1, 0, 'C');
			WriteLCDChar(1, 0, 'l');
			WriteLCDChar(1, 0, 'e');
			WriteLCDChar(1, 0, 'a');
			WriteLCDChar(1, 0, 'r');
			WriteLCDChar(1, 0, '/');
			WriteLCDChar(1, 0, 'P');
			WriteLCDChar(1, 0, 'W');
			WriteLCDChar(1, 0, 'M');

			//TODO itt UART kuldes?
			DelayWithPWM(800);
			break;
		/*case '+':
			PWMLVL = 1 - (1-PWMLVL)/2 ;
			break;
		case '-':
			PWMLVL = 1 - (1-PWMLVL)*2 ;
			break;*/
		default: break;
	}

	if(sensorArray.irs0>0){
		mainstate = firstNumber;

		storedValues.storedChar='c';
	}
	if(sensorArray.irsdot>0){
		mainstate = firstNumber;
		storedValues.storedChar='p';
	}


}

void SecondNumberStateFunction(void){

	switch(storedValues.storedChar){

		case '+':
			WriteLCDChar(1, 0, '+');
			storedValues.storedChar='a';
			break;
		case '-':
			WriteLCDChar(1, 0, '-');
			storedValues.storedChar='t';
			break;
		case 'x':
			WriteLCDChar(1, 0, '*');
			storedValues.storedChar='m';
			break;
		case '/':
			WriteLCDChar(1, 0, '/');
			storedValues.storedChar='d';
			break;

		default: break; //cases 'a', 't', 'm', 'd'
	}



	if(sensorArray.irsdot>0){
			if(storedValues.statusDot==0){
				storedValues.statusDot=1;
				WriteLCDChar(1, 0, '.');
			}

	}

	if(sensorArray.irsequal>0 ){
		mainstate = resultOnDisplay;
	}



	if(sensorArray.irs0>0){
		storedValues.secondStoredNumber= storedValues.statusDot<1? 2*storedValues.secondStoredNumber + 0 : storedValues.secondStoredNumber + 0 * power(0.5, storedValues.statusDot);
		if(storedValues.statusDot>0)
			storedValues.statusDot++;
		WriteLCDChar(1, 0, '0');
		setLCDstringbydoublewithNewline(storedValues.secondStoredNumber); //TODO felturbozni hogy latszon muvelet
		UART_SendString(LCDstring);
		DelayWithPWM(1500);
	}

	if(sensorArray.irs1>0){
		storedValues.secondStoredNumber= storedValues.statusDot<1? 2*storedValues.secondStoredNumber + 1 : storedValues.secondStoredNumber + 1 * power(0.5, storedValues.statusDot);
		if(storedValues.statusDot>0)
			storedValues.statusDot++;
		WriteLCDChar(1, 0, '1');
		setLCDstringbydoublewithNewline(storedValues.secondStoredNumber);
		UART_SendString(LCDstring);
		DelayWithPWM(1500);
	}

	if(storedValues.secondStoredNumber*power(10, storedValues.statusDot)>power(10, 15)-1){ //TODO felturbozni
			mainstate = overError;
			storedValues.storedChar='B';
	}

}

void ResultOnDisplayStateFunction(void){
	Return2ndRow();
	switch(storedValues.storedChar){
		case 'a':
			if(storedValues.firstStoredNumber+storedValues.secondStoredNumber<power(10, 15)){
				NumberToLCD(storedValues.firstStoredNumber+storedValues.secondStoredNumber);
				result =storedValues.firstStoredNumber+storedValues.secondStoredNumber;
				setLCDstringbydoublewithNewline(result);
				UART_SendString(LCDstring);
				DelayWithPWM(1500);
				ADConversion();
				//esetleg 1st numberbe el is tarolni eredmenyt
				storedValues.storedChar='s';
			}
			else{
				mainstate = overError;
				storedValues.storedChar='C';
			}
			break;
		case 't':
				NumberToLCD(storedValues.firstStoredNumber-storedValues.secondStoredNumber);
				result =storedValues.firstStoredNumber-storedValues.secondStoredNumber;
				setLCDstringbydoublewithNewline(result);
				UART_SendString(LCDstring);

				DelayWithPWM(1500);
				ADConversion();
				storedValues.storedChar='s';
			break;
		case 'm':
			if(storedValues.firstStoredNumber*storedValues.secondStoredNumber<power(10, 15)){
				NumberToLCD(storedValues.firstStoredNumber*storedValues.secondStoredNumber);
				result =storedValues.firstStoredNumber*storedValues.secondStoredNumber;
				setLCDstringbydoublewithNewline(result);
				UART_SendString(LCDstring);

				DelayWithPWM(1500);
				ADConversion();
				storedValues.storedChar='s';
			}
			else{
				mainstate = overError;
				storedValues.storedChar='C';
			}
			break;
		case 'd':
			NumberToLCD(storedValues.firstStoredNumber/storedValues.secondStoredNumber);

			result =shortendoubleto16digits(storedValues.firstStoredNumber/storedValues.secondStoredNumber);
			setLCDstringbydoublewithNewline(result);
			UART_SendString(LCDstring);

			DelayWithPWM(1500);
			storedValues.storedChar='s';
			break;
	}
	if(sensorArray.irsdot>0 && storedValues.storedChar!='C'){
		DelayWithPWM(500);
		if(sensorArray.irsdot>0){
			DelayWithPWM(500);
			if(sensorArray.irsdot>0){
				mainstate=firstNumber;
				storedValues.storedChar='c';
			}
		}

	}
	if(sensorArray.irsplus>0 && storedValues.storedChar!='C'){
		mainstate=secondNumber;
		storedValues.storedChar='+';
		storedValues.statusDot=0;
		storedValues.firstStoredNumber+=storedValues.secondStoredNumber;
		storedValues.secondStoredNumber=0;
	}

	if(sensorArray.irsminus>0 && storedValues.storedChar!='C'){
		mainstate=secondNumber;
		storedValues.storedChar='-';
		storedValues.statusDot=0;
		storedValues.firstStoredNumber-=storedValues.secondStoredNumber;
		storedValues.secondStoredNumber=0;
	}
	if(sensorArray.irsx>0 && storedValues.storedChar!='C'){
		mainstate=secondNumber;
		storedValues.storedChar='x';
		storedValues.statusDot=0;
		storedValues.firstStoredNumber*=storedValues.secondStoredNumber;
		storedValues.secondStoredNumber=0;
	}
	if(sensorArray.irsdivide>0 && storedValues.storedChar!='C'){
		mainstate=secondNumber;
		storedValues.storedChar='/';
		storedValues.statusDot=0;
		storedValues.firstStoredNumber= shortendoubleto16digits(storedValues.firstStoredNumber / storedValues.secondStoredNumber);
		storedValues.secondStoredNumber=0;

	}
}

void OverErrorStateFunction(void){
	switch(storedValues.storedChar){
		case 'A':
			ClearScreen();
			WriteLCDChar(1, 0, 'T');
			WriteLCDChar(1, 0, 'U');
			WriteLCDChar(1, 0, 'L');
			WriteLCDChar(1, 0, ' ');
			WriteLCDChar(1, 0, 'N');
			WriteLCDChar(1, 0, 'A');
			WriteLCDChar(1, 0, 'G');
			WriteLCDChar(1, 0, 'Y');
			storedValues.storedChar='s';
			DelayWithPWM(1500);
			break;
		case 'B':
			ClearScreen();
			WriteLCDChar(1, 0, 'T');
			WriteLCDChar(1, 0, 'U');
			WriteLCDChar(1, 0, 'L');
			WriteLCDChar(1, 0, ' ');
			WriteLCDChar(1, 0, 'N');
			WriteLCDChar(1, 0, 'A');
			WriteLCDChar(1, 0, 'G');
			WriteLCDChar(1, 0, 'Y');
			storedValues.storedChar='s';
			DelayWithPWM(1500);
			break;
		case 'C':
			ClearScreen();
			WriteLCDChar(1, 0, 'T');
			WriteLCDChar(1, 0, 'U');
			WriteLCDChar(1, 0, 'L');
			WriteLCDChar(1, 0, ' ');
			WriteLCDChar(1, 0, 'N');
			WriteLCDChar(1, 0, 'A');
			WriteLCDChar(1, 0, 'G');
			WriteLCDChar(1, 0, 'Y');
			storedValues.storedChar='s';
			DelayWithPWM(1500);
			break;
		default: break;
	}

	if(sensorArray.irsdivide>0){
		mainstate = firstNumber;
		storedValues.storedChar='c';
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
