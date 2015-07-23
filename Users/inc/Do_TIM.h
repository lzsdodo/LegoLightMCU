#ifndef __Do_TIM_H
#define __Do_TIM_H

/* Private include ------------------------------------------------------------*/
#include "stm32f4xx_tim.h"
#include "stm32f4xx.h"
#include "Do_Com.h"
#include "Do_GPIO.h"

/* Private define ------------------------------------------------------------*/
#define ENABLE_TIMER  TIM_ClearFlag(TIM2, TIM_FLAG_Update);TIM_Cmd(TIM2, ENABLE);
#define DISABLE_TIMER TIM_Cmd(TIM2, DISABLE);

/* Private functions ---------------------------------------------------------*/
void TIM_PWM_Config(void);
void TIM_Timer_Config(void);
void PWM_RGBOutput( u8 Red, u8 Green, u8 Blue );

#endif