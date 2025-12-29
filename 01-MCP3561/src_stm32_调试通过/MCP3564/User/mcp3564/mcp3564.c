#include "mcp3564.h"
#include "string.h"

_mcp3564_config MCP3564_Config;
_mcp3564_data MCP3564_Data;

static __IO uint32_t  SPITimeout = SPIT_LONG_TIMEOUT;    
static uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode);
static void MCP3564_CS_LOW(void);
static void MCP3564_CS_HIGH(void);

const u8 mcp3564_config[12]=
{						//从config0开始写
	0x30, 				//config0：0011 0011，内部时钟，ADC关断模式
	0x0C,				//config1：0000 1100，OSR256  转换时间，768个DMCLK周期
	0x8B, 				//config2：1000 1011，增益为1，
	0xF3, 				//config3：1111 0011，连续转换模式，32位数据，使能数字失调校准,使能数字增益校准
	0x73,				//IRQ：IRQ
	0x01,				//mux：
	0xE0,0x00,0xFF,		//SCAN：512个DMCLK间隔，... ... 7 6 5 4 3 2 1 0
	0x00,0x00,0x00,		//timer：无延时
};//配置0-3

void SPI1_Init(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(	RCC_AHBPeriph_GPIOA,  ENABLE);
	
	/* 使能SPI时钟 */
	RCC_APB2PeriphClockCmd ( RCC_APB2Periph_SPI1, ENABLE );

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_5);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_5);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_5);
	
	/* 配置SPI的 CS引脚，普通IO即可 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* 配置SPI的 SCK引脚*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* 配置SPI的 MISO引脚*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* 配置SPI的 MOSI引脚*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* 停止信号 CS引脚高电平*/
	MCP3564_CS_HIGH();

	/* SPI 模式配置 */
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;	//空闲时CLK为低电平，3564工作在SPI00模式
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 0;
	SPI_Init(SPI1 , &SPI_InitStructure);

	SPI_RxFIFOThresholdConfig(SPI1, SPI_RxFIFOThreshold_QF);

	/* 使能 SPI  */
	SPI_Cmd(SPI1 , ENABLE);
}

static void MCP3564_CS_LOW(void)
{
	GPIO_ResetBits( GPIOA, GPIO_Pin_4 );
}

static void MCP3564_CS_HIGH(void)
{
	GPIO_SetBits( GPIOA, GPIO_Pin_4 );
}

u8 MCP3564_SendByte(u8 byte)
{
	 SPITimeout = SPIT_FLAG_TIMEOUT;
	/* 等待发送缓冲区为空，TXE事件 */
	while (SPI_I2S_GetFlagStatus(SPI1 , SPI_I2S_FLAG_TXE) == RESET)
	{
		if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(0);
	}

	/* 写入数据寄存器，把要写入的数据写入发送缓冲区 */
	SPI_SendData8(SPI1 , byte);

	SPITimeout = SPIT_FLAG_TIMEOUT;
	/* 等待接收缓冲区非空，RXNE事件 */
	while (SPI_I2S_GetFlagStatus(SPI1 , SPI_I2S_FLAG_RXNE) == RESET)
	{
		if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(1);
	}

	/* 读取数据寄存器，获取接收缓冲区数据 */
	return SPI_ReceiveData8(SPI1 );
}

void MCP3564_Write(u8* pBuffer, u8 Cmd, u8 NumByteToWrite)
{
	MCP3564_CS_LOW();
	
	MCP3564_SendByte(Cmd);

	while (NumByteToWrite--)
	{
		MCP3564_SendByte(*pBuffer);
		pBuffer++;
	}

	MCP3564_CS_HIGH();
}

void MCP3564_Read(u8* pBuffer, u8 Cmd, u8 NumByteToRead)
{	
	MCP3564_CS_LOW();

	MCP3564_SendByte(Cmd);

	while (NumByteToRead--)
	{
		*pBuffer = MCP3564_SendByte(Dummy_Byte);
		pBuffer++;
	}
	
	MCP3564_CS_HIGH();
}

u8 MCP3564_GetStatus(void)
{
	u8 status=0;
	
	MCP3564_CS_LOW();

	status=MCP3564_SendByte(FASTCMD_UNUSE);//0100 0000

	MCP3564_CS_HIGH();
	
	return status;
}

void MCP3564_FastCMD(u8 Cmd)
{
	MCP3564_CS_LOW();

	MCP3564_SendByte(Cmd);
	
	MCP3564_CS_HIGH();
}

void MCP3564_SendConfig(void)
{
	MCP3564_Write((u8*)mcp3564_config,CONFIG0_WRITE,sizeof(mcp3564_config));
}

static uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{
	SPI_ERROR("SPI 等待超时!errorCode = %d",errorCode);
	return 0;
}




