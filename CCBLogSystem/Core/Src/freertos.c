/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usart.h"
#include "fatfs.h"
#include "spi.h"
#include "w25qxx.h"
#include "usb_host.h"

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
/* USER CODE BEGIN Variables */
#define CCB_LOG_PRINT(fmt, ...)  CCB_logSystemPrint(__LINE__, __FILE__, __FUNCTION__, fmt, ##__VA_ARGS__)


/* USER CODE END Variables */
osThreadId ledTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void ReadID(void);

/* USER CODE END FunctionPrototypes */

void ledStartTask(void const * argument);

extern void MX_USB_HOST_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of ledTask */
  osThreadDef(ledTask, ledStartTask, osPriorityNormal, 0, 1024);
  ledTaskHandle = osThreadCreate(osThread(ledTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_ledStartTask */
/**
  * @brief  Function implementing the ledTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_ledStartTask */
void ledStartTask(void const * argument)
{
  /* init code for USB_HOST */
  MX_USB_HOST_Init();
  /* USER CODE BEGIN ledStartTask */
  printf(" mian£¡ \r\n");
  HAL_GPIO_WritePin(USB_PWR_GPIO_Port, USB_PWR_Pin, GPIO_PIN_SET);

  //SD_init();

  /* Infinite loop */
  for(;;)
  {
    HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
	//W25qxx_Init();
	//mount_disk();
	//format_disk();
	//create_file();
	//read_file();
	//USB_hardWareTest();
	//CCB_logSystemPrint("CCB Log System Test!!!!", __LINE__, __FILE__, __FUNCTION__);
	//CCB_LOG_PRINT("CCB Log System Test!!!!\r\n");
	
	 //CCB_LOG_PRINT("CCB Log System Test Num:%d\r\n", 123);
	 //CCB_logSystemPrint("CCB Log System Test\r\n", __LINE__, __FILE__, __FUNCTION__);
	
	  //CCB_logSystemPrint("CCB Log System Test num :%d\r\n", __LINE__, __FILE__, __FUNCTION__, 123);
	CCB_LOG_PRINT("CCB Log System Test!!!!\r\n");
	
	CCB_LOG_PRINT("output string:%s\r\n", "Chenzx!");
	
	CCB_LOG_PRINT("output int:%d\r\n", 6);

	osDelay(500);
		
	//SD_test();
	//printf("Hello World!!\r\n");
  }
  /* USER CODE END ledStartTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
