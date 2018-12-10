/*******************(C)�������������¼����������޹�˾��Ȩ����****************************
**  �ļ�����:   MsgCode.h
**  ��������:   ŷ����
**  ��ǰ�汾:   1.0.0.0
**  ��������:   20130621
**  ����˵��:   ���ڶ���������Ϣ�������ֻ�곣��
**  �޸ļ�¼:        
**  ��    ע:   
**********************************(C)copyright 2013**************************************/


#ifndef __MSG_CODE_H_
#define __MSG_CODE_H_

//LCD��������
#define LCD_ENTER					1
#define LCD_DOWN					2
#define LCD_UP						3
#define LCD_BACK					4
//LCD��ʾģʽ
#define LCD_MENU_MODE				0
#define LCD_SINGLE_TEST_MODE		1
#define LCD_AUTO_TEST_MODE			2
#define LCD_DISPLAY_MODE			3

//FCTЭ��������
#define EXECUTE_FAIL				"1"
#define EXECUTE_TIMEOUT			"2"
#define HARDWARE_ERR				"3"
#define COMMAND_ERR				"4"
#define CHECKSELF_FAIL				"5"
//XHV��ѹ
#define SET_XHV_0V					0
#define SET_XHV_8V					8
#define SET_XHV_10V				10
#define SET_XHV_12V				12
#define SET_XHV_21V				21
#define SET_XHV_24V				24
#define SET_XHV_40V				40
//XBAT��ѹ(��ѹֵ*10)
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


//PC-FCT�궨��
#define FCT_FRAME_HEAD					"EYE+"
#define FCT_FRAME_HEAD_LEN				4
#define FCT_FRAME_TAIL					0x0D
#define FCT_FRAME_TAIL_LEN				1

//PC-FCTͨѶ������

//IOλ



#endif
