/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern __IO u32 TimingDelay; 

u8 U1_Snd_Fin = TRUE;             // USART1~6 DMA发送完成标志位
u8 U2_Snd_Fin = TRUE;
u8 U3_Snd_Fin = TRUE;
u8 U4_Snd_Fin = TRUE;
u8 U5_Snd_Fin = TRUE;
u8 U6_Snd_Fin = TRUE;

volatile u8  U1_Snd_Cnt = 0;      // USART 发送计数变量
volatile u8  U2_Snd_Cnt = 0;
volatile u8  U3_Snd_Cnt = 0;
volatile u8  U4_Snd_Cnt = 0;
volatile u8  U5_Snd_Cnt = 0;
volatile u8  U6_Snd_Cnt = 0;

extern u8 U1_Snd_Buf[BUF_SIZE];   // USART1~6 发送缓存区
extern u8 U2_Snd_Buf[BUF_SIZE];
extern u8 U3_Snd_Buf[BUF_SIZE];
extern u8 U4_Snd_Buf[BUF_SIZE];
extern u8 U5_Snd_Buf[BUF_SIZE];
extern u8 U6_Snd_Buf[BUF_SIZE];

extern u8 U1_Rev_Buf[BUF_SIZE];   // USART1~6 接收缓存区
extern u8 U2_Rev_Buf[BUF_SIZE];
extern u8 U3_Rev_Buf[BUF_SIZE];
extern u8 U4_Rev_Buf[BUF_SIZE];
extern u8 U5_Rev_Buf[BUF_SIZE];
extern u8 U6_Rev_Buf[BUF_SIZE];

/* 接收中断次数变量 */
volatile u8  U1_i = 0;
volatile u8  U2_i = 0;
volatile u8  U3_i = 0;
volatile u8  U4_i = 0;
volatile u8  U5_i = 0;
volatile u8  U6_i = 0;

// 进入10ms定时器中断标志
volatile u8  Timer_Status = FALSE;

/* Private function prototypes -----------------------------------------------*/

/******************************************************************************/
/*                Cortex-M4 Processor Exceptions Handlers                     */
/*              进入中断第一件事必须是清除该中断的标志位！！！                */
/******************************************************************************/
/* 定时器2中断 */
void TIM2_IRQHandler(void)
{
    //if( TIM_GetFlagStatus(TIM2, TIM_FLAG_Update) == SET )
    if((TIM2->SR & TIM_FLAG_Update) == SET)    
    {
        //TIM_ClearITPendingBit(TIM2, TIM_FLAG_Update);
        TIM2->SR = ~TIM_IT_Update;
        LED_GRN_TOG;  
        // 1.进入10ms标志 2.关闭定时器中断
        Timer_Status = TRUE;
        DISABLE_TIMER;
    }
}

/* 串口USART接收中断 */
/*
    printf("In xxx IRQn \n");
    USART_ReceiveData( USART1 );
    if( USART_GetFlagStatus( USARTx, USART_FLAG_RXNE ) != RESET )
    USART_ClearITPendingBit( USARTx, USART_IT_RXNE );
*/

// 串口1中断服务函数
void USART1_IRQHandler(void)
{
    if( USART_GetFlagStatus( USART1, USART_FLAG_RXNE ) != RESET )
    {   // 1.清除串口接收中断标志   2.串口中断 蓝色LED变化  
        USART_ClearITPendingBit( USART1, USART_IT_RXNE );
        LED_BLU_TOG;
        // 3.串口中断应答函数
        u8 Tmp_Rev1 = USART_ReceiveData(USART1);
        switch( U1_i )
        {
            case 0:
            case 1:
                {
                    if( 0xff == Tmp_Rev1 ) 
                        U1_Rev_Buf[U1_i++] = 0xff;
                    else 
                        U1_i = 0; 
                    break;
                }
            case 2:
                {
                    if( 0xff == Tmp_Rev1 ) 
                        U1_i = 1; 
                }
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
            case 10:
            case 11:
            case 12:
                {
                    U1_Rev_Buf[U1_i++] = Tmp_Rev1;
                    break;
                }
            case 13:
                {
                    if( Tmp_Rev1 == Cal_Check_Sum( U1_Rev_Buf ) ) 
                    {   
                        // 1.确认数据包正确接收 2.存储校验和值 3.进入应答函数 4.清空接收缓存
                        U1_Rev_Buf[BUF_CHK] = Tmp_Rev1;
                        U1_Rev_Hnd(); 
                        Clr_Buf_Data( U1_Rev_Buf );
                    }
                    U1_i = 0;
                    break;
                }
            default:
                {
                    U1_i = 0;
                    break;
                }
        }
    }
}

// 串口2~6中断服务函数
void USART2_IRQHandler(void)
{
    if( USART_GetFlagStatus( USART2, USART_FLAG_RXNE ) != RESET )
    {  
        USART_ClearITPendingBit( USART2, USART_IT_RXNE ); 
        LED_BLU_TOG;
        // 3.串口中断应答函数
        u8 Tmp_Rev2 = USART_ReceiveData(USART2); 
        switch( U2_i )
        {
            case 0:
            case 1:
            {
                if( 0xff == Tmp_Rev2 ) 
                    U2_Rev_Buf[U2_i++] = 0xff;
                else 
                    U2_i = 0; 
                break;
            }
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
            case 10:
            case 11:
            case 12:
            {
                U2_Rev_Buf[U2_i++] = Tmp_Rev2;
                break;
            }
            case 13:
            {
                if( Tmp_Rev2 == Cal_Check_Sum( U2_Rev_Buf ) ) 
                {   
                    U2_Rev_Buf[BUF_CHK] = Tmp_Rev2;
                    U2_Rev_Hnd();  
                    Clr_Buf_Data( U2_Rev_Buf );
                }
                U2_i = 0;
                break;
            }
            default:
            {
                U2_i = 0;
                break;
            }
        }
    }
}

void USART3_IRQHandler(void)
{
    if( USART_GetFlagStatus( USART3, USART_FLAG_RXNE ) != RESET )
    {
        USART_ClearITPendingBit( USART3, USART_IT_RXNE );
        LED_BLU_TOG;
        // 3.串口中断应答函数
        u8 Tmp_Rev3 = USART_ReceiveData(USART3); 
        switch( U3_i )
        {
            case 0:
            case 1:
            {
                if( 0xff == Tmp_Rev3 ) 
                    U3_Rev_Buf[U3_i++] = 0xff;
                else 
                    U3_i = 0; 
                break;
            }
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
            case 10:
            case 11:
            case 12:
            {
                U3_Rev_Buf[U3_i++] = Tmp_Rev3;
                break;
            }
            case 13:
            {
                if( Tmp_Rev3 == Cal_Check_Sum( U3_Rev_Buf ) ) 
                {   
                    U3_Rev_Buf[BUF_CHK] = Tmp_Rev3;
                    U3_Rev_Hnd();   
                    Clr_Buf_Data( U3_Rev_Buf );
                }
                U3_i = 0;
                break;
            }
            default:
            {
                U3_i = 0;
                break;
            }
        }
    }
}

void UART4_IRQHandler(void)
{
    if( USART_GetFlagStatus( UART4, USART_FLAG_RXNE ) != RESET )
    {
        USART_ClearITPendingBit( UART4, USART_IT_RXNE ); 
        LED_BLU_TOG;
        // 3.串口中断应答函数
        u8 Tmp_Rev4 = USART_ReceiveData(UART4);  
        switch( U4_i )
        {
            case 0:
            case 1:
            {
                if( 0xff == Tmp_Rev4 ) 
                    U4_Rev_Buf[U4_i++] = 0xff;
                else 
                    U4_i = 0; 
                break;
            }
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
            case 10:
            case 11:
            case 12:
            {
                U4_Rev_Buf[U4_i++] = Tmp_Rev4;
                break;
            }
            case 13:
            {
                if( Tmp_Rev4 == Cal_Check_Sum( U4_Rev_Buf ) ) 
                {
                    U4_Rev_Buf[BUF_CHK] = Tmp_Rev4;
                    U4_Rev_Hnd();   
                    Clr_Buf_Data( U4_Rev_Buf );
                }
                U4_i = 0;
                break;
            }
            default:
            {
                U4_i = 0;
                break;
            }
        }
    }
}

void UART5_IRQHandler(void)
{
    if( USART_GetFlagStatus( UART5, USART_FLAG_RXNE ) != RESET )
    {
        USART_ClearITPendingBit( UART5, USART_IT_RXNE ); 
        LED_BLU_TOG;
        // 3.串口中断应答函数
        u8 Tmp_Rev5 = USART_ReceiveData(UART5);   
        switch( U5_i )
        {
            case 0:
            case 1:
            {
                if( 0xff == Tmp_Rev5 ) 
                    U5_Rev_Buf[U5_i++] = 0xff;
                else 
                    U5_i = 0; 
                break;
            }
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
            case 10:
            case 11:
            case 12:
            {
                U5_Rev_Buf[U5_i++] = Tmp_Rev5;
                break;
            }
            case 13:
            {
                if( Tmp_Rev5 == Cal_Check_Sum( U5_Rev_Buf ) ) 
                {
                    U5_Rev_Buf[BUF_CHK] = Tmp_Rev5;
                    U5_Rev_Hnd();   
                    Clr_Buf_Data( U5_Rev_Buf );
                }
                U5_i = 0;
                break;
            }
            default:
            {
                U5_i = 0;
                break;
            }
        }
    }
}

void USART6_IRQHandler(void)
{
    if( USART_GetFlagStatus( USART6, USART_FLAG_RXNE ) != RESET )
    {
        USART_ClearITPendingBit( USART6, USART_IT_RXNE );
        LED_BLU_TOG;
        // 3.串口中断应答函数
        u8 Tmp_Rev6 = USART_ReceiveData(USART6);
        switch( U6_i )
        {
            case 0:
            case 1:
            {
                if( 0xff == Tmp_Rev6 ) 
                    U6_Rev_Buf[U6_i++] = 0xff;
                else 
                    U6_i = 0; 
                break;
            }
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
            case 10:
            case 11:
            case 12:
            {
                U6_Rev_Buf[U6_i++] = Tmp_Rev6;
                break;
            }
            case 13:
            {
                if( Tmp_Rev6 == Cal_Check_Sum( U6_Rev_Buf ) ) 
                {
                    U6_Rev_Buf[BUF_CHK] = Tmp_Rev6;
                    U6_Rev_Hnd();  
                    Clr_Buf_Data( U6_Rev_Buf );
                }
                U6_i = 0;
                break;
            }
            default:
            {
                U6_i = 0;
                break;
            }
        }
    }
}

/* 串口DMA发送完成中断 */
/*
    if( DMA_GetITStatus( DMAx_Streamx, DMA_IT_TCIFx ) != RESET )  
    DMA_ClearITPendingBit( DMAx_Streamx, DMA_IT_TCIFx ); 
    DMA_Reset( USARTx );
*/
void DMA2_Stream7_IRQHandler(void)//USART1  
{     
    if( DMA_GetITStatus( DMA2_Stream7, DMA_IT_TCIF7 ) != RESET )  
    {   // 1.清除DMA发送完成中断标志  2.绿色LED变化
        DMA_ClearITPendingBit( DMA2_Stream7, DMA_IT_TCIF7 ); 
        LED_GRN_TOG;
        
        // 1.DMA发送完成次数自加记录  2.DMA发送重新设置 3.发送次数超出 默认为发送完毕处理
        U1_Snd_Cnt++;
        DMA_Reset( USART1 );
        if( U1_Snd_Cnt > RETURNTIMES ) 
        {
            // 发送完毕处理：1.停止发送 2.计数清零 3.清空发送队列 4.回送ID完毕 清除返回状态
            DISABLE_U1_DMA_SND; 
            Clr_Buf_Data( U1_Snd_Buf );
            U1_Snd_Cnt  = 0;
            U1_Snd_Fin  = TRUE;
            // 打印信息 可删**************************dddd**************************
            //printf("U1 DMA Fin.\n"); 
        }
        
    }     
}  

void DMA1_Stream6_IRQHandler(void)//USART2  
{      
    if( DMA_GetITStatus( DMA1_Stream6, DMA_IT_TCIF6 ) != RESET )  
    {
        DMA_ClearITPendingBit( DMA1_Stream6, DMA_IT_TCIF6 ); 
        LED_GRN_TOG;
        
        U2_Snd_Cnt++;
        DMA_Reset( USART2 );
        if( U2_Snd_Cnt > SENDTIMES ) 
        {
            // 单个口发送完毕处理：1.停止发送 2.计数清零 3.清空发送队列 4.发送状态在主函数确认
            DISABLE_U2_DMA_SND;
            U2_Snd_Cnt  = 0;
            U2_Snd_Fin  = TRUE;  
            // 打印信息 可删**************************dddd**************************
            //printf("U2 DMA Fin.\n"); 
        }
    }     
}  

void DMA1_Stream3_IRQHandler(void)//USART3  
{     
    if( DMA_GetITStatus( DMA1_Stream3, DMA_IT_TCIF3 ) != RESET )  
    {
        DMA_ClearITPendingBit( DMA1_Stream3, DMA_IT_TCIF3 ); 
        LED_GRN_TOG;
        
        U3_Snd_Cnt++;
        DMA_Reset( USART3 );
        if( U3_Snd_Cnt > SENDTIMES ) 
        {
            DISABLE_U3_DMA_SND;
            U3_Snd_Cnt  = 0;
            U3_Snd_Fin  = TRUE; 
            // 打印信息 可删**************************dddd**************************
            //printf("U3 DMA Fin.\n"); 
        }  
    }     
}  

void DMA1_Stream4_IRQHandler(void)//UART4  
{     
    if( DMA_GetITStatus( DMA1_Stream4, DMA_IT_TCIF4 ) != RESET )  
    {
        DMA_ClearITPendingBit( DMA1_Stream4, DMA_IT_TCIF4 ); 
        LED_GRN_TOG;
        
        U4_Snd_Cnt++;
        DMA_Reset( UART4 );
        if( U4_Snd_Cnt > SENDTIMES ) 
        {
            DISABLE_U4_DMA_SND;
            U4_Snd_Cnt  = 0;
            U4_Snd_Fin  = TRUE; 
            // 打印信息 可删**************************dddd**************************
            //printf("U4 DMA Fin.\n"); 
        }
    }     
}  

void DMA1_Stream7_IRQHandler(void)//UART5  
{     
    if( DMA_GetITStatus( DMA1_Stream7, DMA_IT_TCIF7 ) != RESET )  
    {
        DMA_ClearITPendingBit( DMA1_Stream7, DMA_IT_TCIF7 ); 
        LED_GRN_TOG;
        
        U5_Snd_Cnt++;
        DMA_Reset( UART5 );
        if( U5_Snd_Cnt > SENDTIMES ) 
        {
            DISABLE_U5_DMA_SND;
            U5_Snd_Cnt  = 0;
            U5_Snd_Fin  = TRUE; 
            // 打印信息 可删**************************dddd**************************
            //printf("U5 DMA Fin.\n"); 
        }
    }     
}  

void DMA2_Stream6_IRQHandler(void)//USART6  
{     
    if( DMA_GetITStatus( DMA2_Stream6, DMA_IT_TCIF6 ) != RESET )  
    {
        DMA_ClearITPendingBit( DMA2_Stream6, DMA_IT_TCIF6 ); 
        LED_GRN_TOG;
        
        U6_Snd_Cnt++;
        DMA_Reset( USART6 );
        if( U6_Snd_Cnt > SENDTIMES ) 
        {
            DISABLE_U6_DMA_SND;
            U6_Snd_Cnt  = 0;
            U6_Snd_Fin  = TRUE; 
            // 打印信息 可删**************************dddd**************************
            //printf("U6 DMA Fin.\n"); 
        } 
    }     
}  


/**
  * @brief   This function handles NMI exception.
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  */
void SysTick_Handler(void)
{
    TimingDelay_Decrement();
}



/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
