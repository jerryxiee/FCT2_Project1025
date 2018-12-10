
#ifndef __TEST_H_
#define __TEST_H_
#include "Include.h"

typedef struct 	__TEST_INFO_
{
	u32	nUart1RcvCnt;		//统计串口1接收到数据的次数	
	u32	nUart2RcvCnt;		//统计串口2接收到数据的次数	
	u32	nUart3RcvCnt;		//统计串口3接收到数据的次数	
	u32 nUart4RcvCnt;		//统计串口4接收到数据的次数
	u32 nUart5RcvCnt;		//统计串口5接收到数据的次数
	u32 nUart6RcvCnt;		//统计串口6接收到数据的次数
	u32 IoFeedBackState;	//输出反馈0:低电平1:高电平
	u32 IoInputState;		//IN21_IN28的输入0:低电平1:高电平
	u32 nUart4Sum;
	u32 nUart4Sum2;
	u32 nSysTickCnt;
}TEST_INFO;

extern TEST_INFO g_TestInfo;

#endif
