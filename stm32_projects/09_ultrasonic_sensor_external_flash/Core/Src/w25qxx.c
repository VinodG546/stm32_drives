/*
 * w25qxx.c
 *
 *  Created on: Sep 16, 2025
 *      Author: VINOD
 */

#include"w25qxx.h"
#include"main.h"

extern SPI_HandleTypeDef hspi2;
#define W25Q_SPI hspi2

#define csLOW()    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_RESET)
#define csHIGH()   HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_SET)


#define numBLOCK 256

void SPI_Write(uint8_t *data,uint8_t len)
{
	HAL_SPI_Transmit(&W25Q_SPI, data ,len, 2000);
}
void SPI_Read(uint8_t *data,uint8_t len)
{
	HAL_SPI_Receive(&W25Q_SPI, data, len, 5000);
}
void W25Q_RESET(void)
{
	uint8_t tData[2];
	tData[0]=0x66;
	tData[1]=0x99;
	csLOW();
	HAL_SPI_Transmit(&W25Q_SPI, tData, 2, 1000);
	csHIGH();
	HAL_Delay(100);
}

uint32_t W25_ReadID(void)
{
	uint8_t tData = 0x9f;
	uint8_t rData[3];
	csLOW();
	HAL_SPI_Transmit(&W25Q_SPI, &tData, 1, 1000);
	HAL_SPI_Receive(&W25Q_SPI, rData, 3, 1000);
	csHIGH();
	return ((rData[0]<<16)|(rData[1]<<8)|rData[2]);
}

void W25q_Read(uint32_t startpage , uint8_t offset,uint32_t size,uint8_t *rData)
{
	uint8_t tData[4];
	uint32_t memAddr = (startpage*256)+offset;
	tData[0]=0x03; //enable read
	tData[1]=(memAddr >> 16)&0xff;
	tData[2]=(memAddr >> 8) & 0xff;
	tData[3]=(memAddr)&0xff;
	csLOW();
	SPI_Write(tData,4);
	SPI_Read(rData, size);
	csHIGH();
}
void write_enable(void)
{
	uint8_t tData = 0x6;
	csLOW();
	SPI_Write(&tData, 1);
	csHIGH();
	HAL_Delay(5);
}

void write_disable(void)
{
	uint8_t tData = 0x4;
	csLOW();
	SPI_Write(&tData, 1);
	csHIGH();
	HAL_Delay(5);
}

void Flash_Erase_Sector(uint16_t sector)
{
	uint8_t Erase_addr[4];
	uint32_t memAddr = sector*16*256;

	write_enable();

	Erase_addr[0]=0x20;
	Erase_addr[1]=(memAddr >> 16)&0xff;
	Erase_addr[2]=(memAddr >> 8) & 0xff;
	Erase_addr[3]=(memAddr)&0xff;

	csLOW();
	SPI_Write(Erase_addr, 4);
	csHIGH();
	HAL_Delay(450);
	write_disable();

}
uint32_t bytes_to_write(uint32_t size, uint16_t offset)
{
	if((size+offset)<256) return size;
	else return (256-offset);
}
uint32_t bytes_to_modify(uint32_t size, uint16_t offset)
{
	if((size+offset)<4096) return size;
	else return (4096-offset);
}
void flash_write(uint32_t page,uint16_t offset,uint32_t size,uint8_t *tdata)
{
	uint8_t data[266];
	uint32_t startpage=page;
	uint32_t endpage=startpage+((size+offset-1)/256);
	uint32_t numpages=endpage-startpage+1;

	uint16_t startsector = startpage/16;
	uint16_t endsector = endpage/16;
	uint16_t numsectors=endsector-startsector;
	for(uint32_t i=0;i<numsectors ; i++)
	{
		Flash_Erase_Sector(startsector++);
	}

	uint32_t dataposition = 0;
	for(uint32_t i=0;i<numpages;i++)
	{
		uint32_t memAddr  = (startpage*256)+offset;
		uint16_t bytesremaining=bytes_to_write(size,offset);

		uint32_t indx = 0;

		write_enable();

		data[0] = 0x02;
		data[1] = (memAddr >> 16) & 0xFF;
		data[2] = (memAddr >> 8) & 0xFF;
		data[3] = memAddr & 0xFF;

		indx = 4;

		uint16_t bytestosend = bytesremaining + indx;

		for(uint16_t i=0;i<bytesremaining;i++)
		{
			data[indx++]=tdata[i+dataposition];
		}
		if(bytestosend > 250)
		{
			csLOW();
			SPI_Write(data, 100);
			SPI_Write(data, bytestosend-100);
			csHIGH();
		}
		else {
			csLOW();
			SPI_Write(data, bytestosend);
			csHIGH();
		}

		startpage++;
		offset = 0;
		size = size - bytesremaining;
		dataposition = dataposition + bytesremaining;

		HAL_Delay(5);

		write_disable();
	}
}

uint8_t Flash_ReadStatus(void)
{
    uint8_t cmd = 0x05; // Read Status Register
    uint8_t status = 0;

    csLOW();
    SPI_Write(&cmd, 1);
    SPI_Read(&status, 1);
    csHIGH();

    return status;
}

/*
 * This function helps us to overwrite the sector
 * for example we are trying to enter logs into the same page
 * first we are reading data from the page into buffer
 * after reading we will erase that page
 * we will do this until the last page where the data is still remaining
 * to that buffer data we will add our log and again write into flash
 */
void Flash_Write_Update(uint32_t page , uint16_t offset,uint32_t size,uint8_t *data)
{
	uint16_t startsector = page/16;
	uint16_t endsector  = (page+((size+offset-1)/256))/16;
	uint16_t numsector = endsector - startsector + 1;
	uint8_t previousData[4096];
	/*consider i want to update daat at page 20 and 100 th byte of that page
	 * page = 20 , offset =100
	 * 20 % 16 =4;
	 * 4*256 + 100 = 1124
	 */
	uint32_t sectoroffset = (page % 16)*256 +offset;
	uint32_t dataIndex = 0;

	for(uint16_t s=0 ; s < numsector ; s++)
	{
		uint32_t startpage = startsector *16;

		W25q_Read(startpage, 0 , 4096, previousData);

		uint16_t bytesRemainig = bytes_to_modify(size,sectoroffset);

		bool erase = false;
		for(uint16_t i=0;i<bytesRemainig ;i++)
		{
			if(previousData[sectoroffset + i] != 0xff)
			{
				erase = true;
				break;
			}
		}

		for(uint16_t i=0 ;i<bytesRemainig ;i++)
		{
			if((sectoroffset +i)<4096 && (dataIndex + i)<size)
			{
				previousData[sectoroffset+i] = data[dataIndex+i];
			}
		}

		if(erase)
		{
			Flash_Erase_Sector(startsector);
		}
		startsector++;
		sectoroffset = 0;
		dataIndex += bytesRemainig;
		size-=bytesRemainig;
	}
}

uint32_t log_page = 0;
uint16_t log_offset = 0;

void Flash_AppendLog(char *msg)
{
    uint32_t len = strlen(msg);

    // write log at current location
    Flash_Write_Update(log_page, log_offset, len, (uint8_t*)msg);

    // advance pointer
    log_offset += len;
    if (log_offset >= 256) {
        log_page++;
        log_offset = 0;
    }
}









