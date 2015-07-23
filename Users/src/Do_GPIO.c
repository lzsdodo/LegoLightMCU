#include "Do_GPIO.h"

//  GPIO Config
void GPIO_Config(void)
{
  /* GPIO TypeDef InitStructure */
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  /* GPIO clock enable */
  RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA, ENABLE );
  RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOB, ENABLE );
  RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOD, ENABLE );
  
  /* GPIOD Configuration: LED on the discovery board Config PD 12 13 14 15 */
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init( GPIOD, &GPIO_InitStructure );
  GPIOD->BSRRH  |=  0xF000;     //All LED Turn Off
  
  /* GPIO Configuration: Button on the discovery board Config PA00 */
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init( GPIOA, &GPIO_InitStructure );
  
  /* GPIO Configuration: Button1 & Button2 Config PE 04 & 05 */
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init( GPIOE, &GPIO_InitStructure );
  
  /* GPIO Configuration: PWM Config of PB 04(R),05(G),00(B) */
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_4 | GPIO_Pin_5 ;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL ;
  GPIO_Init( GPIOB, &GPIO_InitStructure ); 
  GPIO_PinAFConfig( GPIOB, GPIO_PinSource0, GPIO_AF_TIM3 );
  GPIO_PinAFConfig( GPIOB, GPIO_PinSource4, GPIO_AF_TIM3 ); 
  GPIO_PinAFConfig( GPIOB, GPIO_PinSource5, GPIO_AF_TIM3 );
}

  /* 
    GPIO�ṹ�嶨��
    GPIOD Periph clock enable ʹ��ʱ��
    ѡ��Ҫ���Ƶ�GPIOx����
    ��ѡ�ܽŵ�ģʽ�����롢��������ա�ģ�� 
    ������������ 
    output type �������ŵĿ�©: ���췽ʽ��� ���� ���缫��·��ʽ���
    Pull-up/Pull down ����ʱ��(��)��״̬:���� ( GPIO_PuPd_UP / GPIO_PuPd_DOWN / GPIO_PuPd_NOPULL )
    ���ÿ⺯��,��ʼ��GPIOx 
  */
  /*
  //Example of LED on the discovery board
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  */