#ifndef __Do_USART_H
#define __Do_USART_H
#include "stm32f4xx.h"
#include "stm32f4xx_usart.h"
#include "Do_DMA.h"

/* Private define ------------------------------------------------------------*/
/* ������ر� USART1���ڽ����ж� */
#define ENABLE_U1_REV_IRQ   USART_ITConfig( USART1, USART_IT_RXNE, ENABLE );
#define ENABLE_U2_REV_IRQ   USART_ITConfig( USART2, USART_IT_RXNE, ENABLE );
#define ENABLE_U3_REV_IRQ   USART_ITConfig( USART3, USART_IT_RXNE, ENABLE );
#define ENABLE_U4_REV_IRQ   USART_ITConfig( UART4,  USART_IT_RXNE, ENABLE );
#define ENABLE_U5_REV_IRQ   USART_ITConfig( UART5,  USART_IT_RXNE, ENABLE );
#define ENABLE_U6_REV_IRQ   USART_ITConfig( USART6, USART_IT_RXNE, ENABLE );

#define DISABLE_U1_REV_IRQ  USART_ITConfig( USART1, USART_IT_RXNE, DISABLE );
#define DISABLE_U2_REV_IRQ  USART_ITConfig( USART2, USART_IT_RXNE, DISABLE );
#define DISABLE_U3_REV_IRQ  USART_ITConfig( USART3, USART_IT_RXNE, DISABLE );
#define DISABLE_U4_REV_IRQ  USART_ITConfig( UART4,  USART_IT_RXNE, DISABLE );
#define DISABLE_U5_REV_IRQ  USART_ITConfig( UART5,  USART_IT_RXNE, DISABLE );
#define DISABLE_U6_REV_IRQ  USART_ITConfig( USART6, USART_IT_RXNE, DISABLE );

/* ------------------------- Private functions --------------------------------*/
/* �����ڳ�ʼ�� */
void USART_All_Config(void);
void USART_Config( USART_TypeDef* USARTx );

/* �ر��뿪������USART�����ж� */
void USART_All_Rev_IRQ_OFF(void);
void USART_All_Rev_IRQ_ON(void);

#endif