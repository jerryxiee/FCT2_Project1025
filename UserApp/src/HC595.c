/*******************(C)�������������¼����������޹�˾��Ȩ����****************************
**  �ļ�����:   74HC595.c
**  ��������:   ŷ����
**  ��ǰ�汾:   1.0.0.0
**  ��������:   20130623
**  ����˵��:   74HC595��������
**  �޸ļ�¼:        
**  ��    ע:   
**********************************(C)copyright 2013**************************************/
#include "HC595.h"
__IO u32 g_HC595State = 0;
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
void HC595_GPIO_Configuration()
{
	GPIO_InitTypeDef  GPIO_InitStructure;	
	
	//SDI
	RCC_AHB1PeriphClockCmd(HC595_SDI_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = HC595_SDI_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(HC595_SDI_PORT, &GPIO_InitStructure);	
	HC595_SDI_HIGH;
	//SFTCLK
	RCC_AHB1PeriphClockCmd(HC595_SFTCLK_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = HC595_SFTCLK_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(HC595_SFTCLK_PORT, &GPIO_InitStructure);
	HC595_SFTCLK_LOW;
	//LCHCLK
	RCC_AHB1PeriphClockCmd(HC595_LCHCLK_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = HC595_LCHCLK_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(HC595_LCHCLK_PORT, &GPIO_InitStructure);	
	HC595_LCHCLK_LOW;
	//OE
	RCC_AHB1PeriphClockCmd(HC595_OE_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = HC595_OE_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(HC595_OE_PORT, &GPIO_InitStructure);	
	HC595_OE_LOW;

}

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
void HC595_Shift(u8 nData)
{
	u8 i;
	for (i = 0; i < 8; i++)  
	{
		if((nData & 0x80) == 0)
		{
			HC595_SDI_LOW;
		}
		else
		{
			HC595_SDI_HIGH;
		}
		HC595_SFTCLK_LOW;			   //�����ط�����λ
		nData = nData << 1;
		HC595_SFTCLK_HIGH;
	} 	   
}

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
void HC595_Out()
{
	HC595_LCHCLK_LOW;
	Delay_us(1);
	HC595_LCHCLK_HIGH;
	Delay_us(1);
	HC595_LCHCLK_LOW;
}

/*******************************************************************************
**  ��������  : void HC595_Renew(u32 NewState)
**  ��������  : ����4��595�����
**  ��    ��  : ��
**  ��    ��  : ��
**  ȫ�ֱ���  : ��
**  ���ú���  : ��
**  �ж���Դ  : ��
**  ��    ע  :
*******************************************************************************/
void HC595_Renew()
{
		signed char i = 3;
			
		for(i = 3; i >= 0;i--)
		{
			HC595_Shift(*((u8 *)&g_HC595State+ i));
			Delay_us(1);			
		}
		HC595_Out();
		Delay(1);		
}


