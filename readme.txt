2014.09.11
1:实现32位ID
2:成功返回ID
3:开始优化功能函数 

2014.09.12
1.功能函数： （未测试） 
            某层变色；
            某特定设备后一层变色； 
            某特定设备后所有变色；
2.功能函数： （未测试）
            闪烁n次-编写中
            树状n次-编写中

      else if( TRUE == Exeing_Flk_Status )
      {
          ENABLE_TIMER;
          Time_Stuate++;
          if( Flicker_Times_Old != Flicker_Times )
          {
              Flicker_Times_Old = Flicker_Times;
              if( Flicker_Times < 10 )
              {
                Exeing_Buf[BUF_L] -= 5;//直到 150
                printf("Flicker_Times=%d\t Time_Stuate=%d\n",Flicker_Times,Time_Stuate);
                Change_Lmn( Exeing_Buf[BUF_R], Exeing_Buf[BUF_G], Exeing_Buf[BUF_B], Exeing_Buf[BUF_L] );
              }
              else if( Flicker_Times <= 15 && Flicker_Times > 10 )
              {
                if( Exeing_Buf[BUF_L] >= 30 )
                  Exeing_Buf[BUF_L] -=30  ;//直到 0
                else
                  Exeing_Buf[BUF_L] = 0;
                printf("Flicker_Times=%d\t Time_Stuate=%d\n",Flicker_Times,Time_Stuate);
                Change_Lmn( Exeing_Buf[BUF_R], Exeing_Buf[BUF_G], Exeing_Buf[BUF_B], Exeing_Buf[BUF_L] );
                
              }
              else if( Flicker_Times > 25 && Flicker_Times <= 26 )
              {
                Exeing_Buf[BUF_L] +=0  ;// 保持熄灭 1s
                printf("Flicker_Times=%d\t Time_Stuate=%d\n",Flicker_Times,Time_Stuate);
                Change_Lmn( Exeing_Buf[BUF_R], Exeing_Buf[BUF_G], Exeing_Buf[BUF_B], Exeing_Buf[BUF_L] );
              }
              else if( Flicker_Times > 26 && Flicker_Times <= 31 )
              {
                Exeing_Buf[BUF_L] +=30  ;//直到 150
                printf("Flicker_Times=%d\t Time_Stuate=%d\n",Flicker_Times,Time_Stuate);
                Change_Lmn( Exeing_Buf[BUF_R], Exeing_Buf[BUF_G], Exeing_Buf[BUF_B], Exeing_Buf[BUF_L] );
              }
              else if( Flicker_Times > 31 && Flicker_Times <= 41 )
              {
                Exeing_Buf[BUF_L] +=5  ;//直到 200
                printf("Flicker_Times=%d\t Time_Stuate=%d\n",Flicker_Times,Time_Stuate);
                Change_Lmn( Exeing_Buf[BUF_R], Exeing_Buf[BUF_G], Exeing_Buf[BUF_B], Exeing_Buf[BUF_L] );
              }
              else if( Flicker_Times > 41  )
              {
                Flicker_Times = 0;
                Exeing_Buf[BUF_L] =200  ;
                printf("Flicker_Times=%d\t Time_Stuate=%d\n",Flicker_Times,Time_Stuate);
                Change_Lmn( Exeing_Buf[BUF_R], Exeing_Buf[BUF_G], Exeing_Buf[BUF_B], Exeing_Buf[BUF_L] );
                TIM_Cmd(TIM2, DISABLE);
                Exeing_Flk_Status = FALSE;
                Exeing_Status = FALSE;
              }
          }
      }