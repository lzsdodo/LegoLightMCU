2014.09.11
1:ʵ��32λID
2:�ɹ�����ID
3:��ʼ�Ż����ܺ��� 

2014.09.12
1.���ܺ����� ��δ���ԣ� 
            ĳ���ɫ��
            ĳ�ض��豸��һ���ɫ�� 
            ĳ�ض��豸�����б�ɫ��
2.���ܺ����� ��δ���ԣ�
            ��˸n��-��д��
            ��״n��-��д��

      else if( TRUE == Exeing_Flk_Status )
      {
          ENABLE_TIMER;
          Time_Stuate++;
          if( Flicker_Times_Old != Flicker_Times )
          {
              Flicker_Times_Old = Flicker_Times;
              if( Flicker_Times < 10 )
              {
                Exeing_Buf[BUF_L] -= 5;//ֱ�� 150
                printf("Flicker_Times=%d\t Time_Stuate=%d\n",Flicker_Times,Time_Stuate);
                Change_Lmn( Exeing_Buf[BUF_R], Exeing_Buf[BUF_G], Exeing_Buf[BUF_B], Exeing_Buf[BUF_L] );
              }
              else if( Flicker_Times <= 15 && Flicker_Times > 10 )
              {
                if( Exeing_Buf[BUF_L] >= 30 )
                  Exeing_Buf[BUF_L] -=30  ;//ֱ�� 0
                else
                  Exeing_Buf[BUF_L] = 0;
                printf("Flicker_Times=%d\t Time_Stuate=%d\n",Flicker_Times,Time_Stuate);
                Change_Lmn( Exeing_Buf[BUF_R], Exeing_Buf[BUF_G], Exeing_Buf[BUF_B], Exeing_Buf[BUF_L] );
                
              }
              else if( Flicker_Times > 25 && Flicker_Times <= 26 )
              {
                Exeing_Buf[BUF_L] +=0  ;// ����Ϩ�� 1s
                printf("Flicker_Times=%d\t Time_Stuate=%d\n",Flicker_Times,Time_Stuate);
                Change_Lmn( Exeing_Buf[BUF_R], Exeing_Buf[BUF_G], Exeing_Buf[BUF_B], Exeing_Buf[BUF_L] );
              }
              else if( Flicker_Times > 26 && Flicker_Times <= 31 )
              {
                Exeing_Buf[BUF_L] +=30  ;//ֱ�� 150
                printf("Flicker_Times=%d\t Time_Stuate=%d\n",Flicker_Times,Time_Stuate);
                Change_Lmn( Exeing_Buf[BUF_R], Exeing_Buf[BUF_G], Exeing_Buf[BUF_B], Exeing_Buf[BUF_L] );
              }
              else if( Flicker_Times > 31 && Flicker_Times <= 41 )
              {
                Exeing_Buf[BUF_L] +=5  ;//ֱ�� 200
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