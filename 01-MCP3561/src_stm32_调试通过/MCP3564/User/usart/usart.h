#ifndef __USART_H
#define __USART_H

#include "stm32f37x.h"
#include <stdio.h>

// ´®¿Ú1-USART1	//PC
#define  USART1_TX_GPIO_PORT       GPIOA   
#define  USART1_TX_GPIO_PIN        GPIO_Pin_9
#define  USART1_RX_GPIO_PORT       GPIOA
#define  USART1_RX_GPIO_PIN        GPIO_Pin_10

 //´®¿Ú2-USART2	//485
#define  USART2_TX_GPIO_PORT       GPIOA   
#define  USART2_TX_GPIO_PIN        GPIO_Pin_2
#define  USART2_RX_GPIO_PORT       GPIOA
#define  USART2_RX_GPIO_PIN        GPIO_Pin_3

 //´®¿Ú3-USART3
#define  USART3_TX_GPIO_PORT       GPIOD   
#define  USART3_TX_GPIO_PIN        GPIO_Pin_8
#define  USART3_RX_GPIO_PORT       GPIOD
#define  USART3_RX_GPIO_PIN        GPIO_Pin_9

#define RS232_COM			3		//USART3
#define RS485_COM   		2		//USART2
#define PC_COM   			1		//USART1

#define DEBUG_USART   		USART1	 //USART1


void USART1_Config(u32 BAUDRATE);
void USART2_Config(u32 BAUDRATE);
void USART3_Config(u32 BAUDRATE);
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);
unsigned char Usart_ReceivByte(USART_TypeDef * pUSARTx);
void Usart_SendArray( USART_TypeDef * pUSARTx, uint8_t *array, uint16_t num);
void Usart_SendString( USART_TypeDef * pUSARTx, char *str);
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch);
void UARTWrite(u8 *Data, u16 NByte,u8 port);
void SendData2TestCOM(u8 *Save_Data,u16 lenth);
#endif /* __USART_H*/

