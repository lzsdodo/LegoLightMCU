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

/* ��ASK ID ʱ��Ҫ�õ� */
#define PORT2       2
#define PORT3       3
#define PORT4       4
#define PORT5       5
#define PORT6       6

/**************************************************************************/
/* Private functions ---------------------------------------------------------*/
void Creat_Fun_Data( u8* Buf1, u8* Buf2, u8 Fun, u32 ID, u8 Layer ); //���ݹ��ܽ������ݰ�
void Get_My_ID_and_Layer( u8* p );                             //��� ID��Layer
void Creat_Rtn_Data( u8* Buf, u32 MyID, u8 MyLayer );          //��������ID�󷵻�ID��Ϣ 
void Creat_Ask_Data( u8* Buf, u32 MyID, u8 MyLayer, u8 Port ); //��������ID��������·���Ѱ���豸��Ϣ
u8   Cal_Check_Sum( u8* p );                                   //����У���

u8    Whose_ID( u8* Buf );                                    //����ID�����ĸ��豸

//void Printf_String( USART_TypeDef* USARTx , char* string );     // ���ض����ڴ�ӡ�ַ���
void Rtn_Rev_Buf( USART_TypeDef* USARTx, u8* Buf );          // ��UARTx,UART ���سɹ��յ��Ļ�������

void Reset_U2to6_Finish(void);                               // �����з��ͱ�־��� ΪFALSE
void Ask_U2to6(void);                                        // ���͸��ڶ�Ӧ���ݷ���U2~6����Buf
void Send_toU2to6_Port(void);                                // ���͵�U2~6����Buf

u8   Send_toWhich_Port( u8* Buf, u8 MyLayer );               // ��÷�����һ���Port��
void Send_toPort( u8 Port );                                 // �����IDӦ������һ��

u8   Cmp_Buf_Data( u8* p1, u8* p2 );                         //�Ƚϻ����������Ƿ�������ͬ
void Cpy_Buf_Data( u8* p1, u8* p2 );                         //���ƻ��������� *p2 = *p1
void Clr_Buf_Data( u8* p );                                  //��ջ���������

void U1_Rev_Hnd(void);                                       //USART1�ɹ��������ݰ�����
void U2_Rev_Hnd(void);                                       //USART2~6�ɹ��������ݰ���Ĵ���
void U3_Rev_Hnd(void); 
void U4_Rev_Hnd(void); 
void U5_Rev_Hnd(void); 
void U6_Rev_Hnd(void); 

#endif