#ifndef __MCP3564_H
#define __MCP3564_H

#include "stm32f37x.h"
#include "stdio.h"

#define Dummy_Byte              0xFF
/*µÈ´ý³¬Ê±Ê±¼ä*/
#define SPIT_FLAG_TIMEOUT       ((uint32_t)0x1000)
#define SPIT_LONG_TIMEOUT       ((uint32_t)(10 * SPIT_FLAG_TIMEOUT))

#define SPI_ERROR(fmt,arg...)   printf("<<-SPI-ERROR->> "fmt"\n",##arg)

#define MCLK 					4915200		//4915200
#define AMCLK 					(MCLK/1)	//4915200
#define DMCLK 					(AMCLK/4)	//1228800

#define DEVICE_ADDRESS 			0x01

//COMMAND×Ö½Ú×é³É
#define DEVICE_ADDR_POS 		6
#define COMMAND_ADDR_POS 		2

#define SREAD_COMMAND_MASK 		0x01 //¾²Ì¬¶ÁÈ¡
#define SREAD_COMMAND 			((DEVICE_ADDRESS<<DEVICE_ADDR_POS) |SREAD_COMMAND_MASK)

#define WRITE_COMMAND_MASK 		0x02 //µÝÔöÐ´Èë
#define WRITE_COMMAND 			((DEVICE_ADDRESS<<DEVICE_ADDR_POS) |WRITE_COMMAND_MASK)

#define IREAD_COMMAND_MASK 		0x03 //µÝÔö¶ÁÈ¡
#define IREAD_COMMAND 			((DEVICE_ADDRESS<<DEVICE_ADDR_POS) |IREAD_COMMAND_MASK)
#define IREAD_CONFIG 			((CONFIG0_ADDR << COMMAND_ADDR_POS) | IREAD_COMMAND)

//STATUS×Ö½Ú×é³É
#define DR_STATUS_MASK 			0x04
#define DEV_ADDR_MASK			0x30

//ADCDATA¼Ä´æÆ÷
#define ADCDATA_ADDR 			0x00

//CONFIG0¼Ä´æÆ÷
#define CONFIG0_ADDR 			0x01
#define CONFIG0_WRITE 			((CONFIG0_ADDR << COMMAND_ADDR_POS) | WRITE_COMMAND)

//CONFIG1¼Ä´æÆ÷
#define CONFIG1_ADDR 			0x02
#define CONFIG1_WRITE 			((CONFIG1_ADDR << COMMAND_ADDR_POS) | WRITE_COMMAND)

//CONFIG2¼Ä´æÆ÷
#define CONFIG2_ADDR 			0x03
#define CONFIG2_WRITE 			((CONFIG2_ADDR << COMMAND_ADDR_POS) | WRITE_COMMAND)

//CONFIG3¼Ä´æÆ÷
#define CONFIG3_ADDR 			0x04
#define CONFIG3_WRITE 			((CONFIG3_ADDR << COMMAND_ADDR_POS) | WRITE_COMMAND)

//IRQ¼Ä´æÆ÷
#define IRQ_ADDR 				0x05
#define IRQ_WRITE 				((IRQ_ADDR << COMMAND_ADDR_POS) | WRITE_COMMAND)

//SCAN¼Ä´æÆ÷
#define SCAN_ADDR 				0x07
#define SCAN_WRITE 				((SCAN_ADDR << COMMAND_ADDR_POS) | WRITE_COMMAND)

//TIMER¼Ä´æÆ÷
#define TIMER_ADDR 				0x08
#define TIMER_WRITE 			((TIMER_ADDR << COMMAND_ADDR_POS) | WRITE_COMMAND)

//OFFSETCAL¼Ä´æÆ÷
#define OFFSETCAL_ADDR 			0x09
#define OFFSETCAL_WRITE 		((OFFSETCAL_ADDR << COMMAND_ADDR_POS) | WRITE_COMMAND)

//¿ìËÙÃüÁî
#define FASTCMD_UNUSE 			((DEVICE_ADDRESS<<DEVICE_ADDR_POS) | (0x00<<COMMAND_ADDR_POS) | 0x00)
#define FASTCMD_ADCSTART 		((DEVICE_ADDRESS<<DEVICE_ADDR_POS) | (0x0A<<COMMAND_ADDR_POS) | 0x00)
#define FASTCMD_STANDBY			((DEVICE_ADDRESS<<DEVICE_ADDR_POS) | (0x0B<<COMMAND_ADDR_POS) | 0x00)
#define FASTCMD_SHUTDOWN 		((DEVICE_ADDRESS<<DEVICE_ADDR_POS) | (0x0C<<COMMAND_ADDR_POS) | 0x00)
#define FASTCMD_FULLSHUTDOWN 	((DEVICE_ADDRESS<<DEVICE_ADDR_POS) | (0x0D<<COMMAND_ADDR_POS) | 0x00)
#define FASTCMD_RESET 			((DEVICE_ADDRESS<<DEVICE_ADDR_POS) | (0x0E<<COMMAND_ADDR_POS) | 0x00)

typedef struct
{
	u8 config0;
	u8 config1;
	u8 config2;
	u8 config3;
	u8 irp;
	u8 mux;
	u8 scan[3];
	u8 timer[3];
	u8 offsetcal[3];
	u8 gaincal[3];
	u8 reserved1[3];
	u8 reserved2;
	u8 lock;
	u8 reserved3[2];
	u8 crccfg[2];
}_mcp3564_config;
typedef struct
{
	u8 status;
	u8 adcdata[4];	//3564·µ»ØµÄadcÔ­Ê¼Êý¾Ý
}_mcp3564_data;

extern _mcp3564_config MCP3564_Config;
extern _mcp3564_data MCP3564_Data;
extern const u8 mcp3564_config[12];

void SPI1_Init(void);
void MCP3564_FastCMD(u8 Cmd);
u8 MCP3564_GetStatus(void);
void MCP3564_SendConfig(void);
void MCP3564_Write(u8* pBuffer, u8 Cmd, u8 NumByteToWrite);
void MCP3564_Read(u8* pBuffer, u8 Cmd, u8 NumByteToRead);
#endif

