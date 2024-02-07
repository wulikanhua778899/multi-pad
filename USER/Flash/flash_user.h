/********************************************************************************
* @author: iLx1
* @email: colorful_ilx1@163.com
* @date: 2023/11/26 14:27
* @version: 1.0
* @description: 
********************************************************************************/


#ifndef MULTI_FLASH_USER_H
#define MULTI_FLASH_USER_H

#include "stm32f1xx_hal.h"

// 最大 FLASH SIZE ->16 M
#define MAX_FLASH_SIZE (16 * 1024 * 1024);
// 块数量
#define BLOCK_NUM 256
// 扇区大小
#define SECTOR_SIZE 4096
// 每一层的存储偏移（* 4）
#define PER_FOLDER_SECTOR_SHIFT 2


uint8_t flash_init_user(void);

void storage_menu_to_flash(uint8_t menu_index, uint8_t * data_buff, uint16_t data_size, uint8_t mode);

void load_menu_from_flash(uint8_t menu_index, uint8_t * data_buff, uint16_t data_size, uint8_t mode);

#endif //MULTI_FLASH_USER_H
