#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "stm32f37x.h"

#define sysclk 72000000

#define delay_us SysTick_Delay_Us
#define delay_ms SysTick_Delay_Ms

void SysTick_Delay_Us( __IO uint32_t us);
void SysTick_Delay_Ms( __IO uint32_t ms);

#endif /*__SYSTICK_H*/

