/*******************(C)深圳市伊爱高新技术开发有限公司版权所有****************************
 
**  文件名称:   FIFOQueue.c
**  创建作者:   欧恩惠
**  当前版本:   1.0.0.0
**  创建日期:   20130526
**  功能说明:   队列操作
**  修改记录:  
**  备    注:   
**********************************(C)copyright 2010**************************************/
#include "FIFOQueue.h"


FIFOQueue g_Uart1RxFifo;
FIFOQueue g_Uart2RxFifo;
FIFOQueue g_Uart3RxFifo;
FIFOQueue g_Uart4RxFifo;
FIFOQueue g_Uart5RxFifo;
FIFOQueue g_Uart6RxFifo;

FIFOQueue g_Can1RxFifo;
FIFOQueue g_Can2RxFifo;

ElemType g_Uart1DataBuf[UART1_FIFO_SIZE] = {0};
ElemType g_Uart2DataBuf[UART2_FIFO_SIZE] = {0};
ElemType g_Uart3DataBuf[UART3_FIFO_SIZE] = {0};
ElemType g_Uart4DataBuf[UART4_FIFO_SIZE] = {0};
ElemType g_Uart5DataBuf[UART5_FIFO_SIZE] = {0};
ElemType g_Uart6DataBuf[UART6_FIFO_SIZE] = {0};
ElemType g_Can1DataBuf[CAN1_FIFO_SIZE] = {0};
ElemType g_Can2DataBuf[CAN2_FIFO_SIZE] = {0};


__IO u8 g_UartFlagGroup = 0;						//串口标志组


/*******************************************************************************
**  函数名称  : void QueueInit(FIFOQueue *Queue)
**  函数功能  : 初始化队列
**  输    入  : Queue 指向队列
**  输    出  :  无
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
*******************************************************************************/
void QueueInit(FIFOQueue *Queue)
{
	Queue->rear= 0;
	Queue->front = Queue->rear;
}

/*******************************************************************************
**  函数名称  : bool QueueIn(FIFOQueue *Queue,ElemType eDat)
**  函数功能  : 将元素放入队列
**  输    入  : Queue 指向队列
**				eDat:元素
**  输    出  :TRUE:成功 FALSE:失败
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
*******************************************************************************/
bool QueueIn(FIFOQueue *Queue,ElemType eDat)
{
	if((Queue->rear + 1 ) % Queue->size == Queue->front)
	{
		return false;	
	}
	
	Queue->dat[Queue->rear] = eDat;
	Queue->rear = (Queue->rear + 1) % Queue->size;
	return true;
}

/*******************************************************************************
**  函数名称  : bool QueueInStr(FIFOQueue *Queue,ElemType *pStr,u16 nLen)
**  函数功能  : 将元素串放入队列
**  输    入  : Queue 指向队列
**				 pStr:指向元素串
**				 nLen:元素串长度
**  输    出  :TRUE:成功 FALSE:失败
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
*******************************************************************************/
bool QueueInStr(FIFOQueue *Queue,ElemType *pStr,u16 nLen)
{
	while(nLen--)
	{
		if(QueueIn(Queue,*pStr) == false)
			return false;
		pStr++;
	}
	return true;
}

/*******************************************************************************
**  函数名称  : bool QueueOut(FIFOQueue *Queue,ElemType *ch)
**  函数功能  : 从队列中取出元素
**  输    入  : Queue 指向队列
**				ch:指向要取出的元素
**  输    出  :	false:失败 true:成功 
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
*******************************************************************************/
bool QueueOut(FIFOQueue *Queue,ElemType *ch)
{
	if(Queue->front == Queue->rear)
	{
		return false;	
	}
	*ch = Queue->dat[Queue->front];
	Queue->front = (Queue->front + 1) % Queue->size;

	return true;
}

/*******************************************************************************
**  函数名称  : QueueStatus isQueueEmpty(const FIFOQueue *Queue)
**  函数功能  : 判断队列是否为空
**  输    入  : Queue 指向队列
**  输    出  : QUEUE_IS_EMPTY:队列为空
**				QUEUE_NOT_EMPTY:队列非空
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
*******************************************************************************/
QueueStatus isQueueEmpty(const FIFOQueue *Queue)
{
	if(Queue->front == Queue->rear)
	{
		return QUEUE_IS_EMPTY;
	}
	else
	{
		return QUEUE_NOT_EMPTY;
	}
}

/*******************************************************************************
**  函数名称  : u32 QueueLength(const FIFOQueue *Queue)
**  函数功能  : 获取队列长度
**  输    入  : Queue 指向队列
**  输    出  : 队列长度
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
*******************************************************************************/
u32 QueueLength(const FIFOQueue *Queue)
{
	return (Queue->rear - Queue->front + Queue->size) % Queue->size;
}

/*******************************************************************************
**  函数名称  : ElemType *QueueFindChar(const FIFOQueue *Queue,const u8 ch)
**  函数功能  : 在队列中找到指定字符并返回第一个指针
**  输    入  : Queue 指向队列
**				ch:查找的字符
**  输    出  : 指向该字符的指针
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
*******************************************************************************/
ElemType *QueueFindChar(const FIFOQueue *Queue,const u8 ch)
{
	uint32_t qLen,i;
	qLen = QueueLength(Queue);

	for(i = 0; i < qLen;i++)
	{
		if(Queue->dat[(Queue->front + i) % Queue->size] == ch)
		{
			return &Queue->dat[(Queue->front + i) % Queue->size];	
		}
	}

	return NULL;
}

/*******************************************************************************
**  函数名称  : ElemType *QueueFindStr(const FIFOQueue *Queue,const char *subStr)
**  函数功能  : 查询队列中字串的地址
**  输    入  : Queue 指向队列
**				subStr:子串
**  输    出  : 指向队列中第一次出现字串的地址
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
*******************************************************************************/
ElemType *QueueFindStr(const FIFOQueue *Queue,const char *subStr)
{
	uint32_t qLen,i,subLen,j;
	
	subLen = strlen(subStr);
	qLen = QueueLength(Queue);
	for(i=0;i<qLen;i++)
	{
		if(Queue->dat[(Queue->front + i) % Queue->size] == *subStr)
		{
			for(j=1;j<subLen;j++)
			{
				if(Queue->dat[(Queue->front + i + j) % Queue->size] != *(subStr + j))	
				{
					break;				
				}
				else 
				{
					if(j == subLen - 1)
					{
						return &Queue->dat[(Queue->front + i) % Queue->size];
					}
				}
			}
		}
	}
	return NULL;
}

/*******************************************************************************
**  函数名称  : FIFOQueue *Get_Uart1_Rx_Fifo(void)
**  函数功能  : 返回指向串口1读队列的指针
**  输    入  : 无
**  输    出  :  返回指向串口1读队列的指针
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
*******************************************************************************/
FIFOQueue *Get_Uart1_Rx_Fifo(void)
{
	return &g_Uart1RxFifo;
}

/*******************************************************************************
**  函数名称  : FIFOQueue *Get_Uart2_Rx_Fifo(void)
**  函数功能  : 返回指向串口2读队列的指针
**  输    入  : 无
**  输    出  :  返回指向串口2读队列的指针
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
*******************************************************************************/
FIFOQueue *Get_Uart2_Rx_Fifo(void)
{
	return &g_Uart2RxFifo;
}
/*******************************************************************************
**  函数名称  : FIFOQueue *Get_Uart3_Rx_Fifo(void)
**  函数功能  : 返回指向串口3读队列的指针
**  输    入  : 无
**  输    出  :  返回指向串口3读队列的指针
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
*******************************************************************************/
FIFOQueue *Get_Uart3_Rx_Fifo(void)
{
	return &g_Uart3RxFifo;
}

/*******************************************************************************
**  函数名称  : FIFOQueue *Get_Uart4_Rx_Fifo(void)
**  函数功能  : 返回指向串口4读队列的指针
**  输    入  : 无
**  输    出  :  返回指向串口4读队列的指针
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
*******************************************************************************/
FIFOQueue *Get_Uart4_Rx_Fifo(void)
{
	return &g_Uart4RxFifo;
}
/*******************************************************************************
**  函数名称  : FIFOQueue *Get_Uart5_Rx_Fifo(void)
**  函数功能  : 返回指向串口5读队列的指针
**  输    入  : 无
**  输    出  :  返回指向串口5读队列的指针
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
*******************************************************************************/
FIFOQueue *Get_Uart5_Rx_Fifo(void)
{
	return &g_Uart5RxFifo;
}

/*******************************************************************************
**  函数名称  : FIFOQueue *Get_Uart6_Rx_Fifo(void)
**  函数功能  : 返回指向串口6读队列的指针
**  输    入  : 无
**  输    出  :  返回指向串口6读队列的指针
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
*******************************************************************************/
FIFOQueue *Get_Uart6_Rx_Fifo(void)
{
	return &g_Uart6RxFifo;
}

/*******************************************************************************
**  函数名称  : FIFOQueue *Get_Can1_Rx_Fifo(void)
**  函数功能  : 返回指向CAN1读队列的指针
**  输    入  : 无
**  输    出  :  返回指向CAN1读队列的指针
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
*******************************************************************************/
FIFOQueue *Get_Can1_Rx_Fifo(void)
{
	return &g_Can1RxFifo;
}

/*******************************************************************************
**  函数名称  : FIFOQueue *Get_Can2_Rx_Fifo(void)
**  函数功能  : 返回指向CAN2读队列的指针
**  输    入  : 无
**  输    出  :  返回指向CAN2读队列的指针
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
*******************************************************************************/
FIFOQueue *Get_Can2_Rx_Fifo(void)
{
	return &g_Can2RxFifo;
}

/*************************************************************
** 函数名称: void FIFO_Queue_Init(void)
** 功能描述:  所有队列初始化
** 输入参数:  无
** 输出参数:  无
** 创建:  oueh 2013.06.22
** Note:
***************************************************************/
void FIFO_Queue_Init(void)
{
	QueueInit(&g_Uart1RxFifo);
	QueueInit(&g_Uart2RxFifo);
	QueueInit(&g_Uart3RxFifo);
	QueueInit(&g_Uart4RxFifo);
	QueueInit(&g_Uart5RxFifo);
	QueueInit(&g_Uart6RxFifo);
	QueueInit(&g_Can1RxFifo);
	QueueInit(&g_Can2RxFifo);

	g_Uart1RxFifo.dat = &g_Uart1DataBuf[0];
	g_Uart1RxFifo.size = UART1_FIFO_SIZE;
	
	g_Uart2RxFifo.dat = &g_Uart2DataBuf[0];
	g_Uart2RxFifo.size = UART2_FIFO_SIZE;
	
	g_Uart3RxFifo.dat = &g_Uart3DataBuf[0];
	g_Uart3RxFifo.size = UART3_FIFO_SIZE;
	
	g_Uart4RxFifo.dat = &g_Uart4DataBuf[0];
	g_Uart4RxFifo.size = UART4_FIFO_SIZE;
	
	g_Uart5RxFifo.dat = &g_Uart5DataBuf[0];
	g_Uart5RxFifo.size = UART5_FIFO_SIZE;
	
	g_Uart6RxFifo.dat = &g_Uart6DataBuf[0];
	g_Uart6RxFifo.size = UART6_FIFO_SIZE;
	
	g_Can1RxFifo.dat = &g_Can1DataBuf[0];
	g_Can1RxFifo.size = CAN1_FIFO_SIZE;
	
	g_Can2RxFifo.dat = &g_Can2DataBuf[0];
	g_Can2RxFifo.size = CAN2_FIFO_SIZE;

}

