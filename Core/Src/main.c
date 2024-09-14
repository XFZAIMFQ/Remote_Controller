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
#include "button.h"
#include "calibration.h"
#include "data_fifo.h"
#include "joystick.h"
#include "retarget.h"
#include "uart_instruction.h"
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


uint32_t CPU_ID[3];

void Get_ChipID(void) {
    CPU_ID[0] = *(__IO uint32_t*)(0X1FFFF7F0); // 高字节
    CPU_ID[1] = *(__IO uint32_t*)(0X1FFFF7EC); //
    CPU_ID[2] = *(__IO uint32_t*)(0X1FFFF7E8); // 低字节
}


void uart_send_data() {
    if (FIFO_IsEmpty(&data_fifo) == 1) {
        return;
    }
    handle_data data;
    FIFO_Read(&data_fifo, &data);

    char buffer[150];
    // char str[] = "{\"id\":%ld,\"adc_x\":%.2f,\"adc_y\":%.2f,\"adc_h0\":%.2f,\"adc_h1\":%.2f,\"b0\":%u,\"b1\":%u,\"b2\":%u,\"b3\":%u,\"b4\":%u}\n";
    char str[] = "{%XH,%.2f,%.2f,%.2f,%.2f,%d,%d,%d,%d}\n";
    // sprintf(buffer,
    //         str,
    //         CPU_ID[2],
    //         ConvertToJoystickValue(data.sliding_filter_dma_adc[2], joystick_x_adc.min, joystick_x_adc.max,
    //                                joystick_x_adc.center),
    //         ConvertToJoystickValue(data.sliding_filter_dma_adc[1], joystick_y_adc.min, joystick_y_adc.max,
    //                                joystick_y_adc.center),
    //         ConvertToJoystickValue(data.sliding_filter_dma_adc[0], 0, 4096, 2048),
    //         ConvertToJoystickValue(data.sliding_filter_dma_adc[3], 0, 4096, 2048),
    //         data.button_state[0], data.button_state[1], data.button_state[2], data.button_state[3]
    // );
    sprintf(buffer,
            str,
            CPU_ID[2],
            data.sliding_filter_dma_adc[0],
            data.sliding_filter_dma_adc[1],
            data.sliding_filter_dma_adc[2],
            data.sliding_filter_dma_adc[3],
            data.button_state[0], data.button_state[1], data.button_state[2], data.button_state[3]
    );
    HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer), 1000);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim) {
    if (htim->Instance == TIM2) {
        //printf("%d,%d\n",dma_adc[2],sliding_filter_dma_adc[2]);
        //printf("%d,%d\n",dma_adc[2],sliding_filter_dma_adc[2]);
        sliding_filter_joystick_value();
        KeyScan();

        // handle_data data;
        // for (int i = 0; i < BUTTON_NUM; ++i) {
        //     data.button_state[i] = buttons[i].state;
        // }
        // for (int i = 0; i < JOYSTICK_VALUE; ++i) {
        //     // data.sliding_filter_dma_adc[i] = sliding_filter_dma_adc[i];
        //     data.sliding_filter_dma_adc[i] = dma_adc[i];
        // }
        // FIFO_Write(&data_fifo, &data);

        if (buttons[1].state && buttons[2].state) {
            recording_value++;
        } else {
            recording_value = 0;
        }
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
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_USART3_UART_Init();
  MX_ADC1_Init();
  MX_I2C1_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */

    Get_ChipID();
    RetargetInit(&huart1); //重定向串口
    // read_calibration_value();                   //读取校准值
    uart_instruction_init();                    //初始化串口指令
    FIFO_Init(&data_fifo, sizeof(handle_data)); //初始化数据缓冲区
    init_joystick();                            // 初始化摇杆
    HAL_TIM_Base_Start_IT(&htim2);              // 开启定时器

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    while (1) {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
        // uart_send_data();
        printf("%d,%d,%d,%d,%d,%d,%d,%d\n", dma_adc[0], dma_adc[1], dma_adc[2], dma_adc[3], buttons[0].state,
               buttons[1].state, buttons[2].state, buttons[3].state);
        HAL_Delay(10);
        if (recording_value >= RECORDING_VALUE) {
            calibration_joystick();
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
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
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
    while (1) {
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
