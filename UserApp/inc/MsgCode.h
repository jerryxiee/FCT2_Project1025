/*******************(C)深圳市伊爱高新技术开发有限公司版权所有****************************
**  文件名称:   MsgCode.h
**  创建作者:   欧恩惠
**  当前版本:   1.0.0.0
**  创建日期:   20130621
**  功能说明:   用于定义所有消息的命令字或宏常量
**  修改记录:        
**  备    注:   
**********************************(C)copyright 2013**************************************/


#ifndef __MSG_CODE_H_
#define __MSG_CODE_H_

//LCD按键代号
#define LCD_ENTER					1
#define LCD_DOWN					2
#define LCD_UP						3
#define LCD_BACK					4
//LCD显示模式
#define LCD_MENU_MODE				0
#define LCD_SINGLE_TEST_MODE		1
#define LCD_AUTO_TEST_MODE			2
#define LCD_DISPLAY_MODE			3

//FCT协议错误代号
#define EXECUTE_FAIL				"1"
#define EXECUTE_TIMEOUT			"2"
#define HARDWARE_ERR				"3"
#define COMMAND_ERR				"4"
#define CHECKSELF_FAIL				"5"
//XHV电压
#define SET_XHV_0V					0
#define SET_XHV_8V					8
#define SET_XHV_10V				10
#define SET_XHV_12V				12
#define SET_XHV_21V				21
#define SET_XHV_24V				24
#define SET_XHV_40V				40
//XBAT电压(电压值*10)
#define SET_XBAT_0V					0
#define SET_XBAT_33V					33
#define SET_XBAT_40V					40
#define SET_XBAT_45V					45

#define XHV_Bit_MASK				((u32)0x0FFFFFFF)
#define XHV_Bit_8V					((u32)0x00000000)
#define XHV_Bit_10V				((u32)0x10000000)
#define XHV_Bit_12V				((u32)0x20000000)
#define XHV_Bit_21V				((u32)0x40000000)
#define XHV_Bit_24V				((u32)0x80000000)
#define XHV_Bit_40V				((u32)0xE0000000)


//PC-FCT宏定义
#define FCT_FRAME_HEAD					"EYE+"
#define FCT_FRAME_HEAD_LEN				4
#define FCT_FRAME_TAIL					0x0D
#define FCT_FRAME_TAIL_LEN				1

//PC-FCT通讯命令字

//IO位



#endif
