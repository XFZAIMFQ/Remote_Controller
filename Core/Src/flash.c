//
// Created by XFZAILDD on 2024/8/2.
//
#include "flash.h"
#include <stdio.h>

static FLASH_EraseInitTypeDef EraseInitStruct;
uint32_t PAGEError = 0;

/**********************************************************************************
 * 函数功能: 页擦除
 * 输入参数: 无
 * 返 回 值: 无
 * 说    明：无
 */
void Flash_Erase(void) {
    EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
    EraseInitStruct.PageAddress = FLASH_USER_START_ADDR;
    EraseInitStruct.NbPages = (FLASH_USER_END_ADDR - FLASH_USER_START_ADDR) / STM_SECTOR_SIZE;

    if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK) {
        HAL_FLASH_Lock();
        printf("Flash_Erase Error...1\n");
        Error_Handler();
    }
}

/**********************************************************************************
 * 函数功能: 数据写入
 * 输入参数: 写入数据缓存数组指针、写入数据数
 * 返 回 值: 无
 * 说    明：无
 */
void Flash_Write(uint32_t *pBuffer, uint32_t NumToWrite) {

    uint16_t i = 0;
    uint32_t Address = FLASH_USER_START_ADDR;
    HAL_FLASH_Unlock();        //解锁
    Flash_Erase();         //先擦除
    //再写入
    printf("Erase Completed Ready To Write...\n");
    while ((Address < FLASH_USER_END_ADDR) && (i < NumToWrite)) {
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address, pBuffer[i]) == HAL_OK) {
            Address = Address + 4;  //地址后移4个字节
            i++;
        } else {
            printf("Flash_Write Error \n");
            Error_Handler();
        }
    }
    HAL_FLASH_Lock();   //上锁
    printf("Flash_Write Completed...\n");
}

/**********************************************************************************
 * 函数功能: 数据读取
 * 输入参数: 读取数据缓存数组指针、读出数据数
 * 返 回 值: 无
 * 说    明：无
 */
void Flash_Read(uint32_t *pBuffer, uint32_t NumToRead) {
    uint16_t i = 0;
    uint32_t Address = FLASH_USER_START_ADDR;

    while ((Address < FLASH_USER_END_ADDR) && (i < NumToRead)) {
        pBuffer[i++] = *(__IO uint32_t *) Address;
        Address = Address + 4;   //地址后移4个字节
    }

}
