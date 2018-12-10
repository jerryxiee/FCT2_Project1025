/*******************(C)�������������¼����������޹�˾��Ȩ����****
 
**  �ļ�����:   Bsp.c
**  ��������:   ŷ����
**  ��ǰ�汾:   1.0.0.0
**  ��������:   20130622
**  ����˵��:   ��ʼ��Ӳ��
**  �޸ļ�¼:   
**  ��    ע:   
**********************************(C)copyright 2013**************************************/
#include "Bsp.h"

__IO u16 ADC3ConvertedValue[ADC3_CHANNEL_NUM] = {0};	//���ͨ����ADCֵ
/******************************���ڶ����**********************/
USART_TypeDef* COM_USART[COMNUM] = {USART1,USART2,USART3,UART4,UART5,USART6}; 
GPIO_TypeDef* COM_TX_PORT[COMNUM] = {GPIOA,GPIOA,GPIOD,GPIOA,GPIOC,GPIOG};
GPIO_TypeDef* COM_RX_PORT[COMNUM] = {GPIOA,GPIOA,GPIOD,GPIOA,GPIOD,GPIOG};
const uint32_t COM_USART_CLK[COMNUM] = {	RCC_APB2Periph_USART1,
											RCC_APB1Periph_USART2,
											RCC_APB1Periph_USART3,
											RCC_APB1Periph_UART4,
											RCC_APB1Periph_UART5,
											RCC_APB2Periph_USART6};

const uint32_t COM_TX_PORT_CLK[COMNUM] = {	RCC_AHB1Periph_GPIOA,
											RCC_AHB1Periph_GPIOA,
											RCC_AHB1Periph_GPIOD,
											RCC_AHB1Periph_GPIOA,
											RCC_AHB1Periph_GPIOC,
											RCC_AHB1Periph_GPIOG};

const uint32_t COM_RX_PORT_CLK[COMNUM] = {	RCC_AHB1Periph_GPIOA,
											RCC_AHB1Periph_GPIOA,
											RCC_AHB1Periph_GPIOD,
											RCC_AHB1Periph_GPIOA,
											RCC_AHB1Periph_GPIOD,
											RCC_AHB1Periph_GPIOG};

const uint16_t COM_TX_PIN[COMNUM] = {	GPIO_Pin_9,GPIO_Pin_2,GPIO_Pin_8,
										GPIO_Pin_0,GPIO_Pin_12,GPIO_Pin_14};

const uint16_t COM_RX_PIN[COMNUM] = {	GPIO_Pin_10,GPIO_Pin_3,GPIO_Pin_9,
										GPIO_Pin_1,GPIO_Pin_2,GPIO_Pin_9};

const uint8_t COM_TX_PIN_SOURCE[COMNUM] = {GPIO_PinSource9,GPIO_PinSource2,
											GPIO_PinSource8,GPIO_PinSource0,
											GPIO_PinSource12,GPIO_PinSource14};

const uint8_t COM_RX_PIN_SOURCE[COMNUM] = {GPIO_PinSource10,GPIO_PinSource3,
											GPIO_PinSource9,GPIO_PinSource1,
											GPIO_PinSource2,GPIO_PinSource9};

const uint8_t COM_TX_AF[COMNUM] = {	GPIO_AF_USART1,GPIO_AF_USART2,GPIO_AF_USART3,
									GPIO_AF_UART4,GPIO_AF_UART5,GPIO_AF_USART6};

const uint8_t COM_RX_AF[COMNUM] = {	GPIO_AF_USART1,GPIO_AF_USART2,GPIO_AF_USART3,
									GPIO_AF_UART4,GPIO_AF_UART5,GPIO_AF_USART6};
/******************************���ڶ����**********************/
//�������������IO
//IO_IN1	:PG5	,IO_IN2 :PG12	,IO_IN3 :PD14
//IO_IN4	:PC13	,IO_IN5 :PD7	,IO_IN6 :PG3
//IO_IN7	:PG15	,IO_IN8 :PG11	,IO_IN9 :PA8
//IO_IN10	:PG2	,IO_IN11:PD13	,IO_IN12:PB3
//IO_IN13	:PD6	,IO_IN14:PG4	,IO_IN15:PG13
//IO_IN16	:PD15	,IO_IN17:PC8	,IO_IN18:PG10
GPIO_TypeDef* FeedBackPortTab[18] = {
GPIOG,GPIOG,GPIOD,GPIOC,GPIOD,GPIOG,GPIOG,GPIOG,GPIOA,
GPIOG,GPIOD,GPIOB,GPIOD,GPIOG,GPIOG,GPIOD,GPIOC,GPIOG};
u16 FeedBackPinTab[18] = {
GPIO_Pin_5,GPIO_Pin_12,GPIO_Pin_14,GPIO_Pin_13,GPIO_Pin_7,GPIO_Pin_3,
GPIO_Pin_15,GPIO_Pin_11,GPIO_Pin_8,GPIO_Pin_2,GPIO_Pin_13,GPIO_Pin_3,
GPIO_Pin_6,GPIO_Pin_4,GPIO_Pin_13,GPIO_Pin_15,GPIO_Pin_8,GPIO_Pin_10
};

/*************************************************************
** ��������: void Uartx_SendMsg(COM_TypeDef COMn,u8 *pSendMsg,u32 nLen) 
** ��������: �򴮿ڷ�������
** �������: COMn:���ں�
** 		 pSendMsg:ָ������
** 		 nLen:���ݳ���	 
** �������: ��
** ����: oueh 2013.06.23
** Note:
Ϊ�˿ɿ��Ĺ�������485����״̬�л�ʱ��Ҫ���ʵ���ʱ���ٽ������ݵ��շ�������������������ݷ���״̬�£��Ƚ����ƶ��á�1������ʱ1ms���ҵ�ʱ�䣬�ٷ�����Ч�����ݣ�һ�����ݷ��ͽ���������ʱ1ms�󣬽����ƶ��á�0��
***************************************************************/
void Uartx_SendMsg(COM_TypeDef COMn,u8 *pSendMsg,u32 nLen)
{
	u32 i = 0;
	
	//485ʹ��д
	if(COM1 == COMn)	
	{
		Uart1_485_Write();
	}
	else if(COM3 == COMn)
	{
		Uart3_485_Write();	
	}

	printf("COM%d:",COMn+1);
	while(nLen > 0)//���Ͳ��ȴ���ֱ�����ͳɹ���
	{
		printf("%c",*(pSendMsg + i));
		USART_SendData(COM_USART[COMn],*(pSendMsg + i));
		while(USART_GetFlagStatus(COM_USART[COMn], USART_FLAG_TXE) == RESET)
		{

		}
		i++;
		nLen--;
		Delay_Cnt(0x9FF);
	}
	printf("\r\n");
	//485ʹ�ܶ�
	if(COM1 == COMn)
	{
		Uart1_485_Read();
	}
	else if(COM3 == COMn)
	{
		Uart3_485_Read();	
	}

}

/*******************************************************************************
**  ��������  : void Buzzer_Beep(u32 nBeepTime)
**  ��������  : ʹ��������һ��ʱ���ֹͣ
**  ��    ��  : nBeepTime:���������ʱ�� ��λ:����
**  ��    ��  : ��
**  ȫ�ֱ���  : ��
**  ���ú���  : ��
**  �ж���Դ  : ��
**  ��    ע  :
*******************************************************************************/
void Buzzer_Beep(u32 nBeepTime)
{
	BZ_OUT_HIGH;
	Delay(nBeepTime);
	BZ_OUT_LOW;
}


/*******************************************************************************
**  ��������  : void NVIC_Configuration()
**  ��������  : �������ʼ��Ӳ��
**  ��    ��  : ��
**  ��    ��  : ��
**  ȫ�ֱ���  : ��
**  ���ú���  : ��
**  �ж���Դ  : ��
**  ��    ע  :
*******************************************************************************/
void NVIC_Configuration()
{
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 		   

	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
	
	NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
	
	NVIC_InitStructure.NVIC_IRQChannel = SPI2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	

}

/*******************************************************************************
**  ��������  : void Output_Feedback_GPIO_Configuration()
**  ��������  : ��ʼ�����������IO
**  ��    ��  : ��	
**  ��    ��  : ��
**  ȫ�ֱ���  : ��
**  ���ú���  : ��
**  �ж���Դ  : ��
**  ��    ע  :
*******************************************************************************/
void Output_Feedback_GPIO_Configuration()
{
	GPIO_InitTypeDef  GPIO_InitStructure;	
	u8 i = 0;
	RCC_AHB1PeriphClockCmd(RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIODEN | RCC_AHB1ENR_GPIOGEN, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	for(i=0;i<18;i++)
	{
		GPIO_InitStructure.GPIO_Pin = FeedBackPinTab[i];	
		GPIO_Init(FeedBackPortTab[i], &GPIO_InitStructure);				
	}
}

/*******************************************************************************
**  ��������  : void Input_GPIO_Configuration()
**  ��������  : ��ʼ�������IO
**  ��    ��  : ��	
**  ��    ��  : ��
**  ȫ�ֱ���  : ��
**  ���ú���  : ��
**  �ж���Դ  : ��
**  ��    ע  :
*******************************************************************************/
void Input_GPIO_Configuration()
{
	GPIO_InitTypeDef  GPIO_InitStructure;	
	
	//IO_IN21	:PF11	,IO_IN22:PF12	,IO_IN23:PG0
	//IO_IN24	:PF15	,IO_IN25:PE7	,IO_IN26:PG1
	//IO_IN27	:PF13	,IO_IN28:PF14	
	RCC_AHB1PeriphClockCmd(RCC_AHB1ENR_GPIOEEN | RCC_AHB1ENR_GPIOFEN | RCC_AHB1ENR_GPIOGEN, ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOE, &GPIO_InitStructure);	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Init(GPIOF, &GPIO_InitStructure);		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_Init(GPIOG, &GPIO_InitStructure);		
	
}

/*******************************************************************************
**  ��������  : void General_GPIO_Configuration()
**  ��������  : ��ʼ�������IO
**  ��    ��  : ��	
**  ��    ��  : ��
**  ȫ�ֱ���  : ��
**  ���ú���  : ��
**  �ж���Դ  : ��
**  ��    ע  :
*******************************************************************************/
void General_GPIO_Configuration()
{
	GPIO_InitTypeDef  GPIO_InitStructure;	
	
	//XVO
	RCC_AHB1PeriphClockCmd(XVO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = XVO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(XVO_PORT, &GPIO_InitStructure);	
	//XVO_DISABLE;
	//X5V
	RCC_AHB1PeriphClockCmd(X5V_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = X5V_PIN;
	GPIO_Init(X5V_PORT, &GPIO_InitStructure);	
	X5V_ENABLE;	
	//485R/D
	RCC_AHB1PeriphClockCmd(UART3_RW_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = UART3_RW_PIN;
	GPIO_Init(UART3_RW_GPIO_PORT, &GPIO_InitStructure);	
	Uart3_485_Read();
	//485R/D2
	RCC_AHB1PeriphClockCmd(UART1_RW_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = UART1_RW_PIN;
	GPIO_Init(UART1_RW_GPIO_PORT, &GPIO_InitStructure);	
	Uart1_485_Read();	
	//GNSS����1��2
	RCC_AHB1PeriphClockCmd(GNSS_ANT1_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GNSS_ANT1_PIN;
	GPIO_Init(GNSS_ANT1_PORT, &GPIO_InitStructure);	
	RCC_AHB1PeriphClockCmd(GNSS_ANT2_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GNSS_ANT2_PIN;
	GPIO_Init(GNSS_ANT2_PORT, &GPIO_InitStructure);	
	//XBAT0_EN
	RCC_AHB1PeriphClockCmd(XBATO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = XBATO_PIN;
	GPIO_Init(XBATO_PORT, &GPIO_InitStructure);	
	XBATO_ENABLE;
	//CHARGE_EN
	RCC_AHB1PeriphClockCmd(CHARGE_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = CHARGE_PIN;
	GPIO_Init(CHARGE_PORT, &GPIO_InitStructure);	
	CHARGE_DISABLE;
	//XBAT_EN
	RCC_AHB1PeriphClockCmd(XBAT_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = XBAT_PIN;
	GPIO_Init(XBAT_PORT, &GPIO_InitStructure);	
	XBAT_DISABLE;
	//XBAT_FB1
	RCC_AHB1PeriphClockCmd(XBAT_FB1_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = XBAT_FB1_PIN;
	GPIO_Init(XBAT_FB1_PORT, &GPIO_InitStructure);	
	//XBAT_FB2
	RCC_AHB1PeriphClockCmd(XBAT_FB2_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = XBAT_FB2_PIN;
	GPIO_Init(XBAT_FB2_PORT, &GPIO_InitStructure);	
	//BZ_OUT
	RCC_AHB1PeriphClockCmd(BZ_OUT_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = BZ_OUT_PIN;
	GPIO_Init(BZ_OUT_PORT, &GPIO_InitStructure);	
	BZ_OUT_LOW;
	//LED_CAN
	RCC_AHB1PeriphClockCmd(LED_CAN_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = LED_CAN_PIN;
	GPIO_Init(LED_CAN_PORT, &GPIO_InitStructure);
	LED_CAN_HIGH;
}

/*************************************************************
** ��������:  void ADC3_GPIO_Configuration(void)
** ��������:  ��ʼ��ADC��IO
** �������:  �� 	
** �������:  ��
** ����:  oueh 2013.06.23
** Note: 
***************************************************************/
void ADC3_GPIO_Configuration(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;	
	RCC_AHB1PeriphClockCmd(ADC_IN_CLK | ADC_KEY_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = ADC_IN1_PIN | ADC_IN2_PIN | ADC_IN3_PIN | ADC_IN4_PIN | ADC_IN5_PIN | ADC_IN6_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(ADC_IN1_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = ADC_KEY_PIN;
	GPIO_Init(ADC_KEY_PORT, &GPIO_InitStructure);	
	
}


/*************************************************************
** ��������:  void GPIO_Configuration(void)
** ��������:  ��ʼ��IO
** �������:  �� 	
** �������:  ��
** ����:  oueh 2013.05.23
** Note: 1.��ʱ��
**		 2.����IO
**		 3.�ó�ʼ״̬
***************************************************************/
void GPIO_Configuration(void)
{	
	HC595_GPIO_Configuration();				//HC595 IO��ʼ��
	LCD_13264_GPIO_Configuration();			//LCD_13264��ʼ��
	Output_Feedback_GPIO_Configuration();	//18·�������IO��ʼ��	
	Input_GPIO_Configuration();				//8·�����ʼ��
	General_GPIO_Configuration();				//ͨ��IO��ʼ��
}


/*******************************************************************************
**  ��������  : void Com_Uart_Init(COM_TypeDef COMn,u32 nBaud)
**  ��������  : ��ʼ������
**  ��    ��  :	COMn:���ں� 
					nBaud:������
**  ��    ��  : ��
**  ȫ�ֱ���  : ��
**  ���ú���  : ��
**  �ж���Դ  : ��
**  ��    ע  :
*******************************************************************************/
void Com_Uart_Init(COM_TypeDef COMn,u32 nBaud)
{
    USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	USART_DeInit(COM_USART[COMn]);
	RCC_AHB1PeriphClockCmd(COM_TX_PORT_CLK[COMn]|COM_RX_PORT_CLK[COMn],ENABLE);
	if(COM1 == COMn || COM6 == COMn)
	{
		RCC_APB2PeriphClockCmd(COM_USART_CLK[COMn],ENABLE);
	}
	else
	{
		RCC_APB1PeriphClockCmd(COM_USART_CLK[COMn],ENABLE);
	}
	//����
	GPIO_PinAFConfig(COM_TX_PORT[COMn],COM_TX_PIN_SOURCE[COMn],COM_TX_AF[COMn]);
	GPIO_PinAFConfig(COM_RX_PORT[COMn],COM_RX_PIN_SOURCE[COMn],COM_RX_AF[COMn]);

	//TX
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = COM_TX_PIN[COMn];
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(COM_TX_PORT[COMn],&GPIO_InitStructure);
	//RX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = COM_RX_PIN[COMn];
	GPIO_Init(COM_RX_PORT[COMn],&GPIO_InitStructure);	

	//��ʼ�����ò���
	USART_InitStructure.USART_BaudRate = nBaud;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(COM_USART[COMn], &USART_InitStructure);	
	
	USART_ITConfig(COM_USART[COMn], USART_IT_RXNE, ENABLE);
	//USART_ITConfig(UART4, USART_IT_TXE, ENABLE);
	USART_Cmd(COM_USART[COMn], ENABLE);
	//��ֹ��һ�������޷��������⡣
	//USART_GetITStatus(UART4, USART_FLAG_TC); 
}

/*************************************************************
** ��������:  void Uart_Configuration()
** ��������:  ��ʼ�����ںʹ��ڶ���
** �������:  ��
** �������:  ��
** ����:  oueh 2013.06.22
** Note:
***************************************************************/
void Uart_Configuration()
{
	FIFO_Queue_Init();				//��ʼ�����д��ڶ���
	Com_Uart_Init(COM1,9600);		//��ʼ������1
	Com_Uart_Init(COM2,115200);		//��ʼ������2
	Com_Uart_Init(COM3,9600);		//��ʼ������3	
	Com_Uart_Init(COM4,9600);		//��ʼ������4
	Com_Uart_Init(COM5,9600);		//��ʼ������5
	Com_Uart_Init(COM6,9600);		//��ʼ������6
	printf("%s:\r\n",__FUNCTION__);
}

/*************************************************************
** ��������:  void CAN1_Configuration(u8 Can_Mode)
** ��������:  ��ʼ��CAN1
** �������:  ��
** �������:  ��
** ����:  oueh 2013.06.22
** Note:
***************************************************************/
void CAN1_Configuration(u8 Can_Mode)
{
	CAN_InitTypeDef 	   CAN_InitStructure;
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	//����ʱ�Ӻ�IO����
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
	RCC_AHB1PeriphClockCmd(CAN1_TX_CLK | CAN1_RX_CLK,ENABLE);
	GPIO_PinAFConfig(CAN1_TX_PORT,CAN1_TX_PinSource,CAN1_TX_AF);
	GPIO_PinAFConfig(CAN1_RX_PORT,CAN1_RX_PinSource,CAN1_RX_AF);
	//��ʼ��RX
	GPIO_InitStructure.GPIO_Pin = CAN1_RX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(CAN1_RX_PORT, &GPIO_InitStructure);	
	//��ʼ��TX
	GPIO_InitStructure.GPIO_Pin = CAN1_TX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(CAN1_TX_PORT, &GPIO_InitStructure);

	CAN_DeInit(CAN1);	
	CAN_StructInit(&CAN_InitStructure);
	CAN_InitStructure.CAN_TTCM = DISABLE;
	CAN_InitStructure.CAN_ABOM = DISABLE;
	CAN_InitStructure.CAN_AWUM = DISABLE;
	CAN_InitStructure.CAN_NART = DISABLE;
	CAN_InitStructure.CAN_RFLM = DISABLE;
	CAN_InitStructure.CAN_TXFP = DISABLE;
	CAN_InitStructure.CAN_Mode = Can_Mode;//CAN_Mode_LoopBack
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
	CAN_InitStructure.CAN_BS1 = CAN_BS1_6tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_8tq;
	CAN_InitStructure.CAN_Prescaler = 8;//120/4/8/(1+6+8)
	CAN_Init(CAN1, &CAN_InitStructure);	

	CAN_FilterInitStructure.CAN_FilterNumber = 0;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;  
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
}

/*************************************************************
** ��������:  void CAN2_Configuration(u8 Can_Mode)
** ��������:  ��ʼ��CAN2
** �������:  ��
** �������:  ��
** ����:  oueh 2013.06.22
** Note:
***************************************************************/
void CAN2_Configuration(u8 Can_Mode)
{
	CAN_InitTypeDef 	   CAN_InitStructure;
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	//����ʱ�Ӻ�IO����
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);
	RCC_AHB1PeriphClockCmd(CAN2_TX_CLK | CAN2_RX_CLK,ENABLE);
	GPIO_PinAFConfig(CAN2_TX_PORT,CAN2_TX_PinSource,CAN2_TX_AF);
	GPIO_PinAFConfig(CAN2_RX_PORT,CAN2_RX_PinSource,CAN2_RX_AF);
	//��ʼ��RX
	GPIO_InitStructure.GPIO_Pin = CAN2_RX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(CAN2_RX_PORT, &GPIO_InitStructure);	
	//��ʼ��TX
	GPIO_InitStructure.GPIO_Pin = CAN2_TX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(CAN2_TX_PORT, &GPIO_InitStructure);

	CAN_DeInit(CAN2);	
	CAN_StructInit(&CAN_InitStructure);
	CAN_InitStructure.CAN_TTCM = DISABLE;
	CAN_InitStructure.CAN_ABOM = DISABLE;
	CAN_InitStructure.CAN_AWUM = DISABLE;
	CAN_InitStructure.CAN_NART = DISABLE;
	CAN_InitStructure.CAN_RFLM = DISABLE;
	CAN_InitStructure.CAN_TXFP = DISABLE;
	CAN_InitStructure.CAN_Mode = Can_Mode;//CAN_Mode_LoopBack
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
	CAN_InitStructure.CAN_BS1 = CAN_BS1_6tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_8tq;
	CAN_InitStructure.CAN_Prescaler = 8;//120/4/8/(1+6+8)
	CAN_Init(CAN2, &CAN_InitStructure); 

	CAN_FilterInitStructure.CAN_FilterNumber = 14;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;  
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
	CAN_ITConfig(CAN2, CAN_IT_FMP0, ENABLE);
}


/*************************************************************
** ��������:  void ADC3_DMA_Configuration()
** ��������:  ��ʼ��ADC3
** �������:  ��
** �������:  ��
** ����:  oueh 2013.06.22
** Note:
***************************************************************/
void ADC3_DMA_Configuration()
{
	ADC_InitTypeDef       ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	DMA_InitTypeDef       DMA_InitStructure;
	
#if 0
	/* Enable ADC3, DMA2 and GPIO clocks ****************************************/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2 | RCC_AHB1Periph_GPIOF, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);

	/* DMA2 Stream0 channel2 configuration **************************************/
	DMA_InitStructure.DMA_Channel = DMA_Channel_2;  
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC3_DR_ADDRESS;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&ADC3ConvertedValue;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = 1;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream0, &DMA_InitStructure);
	DMA_Cmd(DMA2_Stream0, ENABLE);

	/* Configure ADC3 Channel7 pin as analog input ******************************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOF, &GPIO_InitStructure);

	/* ADC Common Init **********************************************************/
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);

	/* ADC3 Init ****************************************************************/
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_Init(ADC3, &ADC_InitStructure);

	/* ADC3 regular channel7 configuration *************************************/
	ADC_RegularChannelConfig(ADC3, ADC_Channel_7, 1, ADC_SampleTime_3Cycles);

	/* Enable DMA request after last transfer (Single-ADC mode) */
	ADC_DMARequestAfterLastTransferCmd(ADC3, ENABLE);

	/* Enable ADC3 DMA */
	ADC_DMACmd(ADC3, ENABLE);

	/* Enable ADC3 */
	ADC_Cmd(ADC3, ENABLE);	
	/* Start ADC3 Software Conversion */ 
	ADC_SoftwareStartConv(ADC3);	
#else

	// DMA1ֻ��ʹ����APB1��MEMORY�ϣ������DMA2
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2 | RCC_AHB1Periph_GPIOF, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);

	DMA_InitStructure.DMA_Channel = DMA_Channel_2;//ͨ��������
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC3_DR_ADDRESS; //���������ַ
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&ADC3ConvertedValue; //����Ĵ�����ַ	
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory; //����DMA����ģʽ�����赽�ڴ�
	DMA_InitStructure.DMA_BufferSize = ADC3_CHANNEL_NUM; //����buffer��С������ͨ��������
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //�����ַ�Ĵ�������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //DMA�Ĵ�����֮���ӣ���ͨ�������ӣ�
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //DMA�������ݴ�С��16λ����
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //DMA�洢�����ݴ�С��16λ����
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; //ģʽΪѭ��ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_High; //DMA���ȼ���
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable; //ֱ�Ӵ���ģʽ 
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream0, &DMA_InitStructure);
	DMA_Cmd(DMA2_Stream0, ENABLE); //ʹ��DMA2��0
	
	ADC3_GPIO_Configuration();				//ADC3��IO��ʼ��
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent; //ADC����ģʽ
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2; //ADC��������2��Ƶ
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles; //��������
	ADC_CommonInit(&ADC_CommonInitStructure); //������ʼ��

	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b; //ADC��������
	ADC_InitStructure.ADC_ScanConvMode = ENABLE; //����ɨ��ģʽ����Ϊ��·
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; //����ɨ��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//���ⲿ����
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //�����Ҷ���
	ADC_InitStructure.ADC_NbrOfConversion = ADC3_CHANNEL_NUM; //����ͨ����
	ADC_Init(ADC3, &ADC_InitStructure);

	ADC_RegularChannelConfig(ADC3, ADC_Channel_9, 1, ADC_SampleTime_112Cycles);
	ADC_RegularChannelConfig(ADC3, ADC_Channel_14, 2, ADC_SampleTime_112Cycles);
	ADC_RegularChannelConfig(ADC3, ADC_Channel_7, 3, ADC_SampleTime_112Cycles);
	ADC_RegularChannelConfig(ADC3, ADC_Channel_5, 4, ADC_SampleTime_112Cycles);
	ADC_RegularChannelConfig(ADC3, ADC_Channel_8, 5, ADC_SampleTime_112Cycles);
	ADC_RegularChannelConfig(ADC3, ADC_Channel_6, 6, ADC_SampleTime_112Cycles);
	ADC_RegularChannelConfig(ADC3, ADC_Channel_10, 7, ADC_SampleTime_112Cycles);

	ADC_DMARequestAfterLastTransferCmd(ADC3, ENABLE);
	ADC_DMACmd(ADC3, ENABLE);
	ADC_Cmd(ADC3, ENABLE);
	ADC_SoftwareStartConv(ADC3); 
#endif			 
}

/*************************************************************
** ��������:  void LCD_SPI_Configuration()
** ��������:  ��ʼ��SPI2s
** �������:  ��
** �������:  ��
** ����:  oueh 2013.07.12
** Note:
***************************************************************/
void LCD_SPI_Configuration()
{	
#if 0
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;

	RCC_APB1PeriphClockCmd(LCD_SPI_CLK,ENABLE);
	RCC_AHB1PeriphClockCmd(LCD_SPI_MOSI_CLK | LCD_SPI_MISO_CLK | 
		   					 LCD_SPI_SCK_CLK|LCD_SPI_CS_CLK,ENABLE);

	//
	GPIO_PinAFConfig(LCD_SPI_MOSI_PORT,LCD_SPI_MOSI_SOURCE,LCD_SPI_MOSI_AF);
	GPIO_PinAFConfig(LCD_SPI_MISO_PORT,LCD_SPI_MISO_SOURCE,LCD_SPI_MISO_AF);
	GPIO_PinAFConfig(LCD_SPI_SCK_PORT,LCD_SPI_SCK_SOURCE,LCD_SPI_SCK_AF);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;	
	
	GPIO_InitStructure.GPIO_Pin =  LCD_SPI_MOSI_PIN;
	GPIO_Init(LCD_SPI_MOSI_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin =  LCD_SPI_MISO_PIN;
	GPIO_Init(LCD_SPI_MISO_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin =  LCD_SPI_SCK_PIN;
	GPIO_Init(LCD_SPI_SCK_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = LCD_SPI_CS_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(LCD_SPI_CS_PORT, &GPIO_InitStructure);	
	LCD_SPI_CS_HIGH;
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(LCD_SPI, &SPI_InitStructure);
	//SPI_I2S_ITConfig(LCD_SPI,SPI_I2S_IT_TXE,ENABLE);
	SPI_Cmd(LCD_SPI, ENABLE);
#endif	
}

/*************************************************************
** ��������:  void Tim3_Pwm_Configuration(u32 nFre,u8 nDuty)
** ��������:  ��ʱ��3��PWM���
** �������:  nFre:Ƶ��HZ/10   ��125��ʾƵ��12.5HZ
					 nDuty:ռ�ձ�(1~1000) ��:125��ʾ ռ�ձ�Ϊ12.5%
** �������:  ��
** ����:  oueh 2013.08.02
** Note:
***************************************************************/
void Tim3_Pwm_Configuration(u32 nFre,u16 nDuty)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;	
	u32 nPeriod = 0;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_AHB1PeriphClockCmd(PWM1_OUT_CLK,ENABLE);

	GPIO_InitStructure.GPIO_Pin = PWM1_OUT_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(PWM1_OUT_PORT, &GPIO_InitStructure); 	

	GPIO_PinAFConfig(PWM1_OUT_PORT,PWM1_OUT_PinSource,PWM1_OUT_AF);

	nPeriod = 1000/nFre*10;//(SystemCoreClock /2)/nFre/10 - 1;
	TIM_TimeBaseStructure.TIM_Period = nPeriod-1;//ARR��ֵ
	TIM_TimeBaseStructure.TIM_Prescaler = 60000;//120M/2/60K = 1KHZ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);	

	/* PWM1 Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = nPeriod*nDuty/1000;//;nPeriod*nDuty/1000;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM3, ENABLE);
	/* TIM3 enable counter */
	TIM_Cmd(TIM3, ENABLE);	
}

/*************************************************************
** ��������:  void SPI1_Flash_Configuration()
** ��������:  ��ʼ��SPI1
** �������:  ��
** �������:  ��
** ����:  oueh 2013.08.20
** Note:
***************************************************************/
void SPI1_Flash_Configuration()
{
 	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;
  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
  	RCC_AHB1PeriphClockCmd(SPI1_SCK_CLK| SPI1_MOSI_CLK| SPI1_MISO_CLK
                          	| SPI1_NSS2_CLK, ENABLE);
	GPIO_PinAFConfig(SPI1_SCK_PORT, SPI1_SCK_PinSource, SPI1_SCK_AF);
	GPIO_PinAFConfig(SPI1_MOSI_PORT, SPI1_MOSI_PinSource, SPI1_MOSI_AF);
  	GPIO_PinAFConfig(SPI1_MISO_PORT, SPI1_MISO_PinSource, SPI1_MISO_AF);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
	    
	GPIO_InitStructure.GPIO_Pin = SPI1_SCK_PIN;
	GPIO_Init(SPI1_SCK_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =  SPI1_MOSI_PIN;
	GPIO_Init(SPI1_MOSI_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =  SPI1_MISO_PIN;
	GPIO_Init(SPI1_MISO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = SPI1_NSS2_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(SPI1_NSS2_PORT, &GPIO_InitStructure);	
	SPI1_NSS2_HIGH;

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;

	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitStructure);
	SPI_Cmd(SPI1, ENABLE);	
}

/*******************************************************************************
**  ��������  : void Bsp_Init()
**  ��������  : �������ʼ��Ӳ��
**  ��    ��  : ��
**  ��    ��  : ��
**  ȫ�ֱ���  : ��
**  ���ú���  : ��
**  �ж���Դ  : ��
**  ��    ע  :
*******************************************************************************/
void Bsp_Init()
{
	SysTick_Config(SystemCoreClock / 1000);	//����ϵͳʱ��
	NVIC_Configuration();						//��ʼ���жϿ�����
	GPIO_Configuration();						//��ʼ��GPIO
	Uart_Configuration();						//��ʼ������
	CAN1_Configuration(CAN_Mode_Normal);		//��ʼ��CAN1
	CAN2_Configuration(CAN_Mode_Normal);		//��ʼ��CAN2
	ADC3_DMA_Configuration();					//��ʼ��ADC3
	Tim3_Pwm_Configuration(10,500);			//��ʼ��PWM1���
	SPI1_Flash_Configuration();				//��ʼ��SPI1
}

/*******************************************************************************
**  ��������  : void LCD_Configuration(void)
**  ��������  : ��ʼ��LCD
**  ��    ��  : ��
**  ��    ��  : ��
**  ȫ�ֱ���  : ��
**  ���ú���  : ��
**  �ж���Դ  : ��
**  ��    ע  :
*******************************************************************************/
void LCD_Configuration(void)
{
	LCD_RST_ON; 
	Delay(50);
	LCD_RST_OFF;
	LCD_Init();
	LCD_Screen_Clear();
	Delay(100);
	LCD_Home_Page();
}

/*******************************************************************************
**  ��������  : bool Check_Self_Can(void)
**  ��������  : �Լ�2·CAN
**  ��    ��  : ��
**  ��    ��  : false:ʧ�� true:�ɹ�
**  ȫ�ֱ���  : ��
**  ���ú���  : ��
**  �ж���Դ  : ��
**  ��    ע  :
*******************************************************************************/
bool Check_Self_Can(void)
{
	FIFOQueue *pFifo1 = Get_Can1_Rx_Fifo();
	FIFOQueue *pFifo2 = Get_Can2_Rx_Fifo();
	u8 i = 0;
	CanTxMsg TxMessage;
	ElemType sGet[20] = "";

	//��ջ���
	QueueInit(pFifo1);
	QueueInit(pFifo2);		
	//����CAN1��CAN2Ϊ�ػ�ģʽ
	CAN1_Configuration(CAN_Mode_LoopBack);
	CAN2_Configuration(CAN_Mode_LoopBack);		
	TxMessage.StdId = 0;
	TxMessage.ExtId = 0x1234;
	TxMessage.IDE = CAN_ID_EXT;
	TxMessage.RTR = CAN_RTR_DATA;
	TxMessage.DLC = 8;				
	memcpy(TxMessage.Data,"CAN_TEST",8);
	CAN_Transmit(CAN1, &TxMessage); 
	CAN_Transmit(CAN2, &TxMessage);
	Delay_ms(20);
	while(isQueueEmpty(pFifo1) == QUEUE_NOT_EMPTY)
	{
		QueueOut(pFifo1,&sGet[i++]);
	}
	//У��CAN1�ķ��ͺͽ��������Ƿ�һ��
	if(memcmp(TxMessage.Data,sGet,8) != 0)
	{
		printf("%s:CAN1 Err! %s\r\n",__FUNCTION__,sGet);
		return false;
	}
	i = 0;
	memset(sGet,0,sizeof(sGet));
	while(isQueueEmpty(pFifo2) == QUEUE_NOT_EMPTY)
	{
		QueueOut(pFifo2,&sGet[i++]);
	}
	//У��CAN2�ķ��ͺͽ��������Ƿ�һ��
	if(memcmp(TxMessage.Data,sGet,8) != 0)
	{
		printf("%s:CAN2 Err! %s\r\n",__FUNCTION__,sGet);
		return false;
	}	
	//��ջ���
	QueueInit(pFifo1);
	QueueInit(pFifo2);	
	//����CAN1��CAN2Ϊ����
	CAN1_Configuration(CAN_Mode_Normal);
	CAN2_Configuration(CAN_Mode_Normal);		
	return true;
}

/*******************************************************************************
**  ��������  : bool Check_Self_Uart(void)
**  ��������  : �Լ촮��2��5��6
**  ��    ��  : ��
**  ��    ��  : false:ʧ�� true:�ɹ�
**  ȫ�ֱ���  : ��
**  ���ú���  : ��
**  �ж���Դ  : ��
**  ��    ע  :
*******************************************************************************/
bool Check_Self_Uart(void)
{

	//����5,6�Լ�,�̽�5 6
	//FIFOQueue *pFifo2 = Get_Uart2_Rx_Fifo();
	FIFOQueue *pFifo5 = Get_Uart5_Rx_Fifo();
	FIFOQueue *pFifo6 = Get_Uart6_Rx_Fifo();
	ElemType sSend[10] = "CHECK_SELF";
	ElemType sGet[10] = "";
	u8 i = 0;
	
	//QueueInit(pFifo2);
	QueueInit(pFifo5);
	QueueInit(pFifo6);
	//Uartx_SendMsg(COM2,sSend,sizeof(sSend)-1);
	Uartx_SendMsg(COM5,sSend,sizeof(sSend));
	Uartx_SendMsg(COM6,sSend,sizeof(sSend));		

	Delay_ms(100);
	#if 0
	while(isQueueEmpty(pFifo2) == QUEUE_NOT_EMPTY)
	{
		QueueOut(pFifo2,&sGet[i++]);
	}
	
	if(memcmp(sSend,sGet,sizeof(sSend)-1) != 0)
	{
		printf("%s:Uart2 Err! %s\r\n",__FUNCTION__,sGet);
		return false;
	}
	#endif
	i = 0;
	memset(sGet,0,sizeof(sGet));
	while(isQueueEmpty(pFifo5) == QUEUE_NOT_EMPTY)
	{
		QueueOut(pFifo5,&sGet[i]);
		i++;
	}
	
	if(memcmp(sSend,sGet,i) != 0)
	{
		printf("%s:Uart5 Err! %s %d %d\r\n",__FUNCTION__,sGet,sizeof(sSend),i);
		return false;
	}
	i = 0;
	memset(sGet,0,sizeof(sGet));
	while(isQueueEmpty(pFifo6) == QUEUE_NOT_EMPTY)
	{
		QueueOut(pFifo6,&sGet[i]);
		i++;
	}
	
	if(memcmp(sSend,sGet,i) != 0)
	{
		printf("%s:Uart6 Err! %s %d %d\r\n",__FUNCTION__,sGet,sizeof(sSend),i);
		return false;
	}		
	//�������������ڻ���
	//QueueInit(pFifo2);
	printf("Uart Check OK!\r\n");
	QueueInit(pFifo5);
	QueueInit(pFifo6);
	return true;

	
}

/*******************************************************************************
**  ��������  : bool Check_Self_Volt(void)
**  ��������  : �Լ�2·��ѹ���
**  ��    ��  : ��
**  ��    ��  : false:ʧ�� true:�ɹ�
**  ȫ�ֱ���  : ��
**  ���ú���  : ��
**  �ж���Դ  : ��
**  ��    ע  :
*******************************************************************************/
bool Check_Self_Volt(void)
{
	u8 Volt1 = 0,Volt2 = 0;

	// 2·��ѹ���8V�����ȡ������ADC��ֵ����У��
	g_HC595State |= 0x000C0000;
	HC595_Renew();	
	//XVO_DISABLE;
	XHV_Volt_Set(8);	
	Delay(1000);//������Ҫ�ȴ�ADC�ȶ���ɡ�
	//printf("%d %d %d %d %d %d\r\n",ADC3ConvertedValue[0],ADC3ConvertedValue[1],
	//	ADC3ConvertedValue[2],ADC3ConvertedValue[3],
	//	ADC3ConvertedValue[4],ADC3ConvertedValue[5]);	
	Volt1 = (int)((double)ADC3ConvertedValue[0] /4096*3.3*4+0.5);
	Volt2 = (int)((double)ADC3ConvertedValue[1] /4096*3.3*4+0.5);
	g_HC595State &= ~0x000C0000;
	HC595_Renew();		
	//XVO_ENABLE;
	XHV_Volt_Set(0);
	Delay(100);
	if(Volt1 != 8 || Volt2 != 8)
	{
		printf("Volt Check Self Err! %d %d\r\n",Volt1,Volt2);
		return false;
	}
	return true;
	
}
#if 1
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
/*******************************************************************************
* Function Name  : PUTCHAR_PROTOTYPE
* Description    : Retargets the C library printf function to the USART.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
PUTCHAR_PROTOTYPE
{
#if 0
	USART_SendData(USART6, (u8) ch); 
	/* Loop until the end of transmission */
	while(USART_GetFlagStatus(USART6, USART_FLAG_TXE) == RESET)
	{
	}
	return ch;
#endif
	return ch;

}
#endif

/*******************************************************************************
**  ��������  : bool Check_Self_IO(void)
**  ��������  : �Լ�IO
**  ��    ��  : ��
**  ��    ��  : false:ʧ�� true:�ɹ�
**  ȫ�ֱ���  : ��
**  ���ú���  : ��
**  �ж���Դ  : ��
**  ��    ע  :
*******************************************************************************/
bool Check_Self_IO(void)
{


    u8 i = 0;

	XHV_Volt_Set(8);
#if 1
	for(i=0;i<18;i++)
	{
		g_HC595State |= 1<<i;//�ߵ�ƽ����Բ�
		HC595_Renew();	
        Delay(200);
		if(GPIO_ReadInputDataBit(FeedBackPortTab[i],FeedBackPinTab[i]) == Bit_RESET)
		//if(GPIO_ReadInputDataBit(FeedBackPortTab[i],FeedBackPinTab[i]) == Bit_SET)
		{
			printf("%s:Set High ERR %d \r\n",__FUNCTION__,i+1);
#if 1	
			g_HC595State &= ~0x0003FFFF;
			HC595_Renew();
			XHV_Volt_Set(0);
			return false;
#endif			
		}
		g_HC595State &= ~(1<<i);
		#if 0
		g_HC595State &= ~(1<<i);//�͵�ƽ����Բ�
		HC595_Renew();	
        Delay(200);
		if(GPIO_ReadInputDataBit(FeedBackPortTab[i],FeedBackPinTab[i]) == Bit_SET)
		{
			printf("%s:Set Low ERR %d \r\n",__FUNCTION__,i+1);
			XHV_Volt_Set(0);
			return false;
		}			
		#endif
	}
#else	
	g_HC595State |= 0x0003FFFF;
	//g_HC595State |= 0x00038000;
	HC595_Renew();	
	Delay(100);
	for(i = 0;i<18;i++)
	{
		if(GPIO_ReadInputDataBit(FeedBackPortTab[i],FeedBackPinTab[i]) == Bit_RESET)
		{
			printf("%s:Set High ERR %d \r\n",__FUNCTION__,i+1);
#if 1	
			g_HC595State &= ~0x0003FFFF;
			HC595_Renew();
			XHV_Volt_Set(0);
			return false;
#endif			
		}
	}
	//18·�͵�ѹ��������
	g_HC595State &= ~0x0003FFFF;
	HC595_Renew();
	Delay(100);
	for(i = 0;i<18;i++)
	{
		if(GPIO_ReadInputDataBit(FeedBackPortTab[i],FeedBackPinTab[i]) == Bit_SET)
		{
			printf("%s:Set Low ERR %d \r\n",__FUNCTION__,i+1);
			XHV_Volt_Set(0);
			return false;
		}
	}
#endif		
	g_HC595State &= ~0x0003FFFF;
	XHV_Volt_Set(0);	
	return true;

}

/*******************************************************************************
**  ��������  : bool Fct_Check_Self(void)
**  ��������  : FCT�Լ�
**  ��    ��  : ��
**  ��    ��  : false:ʧ�� true:�ɹ�
**  ȫ�ֱ���  : ��
**  ���ú���  : ��
**  �ж���Դ  : ��
**  ��    ע  :
*******************************************************************************/
bool Fct_Check_Self(void)
{ 
	do
	{
		if(Check_Self_IO() == false)		//�Լ�18·IO���
		{
			break;
		}
		if(Check_Self_Volt() == false)		//�Լ�2·��ѹ���
		{
			break;	
		}
		if(Check_Self_Can() == false)		//�Լ�CAN1��CAN2
		{
			break;
		} 
		return true;
	}while(0); 
	//����FCT�Լ�ʧ��
	Pc_Ack(FCT_ERROR,CHECKSELF_FAIL,1);
	return false;
}


