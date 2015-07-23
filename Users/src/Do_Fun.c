#include "Do_Fun.h"

extern u32  MyOwnID;
extern u8   MyLayer;
extern u8   Sav_Hue[4];

extern u8   Que_Snd[QUEUE_SND_SIZE];
extern u8   Head_Snd;
extern u8   Rear_Snd;

extern u8   ExeCtn_Status;
extern u8   Timer_Status;

static u8   Exe_Tmp_Buf[BUF_SIZE];
static u8   Delay_Cnt = 0;

#define FLK_DARK1   0
#define FLK_DELAY   1
#define FLK_SHINE   2
#define FLK_DARK2   3
#define FLK_RST1    4
#define FLK_RST2    5

static  u8  Flk_Status = FLK_DARK1;
static  u8  Flk_R      = 0;
static  u8  Flk_G      = 0;
static  u8  Flk_B      = 0;
static  u8  Flk_L      = 0;
static  u8  Flk_T      = 0;
static  u8  Chg_Fun    = 0;

#define UFD_BRIGHT  0
#define UFD_DARK    1

#define UFD_DARK1   0
#define UFD_DELAY   1
#define UFD_SHINE   2
#define UFD_DARK2   3
#define UFD_RST     4

static  u8  Ufd_Status = UFD_DARK1;
static  u8  Ufd_Sav_L  = 0;
static  u8  Ufd_L      = 0;
static  u8  Ufd_T      = 0;

void Executing_Fun( u8* Buf )
{
    //printf("%d\n.......", Buf[BUF_FUN]);
    switch( Buf[BUF_FUN] )
    {
        case FUN_CHG_HUE_LYR:   // 改变某层颜色 公共ID
        {   // 1.检查是否同一层  
            //printf("FUN_CHG_HUE_LYR...\n");
        }
        case FUN_CHG_HUE:       // 改变颜色 (特定ID/公共ID)
        {   // 1.改变色彩 2.保存颜色 
            Change_Hue( Buf[BUF_R], Buf[BUF_G], Buf[BUF_B], Buf[BUF_L] );
            Save_Hue( Buf );
            break;
        }
        case FUN_FLK_ORG_LYR:   // 使得某层闪烁 公共ID
        {   // 1.不为同一层 退出  
            //printf("FUN_FLK_ORG_LYR...\n");
        }
        case FUN_FLK_ORG:       // 闪烁n次
        {   
            Flk_R         = Sav_Hue[0];
            Flk_G         = Sav_Hue[1];
            Flk_B         = Sav_Hue[2];
            Flk_L         = Sav_Hue[3];
            Flk_T         = Buf[BUF_T];
            Flk_Status    = FLK_DARK1;
            ExeCtn_Status = TRUE;
            break;
        }
        case FUN_FLK_HUE_LYR:   // 使得某层特定颜色闪烁 公共ID
        {   // 1.不为同一层 退出  
            //printf("FUN_FLK_HUE_LYR...\n");
        }
        case FUN_FLK_HUE:       // 特定颜色闪烁
        {   
            Flk_R         = Buf[BUF_R];
            Flk_G         = Buf[BUF_G];
            Flk_B         = Buf[BUF_B];
            Flk_L         = Buf[BUF_L];
            Flk_T         = Buf[BUF_T];
            Flk_Status    = FLK_DARK1;
            ExeCtn_Status = TRUE;
            break;
        }
        case FUN_UFD_BRT:       // 树状扩散n次 变亮
        {
            Ufd_Sav_L     = Buf[BUF_L];
            Ufd_L         = Buf[BUF_L];
            Ufd_T         = Buf[BUF_T];
            Ufd_Status    = UFD_DARK1;
            ExeCtn_Status = TRUE; 
            break;
        }
        case FUN_UFD_DRK:       // 树状扩散n次 变暗
        {   
            Ufd_Sav_L     = Buf[BUF_L];
            Ufd_L         = Buf[BUF_L];
            Ufd_T         = Buf[BUF_T];
            Ufd_Status    = UFD_DARK1;
            ExeCtn_Status = TRUE; 
            break;
        }
        case FUN_CHG_HUE_SPC_LYR:       // 后一层的颜色 特定ID
        {   Chg_Fun = FUN_CHG_HUE_LYR;  break;  }
        case FUN_CHG_HUE_SPC_BHD:       // 后所有的颜色 特定ID
        {   Chg_Fun = FUN_CHG_HUE;      break;  }
        case FUN_FLK_ORG_SPC_LYR:       // 后一层 原颜色 闪烁
        {   Chg_Fun = FUN_FLK_ORG_LYR;  break;  }
        case FUN_FLK_ORG_SPC_BHD:       // 后所有 原颜色 闪烁
        {   Chg_Fun = FUN_FLK_ORG;      break;  }
        case FUN_FLK_HUE_SPC_LYR:       // 后一层 特定颜色 闪烁
        {   Chg_Fun = FUN_FLK_HUE_LYR;  break;  }
        case FUN_FLK_HUE_SPC_BHD:       // 后所有 特定颜色 闪烁
        {   Chg_Fun = FUN_FLK_HUE;      break;  }
        default:break;
    }   
    // 1.构造新的数据包 2.存放到发送队列
    if( Chg_Fun != 0 )
    {
        Creat_Fun_Data( Buf, Exe_Tmp_Buf, Chg_Fun, PUBLIC_ID, (MyLayer+1) );
        Rear_Snd = Insert_Queue( Head_Snd, Rear_Snd, Que_Snd, Exe_Tmp_Buf );
        Chg_Fun = 0;
    }
}

void  Executing_Continually_Fun( u8* Buf )
{
    switch( Buf[BUF_FUN] )
    {
        case FUN_FLK_ORG:       // 原颜色   闪烁
        case FUN_FLK_HUE:       // 特定颜色 闪烁
        case FUN_FLK_ORG_LYR:   // x层 原颜色   闪烁
        case FUN_FLK_HUE_LYR:   // x层 特定颜色 闪烁
        {   // 闪烁函数
            Flicking_Fun( Buf[BUF_L] );
            break;
        }
        case FUN_UFD_BRT:
        {   // 展开函数 亮
            Unfolding_Fun( Buf, UFD_BRIGHT );
            break;
        }
        case FUN_UFD_DRK:
        {   // 展开函数 暗
            Unfolding_Fun( Buf, UFD_DARK );
            break;
        }
        default:break;
    }   
}

void Unfolding_Fun( u8* Buf, u8 Toward )
{
    if( TRUE == Timer_Status )
    {
        switch( Ufd_Status )
        { 
            case FLK_DARK1:
            {   
                if(  Delay_Cnt++ > 5 )
                {
                    Delay_Cnt = 0;
                    if( Ufd_L == 0 )
                        Ufd_Status  = UFD_DELAY; 
                    else
                        Change_Hue( Buf[BUF_R], Buf[BUF_G], Buf[BUF_B], Ufd_L-- );
                    break;
                }
                break;
            }
            case UFD_DELAY:
            {   // 1.延迟
                if( Delay_Cnt++ > 200 )
                {
                    Delay_Cnt= 0;
                    Ufd_Status  = UFD_SHINE;
                    if( Ufd_T == Buf[BUF_T] )
                    {
                        if( Toward == UFD_BRIGHT )
                            Creat_Fun_Data( Buf, Exe_Tmp_Buf, FUN_UFD_BRT, PUBLIC_ID, 0 );
                        else
                            Creat_Fun_Data( Buf, Exe_Tmp_Buf, FUN_UFD_DRK, PUBLIC_ID, 0 );
                        Rear_Snd = Insert_Queue( Head_Snd, Rear_Snd, Que_Snd, Exe_Tmp_Buf );
                    }
                }
                break;
            }
            case UFD_SHINE:
            {   // 灯亮 1.亮度增加 2.最亮后切换至恢复状态
                if(  Delay_Cnt++ > 5 )
                {
                    Delay_Cnt = 0;
                    if( Ufd_L == Ufd_Sav_L )
                    {
                        Ufd_T--;
                        //printf("%d\n", Ufd_T);
                        if( Ufd_T == 0 )
                            Ufd_Status  = UFD_DARK2;
                        else
                            Ufd_Status  = UFD_DARK1;
                    }
                    else
                        Change_Hue( Buf[BUF_R], Buf[BUF_G], Buf[BUF_B], Ufd_L++ );
                    break;
                }
                break;
            }
            case UFD_DARK2:
            {
                if( Ufd_L == 0 )
                    Ufd_Status  = UFD_RST; 
                else
                    Change_Hue( Buf[BUF_R], Buf[BUF_G], Buf[BUF_B], Ufd_L-- ); 
                break;
            }
            case UFD_RST:
            {
                if( Ufd_L == Sav_Hue[3] )
                {
                    Restore_Hue();
                    Ufd_Status    = UFD_DARK1;
                    ExeCtn_Status = FALSE;
                    break;
                }    
                else    
                    Change_Hue( Sav_Hue[0], Sav_Hue[1], Sav_Hue[2], Ufd_L++ );
                break;
            }
            
        }
        // 清除定时器10ms中断并开启定时器中断
        Timer_Status = FALSE;
        ENABLE_TIMER;
    }
}

void Flicking_Fun( u8 Luminance )
{
    if( TRUE == Timer_Status )
    {   
        switch( Flk_Status )
        {   // 1.开始变暗 2.灯灭1s 3.变最亮 4.恢复原来颜色
            case FLK_DARK1:
            {   //变暗 1.随时间不断减亮度 2.灯灭 3.切换状态
                if( Flk_L== 0 )
                {
                    Delay_Cnt   = 0;
                    Flk_Status  = FLK_DELAY; 
                }
                else
                    Change_Hue( Flk_R, Flk_G, Flk_B, Flk_L-- );
                break;
            }
            case FLK_DELAY:
            {   // 灯灭1s 1.10ms计数 2.1s后切换至变量状态
                if( Delay_Cnt++ > 128 )
                    Flk_Status  = FLK_SHINE;
                break;
            }
            case FLK_SHINE:
            {   // 灯亮 1.亮度增加 2.最亮后切换至恢复状态
                if( Flk_L == 100 )
                    Flk_Status  = FLK_DARK2;
                else
                    Change_Hue( Flk_R, Flk_G, Flk_B, Flk_L++ );
                break;
            }
            case FLK_DARK2:
            {   // 恢复颜色 1.减少亮度 2.恢复至原来亮度 3.切换至开始状态 4.检测闪烁次数
                if( Flk_L == Luminance )
                {
                    Flk_Status = FLK_DARK1;
                    if( Flk_T-- == 0 )
                        Flk_Status  = FLK_RST1;
                }
                else
                    Change_Hue( Flk_R, Flk_G, Flk_B, Flk_L-- );
                break;
            }
            case FLK_RST1:
            {   // 1恢复颜色 1.减少亮度
                if( Flk_L == 0 )
                    Flk_Status = FLK_RST2;
                else
                    Change_Hue( Flk_R, Flk_G, Flk_B, Flk_L-- );
                break;
            }
            case FLK_RST2:
            {   // 2恢复颜色 1.改变颜色 2.增加亮度
                if( Flk_L == Sav_Hue[3] )
                {
                    Restore_Hue();
                    Flk_Status    = FLK_DARK1;
                    ExeCtn_Status = FALSE;
                }
                else
                    Change_Hue( Sav_Hue[0], Sav_Hue[1], Sav_Hue[2], Flk_L++ );
                break;
            }
            default:break;
        }
        // 清除定时器10ms中断并开启定时器中断
        Timer_Status = FALSE;
        ENABLE_TIMER;
    }
}

// R,G,B(0-1) --> L(0-199)
void Change_Hue(u8 R, u8 G, u8 B, u8 L )
{
    float h,s,l,p,q;
 
    // found the max and min
    u8 RGB[3];
    if( R > G )
    {
        if( B > R )
        {
            RGB[0] = B;
            RGB[1] = R;
            RGB[2] = G;
        }  
        else 
        {
            if( B > G )
            {
                RGB[0] = R;
                RGB[1] = B;
                RGB[2] = G;
            }
            else
            {
                RGB[0] = R;
                RGB[1] = G;
                RGB[2] = B;
            }
        }
    }
    else
    {
        if( B > G )
        {
            RGB[0] = B;
            RGB[1] = G;
            RGB[2] = R;
        }  
        else 
        {
            if( B > R )
            {
                RGB[0] = G;
                RGB[1] = B;
                RGB[2] = R;
            }
            else
            {
                RGB[0] = G;
                RGB[1] = R;
                RGB[2] = B;
            }
        }
    }
        
    if( RGB[0] == RGB[2] )
    {
        h = 0.0;
    }
    
    float r = R   / 255.0;
    float g = G / 255.0;
    float b = B  / 255.0;
    float max = RGB[0] / 255.0;
    float min = RGB[2] / 255.0;
    
    // R,G,B-->H
    if( max == r && g >= b)
    {
        h = 60.0 * ( g - b ) / ( max - min );
    }
    else if( max == r && g < b)
    {
        h = 60.0 * ( g - b ) / ( max - min ) + 360.0; 
    }
    else if( max == g)
    {
        h = 60.0 * ( b - r ) / ( max - min ) + 120.0; 
    }
    else if( max == b)
    {
        h = 60.0 * ( r - g ) / ( max - min ) + 240.0; 
    }
        
    // modificated the value of Lightness
    if( L >= 200 )
    {
        l = 0.995;
    }
    else
    {
        l = 0.005 * L;
    }
    
    // R,G,B-->S
    if( l == 0 || max == min)
    {
        s = 0;
    }
    else if( l > 0 && l <= 0.5)
    {
        s = ( max - min ) / ( max + min );
    }
    else 
    {
        s = ( max - min ) / ( 2.0 - 2.0 * l );
    }
        
    //H,S,L-->R,G,B
    if( l < 0.5 ) 
    {
        q = ( 1.0 + s ) * l;
    }
    else
    {
        q = l + s - l * s;
    }
    p = 2 * l - q;
    
    r = 255 * Hue2Rgb( p, q, h/360.0+0.33 );
    g = 255 * Hue2Rgb( p, q, h/360.0 );
    b = 255 * Hue2Rgb( p, q, h/360.0-0.33 );
    
    // 控制输出范围 0~255
    if( r > 255 )     r = 255;
    else if( r < 0 )  r = 0;
    if( g > 255 )     g = 255; 
    else if( g < 0 )  g = 0;
    if( b > 255 )     b = 255;
    else if( b < 0 )  b = 0;
    
    PWM_RGBOutput( (u8)r, (u8)g, (u8)b );
    // 打印信息 可删**************************dddd**************************
    //printf("RGBL:%d,%d,%d,%d\n", (u8)r, (u8)g, (u8)b, L);
}

void  Change_Lmn( u8 L )
{
   Change_Hue( Sav_Hue[0], Sav_Hue[1], Sav_Hue[2], L ); 
}

float Hue2Rgb(float p, float q, float t )
{
    if( t < 0 ) t += 1;
    else if ( t > 1 ) t -= 1;
    
    if(t < 0.17 ) return p+((q-p)*6.0*t);
    else if (t < 0.5 && t >= 0.17) return q;
    else if (t < 0.67 && t >= 0.5) return p+((q-p)*6.0*(0.67-t));
    else return p;
}

// 保存颜色
void Save_Hue( u8* p )
{
    Sav_Hue[0] = p[BUF_R];
    Sav_Hue[1] = p[BUF_G];
    Sav_Hue[2] = p[BUF_B];
    Sav_Hue[3] = p[BUF_L];
}

// 恢复颜色
void Restore_Hue(void)
{
    Change_Hue( Sav_Hue[0], Sav_Hue[1], Sav_Hue[2], Sav_Hue[3] );
}
