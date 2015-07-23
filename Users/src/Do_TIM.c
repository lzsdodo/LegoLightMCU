#include "Do_TIM.h"

void PWM_RGBOutput( u8 Red, u8 Green, u8 Blue )
{
    //Output
    TIM3->CCR2 = (u16) ( 839 * (  Red   / 255.0 ));
    TIM3->CCR1 = (u16) ( 839 * (  Green / 255.0 ));
    TIM3->CCR3 = (u16) ( 839 * (  Blue  / 255.0 ));
}

void TIM_PWM_Config(void)
{
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_OCInitTypeDef       TIM_OCInitStructure;
  
  //TimerPeriod = (( SystemCoreClock / 2 ) / 100000 ) - 1;  //100000 means 100KHz
  /* TIM3 clock enable */
  RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM3, ENABLE );
  /* Time base configuration */
  //TIM3->ARR = TimerPeriod;                        //TIM_Period;
  //TIM3->PSC = 0;                                  //TIM_Prescaler;
  TIM_TimeBaseStructure.TIM_Period        = 839;    //TimerPeriod;                  
  TIM_TimeBaseStructure.TIM_Prescaler     = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
  /* PWM1 Mode configuration: Channel1 */
  //TIM3->CCR1 = CCR1_Val;                          //TIM_Pulse;设置第一通道占空比
  TIM_OCInitStructure.TIM_OCMode      = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse       = 0;          //TIM3->CCR1
  TIM_OCInitStructure.TIM_OCPolarity  = TIM_OCPolarity_Low;
  TIM_OC1Init( TIM3, &TIM_OCInitStructure );
  TIM_OC1PreloadConfig( TIM3, TIM_OCPreload_Enable );
  /* PWM1 Mode configuration: Channel2 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse       = 0;          //TIM3->CCR2
  TIM_OC2Init( TIM3, &TIM_OCInitStructure );
  TIM_OC2PreloadConfig( TIM3, TIM_OCPreload_Enable );
  /* PWM1 Mode configuration: Channel3 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse       = 0;          //TIM3->CCR3
  TIM_OC3Init( TIM3, &TIM_OCInitStructure );
  TIM_OC3PreloadConfig( TIM3, TIM_OCPreload_Enable ); //使能TIMx在CCR上的预装载寄存器
  //使能TIMx在ARR上所有的预装载寄存器
  TIM_ARRPreloadConfig( TIM3, ENABLE );               
  /* TIM3 enable counter */
  TIM_Cmd( TIM3, ENABLE );  
}

void TIM_Timer_Config(void)
{
    NVIC_InitTypeDef        NVIC_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    /* Enable the Clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    /* TIM2 IRQn Config */
    NVIC_PriorityGroupConfig( NVIC_PriorityGroup_2 ); 
    NVIC_InitStructure.NVIC_IRQChannel                    = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority  = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority         = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd                 = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /*
        設置自動裝入的值
        設置預分頻係數
        設置時鐘分割
        設置計數器模式
        每次向上溢出都產生更新事件
        開啟 TIM2 的中斷
        啟用 TIM2 計數器
        //重新將定時器設置為缺省值                TIM_DeInit的（TIM2）;
        //採用內部時鐘給TIM2提供時鐘源            TIM_InternalClockConfig(TIM2);      
        //清中斷，以免一啟用中斷後立即產生中斷    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
        //清除溢出中斷“標誌                      TIM_ClearFlag(TIM2, TIM_FLAG_Update);
        //禁止ARR預裝載緩衝器                     TIM_ARRPreloadConfig(TIM2, DISABLE);
    */
    // Period = 10000 - 1s; 1000 - 100ms; 100 - 10ms; 10 - 1ms;
    TIM_TimeBaseStructure.TIM_Period            = 100;
    TIM_TimeBaseStructure.TIM_Prescaler         = 8399;
    TIM_TimeBaseStructure.TIM_ClockDivision     = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode       = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM2, ENABLE);
    TIM_Cmd(TIM2, DISABLE);
}
  /*
    TIM(1,8,9-11)  input clock (TIM CLK) is set to 2 * APB2 clock (PCLK2),
    TIM(2-7,12-14) input clock (TIM CLK) is set to 2 * APB1 clock (PCLK1),
    TIM CLK = 2 * PCLKx;
  */
  /*
    TIM3 CLK = 2 * PCLK1 = HCLK / 2 = SystemCoreClock / 2
      => TIM3 CLK = SystemCoreClock / 2 ；
    Prescaler = (TIM3 CLK / TIM3 counter clock) - 1
      => Prescaler = ( ( SystemCoreClock / 2 ) / TIM3 counter clock ) - 1
      => TIM3 counter clock = 84 MHz;
    TIM output clock at 100 KHz,the period (ARR)) is computed as follows:
    ARR = (TIM counter clock / TIM output clock) - 1 ;
      =>  Period = (TIM counter clock / TIM output clock) - 1 = 100K
  */

/*
      Note: 
     SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f4xx.c file.
     Each time the core clock (HCLK) changes, user had to call SystemCoreClockUpdate()
     function to update SystemCoreClock variable value. Otherwise, any configuration
     based on this variable will be incorrect.  
*/


/*
  TIM1
  GPIO_InitTypeDef        GPIO_InitStructure;
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_OCInitTypeDef       TIM_OCInitStructure;
  
  // GPIO Configuration: PWM of TIM4 Output 
  RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOE, ENABLE );
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9 | GPIO_Pin_11 | GPIO_Pin_13 ;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init( GPIOE, &GPIO_InitStructure );  
  GPIO_PinAFConfig( GPIOE, GPIO_PinSource9,  GPIO_AF_TIM1 );
  GPIO_PinAFConfig( GPIOE, GPIO_PinSource11,  GPIO_AF_TIM1 ); 
  GPIO_PinAFConfig( GPIOE, GPIO_PinSource13, GPIO_AF_TIM1 ); 
  
  
  //TimerPeriod = (( SystemCoreClock / 2 ) / 100000 ) - 1;  //100000 means 100KHz

  // TIM3 clock enable //
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_TIM1, ENABLE );

  // Time base configuration //
  //TIM3->ARR = TimerPeriod;                            //TIM_Period;
  //TIM3->PSC = 0;                                      //TIM_Prescaler;
  TIM_TimeBaseStructure.TIM_Prescaler         = 0;
  TIM_TimeBaseStructure.TIM_CounterMode       = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period            = 1679;   //TimerPeriod;                  
  TIM_TimeBaseStructure.TIM_ClockDivision     = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit( TIM1, &TIM_TimeBaseStructure );

  // PWM1 Mode configuration: Channel1 //
  //TIMx->CCR1 = CCR1_Val;                          //TIM_Pulse;设置第一通道占空比
  TIM_OCInitStructure.TIM_OCMode      = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse       = 200;          //TIMx->CCR1
  TIM_OCInitStructure.TIM_OCPolarity  = TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
  TIM_OC1Init( TIM1, &TIM_OCInitStructure );

  // PWM1 Mode configuration: Channel2 //
  TIM_OCInitStructure.TIM_Pulse       = 400;          //TIMx->CCR2
  TIM_OC1Init( TIM1, &TIM_OCInitStructure );

  // PWM1 Mode configuration: Channel3 //
  TIM_OCInitStructure.TIM_Pulse       = 800;          //TIMx->CCR3
  TIM_OC1Init( TIM1, &TIM_OCInitStructure );
               
  // TIM1 enable counter //
  TIM_Cmd( TIM1, ENABLE );
  // Enable PWM Output //
  TIM_CtrlPWMOutputs( TIM1, ENABLE );   
  */