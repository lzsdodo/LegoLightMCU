#ifndef __Do_Fun_H
#define __Do_Fun_H

/* Private include ------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "Do_TIM.h"
#include "Do_Com.h"
#include "Do_Queue.h"
#include <stdio.h>

/* Private define ------------------------------------------------------------*/
/* Function defination */
#define	FUN_ASK_ID	        0       // Ѱ���豸������ID
#define FUN_RTN_ID	        1       // ���������豸IDֵ

#define FUN_CHG_HUE_LYR         3       // x�� ��ɫ           PUB_ID 
#define FUN_CHG_HUE             4       // �ı���ɫ           PUB_ID

#define FUN_FLK_ORG_LYR         5       // x�� ԭ��ɫ ��˸    PUB_ID
#define FUN_FLK_ORG             6       // ԭ��ɫ ��˸        PUB_ID

#define FUN_FLK_HUE_LYR         7       // x�� �ض���ɫ ��˸  PUB_ID
#define FUN_FLK_HUE             8       // �ض���ɫ ��˸      PUB_ID

#define FUN_CHG_HUE_SPC_LYR     9       // ��һ�� ��ɫ
#define FUN_CHG_HUE_SPC_BHD     10      // ������ ��ɫ
#define FUN_FLK_ORG_SPC_LYR     11      // ��һ�� ԭ��ɫ   ��˸
#define FUN_FLK_ORG_SPC_BHD     12      // ������ ԭ��ɫ   ��˸
#define FUN_FLK_HUE_SPC_LYR     13      // ��һ�� �ض���ɫ ��˸
#define FUN_FLK_HUE_SPC_BHD     14      // ������ �ض���ɫ ��˸

#define FUN_UFD_BRT             15      // ��״����n�� 
#define FUN_UFD_DRK             16      // ��״�䰵n��

// Functions
void  Executing_Fun( u8* Buf );               // ִ�к���
void  Executing_Continually_Fun( u8* Buf );   // ����ִ�к���

void  Flicking_Fun( u8 Luminance );         // ��˸����
void  Unfolding_Fun( u8* Buf, u8 Toward );  // չ������

void  Save_Hue( u8* p );    // ����ɫ��ֵ
void  Restore_Hue(void);    // �ָ�ɫ��

void  Change_Hue(u8 R, u8 G, u8 B, u8 L );    // �ı�ɫ��
void  Change_Lmn( u8 L );                     // �����ı�����
float Hue2Rgb(float p, float q, float t );    // �м亯��


#endif