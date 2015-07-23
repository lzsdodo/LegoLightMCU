/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static volatile vs32 TimingDelay = 0; 

/* My Equipment Variables */
extern u8   Que_Exe[QUEUE_EXE_SIZE];    // ִ�ж���
extern u8   Que_Snd[QUEUE_SND_SIZE];    // ���Ͷ���
extern u8   Que_Rtn[QUEUE_RTN_SIZE];    // ���ض���
  
extern u8   Head_Exe, Head_Snd, Head_Rtn; // ִ��(���͡�����) ����ͷ       
extern u8   Rear_Exe, Rear_Snd, Rear_Rtn; // ִ��(���͡�����) ����β

extern u8   U1_Snd_Buf[BUF_SIZE];     // USART1~6 ���ͻ�����
extern u8   U2_Snd_Buf[BUF_SIZE];           
extern u8   U3_Snd_Buf[BUF_SIZE];
extern u8   U4_Snd_Buf[BUF_SIZE];
extern u8   U5_Snd_Buf[BUF_SIZE];
extern u8   U6_Snd_Buf[BUF_SIZE];

extern u8   U1_Rev_Buf[BUF_SIZE];     // USART1~6 ���ջ�����
extern u8   U2_Rev_Buf[BUF_SIZE];
extern u8   U3_Rev_Buf[BUF_SIZE];
extern u8   U4_Rev_Buf[BUF_SIZE];
extern u8   U5_Rev_Buf[BUF_SIZE];
extern u8   U6_Rev_Buf[BUF_SIZE];

extern u8   U1_Snd_Fin;               // USART1~6 DMA������ɱ�־λ
extern u8   U2_Snd_Fin;
extern u8   U3_Snd_Fin;
extern u8   U4_Snd_Fin;
extern u8   U5_Snd_Fin;
extern u8   U6_Snd_Fin;

u8  Exeing_Buf[BUF_SIZE];     // ִ�ж��г�ȡ������
u8  Snding_Buf[BUF_SIZE];     // ���Ͷ��г�ȡ������

u8  ExeCtn_Status = FALSE;    // ����ִ��״̬ Execute Continually     

const u16 MyPubID = 2;    // ����ID
u32 MyOwnID       = 0;    // �豸ID
u8  MyLayer       = 0;    // �豸����
u8  Red           = 50;                 
u8  Green         = 50; 
u8  Blue          = 50; 
u8  Luminance     = 50;
u8  Sav_Hue[4]    = {0};  // �ı���ɫ�󱣴����ɫֵ

u8  Test = 0;

/* Private function prototypes -----------------------------------------------*/
void SysTick_Init(void);
void Delayms( u16 nTime );

/*************************@brief  Main program************************/
int main(void)
{
    SysTick_Init();                 // System Tick Clock Init
    GPIO_Config();                  // GPIO Config
    //EXTILine_Button_Config();       // EXTI Button Config
    TIM_PWM_Config();               // PWM-TIM Config
    TIM_Timer_Config();             // Timer-TIM Config
    USART_All_Config();             // USART Config
    USART_All_Rev_IRQ_OFF();        // USART All Turn off the Receive IRQn Interrupt
    DMA_All_Config();               // DMA-USART-TX Config
    Delayms(10);                    // �ȴ�DMA�������
    USART_All_Rev_IRQ_ON();         // ������ѭ�����������н����ж�
    PWM_RGBOutput(0,0,0);           // 3·PWM��ʼ��
    printf("System Did Config\n");  // "��ʼ�����"
    
    LED_ALL_ON;                     // LED on the Board All ON
    Delayms(50);                   // Delay 500ms
    LED_ALL_OFF;                    // LED on the Board All OFF
    
    printf("Enter Main Loop\n");    // "������ѭ��"
    while (1)
    {   /* Main Loop */
        
        // һ�����ִ�ж��� �Ƿ�� ************************************************************************************
        if( FALSE == Is_Queue_Empty( Head_Exe, Rear_Exe ) )
        {   // 1.��ճ���ִ��״̬ 2.��ȡ�������� 3.ִ�к��� 4.�˳�ִ��״̬     
            ExeCtn_Status = FALSE;   
            Head_Exe = Push_Queue( Head_Exe, Que_Exe, Exeing_Buf );
            Executing_Fun( Exeing_Buf );
        }    
        else  // ִ�ж���Ϊ�� �Ƿ�Ϊ����״̬��ִ��ָ��
        {   // ��������
            if( TRUE == ExeCtn_Status )
                Executing_Continually_Fun( Exeing_Buf );
        }

        // ������������DMA����״̬ �Ƿ������ ��ɺ� �˳�����״̬ **********************************************************
        if( TRUE == (U2_Snd_Fin & U3_Snd_Fin & U4_Snd_Fin & U5_Snd_Fin & U6_Snd_Fin) )
        {  
            // ��� ���Ͷ��� �Ƿ�Ϊ��
            if( FALSE == Is_Queue_Empty( Head_Snd, Rear_Snd ) )
            {
                // 1.��ȡ�������ݵ��������� 2.��鹦��λ ���ݹ���λȷ�Ϸ���λ��
                Head_Snd = Push_Queue( Head_Snd, Que_Snd, Snding_Buf );
                switch( Snding_Buf[BUF_FUN] )
                {
                    // 1.ΪASK ����Port���޸����ݰ����� ��������DMA���ͻ����� 
                    case FUN_ASK_ID:
                    {
                        Ask_U2to6();              // �����ݴ��͵����д��ڷ���BUF��
                        Reset_U2to6_Finish();     // ����DMA����״̬
                        Enable_U2to6_DMA_Snd();   // ����DMA����
                        break;
                    }
                    // 2.ΪReturn �����޸� ����U2~6��DMA���ͻ����� ������
                    case FUN_RTN_ID:
                    {   // 1.���ݷ��͵�U2~6��Buf�� 2.����DMA����״̬ 3.����DMA����
                        Send_toU2to6_Port();        
                        Reset_U2to6_Finish();    
                        Enable_U2to6_DMA_Snd();   
                        break;
                    }
                    // 3.��ΪASK&Return �ж�:����ID/�ض�ID ѡ�������� �����Ϳ�DMA���ͻ����� 
                    default:
                    {
                        switch( Whose_ID( Snding_Buf ) )
                        {   // �ж�ID������ 1.����ID 2.�ض��豸ID
                            case PUB_ID:
                            {   // 1.���ݷ��͵�U2~6��Buf�� 2.����DMA����״̬ 3.����DMA����
                                Send_toU2to6_Port();        
                                Reset_U2to6_Finish();     
                                Enable_U2to6_DMA_Snd();   
                                break;
                            }
                            default:
                            { 
                                // 1.������Ӧ����DMA����( ���ô������ĸ����� )
                                Send_toPort( Send_toWhich_Port( Snding_Buf, MyLayer ) );
                                break;
                            }
                        }
                        break;
                    }
                } 
            }
        }
        
        // ��:����Ƿ����״̬  ****************************************************************************
        if( TRUE == U1_Snd_Fin )
        {
            // ������ID���� ���� ���д���
            if( FALSE == Is_Queue_Empty( Head_Rtn, Rear_Rtn ) )
            {
                // 1.���뷵��״̬ 2.��ȡ�������� ���� USART1���ͻ����� 3.����DMA���ͣ�ֱ���������
                U1_Snd_Fin = FALSE;
                Head_Rtn = Push_Queue( Head_Rtn, Que_Rtn, U1_Snd_Buf );
                ENABLE_U1_DMA_SND;
            }
        }
        
        /* Running Flag of the Main Loop */ 
        //LED_ORG_TOG;      //  �ȵ���˸: �۲���ѭ����û�ܷ�
        //Delayms(100);     //  Delay 100ms
    }
}


// @brief  SysTick Init Function.
void SysTick_Init(void)
{
  /* 
    SystemCoreClock / 1000    1ms�ж�һ��   168000
    SystemCoreClock / 1000000 1us�ж�һ��   168
                              10ms          16800
  */
  //Captire error
  while(SysTick_Config(168000));  
  //�رյδ�ʱ��
  SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
  //�����δ�ʱ��
  //SysTick->CTRL |=   SysTick_CTRL_ENABLE_Msk;
}

/**
  * @brief  Delay Function.
  * @param  nCount:specifies the Delay time length.
  * @retval None
  */
void Delayms( u16 nTime )
{
    SysTick->CTRL |=   SysTick_CTRL_ENABLE_Msk;
    TimingDelay = nTime;
    while( TimingDelay != 0 ); 
    SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
}

void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0)
    { 
        TimingDelay--;
    }
}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
