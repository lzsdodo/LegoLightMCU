#include "Do_Queue.h"

u8  Que_Exe[QUEUE_LENGTH][QUEUE_EXE_WIDTH];
u8  Que_Snd[QUEUE_LENGTH][QUEUE_SND_WIDTH];
u8  Que_Rtn[QUEUE_LENGTH][QUEUE_RTN_WIDTH];

u8  Head_Exe  = 0;
u8  Rear_Exe  = 0;
u8  Head_Snd  = 0;
u8  Rear_Snd  = 0;
u8  Head_Rtn  = 0;
u8  Rear_Rtn  = 0;

// 检查队列是否为空队列
u8 Is_Queue_Empty( u8 Head, u8 Rear )
{
    if( Head == Rear )
        return TRUE;
    else
        return FALSE;
}
// 检查队列是否为满队列
u8 Is_Queue_Full( u8 Head, u8 Rear )
{
    // (rear + 1) % MaxSize == front。
    if( ( Rear + 1 ) % QUEUE_LENGTH == Head )
        return TRUE;
    else
        return FALSE;
}
/*  Max 100 Min 0 查看队列元素个数
u8 Queue_Element_Num( u8 Head, u8 Rear, u8 Width, u16 Size )
{
    //( rear - front + MaxSize ) % MaxSize
    return ( ( QUEUE_LENGTH - Head + Rear ) % QUEUE_LENGTH );
}
*/

// 插入队列
u8 Insert_Queue( u8 Head, u8 Rear, u8* Queue, u8* Buf )
{
    if( FALSE == Is_Queue_Full( Head, Rear ) )
    {
        u16 i;
        if( Queue == &Que_Exe[0][0] )
        {
            i = Rear * QUEUE_EXE_WIDTH;
            Queue[i++] = Buf[BUF_FUN];
            Queue[i++] = Buf[BUF_R];
            Queue[i++] = Buf[BUF_G];
            Queue[i++] = Buf[BUF_B];
            Queue[i++] = Buf[BUF_L];
            Queue[i]   = Buf[BUF_T];
        }
        else if( Queue == &Que_Snd[0][0] )
        {
            i = Rear * QUEUE_SND_WIDTH;
            Queue[i++] = Buf[BUF_FUN];
            Queue[i++] = Buf[BUF_ID0];
            Queue[i++] = Buf[BUF_ID1];
            Queue[i++] = Buf[BUF_ID2];
            Queue[i++] = Buf[BUF_ID3];
            Queue[i++] = Buf[BUF_LYR];
            Queue[i++] = Buf[BUF_R];
            Queue[i++] = Buf[BUF_G];
            Queue[i++] = Buf[BUF_B];
            Queue[i++] = Buf[BUF_L];
            Queue[i++] = Buf[BUF_T];
            Queue[i]   = Buf[BUF_CHK];
        }
        else if( Queue == &Que_Rtn[0][0] )
        {
            i = Rear * QUEUE_RTN_WIDTH;
            Queue[i++] = Buf[BUF_ID0];
            Queue[i++] = Buf[BUF_ID1];
            Queue[i++] = Buf[BUF_ID2];
            Queue[i++] = Buf[BUF_ID3];
            Queue[i++] = Buf[BUF_LYR];
            Queue[i]   = Buf[BUF_CHK];
        }
        Rear = ( Rear + 1 ) % QUEUE_LENGTH;
    }
    return Rear;
}

// 出队列
u8 Push_Queue( u8 Head, u8* Queue, u8* Buf )
{
    u16 i;
    Buf[0] = 0xff;
    Buf[1] = 0xff;
    if( Queue == &Que_Exe[0][0] )
    {
        i = Head * QUEUE_EXE_WIDTH;
        Buf[BUF_FUN]  = Queue[i++];
        Buf[BUF_R]    = Queue[i++];
        Buf[BUF_G]    = Queue[i++];
        Buf[BUF_B]    = Queue[i++];
        Buf[BUF_L]    = Queue[i++];
        Buf[BUF_T]    = Queue[i];
    }
    else if( Queue == &Que_Snd[0][0] )
    {
        i = Head * QUEUE_SND_WIDTH;
        Buf[BUF_FUN]  = Queue[i++];
        Buf[BUF_ID0]  = Queue[i++];
        Buf[BUF_ID1]  = Queue[i++];
        Buf[BUF_ID2]  = Queue[i++];
        Buf[BUF_ID3]  = Queue[i++];
        Buf[BUF_LYR]  = Queue[i++];
        Buf[BUF_R]    = Queue[i++];
        Buf[BUF_G]    = Queue[i++];
        Buf[BUF_B]    = Queue[i++];
        Buf[BUF_L]    = Queue[i++];
        Buf[BUF_T]    = Queue[i++];
        Buf[BUF_CHK]  = Queue[i];
    }
    else if( Queue == &Que_Rtn[0][0] )
    {
        i = Head * QUEUE_RTN_WIDTH;
        Buf[BUF_FUN]  = FUN_RTN_ID;
        Buf[BUF_ID0]  = Queue[i++];
        Buf[BUF_ID1]  = Queue[i++];
        Buf[BUF_ID2]  = Queue[i++];
        Buf[BUF_ID3]  = Queue[i++];
        Buf[BUF_LYR]  = Queue[i++];
        Buf[BUF_CHK]  = Queue[i];
    }
    Head = ( Head + 1 ) % QUEUE_LENGTH;
    return Head;
}