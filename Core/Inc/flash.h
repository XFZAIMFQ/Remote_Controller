//
// Created by XFZAILDD on 2024/8/2.
//
#ifndef FLASH_H
#define FLASH_H

#include "main.h"

#define STM32_FLASH_SIZE    128     //所选STM32的FLASH容量大小(单位为K)
#if     STM32_FLASH_SIZE < 256      //设置扇区大小
#define STM_SECTOR_SIZE     1024    //1K字节
#else
#define STM_SECTOR_SIZE	    2048    //2K字节
#endif

#define STM32_FLASH_BASE        0x08000000        //STM32 FLASH的起始地址
#define FLASH_USER_START_ADDR   ( STM32_FLASH_BASE + STM_SECTOR_SIZE * 127 ) //写Flash的地址，这里从第127页开始
#define FLASH_USER_END_ADDR     ( STM32_FLASH_BASE + STM_SECTOR_SIZE * 128 ) //写Flash的地址，这里以第128页结束

void Flash_Erase(void);

void Flash_Write(uint32_t *pBuffer, uint32_t NumToWrite);

void Flash_Read(uint32_t *pBuffer, uint32_t NumToRead);


#endif //FLASH_H
