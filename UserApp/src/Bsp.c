/*******************(C)深圳市伊爱高新技术开发有限公司版权所有****
 
**  文件名称:   Bsp.c
**  创建作者:   欧恩惠
**  当前版本:   1.0.0.0
**  创建日期:   20130622
**  功能说明:   初始化硬件
**  修改记录:   
**  备    注:   
**********************************(C)copyright 2013**************************************/
#include "Bsp.h"

__IO u16 ADC3ConvertedValue[ADC3_CHANNEL_NUM] = {0};	//存放通道的ADC值
/******************************串口定义↓**********************/
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
/******************************串口定义↑**********************/
//开关量输出反馈IO
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
** 函数名称: void Uartx_SendMsg(COM_TypeDef COMn,u8 *pSendMsg,u32 nLen) 
** 功能描述: 向串口发送数据
** 输入参数: COMn:串口号
** 		 pSendMsg:指向数据
** 		 nLen:数据长度	 
** 输出参数: 无
** 创建: oueh 2013.06.23
** Note:
为了可靠的工作，在485总线状态切换时需要做适当延时，再进行数据的收发。具体的做法是在数据发送状态下，先将控制端置“1”，延时1ms左右的时间，再发送有效的数据，一包数据发送结束后再延时1ms后，将控制端置“0”
***************************************************************/
void Uartx_SendMsg(COM_TypeDef COMn,u8 *pSendMsg,u32 nLen)
{
	u32 i = 0;
	
	//485使能写
	if(COM1 == COMn)	
	{
		Uart1_485_Write();
	}
	else if(COM3 == COMn)
	{
		Uart3_485_Write();	
	}

	printf("COM%d:",COMn+1);
	while(nLen > 0)//发送并等待，直到发送成功。
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
	//485使能读
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
**  函数名称  : void Buzzer_Beep(u32 nBeepTime)
**  函数功能  : 使蜂鸣器响一段时间后停止
**  输    入  : nBeepTime:蜂鸣器响的时间 单位:毫秒
**  输    出  : 无
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
*******************************************************************************/
void Buzzer_Beep(u32 nBeepTime)
{
	BZ_OUT_HIGH;
	Delay(nBeepTime);
	BZ_OUT_LOW;
}


/*******************************************************************************
**  函数名称  : void NVIC_Configuration()
**  函数功能  : 启动后初始化硬件
**  输    入  : 无
**  输    出  : 无
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
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
**  函数名称  : void Output_Feedback_GPIO_Configuration()
**  函数功能  : 初始化输出反馈的IO
**  输    入  : 无	
**  输    出  : 无
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
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
**  函数名称  : void Input_GPIO_Configuration()
**  函数功能  : 初始化输入的IO
**  输    入  : 无	
**  输    出  : 无
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
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
**  函数名称  : void General_GPIO_Configuration()
**  函数功能  : 初始化输入的IO
**  输    入  : 无	
**  输    出  : 无
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
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
	//GNSS天线1和2
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
** 函数名称:  void ADC3_GPIO_Configuration(void)
** 功能描述:  初始化ADC的IO
** 输入参数:  无 	
** 输出参数:  无
** 创建:  oueh 2013.06.23
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
** 函数名称:  void GPIO_Configuration(void)
** 功能描述:  初始化IO
** 输入参数:  无 	
** 输出参数:  无
** 创建:  oueh 2013.05.23
** Note: 1.打开时钟
**		 2.配置IO
**		 3.置初始状态
***************************************************************/
void GPIO_Configuration(void)
{	
	HC595_GPIO_Configuration();				//HC595 IO初始化
	LCD_13264_GPIO_Configuration();			//LCD_13264初始化
	Output_Feedback_GPIO_Configuration();	//18路输出反馈IO初始化	
	Input_GPIO_Configuration();				//8路输入初始化
	General_GPIO_Configuration();				//通用IO初始化
}


/*******************************************************************************
**  函数名称  : void Com_Uart_Init(COM_TypeDef COMn,u32 nBaud)
**  函数功能  : 初始化串口
**  输    入  :	COMn:串口号 
					nBaud:波特率
**  输    出  : 无
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
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
	//复用
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

	//初始化配置参数
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
	//防止第一个数据无法发送问题。
	//USART_GetITStatus(UART4, USART_FLAG_TC); 
}

/*************************************************************
** 函数名称:  void Uart_Configuration()
** 功能描述:  初始化串口和串口队列
** 输入参数:  无
** 输出参数:  无
** 创建:  oueh 2013.06.22
** Note:
***************************************************************/
void Uart_Configuration()
{
	FIFO_Queue_Init();				//初始化所有串口队列
	Com_Uart_Init(COM1,9600);		//初始化串口1
	Com_Uart_Init(COM2,115200);		//初始化串口2
	Com_Uart_Init(COM3,9600);		//初始化串口3	
	Com_Uart_Init(COM4,9600);		//初始化串口4
	Com_Uart_Init(COM5,9600);		//初始化串口5
	Com_Uart_Init(COM6,9600);		//初始化串口6
	printf("%s:\r\n",__FUNCTION__);
}

/*************************************************************
** 函数名称:  void CAN1_Configuration(u8 Can_Mode)
** 功能描述:  初始化CAN1
** 输入参数:  无
** 输出参数:  无
** 创建:  oueh 2013.06.22
** Note:
***************************************************************/
void CAN1_Configuration(u8 Can_Mode)
{
	CAN_InitTypeDef 	   CAN_InitStructure;
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	//开启时钟和IO复用
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
	RCC_AHB1PeriphClockCmd(CAN1_TX_CLK | CAN1_RX_CLK,ENABLE);
	GPIO_PinAFConfig(CAN1_TX_PORT,CAN1_TX_PinSource,CAN1_TX_AF);
	GPIO_PinAFConfig(CAN1_RX_PORT,CAN1_RX_PinSource,CAN1_RX_AF);
	//初始化RX
	GPIO_InitStructure.GPIO_Pin = CAN1_RX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(CAN1_RX_PORT, &GPIO_InitStructure);	
	//初始化TX
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
** 函数名称:  void CAN2_Configuration(u8 Can_Mode)
** 功能描述:  初始化CAN2
** 输入参数:  无
** 输出参数:  无
** 创建:  oueh 2013.06.22
** Note:
***************************************************************/
void CAN2_Configuration(u8 Can_Mode)
{
	CAN_InitTypeDef 	   CAN_InitStructure;
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	//开启时钟和IO复用
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);
	RCC_AHB1PeriphClockCmd(CAN2_TX_CLK | CAN2_RX_CLK,ENABLE);
	GPIO_PinAFConfig(CAN2_TX_PORT,CAN2_TX_PinSource,CAN2_TX_AF);
	GPIO_PinAFConfig(CAN2_RX_PORT,CAN2_RX_PinSource,CAN2_RX_AF);
	//初始化RX
	GPIO_InitStructure.GPIO_Pin = CAN2_RX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(CAN2_RX_PORT, &GPIO_InitStructure);	
	//初始化TX
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
** 函数名称:  void ADC3_DMA_Configuration()
** 功能描述:  初始化ADC3
** 输入参数:  无
** 输出参数:  无
** 创建:  oueh 2013.06.22
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

	// DMA1只能使用在APB1与MEMORY上，因此用DMA2
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2 | RCC_AHB1Periph_GPIOF, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);

	DMA_InitStructure.DMA_Channel = DMA_Channel_2;//通道非任意
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC3_DR_ADDRESS; //定义外设基址
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&ADC3ConvertedValue; //定义寄存器基址	
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory; //定义DMA传输模式从外设到内存
	DMA_InitStructure.DMA_BufferSize = ADC3_CHANNEL_NUM; //定义buffer大小，根据通道数来定
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //外设地址寄存器不变
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //DMA寄存器随之增加（单通道不增加）
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //DMA外设数据大小，16位半字
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //DMA存储器数据大小，16位半字
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; //模式为循环模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_High; //DMA优先级高
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable; //直接传输模式 
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream0, &DMA_InitStructure);
	DMA_Cmd(DMA2_Stream0, ENABLE); //使能DMA2流0
	
	ADC3_GPIO_Configuration();				//ADC3的IO初始化
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent; //ADC独立模式
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2; //ADC采样周期2分频
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles; //采样周期
	ADC_CommonInit(&ADC_CommonInitStructure); //基本初始化

	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b; //ADC采样精度
	ADC_InitStructure.ADC_ScanConvMode = ENABLE; //采用扫描模式，因为多路
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; //连续扫描模式
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//无外部触发
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //数据右对齐
	ADC_InitStructure.ADC_NbrOfConversion = ADC3_CHANNEL_NUM; //采样通道数
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
** 函数名称:  void LCD_SPI_Configuration()
** 功能描述:  初始化SPI2s
** 输入参数:  无
** 输出参数:  无
** 创建:  oueh 2013.07.12
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
** 函数名称:  void Tim3_Pwm_Configuration(u32 nFre,u8 nDuty)
** 功能描述:  定时器3的PWM输出
** 输入参数:  nFre:频率HZ/10   如125表示频率12.5HZ
					 nDuty:占空比(1~1000) 如:125表示 占空比为12.5%
** 输出参数:  无
** 创建:  oueh 2013.08.02
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
	TIM_TimeBaseStructure.TIM_Period = nPeriod-1;//ARR的值
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
** 函数名称:  void SPI1_Flash_Configuration()
** 功能描述:  初始化SPI1
** 输入参数:  五
** 输出参数:  无
** 创建:  oueh 2013.08.20
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
**  函数名称  : void Bsp_Init()
**  函数功能  : 启动后初始化硬件
**  输    入  : 无
**  输    出  : 无
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
*******************************************************************************/
void Bsp_Init()
{
	SysTick_Config(SystemCoreClock / 1000);	//设置系统时基
	NVIC_Configuration();						//初始化中断控制器
	GPIO_Configuration();						//初始化GPIO
	Uart_Configuration();						//初始化串口
	CAN1_Configuration(CAN_Mode_Normal);		//初始化CAN1
	CAN2_Configuration(CAN_Mode_Normal);		//初始化CAN2
	ADC3_DMA_Configuration();					//初始化ADC3
	Tim3_Pwm_Configuration(10,500);			//初始化PWM1输出
	SPI1_Flash_Configuration();				//初始化SPI1
}

/*******************************************************************************
**  函数名称  : void LCD_Configuration(void)
**  函数功能  : 初始化LCD
**  输    入  : 无
**  输    出  : 无
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
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
**  函数名称  : bool Check_Self_Can(void)
**  函数功能  : 自检2路CAN
**  输    入  : 无
**  输    出  : false:失败 true:成功
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
*******************************************************************************/
bool Check_Self_Can(void)
{
	FIFOQueue *pFifo1 = Get_Can1_Rx_Fifo();
	FIFOQueue *pFifo2 = Get_Can2_Rx_Fifo();
	u8 i = 0;
	CanTxMsg TxMessage;
	ElemType sGet[20] = "";

	//清空缓存
	QueueInit(pFifo1);
	QueueInit(pFifo2);		
	//设置CAN1和CAN2为回环模式
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
	//校验CAN1的发送和接收数据是否一致
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
	//校验CAN2的发送和接收数据是否一致
	if(memcmp(TxMessage.Data,sGet,8) != 0)
	{
		printf("%s:CAN2 Err! %s\r\n",__FUNCTION__,sGet);
		return false;
	}	
	//清空缓存
	QueueInit(pFifo1);
	QueueInit(pFifo2);	
	//设置CAN1和CAN2为正常
	CAN1_Configuration(CAN_Mode_Normal);
	CAN2_Configuration(CAN_Mode_Normal);		
	return true;
}

/*******************************************************************************
**  函数名称  : bool Check_Self_Uart(void)
**  函数功能  : 自检串口2、5、6
**  输    入  : 无
**  输    出  : false:失败 true:成功
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
*******************************************************************************/
bool Check_Self_Uart(void)
{

	//串口5,6自检,短接5 6
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
	//测试完成清除串口缓冲
	//QueueInit(pFifo2);
	printf("Uart Check OK!\r\n");
	QueueInit(pFifo5);
	QueueInit(pFifo6);
	return true;

	
}

/*******************************************************************************
**  函数名称  : bool Check_Self_Volt(void)
**  函数功能  : 自检2路电压输出
**  输    入  : 无
**  输    出  : false:失败 true:成功
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
*******************************************************************************/
bool Check_Self_Volt(void)
{
	u8 Volt1 = 0,Volt2 = 0;

	// 2路电压输出8V，后读取反馈的ADC的值进行校验
	g_HC595State |= 0x000C0000;
	HC595_Renew();	
	//XVO_DISABLE;
	XHV_Volt_Set(8);	
	Delay(1000);//这里需要等待ADC稳定后采。
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
**  函数名称  : bool Check_Self_IO(void)
**  函数功能  : 自检IO
**  输    入  : 无
**  输    出  : false:失败 true:成功
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
*******************************************************************************/
bool Check_Self_IO(void)
{


    u8 i = 0;

	XHV_Volt_Set(8);
#if 1
	for(i=0;i<18;i++)
	{
		g_HC595State |= 1<<i;//高电平输出自测
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
		g_HC595State &= ~(1<<i);//低电平输出自测
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
	//18路低电压反馈测试
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
**  函数名称  : bool Fct_Check_Self(void)
**  函数功能  : FCT自检
**  输    入  : 无
**  输    出  : false:失败 true:成功
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
*******************************************************************************/
bool Fct_Check_Self(void)
{ 
	do
	{
		if(Check_Self_IO() == false)		//自检18路IO输出
		{
			break;
		}
		if(Check_Self_Volt() == false)		//自检2路电压输出
		{
			break;	
		}
		if(Check_Self_Can() == false)		//自检CAN1和CAN2
		{
			break;
		} 
		return true;
	}while(0); 
	//返回FCT自检失败
	Pc_Ack(FCT_ERROR,CHECKSELF_FAIL,1);
	return false;
}


