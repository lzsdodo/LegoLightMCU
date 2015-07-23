#ifndef __Do_Queue_H
#define __Do_Queue_H
#include "stm32f4xx.h"
#include "Do_Com.h"

/* Private define ------------------------------------------------------------*/
/* Queue defination */
#define QUEUE_LENGTH      101
#define QUEUE_EXE_WIDTH	  6     //Fun-1 RGBL-4 TIM-1 = 6; 
#define QUEUE_SND_WIDTH   12    //Fun-1 ID-4 Layer-1 RGBL-4 TIM-1 CHK-1 = 12; 
#define QUEUE_RTN_WIDTH	  6     //ID-4 Layer-1 CHK-1 = 6; 

#define QUEUE_EXE_SIZE    606
#define QUEUE_SND_SIZE    1212
#define QUEUE_RTN_SIZE    606

/* Private functions ---------------------------------------------------------*/
/* 队列操作函数 是否空，是否满，队列元素个数，插入队列，推出队列 */
u8  Is_Queue_Empty( u8 Head, u8 Rear );   
u8  Is_Queue_Full( u8 Head, u8 Rear );
//u8  Queue_Element_Num( u8 Head, u8 Rear, u8 Width, u16 Size );
u8  Insert_Queue( u8 Head, u8 Rear, u8* Queue, u8* Buf );
u8  Push_Queue( u8 Head, u8* Queue, u8* Buf );

#endif
