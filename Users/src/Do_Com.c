#include "Do_Com.h"

// 设备信息 
extern u32  MyOwnID;    //设备ID
extern u8   MyLayer;    //设备层数 

// 执行（发送、返回）队列 
extern u8   Que_Exe[QUEUE_EXE_SIZE];
extern u8   Que_Snd[QUEUE_SND_SIZE];
extern u8   Que_Rtn[QUEUE_RTN_SIZE];

// 执行(发送、返回) 队列头 & 队列尾
extern u8   Head_Exe, Head_Snd, Head_Rtn;
extern u8   Rear_Exe, Rear_Snd, Rear_Rtn;

// USART 接收缓存区 
u8  U1_Rev_Buf[BUF_SIZE];
u8  U2_Rev_Buf[BUF_SIZE];
u8  U3_Rev_Buf[BUF_SIZE];
u8  U4_Rev_Buf[BUF_SIZE];
u8  U5_Rev_Buf[BUF_SIZE];
u8  U6_Rev_Buf[BUF_SIZE];

// USART-DMA发送缓存区 
extern u8 U1_Snd_Buf[BUF_SIZE];
extern u8 U2_Snd_Buf[BUF_SIZE];
extern u8 U3_Snd_Buf[BUF_SIZE];
extern u8 U4_Snd_Buf[BUF_SIZE];
extern u8 U5_Snd_Buf[BUF_SIZE];
extern u8 U6_Snd_Buf[BUF_SIZE];

// DMA发送次数记录 & 完成标志 
extern u8 U1_Snd_Cnt, U2_Snd_Cnt, U3_Snd_Cnt, U4_Snd_Cnt, U5_Snd_Cnt, U6_Snd_Cnt;
extern u8 U1_Snd_Fin, U2_Snd_Fin, U3_Snd_Fin, U4_Snd_Fin, U5_Snd_Fin, U6_Snd_Fin;

// 返回ID数据包 从计算 到 队列 的中间过渡缓存区
u8          Rtn_ID_Buf[BUF_SIZE];
extern u8   Snding_Buf[BUF_SIZE];

/*  ***********USART 正确接收数据包后的处理函数*********** */
void U1_Rev_Hnd(void)
{    // 确认缓冲区完全收到数据包后  1:判断功能类型 2:转移数据包 

    // 与发送缓存区比较，相同则清空发送缓冲区，并关闭DMA发送，等待下一个数据发送
    if( TRUE == Cmp_Buf_Data( U1_Rev_Buf, U1_Snd_Buf ) )
    {
        // 1.停止DMA发送 2.清空发送缓冲区 3.重新计数 4.发送完毕 清除发送状态
        DISABLE_U1_DMA_SND;
        U1_Snd_Cnt  = 0;
        U1_Snd_Fin  = TRUE;
        Clr_Buf_Data( U1_Snd_Buf );
        // 打印信息 可删**************************dddd**************************
        //printf("U1 Suc Rtn ID.\n");
    }
    else// 与发送缓存区不同 则为上级向下级发送指令 成功接收命令
    {
        // 1.关闭USART接收中断 2.返回接收的数据 3.重新开启USART接收中断
        DISABLE_U1_REV_IRQ;
        Rtn_Rev_Buf( USART1, U1_Rev_Buf ); 
        ENABLE_U1_REV_IRQ;
        // 打印信息 可删**************************dddd**************************
        //printf("U1 Rev Data.\n");
        // 检查 数据的功能位 根据功能类型移动数据包位置
        switch( U1_Rev_Buf[BUF_FUN] )
        {
            // 一.寻找设备
            case FUN_ASK_ID:
            {
                // 1.获得ID,层数 2.建立ID数据包 3.将ASK命令插入到 发送队列 
                Get_My_ID_and_Layer( U1_Rev_Buf );
                Creat_Rtn_Data( Rtn_ID_Buf, MyOwnID, MyLayer );
                Rear_Snd = Insert_Queue( Head_Snd, Rear_Snd, Que_Snd, U1_Rev_Buf );
                // 打印信息 可删**************************dddd**************************
                //printf("My ID:%d & Lyr:%d.\n", MyOwnID, MyLayer);
                break;
            }
            // 二.返回设备ID
            case FUN_RTN_ID:
            {
                //  1.将ID数据包插入到 返回队列 2.将Return命令插入到 发送队列  
                Rear_Rtn = Insert_Queue( Head_Rtn, Rear_Rtn, Que_Rtn, Rtn_ID_Buf ); 
                Rear_Snd = Insert_Queue( Head_Snd, Rear_Snd, Que_Snd, U1_Rev_Buf );
                // 打印信息 可删**************************dddd**************************
                //printf("Rtn ID & Snd2 All.\n");
                break;
            }
            // 层数相关功能
            case FUN_CHG_HUE_LYR:
            case FUN_FLK_ORG_LYR:
            case FUN_FLK_HUE_LYR:    
            {
                if( U1_Rev_Buf[BUF_LYR] == MyLayer )
                {
                    Rear_Exe = Insert_Queue( Head_Exe, Rear_Exe, Que_Exe, U1_Rev_Buf );
                    //printf("Lyr:Cpy2 Exe-Que.\n");
                }
                else
                {
                    Rear_Snd = Insert_Queue( Head_Snd, Rear_Snd, Que_Snd, U1_Rev_Buf );
                    // 打印信息 可删**************************dddd**************************
                    //printf("Lyr:Cpy2 Snd-Que.\n");    
                }
                break;
            }
            // 三.普通执行功能
            default: 
            {
                // 1.检查ID所属 2.转移数据包 插入 执行队列 / 发送队列
                switch( Whose_ID( U1_Rev_Buf ) )
                {
                    // 1.其他设备ID 插入 发送队列 
                    case OTH_ID:
                    { 
                        Rear_Snd = Insert_Queue( Head_Snd, Rear_Snd, Que_Snd, U1_Rev_Buf );
                        // 打印信息 可删**************************dddd**************************
                        //printf("OTH:Cpy2 Snd-Que.\n");
                        break;
                    }
                    // 2.自身设备ID 插入 执行队列
                    case OWN_ID: 
                    {
                        Rear_Exe = Insert_Queue( Head_Exe, Rear_Exe, Que_Exe, U1_Rev_Buf );
                        // 打印信息 可删**************************dddd**************************
                        //printf("OWN:Cpy2 Exe-Que.\n");
                        break;
                    }
                    // 3.公共设备ID 插入 执行队列 & 发送队列 
                    case PUB_ID:
                    {
                        Rear_Exe = Insert_Queue( Head_Exe, Rear_Exe, Que_Exe, U1_Rev_Buf );
                        Rear_Snd = Insert_Queue( Head_Snd, Rear_Snd, Que_Snd, U1_Rev_Buf );
                        // 打印信息 可删**************************dddd**************************
                        //printf("PUB:Cpy2 Snd&Exe-Que.\n");
                        break;
                    }
                    default:
                        break;
                }
                break;
            }
        }
    }
}

void U2_Rev_Hnd( void )
{   // 确认缓冲区完全收到数据包后  1:判断功能类型 2:转移数据包 
    // 与发送缓存区比较，相同则清空发送缓冲区，并关闭DMA发送，等待下一个数据发送
    if( TRUE == Cmp_Buf_Data( U2_Rev_Buf, U2_Snd_Buf ) )
    {   
        // 1.停止DMA发送 2.重新计数 3.清空发送缓冲区 4.发送完毕 清除发送状态
        DISABLE_U2_DMA_SND;
        U2_Snd_Cnt  = 0;
        U2_Snd_Fin = TRUE; 
        Clr_Buf_Data( U2_Snd_Buf );
        // 打印信息 可删**************************dddd**************************
        //printf("U2 Suc Snd.\n");
    }
    else//与发送缓存区不同，则是接收下级返回的ID值，转移缓冲区数据到队列，关闭串口接收中断
    {
        if( FUN_RTN_ID == U2_Rev_Buf[BUF_FUN] )
        {
            // 1.关闭USART接收中断 2.数据包插入 回送队列 3.返回接收的数据 4.重开USART接收中断
            DISABLE_U2_REV_IRQ;
            Rear_Rtn = Insert_Queue( Head_Rtn, Rear_Rtn, Que_Rtn, U2_Rev_Buf );
            Rtn_Rev_Buf( USART2, U2_Rev_Buf );    
            ENABLE_U2_REV_IRQ;
            // 打印信息 可删**************************dddd**************************
            //printf("U2 Got ID & Cpy2 Rtn-Que.\n");
        }     
    }
}

void U3_Rev_Hnd( void )
{   //确认缓冲区完全收到数据包后  1:判断功能类型 2:转移数据包 
    //与发送缓存区比较，相同则清空发送缓冲区，并关闭DMA发送，等待下一个数据发送
    if( TRUE == Cmp_Buf_Data( U3_Rev_Buf, U3_Snd_Buf ) )
    {
        DISABLE_U3_DMA_SND;
        U3_Snd_Cnt  = 0;
        U3_Snd_Fin  = TRUE;
        Clr_Buf_Data( U3_Snd_Buf );
        // 打印信息 可删**************************dddd**************************
        //printf("U3 Suc Snd.\n");
    }
    else//与发送缓存区不同，则是接收下级返回的ID值，转移缓冲区数据到队列，关闭串口接收中断
    {
        if( FUN_RTN_ID == U3_Rev_Buf[BUF_FUN] )
        {
            DISABLE_U3_REV_IRQ;
            Rear_Rtn = Insert_Queue( Head_Rtn, Rear_Rtn, Que_Rtn, U3_Rev_Buf );
            Rtn_Rev_Buf( USART3, U3_Rev_Buf );    
            ENABLE_U3_REV_IRQ;
            // 打印信息 可删**************************dddd**************************
            //printf("U3 Got ID & Cpy2 Rtn-Que.\n");
        }
    }
}

void U4_Rev_Hnd( void )
{   //确认缓冲区完全收到数据包后  1:判断功能类型 2:转移数据包 
    //与发送缓存区比较，相同则清空发送缓冲区，并关闭DMA发送，等待下一个数据发送
    if( TRUE == Cmp_Buf_Data( U4_Rev_Buf, U4_Snd_Buf ) )
    {
        DISABLE_U4_DMA_SND;
        U4_Snd_Cnt  = 0;
        U4_Snd_Fin  = TRUE; 
        Clr_Buf_Data( U4_Snd_Buf );
        // 打印信息 可删**************************dddd**************************
        //printf("U4 Suc Snd.\n");
    }
    else//与发送缓存区不同，则是接收下级返回的ID值，转移缓冲区数据到队列，关闭串口接收中断
    {
        if( FUN_RTN_ID == U4_Rev_Buf[BUF_FUN] )
        {
            DISABLE_U4_REV_IRQ;
            Rear_Rtn = Insert_Queue( Head_Rtn, Rear_Rtn, Que_Rtn, U4_Rev_Buf );
            Rtn_Rev_Buf( UART4, U4_Rev_Buf );    
            ENABLE_U4_REV_IRQ;
            // 打印信息 可删**************************dddd**************************
            //printf("U4 Got ID & Cpy2 Rtn-Que.\n");
        }
    }
}

void U5_Rev_Hnd( void )
{   //确认缓冲区完全收到数据包后  1:判断功能类型 2:转移数据包 
    //与发送缓存区比较，相同则清空发送缓冲区，并关闭DMA发送，等待下一个数据发送
    if( TRUE == Cmp_Buf_Data( U5_Rev_Buf, U5_Snd_Buf ) )
    {
        DISABLE_U5_DMA_SND;
        U5_Snd_Cnt  = 0;
        U5_Snd_Fin  = TRUE;
        Clr_Buf_Data( U5_Snd_Buf );
        // 打印信息 可删**************************dddd**************************
        //printf("U5 Suc Snd.\n");
    }
    else//与发送缓存区不同，则是接收下级返回的ID值，转移缓冲区数据到队列，关闭串口接收中断
    {
        if( FUN_RTN_ID == U5_Rev_Buf[BUF_FUN] )
        {
            DISABLE_U5_REV_IRQ;
            Rear_Rtn = Insert_Queue( Head_Rtn, Rear_Rtn, Que_Rtn, U5_Rev_Buf );
            Rtn_Rev_Buf( UART5, U5_Rev_Buf );    
            ENABLE_U5_REV_IRQ;
            // 打印信息 可删**************************dddd**************************
            //printf("U5 Got ID & Cpy2 Rtn-Que.\n");
        }
    }
}

void U6_Rev_Hnd( void )
{   //确认缓冲区完全收到数据包后  1:判断功能类型 2:转移数据包 
    //与发送缓存区比较，相同则清空发送缓冲区，并关闭DMA发送，等待下一个数据发送
    if( TRUE == Cmp_Buf_Data( U6_Rev_Buf, U6_Snd_Buf ) )
    {
        DISABLE_U6_DMA_SND;
        U6_Snd_Cnt  = 0;
        U6_Snd_Fin  = TRUE; 
        Clr_Buf_Data( U6_Snd_Buf );
        // 打印信息 可删**************************dddd**************************
        //printf("U6 Suc Snd.\n");
    }
    else//与发送缓存区不同，则是接收下级返回的ID值，转移缓冲区数据到队列，关闭串口接收中断
    {
        if( FUN_RTN_ID == U6_Rev_Buf[BUF_FUN] )
        {
            DISABLE_U6_REV_IRQ;
            Rear_Rtn = Insert_Queue( Head_Rtn, Rear_Rtn, Que_Rtn, U6_Rev_Buf );
            Rtn_Rev_Buf( USART6, U6_Rev_Buf );    
            ENABLE_U6_REV_IRQ;
            // 打印信息 可删**************************dddd**************************
            //printf("U6 Got ID & Cpy2 Rtn-Que.\n");
        }
    }
}

/* 计算校验和 已测试 */
u8 Cal_Check_Sum( u8* p )
{
    u8 Sum = 0;
    for( u8 i = BUF_FUN; i < BUF_CHK; i++ )
        Sum += p[i];
    Sum = ~Sum;
    return Sum;
}

/* 计算本设备ID 与 Layer 已测试 */
void Get_My_ID_and_Layer( u8* p )
{
    // 没有前ID 认为是第一个设备 ID=0x0001；否则根据前ID计算
    if( NO_ID == p[BUF_ID3] )
    {
        MyOwnID = FIRST_ID;
        MyLayer = FIRST_LYR;    
    }
    else
    {
        MyLayer = p[BUF_LYR];
        MyOwnID = (u32)(p[BUF_ID0]<<24) + (p[BUF_ID1]<<16) + (p[BUF_ID2]<<8) + p[BUF_ID3];
        
    }
}

/* 比较两个缓冲数组是否相同 相同：返回1；不同：返回0； */
u8  Cmp_Buf_Data( u8* p1, u8* p2 )
{
    for( u8 i=0; i<BUF_SIZE; i++ )
    {
        if( *p1++ != *p2++ )
            return FALSE;
    }
    return TRUE;
}
/* 复制一个缓冲区数据到另一个缓冲区 */
void  Cpy_Buf_Data( u8* p1, u8* p2 )
{
    for( u8 i=0; i<BUF_SIZE; i++ )
        *p2++ = *p1++;
}
/* 清空缓冲区数据 */
void Clr_Buf_Data( u8* p )
{
    for( u8 i=0; i<BUF_SIZE; i++ )
        *p++ = 0x00;
}
/* 
  计算ID所属设备 
  OTH_ID(0):其他设备，往下发送；
  OWN_ID(1):自身ID，执行队列；
  PUB_ID(2):公共ID，执行队列，发送队列 
*/
u8  Whose_ID( u8* Buf )
{
    u32 TmpID;
    if( PUBLIC_ID == Buf[BUF_ID3] )
        return PUB_ID;
    else
    {
        TmpID = (u32)(Buf[BUF_ID0]<<24) + (Buf[BUF_ID1]<<16) + (Buf[BUF_ID2]<<8) + Buf[BUF_ID3];
        if( MYOWN_ID == TmpID )
            return OWN_ID;
        else
            return OTH_ID;
    }
}

/* 构造功能函数 */
void Creat_Fun_Data( u8* Buf1, u8* Buf2, u8 Fun, u32 ID, u8 Layer )
{
    Buf2[BUF_FUN] = Fun;   
    Buf2[BUF_ID0] = (u8)( ID>>24 );
    Buf2[BUF_ID1] = (u8)( ID>>16 );
    Buf2[BUF_ID2] = (u8)( ID>>8 );
    Buf2[BUF_ID3] = (u8)( ID );
    Buf2[BUF_LYR] = Layer;
    Buf2[BUF_R]   = Buf1[BUF_R];
    Buf2[BUF_G]   = Buf1[BUF_G];
    Buf2[BUF_B]   = Buf1[BUF_B];
    
    if( Fun == FUN_UFD_BRT )
    {
        if( Buf1[BUF_L] < 50 )
            Buf2[BUF_L] = Buf1[BUF_L] + 10; 
    }
    else if(Fun == FUN_UFD_DRK)
    {
        if( Buf1[BUF_L] > 20 )
          Buf2[BUF_L] = Buf1[BUF_L] - 10; 
    }
    else
       Buf2[BUF_L]   = Buf1[BUF_L]; 
    
    Buf2[BUF_T]   = Buf1[BUF_T];
    
    u8 Sum = 0;
    for( u8 i = BUF_FUN; i < BUF_CHK; i++ )
        Sum += Buf2[i];
    Sum = ~Sum;
    Buf2[BUF_CHK] = Sum;
}

/* 构造返回ID的数据包 已测试 */
void Creat_Rtn_Data( u8* Buf, u32 MyID, u8 MyLayer )
{
    Buf[BUF_FUN] = FUN_RTN_ID;   
    Buf[BUF_ID0] = (u8)( MyID>>24 );
    Buf[BUF_ID1] = (u8)( MyID>>16 );
    Buf[BUF_ID2] = (u8)( MyID>>8 );
    Buf[BUF_ID3] = (u8)( MyID );
    Buf[BUF_LYR] = MyLayer;
    Buf[BUF_CHK] = ~( FUN_RTN_ID+Buf[BUF_ID0]+Buf[BUF_ID1]+Buf[BUF_ID2]+Buf[BUF_ID3]+Buf[BUF_LYR] );
}   
/* 执行向下发送ASK ID 命令时，运用该函数构建出发送函数到所有发送缓冲区中 */
void Creat_Ask_Data( u8* Buf, u32 MyID, u8 MyLayer, u8 Port )
{
    u32 Sum     = 1;
    u32 NextID  = 0;
    for( u8 i = 0; i<MyLayer; i++ )
        Sum *= 10;
    NextID = MyID + Port * Sum;
    Buf[0]       = 0xff;
    Buf[1]       = 0xff;  
    Buf[BUF_FUN] = FUN_ASK_ID;   
    Buf[BUF_ID0] = (u8)( NextID>>24 );
    Buf[BUF_ID1] = (u8)( NextID>>16 );
    Buf[BUF_ID2] = (u8)( NextID>>8 );
    Buf[BUF_ID3] = (u8)( NextID );
    Buf[BUF_LYR] = MyLayer + 1;
    Buf[BUF_R]   = 0;
    Buf[BUF_G]   = 0;
    Buf[BUF_B]   = 0;
    Buf[BUF_L]   = 0;
    Buf[BUF_T]   = 0;
    Buf[BUF_CHK] = ~( FUN_ASK_ID+Buf[BUF_ID0]+Buf[BUF_ID1]+Buf[BUF_ID2]+Buf[BUF_ID3]+Buf[BUF_LYR] );
    //printf("NextID:%d\n", NextID);
    //printf("ID:%d,%d,%d,%d,Lyr:%d\n", Buf[BUF_ID0], Buf[BUF_ID1], Buf[BUF_ID2], Buf[BUF_ID3], Buf[BUF_LYR]);
}

void  Reset_U2to6_Finish(void)
{
    U2_Snd_Fin = FALSE;
    U3_Snd_Fin = FALSE;
    U4_Snd_Fin = FALSE; 
    U5_Snd_Fin = FALSE;
    U6_Snd_Fin = FALSE;
}

void  Ask_U2to6(void)
{
    Creat_Ask_Data( U2_Snd_Buf, MyOwnID, MyLayer, PORT2 );
    Creat_Ask_Data( U3_Snd_Buf, MyOwnID, MyLayer, PORT3 );
    Creat_Ask_Data( U4_Snd_Buf, MyOwnID, MyLayer, PORT4 );
    Creat_Ask_Data( U5_Snd_Buf, MyOwnID, MyLayer, PORT5 );
    Creat_Ask_Data( U6_Snd_Buf, MyOwnID, MyLayer, PORT6 );
}

void  Send_toU2to6_Port(void)
{
    Cpy_Buf_Data( Snding_Buf, U2_Snd_Buf );
    Cpy_Buf_Data( Snding_Buf, U3_Snd_Buf );
    Cpy_Buf_Data( Snding_Buf, U4_Snd_Buf );
    Cpy_Buf_Data( Snding_Buf, U5_Snd_Buf );
    Cpy_Buf_Data( Snding_Buf, U6_Snd_Buf );
}

u8  Send_toWhich_Port( u8* Buf, u8 Layer )
{
    u8  i,Port;
    u32 TmpID;
    u16 Sum = 1;
    TmpID = (u32)(Buf[BUF_ID0]<<24) + (Buf[BUF_ID1]<<16) + (Buf[BUF_ID2]<<8) + Buf[BUF_ID3];
    for( i=0; i<Layer; i++ )
        Sum *= 10;
    Port = TmpID / Sum;
    return Port;
}

void  Send_toPort( u8 Port )
{
    switch(Port)
    {
        case PORT2:
        {
            Cpy_Buf_Data( Snding_Buf, U2_Snd_Buf );
            U2_Snd_Fin = FALSE;
            ENABLE_U2_DMA_SND;
            break;
        }
        case PORT3:
        {
            Cpy_Buf_Data( Snding_Buf, U3_Snd_Buf );
            U3_Snd_Fin = FALSE;
            ENABLE_U3_DMA_SND;
            break;
        }
        case PORT4:
        {
            Cpy_Buf_Data( Snding_Buf, U4_Snd_Buf );
            U4_Snd_Fin = FALSE;
            ENABLE_U4_DMA_SND;
            break;
        }
        case PORT5:
        {
            Cpy_Buf_Data( Snding_Buf, U5_Snd_Buf );
            U5_Snd_Fin = FALSE;
            ENABLE_U5_DMA_SND;
            break;
        }
        case PORT6:
        {
            Cpy_Buf_Data( Snding_Buf, U6_Snd_Buf );
            U6_Snd_Fin = FALSE;
            ENABLE_U6_DMA_SND;
            break;
        }
        default:break;
    }
}

/* 向UARTx,UART 返回成功收到的缓冲数组 */
void Rtn_Rev_Buf( USART_TypeDef* USARTx, u8* Buf )
{
    volatile u8  i = 0;
    //volatile u16 j = 0;
    while( i < BUF_SIZE )
    {
        USARTx->DR = ( Buf[i++] & 0x01FF );
        while( RESET == ( USARTx->SR & USART_FLAG_TC ) );
        //for( j=0; j<500; j++ );
    }
}

/* 向 UARTx,UART 發送字符串 
void Printf_String( USART_TypeDef* USARTx, char* string )
{
    while( *string )
    {
        //USART_SendData( Port, (unsigned short int) *string++ );
        //while ( USART_GetFlagStatus( Port, USART_FLAG_TC ) == RESET );
        USARTx->DR = ( ( (unsigned short int) *string++ ) & 0x01FF );
        while( RESET == ( USARTx->SR & USART_FLAG_TC ) );
    }
}
*/
