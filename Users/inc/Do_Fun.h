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
#define	FUN_ASK_ID	        0       // 寻找设备，设置ID
#define FUN_RTN_ID	        1       // 返回所有设备ID值

#define FUN_CHG_HUE_LYR         3       // x层 颜色           PUB_ID 
#define FUN_CHG_HUE             4       // 改变颜色           PUB_ID

#define FUN_FLK_ORG_LYR         5       // x层 原颜色 闪烁    PUB_ID
#define FUN_FLK_ORG             6       // 原颜色 闪烁        PUB_ID

#define FUN_FLK_HUE_LYR         7       // x层 特定颜色 闪烁  PUB_ID
#define FUN_FLK_HUE             8       // 特定颜色 闪烁      PUB_ID

#define FUN_CHG_HUE_SPC_LYR     9       // 后一层 颜色
#define FUN_CHG_HUE_SPC_BHD     10      // 后所有 颜色
#define FUN_FLK_ORG_SPC_LYR     11      // 后一层 原颜色   闪烁
#define FUN_FLK_ORG_SPC_BHD     12      // 后所有 原颜色   闪烁
#define FUN_FLK_HUE_SPC_LYR     13      // 后一层 特定颜色 闪烁
#define FUN_FLK_HUE_SPC_BHD     14      // 后所有 特定颜色 闪烁

#define FUN_UFD_BRT             15      // 树状变亮n次 
#define FUN_UFD_DRK             16      // 树状变暗n次

// Functions
void  Executing_Fun( u8* Buf );               // 执行函数
void  Executing_Continually_Fun( u8* Buf );   // 持续执行函数

void  Flicking_Fun( u8 Luminance );         // 闪烁函数
void  Unfolding_Fun( u8* Buf, u8 Toward );  // 展开函数

void  Save_Hue( u8* p );    // 保存色彩值
void  Restore_Hue(void);    // 恢复色彩

void  Change_Hue(u8 R, u8 G, u8 B, u8 L );    // 改变色彩
void  Change_Lmn( u8 L );                     // 仅仅改变亮度
float Hue2Rgb(float p, float q, float t );    // 中间函数


#endif