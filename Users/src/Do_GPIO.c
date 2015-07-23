#include "Do_GPIO.h"

//  GPIO Config
void GPIO_Config(void)
{
  /* GPIO TypeDef InitStructure */
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  /* GPIO clock enable */
  RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA, ENABLE );
  RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOB, ENABLE );
  RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOD, ENABLE );
  
  /* GPIOD Configuration: LED on the discovery board Config PD 12 13 14 15 */
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init( GPIOD, &GPIO_InitStructure );
  GPIOD->BSRRH  |=  0xF000;     //All LED Turn Off
  
  /* GPIO Configuration: Button on the discovery board Config PA00 */
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init( GPIOA, &GPIO_InitStructure );
  
  /* GPIO Configuration: Button1 & Button2 Config PE 04 & 05 */
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init( GPIOE, &GPIO_InitStructure );
  
  /* GPIO Configuration: PWM Config of PB 04(R),05(G),00(B) */
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_4 | GPIO_Pin_5 ;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL ;
  GPIO_Init( GPIOB, &GPIO_InitStructure ); 
  GPIO_PinAFConfig( GPIOB, GPIO_PinSource0, GPIO_AF_TIM3 );
  GPIO_PinAFConfig( GPIOB, GPIO_PinSource4, GPIO_AF_TIM3 ); 
  GPIO_PinAFConfig( GPIOB, GPIO_PinSource5, GPIO_AF_TIM3 );
}

  /* 
    GPIO结构体定义
    GPIOD Periph clock enable 使能时钟
    选择要控制的GPIOx引脚
    所选管脚的模式：输入、输出、悬空、模拟 
    设置引脚速率 
    output type 设置引脚的开漏: 推挽方式输出 还是 集电极开路方式输出
    Pull-up/Pull down 输入时上(下)拉状态:上拉 ( GPIO_PuPd_UP / GPIO_PuPd_DOWN / GPIO_PuPd_NOPULL )
    调用库函数,初始化GPIOx 
  */
  /*
  //Example of LED on the discovery board
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  */