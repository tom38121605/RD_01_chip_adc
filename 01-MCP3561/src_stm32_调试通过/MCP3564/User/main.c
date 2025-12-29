#include "usart.h"
#include "systick.h"
#include "mcp3564.h"

/*******************************************************************************
* Function Name  : main
* Description    : 主函数
* Input          : None.
* Return         : None.
*******************************************************************************/
int main(void)
{
	SPI1_Init();
	USART1_Config(115200);
	
	MCP3564_Read(&MCP3564_Config.config0,IREAD_CONFIG, 27);
	if(	(MCP3564_Config.config0 != mcp3564_config[0])||
		(MCP3564_Config.config1 != mcp3564_config[1])||
		(MCP3564_Config.config2 != mcp3564_config[2])||
		(MCP3564_Config.config3 != mcp3564_config[3]))
	{
		MCP3564_SendConfig();
		MCP3564_Read(&MCP3564_Config.config0,IREAD_CONFIG, 27);
	}
	
	printf("MCP3564_Config.config0: 0x%02X\r\n",MCP3564_Config.config0);
	printf("MCP3564_Config.config1: 0x%02X\r\n",MCP3564_Config.config1);
	printf("MCP3564_Config.config2: 0x%02X\r\n",MCP3564_Config.config2);
	printf("MCP3564_Config.config3: 0x%02X\r\n",MCP3564_Config.config3);
	printf("MCP3564_Config.irp: 0x%02X\r\n",MCP3564_Config.irp);
	printf("MCP3564_Config.mux: 0x%02X\r\n",MCP3564_Config.mux);
	printf("MCP3564_Config.scan: 0x%02X%02X%02X\r\n",MCP3564_Config.scan[0],MCP3564_Config.scan[1],MCP3564_Config.scan[2]);
	printf("MCP3564_Config.offsetcal: 0x%02X%02X%02X\r\n",MCP3564_Config.offsetcal[0],MCP3564_Config.offsetcal[1],MCP3564_Config.offsetcal[2]);
	printf("MCP3564_Config.gaincal: 0x%02X%02X%02X\r\n",MCP3564_Config.gaincal[0],MCP3564_Config.gaincal[1],MCP3564_Config.gaincal[2]);

	MCP3564_FastCMD(FASTCMD_ADCSTART);

	while(1)
	{
		if(!(MCP3564_GetStatus()&0x04))	//有DR数据到来
		{
			MCP3564_Read(MCP3564_Data.adcdata,IREAD_COMMAND,4);
			printf("MCP3564_Data : 0x%02X%02X%02X%02X\r\n",MCP3564_Data.adcdata[0],MCP3564_Data.adcdata[1],MCP3564_Data.adcdata[2],MCP3564_Data.adcdata[3]);
		}
		SysTick_Delay_Ms(1);
	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/










