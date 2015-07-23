#ifndef __Do_GPIO_H
#define __Do_GPIO_H
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"

/* Private define ------------------------------------------------------------*/
#define GREEN   0x1000
#define ORANGE  0x2000
#define RED     0x4000
#define BLUE    0x8000
#define ALL     0xF000

#define LED_GRN_ON    GPIOD->BSRRL  |=  GREEN;  
#define LED_GRN_OFF   GPIOD->BSRRH  |=  GREEN;
#define LED_GRN_TOG   GPIOD->ODR    ^=  GREEN;
#define LED_ORG_ON    GPIOD->BSRRL  |=  ORANGE;
#define LED_ORG_OFF   GPIOD->BSRRH  |=  ORANGE;
#define LED_ORG_TOG   GPIOD->ODR    ^=  ORANGE;
#define LED_RED_ON    GPIOD->BSRRL  |=  RED;
#define LED_RED_OFF   GPIOD->BSRRH  |=  RED;
#define LED_RED_TOG   GPIOD->ODR    ^=  RED;
#define LED_BLU_ON    GPIOD->BSRRL  |=  BLUE;
#define LED_BLU_OFF   GPIOD->BSRRH  |=  BLUE;
#define LED_BLU_TOG   GPIOD->ODR    ^=  BLUE;
#define LED_ALL_ON    GPIOD->BSRRL  |=  ALL;
#define LED_ALL_OFF   GPIOD->BSRRH  |=  ALL;
#define LED_ALL_TOG   GPIOD->ODR    ^=  ALL;

/* Private functions ---------------------------------------------------------*/
void GPIO_Config(void);

#endif

/*
	GPIO_SetBits( GPIOx, GPIO_Pin_x );		=>	GPIOx->BSRRL &= 0x0000;	//16位	
	GPIO_ResetBits( GPIOx, GPIO_Pin_x );	=>	GPIOx->BSRRH &= 0x0000;	//16位
	GPIO_ToggleBits( GPIOx, GPIO_Pin_x );	=>	GPIOx->ODR	 ^=	0x0000;	//16位

	16位管脚 对应 0x000
	0x8000 -- 16;	0x0800 -- 12;	0x0080 -- 8;	0x0008 -- 4;
	0x4000 -- 15;	0x0400 -- 11;	0x0040 -- 7;	0x0004 -- 3;
	0x2000 -- 14;	0x0200 -- 10;	0x0020 -- 6;	0x0002 -- 2;
	0x1000 -- 13;	0x0100 -- 9;	0x0010 -- 5;	0x0001 -- 1;
*/

/*
	GPIOD 12,13,14,15
	STM_EVAL_LEDOn( LED3-LED6 );	
	STM_EVAL_LEDOff( LED3-LED6 );
	STM_EVAL_LEDToggle( LED3-LED6 );
*/

