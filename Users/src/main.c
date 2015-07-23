/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static volatile vs32 TimingDelay = 0; 

/* My Equipment Variables */
extern u8   Que_Exe[QUEUE_EXE_SIZE];    // 执行队列
extern u8   Que_Snd[QUEUE_SND_SIZE];    // 发送队列
extern u8   Que_Rtn[QUEUE_RTN_SIZE];    // 返回队列
  
extern u8   Head_Exe, Head_Snd, Head_Rtn; // 执行(发送、返回) 队列头       
extern u8   Rear_Exe, Rear_Snd, Rear_Rtn; // 执行(发送、返回) 队列尾

extern u8   U1_Snd_Buf[BUF_SIZE];     // USART1~6 发送缓存区
extern u8   U2_Snd_Buf[BUF_SIZE];           
extern u8   U3_Snd_Buf[BUF_SIZE];
extern u8   U4_Snd_Buf[BUF_SIZE];
extern u8   U5_Snd_Buf[BUF_SIZE];
extern u8   U6_Snd_Buf[BUF_SIZE];

extern u8   U1_Rev_Buf[BUF_SIZE];     // USART1~6 接收缓存区
extern u8   U2_Rev_Buf[BUF_SIZE];
extern u8   U3_Rev_Buf[BUF_SIZE];
extern u8   U4_Rev_Buf[BUF_SIZE];
extern u8   U5_Rev_Buf[BUF_SIZE];
extern u8   U6_Rev_Buf[BUF_SIZE];

extern u8   U1_Snd_Fin;               // USART1~6 DMA发送完成标志位
extern u8   U2_Snd_Fin;
extern u8   U3_Snd_Fin;
extern u8   U4_Snd_Fin;
extern u8   U5_Snd_Fin;
extern u8   U6_Snd_Fin;

u8  Exeing_Buf[BUF_SIZE];     // 执行队列抽取缓存区
u8  Snding_Buf[BUF_SIZE];     // 发送队列抽取缓存区

u8  ExeCtn_Status = FALSE;    // 持续执行状态 Execute Continually     

const u16 MyPubID = 2;    // 公共ID
u32 MyOwnID       = 0;    // 设备ID
u8  MyLayer       = 0;    // 设备层数
u8  Red           = 50;                 
u8  Green         = 50; 
u8  Blue          = 50; 
u8  Luminance     = 50;
u8  Sav_Hue[4]    = {0};  // 改变颜色后保存的颜色值

u8  Test = 0;

/* Private function prototypes -----------------------------------------------*/
void SysTick_Init(void);
void Delayms( u16 nTime );

/*************************@brief  Main program************************/
int main(void)
{
    SysTick_Init();                 // System Tick Clock Init
    GPIO_Config();                  // GPIO Config
    //EXTILine_Button_Config();       // EXTI Button Config
    TIM_PWM_Config();               // PWM-TIM Config
    TIM_Timer_Config();             // Timer-TIM Config
    USART_All_Config();             // USART Config
    USART_All_Rev_IRQ_OFF();        // USART All Turn off the Receive IRQn Interrupt
    DMA_All_Config();               // DMA-USART-TX Config
    Delayms(10);                    // 等待DMA发送完成
    USART_All_Rev_IRQ_ON();         // 进入主循环，开启所有接收中断
    PWM_RGBOutput(0,0,0);           // 3路PWM初始化
    printf("System Did Config\n");  // "初始化完毕"
    
    LED_ALL_ON;                     // LED on the Board All ON
    Delayms(50);                   // Delay 500ms
    LED_ALL_OFF;                    // LED on the Board All OFF
    
    printf("Enter Main Loop\n");    // "进入主循环"
    while (1)
    {   /* Main Loop */
        
        // 一：检查执行队列 是否空 ************************************************************************************
        if( FALSE == Is_Queue_Empty( Head_Exe, Rear_Exe ) )
        {   // 1.清空持续执行状态 2.提取队列数据 3.执行函数 4.退出执行状态     
            ExeCtn_Status = FALSE;   
            Head_Exe = Push_Queue( Head_Exe, Que_Exe, Exeing_Buf );
            Executing_Fun( Exeing_Buf );
        }    
        else  // 执行队列为空 是否为持续状态的执行指令
        {   // 持续运行
            if( TRUE == ExeCtn_Status )
                Executing_Continually_Fun( Exeing_Buf );
        }

        // 二：检查各串口DMA发送状态 是否发送完成 完成后 退出发送状态 **********************************************************
        if( TRUE == (U2_Snd_Fin & U3_Snd_Fin & U4_Snd_Fin & U5_Snd_Fin & U6_Snd_Fin) )
        {  
            // 检查 发送队列 是否为空
            if( FALSE == Is_Queue_Empty( Head_Snd, Rear_Snd ) )
            {
                // 1.提取队列数据到处理区中 2.检查功能位 根据功能位确认发送位置
                Head_Snd = Push_Queue( Head_Snd, Que_Snd, Snding_Buf );
                switch( Snding_Buf[BUF_FUN] )
                {
                    // 1.为ASK 根据Port口修改数据包内容 存向所有DMA发送缓存区 
                    case FUN_ASK_ID:
                    {
                        Ask_U2to6();              // 把数据传送到所有串口发送BUF中
                        Reset_U2to6_Finish();     // 重置DMA发送状态
                        Enable_U2to6_DMA_Snd();   // 开启DMA发送
                        break;
                    }
                    // 2.为Return 无需修改 存向U2~6的DMA发送缓存区 并发送
                    case FUN_RTN_ID:
                    {   // 1.数据发送到U2~6的Buf中 2.重置DMA发送状态 3.开启DMA发送
                        Send_toU2to6_Port();        
                        Reset_U2to6_Finish();    
                        Enable_U2to6_DMA_Snd();   
                        break;
                    }
                    // 3.不为ASK&Return 判断:公共ID/特定ID 选择发送区域 存向发送口DMA发送缓存区 
                    default:
                    {
                        switch( Whose_ID( Snding_Buf ) )
                        {   // 判断ID所属： 1.公共ID 2.特定设备ID
                            case PUB_ID:
                            {   // 1.数据发送到U2~6的Buf中 2.重置DMA发送状态 3.开启DMA发送
                                Send_toU2to6_Port();        
                                Reset_U2to6_Finish();     
                                Enable_U2to6_DMA_Snd();   
                                break;
                            }
                            default:
                            { 
                                // 1.开启相应串口DMA发送( 检测该传送至哪个串口 )
                                Send_toPort( Send_toWhich_Port( Snding_Buf, MyLayer ) );
                                break;
                            }
                        }
                        break;
                    }
                } 
            }
        }
        
        // 三:检查是否回送状态  ****************************************************************************
        if( TRUE == U1_Snd_Fin )
        {
            // 检查回送ID队列 不空 进行处理
            if( FALSE == Is_Queue_Empty( Head_Rtn, Rear_Rtn ) )
            {
                // 1.进入返回状态 2.提取队列数据 插入 USART1回送缓存区 3.开启DMA发送，直到发送完毕
                U1_Snd_Fin = FALSE;
                Head_Rtn = Push_Queue( Head_Rtn, Que_Rtn, U1_Snd_Buf );
                ENABLE_U1_DMA_SND;
            }
        }
        
        /* Running Flag of the Main Loop */ 
        //LED_ORG_TOG;      //  橙灯闪烁: 观察主循环有没跑飞
        //Delayms(100);     //  Delay 100ms
    }
}


// @brief  SysTick Init Function.
void SysTick_Init(void)
{
  /* 
    SystemCoreClock / 1000    1ms中断一次   168000
    SystemCoreClock / 1000000 1us中断一次   168
                              10ms          16800
  */
  //Captire error
  while(SysTick_Config(168000));  
  //关闭滴答定时器
  SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
  //开启滴答定时器
  //SysTick->CTRL |=   SysTick_CTRL_ENABLE_Msk;
}

/**
  * @brief  Delay Function.
  * @param  nCount:specifies the Delay time length.
  * @retval None
  */
void Delayms( u16 nTime )
{
    SysTick->CTRL |=   SysTick_CTRL_ENABLE_Msk;
    TimingDelay = nTime;
    while( TimingDelay != 0 ); 
    SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
}

void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0)
    { 
        TimingDelay--;
    }
}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
