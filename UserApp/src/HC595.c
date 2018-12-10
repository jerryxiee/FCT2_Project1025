/*******************(C)深圳市伊爱高新技术开发有限公司版权所有****************************
**  文件名称:   74HC595.c
**  创建作者:   欧恩惠
**  当前版本:   1.0.0.0
**  创建日期:   20130623
**  功能说明:   74HC595驱动程序
**  修改记录:        
**  备    注:   
**********************************(C)copyright 2013**************************************/
#include "HC595.h"
__IO u32 g_HC595State = 0;
/*******************************************************************************
**  函数名称  : void HC595_GPIO_Configuration()
**  函数功能  : 初始化74HC595的GPIO
**  输    入  : 无	
**  输    出  : 无
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
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
**  函数名称  : void HC595_Shift(u8 nData)
**  函数功能  : 将数据移位
**  输    入  : 无	
**  输    出  : 无
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
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
		HC595_SFTCLK_LOW;			   //上升沿发生移位
		nData = nData << 1;
		HC595_SFTCLK_HIGH;
	} 	   
}

/*******************************************************************************
**  函数名称  : void HC595_Out()
**  函数功能  : 将数据输出
**  输    入  : 无	
**  输    出  : 无
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
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
**  函数名称  : void HC595_Renew(u32 NewState)
**  函数功能  : 更新4个595的输出
**  输    入  : 无
**  输    出  : 无
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
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


