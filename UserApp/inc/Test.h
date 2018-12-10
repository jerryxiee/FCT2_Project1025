
#ifndef __TEST_H_
#define __TEST_H_
#include "Include.h"

typedef struct 	__TEST_INFO_
{
	u32	nUart1RcvCnt;		//ͳ�ƴ���1���յ����ݵĴ���	
	u32	nUart2RcvCnt;		//ͳ�ƴ���2���յ����ݵĴ���	
	u32	nUart3RcvCnt;		//ͳ�ƴ���3���յ����ݵĴ���	
	u32 nUart4RcvCnt;		//ͳ�ƴ���4���յ����ݵĴ���
	u32 nUart5RcvCnt;		//ͳ�ƴ���5���յ����ݵĴ���
	u32 nUart6RcvCnt;		//ͳ�ƴ���6���յ����ݵĴ���
	u32 IoFeedBackState;	//�������0:�͵�ƽ1:�ߵ�ƽ
	u32 IoInputState;		//IN21_IN28������0:�͵�ƽ1:�ߵ�ƽ
	u32 nUart4Sum;
	u32 nUart4Sum2;
	u32 nSysTickCnt;
}TEST_INFO;

extern TEST_INFO g_TestInfo;

#endif
