#ifndef __Do_Com_H
#define __Do_Com_H
#include "stm32f4xx.h"
#include "Do_USART.h"
#include "Do_DMA.h"
#include "Do_Queue.h"
#include "Do_Fun.h"
#include <stdio.h>

/* Private define ------------------------------------------------------------*/
/**************************************************************************/
/* Logic Judge */
#define TRUE        1
#define FALSE       0

/* Times of the DMA Sending for the same data */
#define SENDTIMES   2
#define RETURNTIMES 3

/* Buf Positions define */
#define BUF_FUN     2     // Function
#define BUF_ID0     3     // ID
#define BUF_ID1     4
#define BUF_ID2     5
#define BUF_ID3     6      
#define BUF_LYR     7     // Layer
#define BUF_R       8     // Red
#define BUF_G       9     // Green
#define BUF_B       10    // Blue
#define BUF_L       11    // Luminance
#define BUF_T       12    // Time or Times
#define BUF_CHK     13    // Check Sum   

/* My Equipment Information */
#define NO_ID       0               //0x0000
#define PUBLIC_ID   2               //0x0002
#define MYOWN_ID    MyOwnID
#define FIRST_ID    1               
#define FIRST_LYR   1

/* Whose ID */
#define OTH_ID      0
#define OWN_ID      1
#define PUB_ID      2

/* 当ASK ID 时需要用到 */
#define PORT2       2
#define PORT3       3
#define PORT4       4
#define PORT5       5
#define PORT6       6

/**************************************************************************/
/* Private functions ---------------------------------------------------------*/
void Creat_Fun_Data( u8* Buf1, u8* Buf2, u8 Fun, u32 ID, u8 Layer ); //根据功能建立数据包
void Get_My_ID_and_Layer( u8* p );                             //获得 ID与Layer
void Creat_Rtn_Data( u8* Buf, u32 MyID, u8 MyLayer );          //计算自身ID后返回ID信息 
void Creat_Ask_Data( u8* Buf, u32 MyID, u8 MyLayer, u8 Port ); //计算自身ID后继续向下发送寻找设备信息
u8   Cal_Check_Sum( u8* p );                                   //计算校验和

u8    Whose_ID( u8* Buf );                                    //计算ID属于哪个设备

//void Printf_String( USART_TypeDef* USARTx , char* string );     // 向特定串口打印字符串
void Rtn_Rev_Buf( USART_TypeDef* USARTx, u8* Buf );          // 向UARTx,UART 返回成功收到的缓冲数组

void Reset_U2to6_Finish(void);                               // 把所有发送标志清除 为FALSE
void Ask_U2to6(void);                                        // 发送各口对应数据放入U2~6串口Buf
void Send_toU2to6_Port(void);                                // 发送到U2~6串口Buf

u8   Send_toWhich_Port( u8* Buf, u8 MyLayer );               // 获得发向下一层的Port口
void Send_toPort( u8 Port );                                 // 计算该ID应发往哪一层

u8   Cmp_Buf_Data( u8* p1, u8* p2 );                         //比较缓冲区数据是否内容相同
void Cpy_Buf_Data( u8* p1, u8* p2 );                         //复制缓冲区数据 *p2 = *p1
void Clr_Buf_Data( u8* p );                                  //清空缓冲区数据

void U1_Rev_Hnd(void);                                       //USART1成功接收数据包后处理
void U2_Rev_Hnd(void);                                       //USART2~6成功接收数据包后的处理
void U3_Rev_Hnd(void); 
void U4_Rev_Hnd(void); 
void U5_Rev_Hnd(void); 
void U6_Rev_Hnd(void); 

#endif