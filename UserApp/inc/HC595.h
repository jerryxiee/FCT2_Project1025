#ifndef __HC595_H_
#define __HC595_H_
#include "Include.h"

extern __IO u32 g_HC595State;

//SDI:PD4 			
#define  	HC595_SDI_PORT				GPIOD
#define  	HC595_SDI_PIN				GPIO_Pin_4
#define 	HC595_SDI_GPIO_CLK			RCC_AHB1ENR_GPIODEN
#define 	HC595_SDI_HIGH				do{GPIO_SetBits(HC595_SDI_PORT,HC595_SDI_PIN);}while(0)
#define 	HC595_SDI_LOW				do{GPIO_ResetBits(HC595_SDI_PORT,HC595_SDI_PIN);}while(0)
//SFTCLK_595:PE10
#define  	HC595_SFTCLK_PORT			GPIOE
#define  	HC595_SFTCLK_PIN			GPIO_Pin_10
#define 	HC595_SFTCLK_GPIO_CLK		RCC_AHB1ENR_GPIOEEN
#define 	HC595_SFTCLK_HIGH			do{GPIO_SetBits(HC595_SFTCLK_PORT,HC595_SFTCLK_PIN);}while(0)
#define 	HC595_SFTCLK_LOW			do{GPIO_ResetBits(HC595_SFTCLK_PORT,HC595_SFTCLK_PIN);}while(0)

//LCHCLK_595:PE11
#define  	HC595_LCHCLK_PORT			GPIOE
#define  	HC595_LCHCLK_PIN			GPIO_Pin_11
#define 	HC595_LCHCLK_GPIO_CLK		RCC_AHB1ENR_GPIOEEN
#define 	HC595_LCHCLK_HIGH			do{GPIO_SetBits(HC595_LCHCLK_PORT,HC595_LCHCLK_PIN);}while(0)
#define 	HC595_LCHCLK_LOW			do{GPIO_ResetBits(HC595_LCHCLK_PORT,HC595_LCHCLK_PIN);}while(0)

//OE :PE12
#define  	HC595_OE_PORT				GPIOE
#define  	HC595_OE_PIN				GPIO_Pin_12
#define 	HC595_OE_GPIO_CLK			RCC_AHB1ENR_GPIOEEN
#define 	HC595_OE_HIGH				do{GPIO_SetBits(HC595_OE_PORT,HC595_OE_PIN);}while(0)
#define 	HC595_OE_LOW				do{GPIO_ResetBits(HC595_OE_PORT,HC595_OE_PIN);}while(0)

/*******************************************************************************
**  ��������  : void HC595_GPIO_Configuration()
**  ��������  : ��ʼ��74HC595��GPIO
**  ��    ��  : ��	
**  ��    ��  : ��
**  ȫ�ֱ���  : ��
**  ���ú���  : ��
**  �ж���Դ  : ��
**  ��    ע  :
*******************************************************************************/
void HC595_GPIO_Configuration();

/*******************************************************************************
**  ��������  : void HC595_Shift(u8 nData)
**  ��������  : ��������λ
**  ��    ��  : ��	
**  ��    ��  : ��
**  ȫ�ֱ���  : ��
**  ���ú���  : ��
**  �ж���Դ  : ��
**  ��    ע  :
*******************************************************************************/
void HC595_Shift(u8 nData);


/*******************************************************************************
**  ��������  : void HC595_Out()
**  ��������  : ���������
**  ��    ��  : ��	
**  ��    ��  : ��
**  ȫ�ֱ���  : ��
**  ���ú���  : ��
**  �ж���Դ  : ��
**  ��    ע  :
*******************************************************************************/
void HC595_Out();

/*******************************************************************************
**  ��������  : void HC595_Renew(u32 NewState)
**  ��������  : ����4��595�����
**  ��    ��  : ��
**  ��    ��  : ��
**  ȫ�ֱ���  : ��
**  ���ú���  : ��
**  �ж���Դ  : ��
**  ��    ע  : ��״̬��g_HC595State��ʶ
*******************************************************************************/
void HC595_Renew();


#endif
