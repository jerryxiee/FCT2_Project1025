/*******************(C)深圳市伊爱高新技术开发有限公司版权所有****
 
**  文件名称:   Bsp.h
**  创建作者:   欧恩惠
**  当前版本:   1.0.0.0
**  创建日期:   20130622
**  功能说明:   初始化硬件
**  修改记录:   
**  备    注:   
**********************************(C)copyright 2013**************************************/
#ifndef __BSP_H_
#define __BSP_H_
#include "Include.h"

#define ADC3_DR_ADDRESS    ((uint32_t)0x4001224C)
#define ADC3_CHANNEL_NUM		7
extern __IO u16 ADC3ConvertedValue[ADC3_CHANNEL_NUM];
extern GPIO_TypeDef* FeedBackPortTab[18];
extern u16 FeedBackPinTab[18];
#define COMNUM     6
typedef enum 
{
  COM1 = 0,
  COM2,
  COM3,
  COM4,
  COM5,
  COM6
}COM_TypeDef;
//SPI1_WP:PF2
#define SPI1_WP_PORT			GPIOF
#define SPI1_WP_PIN			GPIO_Pin_2
#define SPI1_WP_CLK			RCC_AHB1Periph_GPIOC
#define SPI1_WP_HIGH			do{GPIO_SetBits(SPI1_WP_PORT,SPI1_WP_PIN);}while(0)
#define SPI1_WP_LOW			do{GPIO_ResetBits(SPI1_WP_PORT,SPI1_WP_PIN);}while(0)

//SPI1_NSS2:PC4
#define SPI1_NSS2_PORT			GPIOC
#define SPI1_NSS2_PIN			GPIO_Pin_4
#define SPI1_NSS2_CLK			RCC_AHB1Periph_GPIOC
#define SPI1_NSS2_HIGH			do{GPIO_SetBits(SPI1_NSS2_PORT,SPI1_NSS2_PIN);}while(0)
#define SPI1_NSS2_LOW			do{GPIO_ResetBits(SPI1_NSS2_PORT,SPI1_NSS2_PIN);}while(0)

//SPI1_MOSI:PA7
#define SPI1_MOSI_PORT			GPIOA
#define SPI1_MOSI_PIN			GPIO_Pin_7
#define SPI1_MOSI_CLK			RCC_AHB1Periph_GPIOA
#define SPI1_MOSI_PinSource	GPIO_PinSource7
#define SPI1_MOSI_AF			GPIO_AF_SPI1
//SPI1_MISO:PA6
#define SPI1_MISO_PORT			GPIOA
#define SPI1_MISO_PIN			GPIO_Pin_6
#define SPI1_MISO_CLK			RCC_AHB1Periph_GPIOA
#define SPI1_MISO_PinSource	GPIO_PinSource6
#define SPI1_MISO_AF			GPIO_AF_SPI1
//SPI1_SCK:PA5
#define SPI1_SCK_PORT			GPIOA
#define SPI1_SCK_PIN			GPIO_Pin_5
#define SPI1_SCK_CLK			RCC_AHB1Periph_GPIOA
#define SPI1_SCK_PinSource		GPIO_PinSource5
#define SPI1_SCK_AF			GPIO_AF_SPI1
//PWM1:PB1   TIM3_CH4
#define PWM1_OUT_PORT			GPIOB
#define PWM1_OUT_PIN			GPIO_Pin_1
#define PWM1_OUT_CLK			RCC_AHB1Periph_GPIOB
#define PWM1_OUT_PinSource		GPIO_PinSource1
#define PWM1_OUT_AF			GPIO_AF_TIM3
//CAN1_TX:PD1
#define CAN1_TX_PORT			GPIOD
#define CAN1_TX_PIN			GPIO_Pin_1
#define CAN1_TX_CLK			RCC_AHB1ENR_GPIODEN
#define CAN1_TX_PinSource		GPIO_PinSource1
#define CAN1_TX_AF				GPIO_AF_CAN1
// CAN1_RX:PD0
#define CAN1_RX_PORT			GPIOD
#define CAN1_RX_PIN			GPIO_Pin_0
#define CAN1_RX_CLK			RCC_AHB1ENR_GPIODEN
#define CAN1_RX_PinSource		GPIO_PinSource0
#define CAN1_RX_AF				GPIO_AF_CAN1
//CAN2_TX:PB6
#define CAN2_TX_PORT			GPIOB
#define CAN2_TX_PIN			GPIO_Pin_6
#define CAN2_TX_CLK			RCC_AHB1ENR_GPIOBEN
#define CAN2_TX_PinSource		GPIO_PinSource6
#define CAN2_TX_AF				GPIO_AF_CAN2
// CAN2_RX:PB5
#define CAN2_RX_PORT			GPIOB
#define CAN2_RX_PIN			GPIO_Pin_5
#define CAN2_RX_CLK			RCC_AHB1ENR_GPIOBEN
#define CAN2_RX_PinSource		GPIO_PinSource5
#define CAN2_RX_AF				GPIO_AF_CAN2
//XVO:PE13
#define XVO_PORT				GPIOE
#define XVO_PIN					GPIO_Pin_13
#define XVO_CLK					RCC_AHB1ENR_GPIOEEN
#define XVO_ENABLE				do{GPIO_SetBits(XVO_PORT,XVO_PIN);}while(0)
#define XVO_DISABLE			do{GPIO_ResetBits(XVO_PORT,XVO_PIN);}while(0)
//X5V:PC11
#define X5V_PORT				GPIOC
#define X5V_PIN					GPIO_Pin_11
#define X5V_CLK					RCC_AHB1ENR_GPIOCEN
#define X5V_ENABLE				do{GPIO_SetBits(X5V_PORT,X5V_PIN);}while(0)
#define X5V_DISABLE			do{GPIO_ResetBits(X5V_PORT,X5V_PIN);}while(0)
//CH_EN:PA15
#define CHARGE_PORT			GPIOA
#define CHARGE_PIN				GPIO_Pin_15
#define CHARGE_CLK				RCC_AHB1ENR_GPIOAEN
#define CHARGE_ENABLE			do{GPIO_SetBits(CHARGE_PORT,CHARGE_PIN);}while(0)
#define CHARGE_DISABLE			do{GPIO_ResetBits(CHARGE_PORT,CHARGE_PIN);}while(0)
//XBATO_EN:PC10
#define XBATO_PORT				GPIOC
#define XBATO_PIN				GPIO_Pin_10
#define XBATO_CLK				RCC_AHB1ENR_GPIOCEN
#define XBATO_ENABLE			do{GPIO_SetBits(XBATO_PORT,XBATO_PIN);}while(0)
#define XBATO_DISABLE			do{GPIO_ResetBits(XBATO_PORT,XBATO_PIN);}while(0)
//XBAT:PG8
#define XBAT_PORT				GPIOG
#define XBAT_PIN				GPIO_Pin_8
#define XBAT_CLK				RCC_AHB1ENR_GPIOGEN
#define XBAT_ENABLE			do{GPIO_SetBits(XBAT_PORT,XBAT_PIN);}while(0)
#define XBAT_DISABLE			do{GPIO_ResetBits(XBAT_PORT,XBAT_PIN);}while(0)
//XBAT_FB1:PG7
#define XBAT_FB1_PORT			GPIOG
#define XBAT_FB1_PIN			GPIO_Pin_7
#define XBAT_FB1_CLK			RCC_AHB1ENR_GPIOGEN
#define XBAT_FB1_HIGH			do{GPIO_SetBits(XBAT_FB1_PORT,XBAT_FB1_PIN);}while(0)
#define XBAT_FB1_LOW			do{GPIO_ResetBits(XBAT_FB1_PORT,XBAT_FB1_PIN);}while(0)
//XBAT_FB2:PG6
#define XBAT_FB2_PORT			GPIOG
#define XBAT_FB2_PIN			GPIO_Pin_6
#define XBAT_FB2_CLK			RCC_AHB1ENR_GPIOGEN
#define XBAT_FB2_HIGH			do{GPIO_SetBits(XBAT_FB2_PORT,XBAT_FB2_PIN);}while(0)
#define XBAT_FB2_LOW			do{GPIO_ResetBits(XBAT_FB2_PORT,XBAT_FB2_PIN);}while(0)
//BZ_OUT:PB7
#define BZ_OUT_PORT			GPIOB
#define BZ_OUT_PIN				GPIO_Pin_7
#define BZ_OUT_CLK				RCC_AHB1ENR_GPIOBEN
#define BZ_OUT_HIGH			do{GPIO_SetBits(BZ_OUT_PORT,BZ_OUT_PIN);}while(0)
#define BZ_OUT_LOW				do{GPIO_ResetBits(BZ_OUT_PORT,BZ_OUT_PIN);}while(0)
//LED_CAN:PE1
#define LED_CAN_PORT			GPIOE
#define LED_CAN_PIN			GPIO_Pin_14
#define LED_CAN_CLK			RCC_AHB1ENR_GPIOEEN
#define LED_CAN_HIGH			do{GPIO_SetBits(LED_CAN_PORT,LED_CAN_PIN);}while(0)
#define LED_CAN_LOW			do{GPIO_ResetBits(LED_CAN_PORT,LED_CAN_PIN);}while(0)
//GNSS天线 1:PE9 2:PE8
#define GNSS_ANT1_PORT			GPIOE
#define GNSS_ANT1_PIN			GPIO_Pin_9
#define GNSS_ANT1_CLK			RCC_AHB1ENR_GPIOEEN
#define GNSS_ANT1_HIGH			do{GPIO_SetBits(GNSS_ANT1_PORT,GNSS_ANT1_PIN);}while(0)
#define GNSS_ANT1_LOW			do{GPIO_ResetBits(GNSS_ANT1_PORT,GNSS_ANT1_PIN);}while(0)
#define GNSS_ANT2_PORT			GPIOE
#define GNSS_ANT2_PIN			GPIO_Pin_8
#define GNSS_ANT2_CLK			RCC_AHB1ENR_GPIOEEN
#define GNSS_ANT2_HIGH			do{GPIO_SetBits(GNSS_ANT2_PORT,GNSS_ANT2_PIN);}while(0)
#define GNSS_ANT2_LOW			do{GPIO_ResetBits(GNSS_ANT2_PORT,GNSS_ANT2_PIN);}while(0)
//IO输入口
#define IO_IN1_PORT			GPIOG
#define IO_IN1_PIN				GPIO_Pin_5
#define IO_IN2_PORT			GPIOG
#define IO_IN2_PIN				GPIO_Pin_12
#define IO_IN3_PORT			GPIOD
#define IO_IN3_PIN				GPIO_Pin_14
#define IO_IN4_PORT			GPIOC
#define IO_IN4_PIN				GPIO_Pin_13
#define IO_IN5_PORT			GPIOD
#define IO_IN5_PIN				GPIO_Pin_7
#define IO_IN6_PORT			GPIOG
#define IO_IN6_PIN				GPIO_Pin_3
#define IO_IN7_PORT			GPIOG
#define IO_IN7_PIN				GPIO_Pin_15
#define IO_IN8_PORT			GPIOG
#define IO_IN8_PIN				GPIO_Pin_11
#define IO_IN9_PORT			GPIOA
#define IO_IN9_PIN				GPIO_Pin_8
#define IO_IN10_PORT			GPIOG
#define IO_IN10_PIN			GPIO_Pin_2
#define IO_IN11_PORT			GPIOD
#define IO_IN11_PIN			GPIO_Pin_13
#define IO_IN12_PORT			GPIOB
#define IO_IN12_PIN			GPIO_Pin_3
#define IO_IN13_PORT			GPIOD
#define IO_IN13_PIN			GPIO_Pin_6
#define IO_IN14_PORT			GPIOG
#define IO_IN14_PIN			GPIO_Pin_4
#define IO_IN15_PORT			GPIOG
#define IO_IN15_PIN			GPIO_Pin_13
#define IO_IN16_PORT			GPIOD
#define IO_IN16_PIN			GPIO_Pin_15
#define IO_IN17_PORT			GPIOC
#define IO_IN17_PIN			GPIO_Pin_8
#define IO_IN18_PORT			GPIOG
#define IO_IN18_PIN			GPIO_Pin_10
#define IO_IN21_PORT			GPIOF
#define IO_IN21_PIN			GPIO_Pin_11
#define IO_IN22_PORT			GPIOF
#define IO_IN22_PIN			GPIO_Pin_12
#define IO_IN23_PORT			GPIOG
#define IO_IN23_PIN			GPIO_Pin_0
#define IO_IN24_PORT			GPIOF
#define IO_IN24_PIN			GPIO_Pin_15
#define IO_IN25_PORT			GPIOE
#define IO_IN25_PIN			GPIO_Pin_7
#define IO_IN26_PORT			GPIOG
#define IO_IN26_PIN			GPIO_Pin_1
#define IO_IN27_PORT			GPIOF
#define IO_IN27_PIN			GPIO_Pin_13
#define IO_IN28_PORT			GPIOF
#define IO_IN28_PIN			GPIO_Pin_14
//4个级联的595输出
#define HC595_OUT_C1			((uint32_t) 0x00000001)							
#define HC595_OUT_C2			((uint32_t) 0x00000002)	
#define HC595_OUT_C3			((uint32_t) 0x00000004)							
#define HC595_OUT_C4			((uint32_t) 0x00000008)
#define HC595_OUT_C5			((uint32_t) 0x00000010)							
#define HC595_OUT_C6			((uint32_t) 0x00000020)
#define HC595_OUT_C7			((uint32_t) 0x00000040)							
#define HC595_OUT_C8			((uint32_t) 0x00000080)
#define HC595_OUT_C9			((uint32_t) 0x00000100)							
#define HC595_OUT_C10			((uint32_t) 0x00000200)	
#define HC595_OUT_C11			((uint32_t) 0x00000400)							
#define HC595_OUT_C12			((uint32_t) 0x00000800)
#define HC595_OUT_C13			((uint32_t) 0x00001000)							
#define HC595_OUT_C14			((uint32_t) 0x00002000)
#define HC595_OUT_C15			((uint32_t) 0x00004000)							
#define HC595_OUT_C16			((uint32_t) 0x00008000)
#define HC595_OUT_C17			((uint32_t) 0x00010000)							
#define HC595_OUT_C18			((uint32_t) 0x00020000)	
#define HC595_OUT_C19			((uint32_t) 0x00040000)							
#define HC595_OUT_C20			((uint32_t) 0x00080000)
#define HC595_OUT_C21			((uint32_t) 0x00100000)							
#define HC595_OUT_C22			((uint32_t) 0x00200000)
#define HC595_OUT_C23			((uint32_t) 0x00400000)							
#define HC595_OUT_C24			((uint32_t) 0x00800000)
#define HC595_OUT_C25			((uint32_t) 0x01000000)							
#define HC595_OUT_C26			((uint32_t) 0x02000000)	
#define HC595_OUT_C27			((uint32_t) 0x04000000)							
#define HC595_OUT_C28			((uint32_t) 0x08000000)
#define HC595_XHV_FB1			((uint32_t) 0x10000000)							
#define HC595_XHV_FB2			((uint32_t) 0x20000000)
#define HC595_XHV_FB3			((uint32_t) 0x40000000)							
#define HC595_XHV_FB4			((uint32_t) 0x80000000)
//2路ADC反馈和4路ADC输入
#define ADC_IN1_PORT			GPIOF
#define ADC_IN1_PIN			GPIO_Pin_3
#define ADC_IN2_PORT			GPIOF
#define ADC_IN2_PIN			GPIO_Pin_4
#define ADC_IN3_PORT			GPIOF
#define ADC_IN3_PIN			GPIO_Pin_9
#define ADC_IN4_PORT			GPIOF
#define ADC_IN4_PIN			GPIO_Pin_7
#define ADC_IN5_PORT			GPIOF
#define ADC_IN5_PIN			GPIO_Pin_10
#define ADC_IN6_PORT			GPIOF
#define ADC_IN6_PIN			GPIO_Pin_8
#define ADC_IN_CLK				RCC_AHB1ENR_GPIOFEN

#define ADC_KEY_PORT			GPIOC
#define ADC_KEY_PIN			GPIO_Pin_0
#define ADC_KEY_CLK			RCC_AHB1ENR_GPIOCEN
//485R/D  USART3   PB12
#define UART3_RW_PIN			GPIO_Pin_12
#define UART3_RW_GPIO_PORT		GPIOB
#define UART3_RW_GPIO_CLK		RCC_AHB1Periph_GPIOB
#define Uart3_485_Read()		do													\
								{													\
									Delay_ms(1);									\
									GPIO_ResetBits(UART3_RW_GPIO_PORT,UART3_RW_PIN);\
								}while(0)
								
#define Uart3_485_Write()		do													\
								{													\
									GPIO_SetBits(UART3_RW_GPIO_PORT,UART3_RW_PIN);	\
									Delay_ms(1);									\
								}while(0)
//485R/D2  USART1   PC9
#define UART1_RW_PIN			GPIO_Pin_9
#define UART1_RW_GPIO_PORT		GPIOC
#define UART1_RW_GPIO_CLK		RCC_AHB1Periph_GPIOC
#define Uart1_485_Read()		do													\
								{													\
									Delay_ms(1);									\
									GPIO_ResetBits(UART1_RW_GPIO_PORT,UART1_RW_PIN);\
								}while(0)
									
#define Uart1_485_Write()		do													\
								{													\
									GPIO_SetBits(UART1_RW_GPIO_PORT,UART1_RW_PIN);	\
									Delay_ms(1);									\
								}while(0)

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
void Uartx_SendMsg(COM_TypeDef COMn,u8 *pSendMsg,u32 nLen);

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
void NVIC_Configuration();

/*************************************************************
** 函数名称:  void GPIO_Configuration(void)
** 功能描述:  初始化IO
** 输入参数:  无 	
** 输出参数:  无
** 创建:  oueh 2013.05.23
** Note: 1.打开时钟
**		2.配置IO
**		3.置初始状态
***************************************************************/
void GPIO_Configuration(void);

/*******************************************************************************
**  函数名称  : Uart4_Init
**  函数功能  : 初始化串口4
**  输    入  : nBaud 设置波特率
**  输    出  : 无
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
*******************************************************************************/
void Uart4_Init(u32 nBaud);

/*******************************************************************************
**  函数名称  : void Uart2_Fifo_Queue_Init(void)
**  函数功能  : 初始化串口2读队列
**  输    入  : 无
**  输    出  :  无
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
*******************************************************************************/
void Uart2_Fifo_Queue_Init(void);

/*************************************************************
** 函数名称:  void Uart_Configuration()
** 功能描述:  初始化串口
** 输入参数:  无
** 输出参数:  无
** 创建:  oueh 2013.06.22
** Note:
***************************************************************/
void Uart_Configuration(void);


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
void Bsp_Init();

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
bool Fct_Check_Self(void);

/*************************************************************
** 函数名称:  void CAN1_Configuration(u8 Can_Mode)
** 功能描述:  初始化CAN1
** 输入参数:  无
** 输出参数:  无
** 创建:  oueh 2013.06.22
** Note:
***************************************************************/
void CAN1_Configuration(u8 Can_Mode);

/*************************************************************
** 函数名称:  void CAN2_Configuration(u8 Can_Mode)
** 功能描述:  初始化CAN2
** 输入参数:  无
** 输出参数:  无
** 创建:  oueh 2013.06.22
** Note:
***************************************************************/
void CAN2_Configuration(u8 Can_Mode);

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
void Buzzer_Beep(u32 nBeepTime);

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
void LCD_Configuration(void);

/*************************************************************
** 函数名称:  void Tim3_Pwm_Configuration(u32 nFre,u8 nDuty)
** 功能描述:  定时器3的PWM输出
** 输入参数:  nFre:频率HZ/10   如125表示频率12.5HZ
					 nDuty:占空比(1~1000) 如:125表示 占空比为12.5%
** 输出参数:  无
** 创建:  oueh 2013.08.02
** Note:
***************************************************************/
void Tim3_Pwm_Configuration(u32 nFre,u16 nDuty);

#endif
