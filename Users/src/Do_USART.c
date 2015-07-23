#include "Do_USART.h"

/* Turn Off All the USART Receive IRQn */
void USART_All_Rev_IRQ_OFF(void)
{
  DISABLE_U1_REV_IRQ;
  DISABLE_U2_REV_IRQ;
  DISABLE_U3_REV_IRQ;
  DISABLE_U4_REV_IRQ;
  DISABLE_U5_REV_IRQ;
  DISABLE_U6_REV_IRQ;
}

/* Turn On All the USART Receive IRQn */
void USART_All_Rev_IRQ_ON(void)
{
  ENABLE_U1_REV_IRQ;
  ENABLE_U2_REV_IRQ;
  ENABLE_U3_REV_IRQ;
  ENABLE_U4_REV_IRQ;
  ENABLE_U5_REV_IRQ;
  ENABLE_U6_REV_IRQ;
}

// USART All Config 
void USART_All_Config(void)
{
    
    USART_Config(USART1);
    USART_Config(USART2);
    USART_Config(USART3);
    USART_Config(UART4);
    USART_Config(UART5);
    USART_Config(USART6);
}

// USARTx Config
void USART_Config( USART_TypeDef* USARTx )
{
        
    /* NVIC,GPIO,USART configuration */
    NVIC_InitTypeDef    NVIC_InitStructure;
    GPIO_InitTypeDef    GPIO_InitStructure;
    USART_InitTypeDef   USART_InitStructure;

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
        
    if( USARTx == USART1 )
    { 
        RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOB,  ENABLE );
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
        GPIO_Init( GPIOB, &GPIO_InitStructure );
        GPIO_PinAFConfig( GPIOB, GPIO_PinSource6,  GPIO_AF_USART1 );
        GPIO_PinAFConfig( GPIOB, GPIO_PinSource7,  GPIO_AF_USART1 );
        
        NVIC_InitStructure.NVIC_IRQChannel  = USART1_IRQn;
        RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1, ENABLE ); 
    }
    else if( USARTx == USART2 )
    {
        RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA, ENABLE );
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
        GPIO_Init( GPIOA, &GPIO_InitStructure );
        GPIO_PinAFConfig( GPIOA, GPIO_PinSource2,  GPIO_AF_USART2 );
        GPIO_PinAFConfig( GPIOA, GPIO_PinSource3,  GPIO_AF_USART2 );
        
        NVIC_InitStructure.NVIC_IRQChannel  = USART2_IRQn;
        RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2, ENABLE ); 
    }
    else if( USARTx == USART3 )
    {
        RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOB, ENABLE );
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
        GPIO_Init( GPIOB, &GPIO_InitStructure );
        GPIO_PinAFConfig( GPIOB, GPIO_PinSource10,  GPIO_AF_USART3 );
        GPIO_PinAFConfig( GPIOB, GPIO_PinSource11,  GPIO_AF_USART3 );
        
        NVIC_InitStructure.NVIC_IRQChannel  = USART3_IRQn;
        RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART3, ENABLE );
    }
    else if( USARTx == UART4 )
    {
        RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOC, ENABLE );
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
        GPIO_Init( GPIOC, &GPIO_InitStructure );
        GPIO_PinAFConfig( GPIOC, GPIO_PinSource10,  GPIO_AF_UART4 );
        GPIO_PinAFConfig( GPIOC, GPIO_PinSource11,  GPIO_AF_UART4 );
  
        NVIC_InitStructure.NVIC_IRQChannel  = UART4_IRQn;
        RCC_APB1PeriphClockCmd( RCC_APB1Periph_UART4, ENABLE );  
    }
    else if( USARTx == UART5 )
    {
        RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOC, ENABLE );
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
        GPIO_Init( GPIOC, &GPIO_InitStructure );
        GPIO_PinAFConfig( GPIOC, GPIO_PinSource12,  GPIO_AF_UART5 );
        RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOD, ENABLE );
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
        GPIO_Init( GPIOD, &GPIO_InitStructure );
        GPIO_PinAFConfig( GPIOD, GPIO_PinSource2,  GPIO_AF_UART5 );
  
        NVIC_InitStructure.NVIC_IRQChannel  = UART5_IRQn; 
        RCC_APB1PeriphClockCmd( RCC_APB1Periph_UART5, ENABLE );
    }  
    else if( USARTx == USART6 )
    {
        RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOC, ENABLE );
        GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_6 | GPIO_Pin_7;
        GPIO_Init( GPIOC, &GPIO_InitStructure );
        GPIO_PinAFConfig( GPIOC, GPIO_PinSource6,  GPIO_AF_USART6 );
        GPIO_PinAFConfig( GPIOC, GPIO_PinSource7,  GPIO_AF_USART6 );
        
        NVIC_InitStructure.NVIC_IRQChannel  = USART6_IRQn;
        RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART6, ENABLE );   
    }
  
    /* NVIC Config */
    NVIC_PriorityGroupConfig( NVIC_PriorityGroup_1 );
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority  = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority         = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                 = ENABLE;
    NVIC_Init( &NVIC_InitStructure );
  
    /* USART  configuration */
    USART_InitStructure.USART_BaudRate            = 115200;
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;
    USART_InitStructure.USART_Parity              = USART_Parity_No ;
    USART_InitStructure.USART_Mode                = USART_Mode_Tx | USART_Mode_Rx;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init( USARTx, &USART_InitStructure );
    USART_ITConfig( USARTx, USART_IT_RXNE, ENABLE );
    USART_Cmd( USARTx, ENABLE );
}