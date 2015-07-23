#include "Do_DMA.h"

u8 U1_Snd_Buf[BUF_SIZE];
u8 U2_Snd_Buf[BUF_SIZE];
u8 U3_Snd_Buf[BUF_SIZE];
u8 U4_Snd_Buf[BUF_SIZE];
u8 U5_Snd_Buf[BUF_SIZE];
u8 U6_Snd_Buf[BUF_SIZE];

// All USART DMA Config
void DMA_All_Config(void)
{
    DMA_Config(USART1);
    DMA_Config(USART2);
    DMA_Config(USART3);
    DMA_Config(UART4);
    DMA_Config(UART5);
    DMA_Config(USART6);
}

// Enable USART(2~6) to Send Data by DMA 
void Enable_U2to6_DMA_Snd(void)
{
    ENABLE_U2_DMA_SND;
    ENABLE_U3_DMA_SND;
    ENABLE_U4_DMA_SND;
    ENABLE_U5_DMA_SND;
    ENABLE_U6_DMA_SND;
}
// DMA Reset
void DMA_Reset( USART_TypeDef* USARTx )
{ 
    DMA_Stream_TypeDef* DMAy_Streamx;
    uint32_t  Buf_Address;
    
    if( USARTx == USART1 )
    { 
        DMAy_Streamx  = DMA2_Stream7;
        Buf_Address   = (uint32_t)U1_Snd_Buf;  
    }
    else if( USARTx == USART2 )
    {
        DMAy_Streamx  = DMA1_Stream6;
        Buf_Address   = (uint32_t)U2_Snd_Buf;
    }
    else if( USARTx == USART3 )
    {
        DMAy_Streamx  = DMA1_Stream3;
        Buf_Address   = (uint32_t)U3_Snd_Buf;
    }
    else if( USARTx == UART4 )
    {
        DMAy_Streamx  = DMA1_Stream4;
        Buf_Address   = (uint32_t)U4_Snd_Buf;
    }
    else if( USARTx == UART5 )
    {
        DMAy_Streamx  = DMA1_Stream7;
        Buf_Address   = (uint32_t)U5_Snd_Buf;
    }  
    else if( USARTx == USART6 )
    {
        DMAy_Streamx  = DMA2_Stream6;
        Buf_Address   = (uint32_t)U6_Snd_Buf;
    }
    DMA_Cmd( DMAy_Streamx, DISABLE );
    DMAy_Streamx->M0AR  = Buf_Address;
    DMAy_Streamx->NDTR  = BUF_SIZE;
    DMA_Cmd( DMAy_Streamx, ENABLE );
}

// DMA Config for USARTx
void DMA_Config( USART_TypeDef* USARTx )
{
    NVIC_InitTypeDef  NVIC_InitStructure;
    DMA_InitTypeDef   DMA_InitStructure; 
    
    DMA_Stream_TypeDef* DMAy_Streamx;
    
    uint8_t   IRQ_Channel;
    uint32_t  RCC_AHB1Periph;
    uint32_t  DMA_Channel;
    uint32_t  Buf_Address;
    
    if( USARTx == USART1 )
    { 
        DMAy_Streamx    = DMA2_Stream7;
        IRQ_Channel     = DMA2_Stream7_IRQn;
        RCC_AHB1Periph  = RCC_AHB1Periph_DMA2;
        DMA_Channel     = DMA_Channel_4;
        Buf_Address     = (uint32_t)U1_Snd_Buf;
    }
    else if( USARTx == USART2 )
    {
        DMAy_Streamx    = DMA1_Stream6;
        IRQ_Channel     = DMA1_Stream6_IRQn;
        RCC_AHB1Periph  = RCC_AHB1Periph_DMA1;
        DMA_Channel     = DMA_Channel_4;
        Buf_Address     = (uint32_t)U2_Snd_Buf;
    }
    else if( USARTx == USART3 )
    {
        DMAy_Streamx    = DMA1_Stream3;
        IRQ_Channel     = DMA1_Stream3_IRQn;
        RCC_AHB1Periph  = RCC_AHB1Periph_DMA1;
        DMA_Channel     = DMA_Channel_4;
        Buf_Address     = (uint32_t)U3_Snd_Buf;
    }
    else if( USARTx == UART4 )
    {
        DMAy_Streamx    = DMA1_Stream4;
        IRQ_Channel     = DMA1_Stream4_IRQn;
        RCC_AHB1Periph  = RCC_AHB1Periph_DMA1;
        DMA_Channel     = DMA_Channel_4;
        Buf_Address     = (uint32_t)U4_Snd_Buf;
    }
    else if( USARTx == UART5 )
    {
        DMAy_Streamx    = DMA1_Stream7;
        IRQ_Channel     = DMA1_Stream7_IRQn;
        RCC_AHB1Periph  = RCC_AHB1Periph_DMA1;
        DMA_Channel     = DMA_Channel_4;
        Buf_Address     = (uint32_t)U5_Snd_Buf;
    }  
    else if( USARTx == USART6 )
    {
        DMAy_Streamx    = DMA2_Stream6;
        IRQ_Channel     = DMA2_Stream6_IRQn;
        RCC_AHB1Periph  = RCC_AHB1Periph_DMA2;
        DMA_Channel     = DMA_Channel_5;
        Buf_Address     = (uint32_t)U6_Snd_Buf;
    }
    
    // NVIC Config
    NVIC_PriorityGroupConfig( NVIC_PriorityGroup_2 );  
    NVIC_InitStructure.NVIC_IRQChannel                    = IRQ_Channel;  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority  = 2;  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority         = 2;  
    NVIC_InitStructure.NVIC_IRQChannelCmd                 = ENABLE;  
    NVIC_Init(&NVIC_InitStructure);  
    // Enable DMA Clock
    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph, ENABLE ); 
    // DMA Config
    DMA_DeInit( DMAy_Streamx );
    DMA_InitStructure.DMA_Channel             = DMA_Channel;
    DMA_InitStructure.DMA_PeripheralBaseAddr  = (uint32_t)&USARTx->DR; 
    DMA_InitStructure.DMA_Memory0BaseAddr     = (uint32_t)Buf_Address;
    DMA_InitStructure.DMA_DIR                 = DMA_DIR_MemoryToPeripheral;
    DMA_InitStructure.DMA_BufferSize          = BUF_SIZE;
    DMA_InitStructure.DMA_PeripheralInc       = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc           = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize  = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize      = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode                = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority            = DMA_Priority_Medium;
    DMA_InitStructure.DMA_FIFOMode            = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold       = DMA_FIFOThreshold_HalfFull;
    DMA_InitStructure.DMA_MemoryBurst         = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst     = DMA_PeripheralBurst_Single;
    DMA_Init( DMAy_Streamx, &DMA_InitStructure ); 
    DMA_ITConfig( DMAy_Streamx, DMA_IT_TC, ENABLE );
    /* Enable DMA */
    DMA_Cmd( DMAy_Streamx, ENABLE );  
    USART_DMACmd( USARTx, USART_DMAReq_Tx, ENABLE );
}

  /*  
      重置寄存器
      设置DMA的传输通道：DMA_Channel_4
      设置DMA源：内存地址&串口数据寄存器地址
      内存地址(要传输的变量的指针)
      方向：从内存到外设
      传输大小DMA_BufferSize = SEND_BUF_SIZE;
      外设地址不增
      内存地址自增
      外设数据单位
      内存数据单位 8bit
      DMA模式：一次传输，循环
      优先级：中
      指定如果FIFO模式或直接模式将用于指定的流 ： 不使能FIFO模式
      指定了FIFO阈值水平
      指定的Burst转移配置内存传输
      指定的Burst转移配置外围转移
      配置DMA的通道
      开启DMA发送完成中断
  */