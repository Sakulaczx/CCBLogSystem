/**
  ******************************************************************************
  * @file   fatfs.c
  * @brief  Code for fatfs applications
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

#include "fatfs.h"

uint8_t retUSBH;    /* Return value for USBH */
char USBHPath[4];   /* USBH logical drive path */
FATFS USBHFatFS;    /* File system object for USBH logical drive */
FIL USBHFile;       /* File object for USBH */
uint8_t retUSER;    /* Return value for USER */
char USERPath[4];   /* USER logical drive path */
FATFS USERFatFS;    /* File system object for USER logical drive */
FIL USERFile;       /* File object for USER */

/* USER CODE BEGIN Variables */
#include "stdio.h"
#include <stdarg.h>
#include "usb_host.h"
#include "rtc.h"

extern ApplicationTypeDef Appli_state;


/* USER CODE END Variables */

void MX_FATFS_Init(void)
{
  /*## FatFS: Link the USBH driver ###########################*/
  retUSBH = FATFS_LinkDriver(&USBH_Driver, USBHPath);
  /*## FatFS: Link the USER driver ###########################*/
  retUSER = FATFS_LinkDriver(&USER_Driver, USERPath);

  /* USER CODE BEGIN Init */
  /* additional user code for init */
  /* USER CODE END Init */
}

/**
  * @brief  Gets Time from RTC
  * @param  None
  * @retval Time in DWORD
  */
DWORD get_fattime(void)
{
  /* USER CODE BEGIN get_fattime */              
	DWORD timeBuff = 0;      
	RTC_TimeTypeDef fTime = {0};            
	RTC_DateTypeDef fDate = {0};  
	
	//获取当前日期时间        
	HAL_RTC_GetTime(&hrtc, &fTime, RTC_FORMAT_BIN);      
	HAL_RTC_GetDate(&hrtc, &fDate, RTC_FORMAT_BIN);  

	//按照FatFs的时间格式组    
	timeBuff |= ((fDate.Year + 2000 - 1980) << 25); //年
	timeBuff |= (fDate.Month << 21); //月
	timeBuff |= (fDate.Date << 16); //日
	timeBuff |= (fTime.Hours << 11); //时 
	timeBuff |= (fTime.Minutes << 5); //分
	timeBuff |= (fTime.Seconds / 2); //秒
	
    return timeBuff;
  /* USER CODE END get_fattime */
}

/* USER CODE BEGIN Application */
//void SD_init()
//{
//	if (f_mount(&USERFatFS, (const TCHAR *)USERPath, 0) != FR_OK)
//		printf("error!\r\n");
//
//	f_getfree("", &fre_clust, &pfs);
//	total = (uint32_t)((pfs->n_fatent - 2) * pfs->csize * 0.5);
//
//	printf("SD CARD total Space:%lu\r\n", total);
//
//	free_space = (uint32_t)(fre_clust * pfs->csize * 0.5);
//	
//	printf("SD CARD Free Space:%lu\r\n", free_space);
//}
extern volatile uint16_t userTimer1, userTimer2;					/* 1ms Timer Counter */

void mount_disk(void)
{
  uint8_t res = f_mount(&USERFatFS, USERPath, 0);
  if (res != FR_OK)
  {
    printf("FAILED: %d\n",res);
   return;
 }
  printf("MOUNT OK\n");
}
 
void format_disk(void)
{
  uint8_t res = 0;
      
  printf("PROCESSING...\n");
  //res = f_mkfs(USERPath, 1, 4096);
  if (res == FR_OK)
  {
    printf("OK!\n");
  }
  else
  {
    printf("failed with: %d\n",res);
  }
}
 
void create_file(void)
{
  FIL file;
  FIL *pf = &file;
  uint8_t res;
	
  res = f_open(pf, "0:/test.txt", FA_OPEN_ALWAYS | FA_WRITE);
  if (res == FR_OK)
  {
    printf("creat ok\n"); 				
  }
  else
  {
    printf("creat failed\n");
    printf("error code: %d\n",res);	
  }
  
  f_printf(pf, "hello fatfs!\n");
  
  res = f_close(pf);
    if (res != FR_OK)
    {
      printf("close file error\n");
      printf("error code: %d\n",res);				
    }
}
 
void get_disk_info(void)
{
	FATFS fs;
	FATFS *fls = &fs;
	FRESULT res;
	DWORD fre_clust;	
 
	res = f_getfree("/",&fre_clust,&fls);         /* Get Number of Free Clusters */
	if (res == FR_OK) 
	{
	                                             /* Print free space in unit of MB (assuming 4096 bytes/sector) */
        printf("%d KB Total Drive Space.\n"
               "%d KB Available Space.\n",
               ((fls->n_fatent-2)*fls->csize)*4,(fre_clust*fls->csize)*4);
	}
	else
	{
		printf("get disk info error\n");
		printf("error code: %d\n",res);
	}																									   
}
 
void read_file(void)
{
  FIL file;
  FRESULT res;
  UINT bw;
  uint8_t rbuf[100] = {0};
  
  res = f_open(&file, "0:/test.txt", FA_READ);
  if (res != FR_OK)
  {
    printf("open error: %d\n",res);
    return;
  }
  f_read(&file, rbuf, 20, &bw);
  printf("%s\n", rbuf);
  
  res = f_close(&file);
  if (res != FR_OK)
  {
     printf("close file error\n");
     printf("error code: %d\n",res);				
  }
}

void SD_test()
{
	UINT bw;
	int ret;
	uint8_t text[] = "czxczx!\r\n";

	//printf("time1:%d time2:%d \r\n", userTimer1, userTimer2);

	
	ret = f_open(&USERFile, "111.txt", FA_OPEN_ALWAYS | FA_OPEN_APPEND | FA_WRITE);
	if (ret != FR_OK)
        printf("Open fail!  error:%d \r\n", ret);
	f_write(&USERFile, text, sizeof(text), &bw);
	f_close(&USERFile);
}

void USB_hardWareTest()
{
 	UINT bw;
 	int ret;
 	uint8_t FileBuf[] = "czxczx!!!!!!!!!!!!!!!\r\n";
	
	// open power
 	HAL_GPIO_WritePin(USB_PWR_GPIO_Port, USB_PWR_Pin, GPIO_PIN_SET);
	
    printf("Appli_state:%d", Appli_state);
	
	if (Appli_state == APPLICATION_READY) {	   //U盘已经加载完成
		retUSBH = f_mount(&USBHFatFS, (TCHAR const*)USBHPath, 1);		
		if (retUSBH == FR_OK) { 
			printf("挂载U盘成功!\r\n");
		}		
		
		printf("写入文件测试!\r\n");
		retUSBH = f_open(&USBHFile,(const char*)"czx.txt",FA_OPEN_ALWAYS|FA_WRITE|FA_OPEN_APPEND);
		if (retUSBH == FR_OK) {
			printf("打开文件\"czx.txt\"成功!\r\n");
		}
		retUSBH = f_write(&USBHFile,FileBuf,sizeof(FileBuf),&bw);
		if(retUSBH	== FR_OK) {
			printf("写入文件成功!\r\n");
		}			
		retUSBH = f_close(&USBHFile);
		if (retUSBH == FR_OK) {
			printf("关闭文件成功!\r\n\r\n");
		}
		
		printf("读取文件测试!\r\n");
		retUSBH = f_open(&USBHFile,(const char*)"czx.txt",FA_OPEN_EXISTING|FA_READ);
		if (retUSBH == FR_OK) {
			printf("打开文件\"czx.txt\"成功!\r\n");
		}
		retUSBH = f_read(&USBHFile,FileBuf,bw,&bw);
		if (retUSBH==FR_OK) {
			printf("读取文件成功!文件内容：%s", FileBuf);
		}
		retUSBH = f_close(&USBHFile);
		if (retUSBH == FR_OK) {
			printf("关闭文件成功!\r\n");
		}
	}
}

void test(const char * format, ...)
{
     char buf[4069];
     va_list list;
     va_start(list, format);
     vsnprintf(buf, 4069, format, list);
     va_end(list);
     printf("%s\n", buf);
 } 

void CCB_logSystemPrint(uint8_t line, uint8_t *file, const char *function, const char *logBuff, ...)
{
	if (logBuff == NULL) {
		return;	
	}

	const char *logFileName = "CCBLogFile.txt";
	DWORD timeStamp = get_fattime();
	char buff[250] = {0};	
	char vLogBuf[200] = {0};
 	UINT bw;


	va_list args;
	
    va_start(args, logBuff);
	vsnprintf(vLogBuf, 250, logBuff, args);	
	va_end(args);
	
	sprintf(buff, "[%lu]%s <line:%d> %s() %s", timeStamp, file, line, 
		function, vLogBuf);
	//test("12345%s, %d_%s", "this is test", 6, "abc");
	printf("%s\r\n", buff);
	
	if (Appli_state == APPLICATION_READY) {
		retUSBH = f_mount(&USBHFatFS, (TCHAR const*)USBHPath, 1);
		
		if (retUSBH == FR_OK) { 
			printf("挂载U盘成功!\r\n");
		}		
		retUSBH = f_open(&USBHFile, logFileName, FA_OPEN_ALWAYS | 
												 FA_OPEN_APPEND |
												 FA_WRITE);		
		if (retUSBH == FR_OK) {
			printf("打开文件成功!\r\n");
		}
		
		retUSBH = f_write(&USBHFile, buff, strlen(buff), &bw);		
		if(retUSBH	== FR_OK) {
			printf("写入文件成功!\r\n");
		}
		
		retUSBH = f_close(&USBHFile);			
		if (retUSBH == FR_OK) {
			printf("关闭文件成功!\r\n\r\n");
		}
	}
}
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
