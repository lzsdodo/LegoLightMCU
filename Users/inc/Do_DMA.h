#ifndef __Do_DMA_H
#define __Do_DMA_H
#include "stm32f4xx.h"
#include "stm32f4xx_dma.h"

/* -------------------------- Private define ----------------------------------*/
#define BUF_SIZE 14

/* 串口向DMA发出请求 */
#define ENABLE_U1_DMA_SND   USART_DMACmd( USART1, USART_DMAReq_Tx, ENABLE ); 
#define ENABLE_U2_DMA_SND   USART_DMACmd( USART2, USART_DMAReq_Tx, ENABLE ); 
#define ENABLE_U3_DMA_SND   USART_DMACmd( USART3, USART_DMAReq_Tx, ENABLE ); 
#define ENABLE_U4_DMA_SND   USART_DMACmd( UART4,  USART_DMAReq_Tx, ENABLE ); 
#define ENABLE_U5_DMA_SND   USART_DMACmd( UART5,  USART_DMAReq_Tx, ENABLE ); 
#define ENABLE_U6_DMA_SND   USART_DMACmd( USART6, USART_DMAReq_Tx, ENABLE ); 
#define DISABLE_U1_DMA_SND  USART_DMACmd( USART1, USART_DMAReq_Tx, DISABLE ); 
#define DISABLE_U2_DMA_SND  USART_DMACmd( USART2, USART_DMAReq_Tx, DISABLE ); 
#define DISABLE_U3_DMA_SND  USART_DMACmd( USART3, USART_DMAReq_Tx, DISABLE ); 
#define DISABLE_U4_DMA_SND  USART_DMACmd( UART4,  USART_DMAReq_Tx, DISABLE ); 
#define DISABLE_U5_DMA_SND  USART_DMACmd( UART5,  USART_DMAReq_Tx, DISABLE ); 
#define DISABLE_U6_DMA_SND  USART_DMACmd( USART6, USART_DMAReq_Tx, DISABLE ); 

/* --------------------------- Private functions ------------------------------*/
void DMA_Config( USART_TypeDef* USARTx );
void DMA_All_Config(void);
void DMA_Reset( USART_TypeDef* USARTx );
void Enable_U2to6_DMA_Snd(void);

#endif