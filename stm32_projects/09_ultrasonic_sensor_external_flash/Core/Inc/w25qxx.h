/*
 * w25qxx.h
 *
 *  Created on: Sep 16, 2025
 *      Author: VINOD
 */

#ifndef INC_W25QXX_H_
#define INC_W25QXX_H_
#include<stdint.h>
#include<stdbool.h>

void W25Q_RESET(void);
uint32_t W25_ReadID(void);
void W25q_Read(uint32_t startpage , uint8_t offset,uint32_t size,uint8_t *rData);
void Flash_Erase_Sector(uint16_t sector);
void write_disable(void);
void write_enable(void);
void flash_write(uint32_t page,uint16_t offset,uint32_t size,uint8_t *tdata);
uint32_t bytes_to_write(uint32_t size, uint16_t offset);
uint8_t Flash_ReadStatus(void);
void Flash_Write_Update(uint32_t Page , uint16_t offset,uint32_t size,uint8_t *data);
uint32_t bytes_to_modify(uint32_t size, uint16_t offset);
void Flash_AppendLog(char *msg);


#endif /* INC_W25QXX_H_ */
