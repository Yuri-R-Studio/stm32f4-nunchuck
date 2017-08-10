/*
 * flash_interface.c
 *
 *  Created on: 8Aug.,2017
 *      Author: yuri
 */
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include "main.h"
#include "flash_interface.h"
#include "stm32f4xx_hal_flash_ex.h"
#include "error_code.h"

/**
  * @brief  Unlocks Flash for write access
  * @param  None
  * @retval None
  */
void FLASH_If_Init(void)
{
   HAL_FLASH_Unlock();
}

void FLASH_If_DeInit(void)
{
    HAL_FLASH_Lock();
}

/**
  * @brief  This function does an erase of all user flash area
  * @param  StartSector: start of user flash area
  * @param  nbSections: number of sections
  * @retval 0: user flash area successfully erased
  *         1: error occurred
  */
error_code_t FLASH_If_Erase(uint32_t StartSector, uint32_t nbSectors)
{

  /* Device voltage range supposed to be [2.7V to 3.6V], the operation will
     be done by word */

  if (StartSector <= FLASH_SECTOR_11 && nbSectors <= (FLASH_SECTOR_11 - StartSector + 1UL))
  {
    FLASH_EraseInitTypeDef FLASH_EraseInitStruct;
    uint32_t sectornb = 0;

    FLASH_EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
    FLASH_EraseInitStruct.Sector = StartSector;
    FLASH_EraseInitStruct.NbSectors = nbSectors;
    FLASH_EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;

    HAL_FLASH_Unlock();

    if (HAL_FLASHEx_Erase(&FLASH_EraseInitStruct, &sectornb) != HAL_OK)
      return (ERROR_FLASH_ERASE);
  }
  else
  {
    return (ERROR_FLASH_ERASE);
  }

  return (ERROR_OK);
}
/**
  * @brief  This function writes a data buffer in flash (data are 32-bit aligned).
  * @note   After writing data buffer, the flash content is checked.
  * @param  FlashAddress: start address for writing data buffer
  * @param  Data: pointer on data buffer
  * @param  DataLength: length of data buffer
  * @retval 0: Data successfully written to Flash memory
  *         1: Error occurred while writing data in Flash memory
  *         2: Written Data in flash memory is different from expected one
  */
error_code_t FLASH_If_Write(__IO uint32_t* FlashAddress, uint8_t* Data ,uint16_t DataLength)
{
  uint32_t i = 0;

  for (i = 0; (i < DataLength) && (*FlashAddress <= (USER_FLASH_END_ADDRESS-1)); i++)
  {
    /* Device voltage range supposed to be [2.7V to 3.6V], the operation will
       be done by word */
    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, *FlashAddress,  *(uint8_t*)(Data+i)) == HAL_OK)
    {
     /* Check the written value */
      if (*(uint8_t*)*FlashAddress != *(uint8_t*)(Data+i))
      {
        /* Flash content doesn't match SRAM content */
        return(ERROR_FLASH_WRITE);
      }
      /* Increment FLASH destination address */
      *FlashAddress += 1;
    }
    else
    {
      /* Error occurred while writing data in Flash memory */
      return (ERROR_FLASH_WRITE);
    }
  }

  return (ERROR_OK);
}
/**
 * Read array of bytes from the flash memory
 * @param src
 * @param dst
 * @param size
 * @return
 */
uint16_t read_flash(uint8_t *src, uint8_t* dst, const uint32_t size)
{
    uint16_t i = 0;

    if (size > FLASH_DATA_SIZE) return 0;

    for (i=0; i<size; i++)
    {
        if ((NULL == (dst+i)) || (NULL == (src+i)) ) break;
        *(dst+i) = *(src+i);
    }

    return i;
}
/**
 * Write the array of bytes into the flash memory
 * @param src
 * @param size
 * @return
 */
error_code_t write_flash (uint8_t *src, const uint32_t size)
{
	 __IO uint32_t flash_addr = FLASH_DATA_ADDR_START;
    if (size > FLASH_DATA_SIZE-1 || size < 2) {
        return ERROR_FLASH_WRITE;
    }
    if (NULL == src) {
        return ERROR_FLASH_WRITE;
    }

    FLASH_If_Init();

    // Erase the flash
    if (ERROR_OK !=FLASH_If_Erase(FLASH_DATA_SECTOR, 1)) {
        return ERROR_FLASH_ERASE;
    }

    // Write the data to flash
    if (FLASH_If_Write(&flash_addr, src, size) !=0) {
        return ERROR_FLASH_WRITE;
    }

    return ERROR_OK;
}
