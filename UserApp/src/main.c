/*******************(C)深圳市伊爱高新技术开发有限公司版权所有****
 
**  文件名称:   main.c
**  创建作者:   欧恩惠
**  当前版本:   1.0.0.0
**  创建日期:   20130622
**  功能说明:   主函数
**  修改记录:   
**  备    注:   
1.硬件初始化
2.FCT自检
3.对PC下发的指令进行处理

历史版本：
V1.0.0.1，2013.10.25，卢洪棚:添加握手重发功能。
V1.0.0.2，2013.10.26，卢洪棚:修正接收指令包错误的Bug。
V1.0.0.3，2013.10.28，卢洪棚:修正当AT测试指令执行正常但AT指令返回数据中含有ERR时，
                             自动测试报误报成测试失败的问题。

**********************************(C)copyright 2013**************************************/
#include "Include.h"


int main()
{
	Bsp_Init();									//硬件初始化
	App_Init();									//应用程序初始化
	XVO_DISABLE;
	Delay(400);
	XVO_ENABLE;
	while(1)
	{
		IWDG_ReloadCounter();					//喂狗
		RcvFifo4Msg();							//接收PC下发的数据
		Delay_ms(200);						    //循环延迟
		LCD_Key_Hanle();						//处理LCD的按键
		AutoTest_Packet_Send();				    //自动测试包发送
	}
}
