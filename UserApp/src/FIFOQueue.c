/*******************(C)�������������¼����������޹�˾��Ȩ����****************************
 
**  �ļ�����:   FIFOQueue.c
**  ��������:   ŷ����
**  ��ǰ�汾:   1.0.0.0
**  ��������:   20130526
**  ����˵��:   ���в���
**  �޸ļ�¼:  
**  ��    ע:   
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


__IO u8 g_UartFlagGroup = 0;						//���ڱ�־��


/*******************************************************************************
**  ��������  : void QueueInit(FIFOQueue *Queue)
**  ��������  : ��ʼ������
**  ��    ��  : Queue ָ�����
**  ��    ��  :  ��
**  ȫ�ֱ���  : ��
**  ���ú���  : ��
**  �ж���Դ  : ��
**  ��    ע  :
*******************************************************************************/
void QueueInit(FIFOQueue *Queue)
{
	Queue->rear= 0;
	Queue->front = Queue->rear;
}

/*******************************************************************************
**  ��������  : bool QueueIn(FIFOQueue *Queue,ElemType eDat)
**  ��������  : ��Ԫ�ط������
**  ��    ��  : Queue ָ�����
**				eDat:Ԫ��
**  ��    ��  :TRUE:�ɹ� FALSE:ʧ��
**  ȫ�ֱ���  : ��
**  ���ú���  : ��
**  �ж���Դ  : ��
**  ��    ע  :
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
**  ��������  : bool QueueInStr(FIFOQueue *Queue,ElemType *pStr,u16 nLen)
**  ��������  : ��Ԫ�ش��������
**  ��    ��  : Queue ָ�����
**				 pStr:ָ��Ԫ�ش�
**				 nLen:Ԫ�ش�����
**  ��    ��  :TRUE:�ɹ� FALSE:ʧ��
**  ȫ�ֱ���  : ��
**  ���ú���  : ��
**  �ж���Դ  : ��
**  ��    ע  :
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
**  ��������  : bool QueueOut(FIFOQueue *Queue,ElemType *ch)
**  ��������  : �Ӷ�����ȡ��Ԫ��
**  ��    ��  : Queue ָ�����
**				ch:ָ��Ҫȡ����Ԫ��
**  ��    ��  :	false:ʧ�� true:�ɹ� 
**  ȫ�ֱ���  : ��
**  ���ú���  : ��
**  �ж���Դ  : ��
**  ��    ע  :
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
**  ��������  : QueueStatus isQueueEmpty(const FIFOQueue *Queue)
**  ��������  : �ж϶����Ƿ�Ϊ��
**  ��    ��  : Queue ָ�����
**  ��    ��  : QUEUE_IS_EMPTY:����Ϊ��
**				QUEUE_NOT_EMPTY:���зǿ�
**  ȫ�ֱ���  : ��
**  ���ú���  : ��
**  �ж���Դ  : ��
**  ��    ע  :
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
**  ��������  : u32 QueueLength(const FIFOQueue *Queue)
**  ��������  : ��ȡ���г���
**  ��    ��  : Queue ָ�����
**  ��    ��  : ���г���
**  ȫ�ֱ���  : ��
**  ���ú���  : ��
**  �ж���Դ  : ��
**  ��    ע  :
*******************************************************************************/
u32 QueueLength(const FIFOQueue *Queue)
{
	return (Queue->rear - Queue->front + Queue->size) % Queue->size;
}

/*******************************************************************************
**  ��������  : ElemType *QueueFindChar(const FIFOQueue *Queue,const u8 ch)
**  ��������  : �ڶ������ҵ�ָ���ַ������ص�һ��ָ��
**  ��    ��  : Queue ָ�����
**				ch:���ҵ��ַ�
**  ��    ��  : ָ����ַ���ָ��
**  ȫ�ֱ���  : ��
**  ���ú���  : ��
**  �ж���Դ  : ��
**  ��    ע  :
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
**  ��������  : ElemType *QueueFindStr(const FIFOQueue *Queue,const char *subStr)
**  ��������  : ��ѯ�������ִ��ĵ�ַ
**  ��    ��  : Queue ָ�����
**				subStr:�Ӵ�
**  ��    ��  : ָ������е�һ�γ����ִ��ĵ�ַ
**  ȫ�ֱ���  : ��
**  ���ú���  : ��
**  �ж���Դ  : ��
**  ��    ע  :
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
**  ��������  : FIFOQueue *Get_Uart1_Rx_Fifo(void)
**  ��������  : ����ָ�򴮿�1�����е�ָ��
**  ��    ��  : ��
**  ��    ��  :  ����ָ�򴮿�1�����е�ָ��
**  ȫ�ֱ���  : ��
**  ���ú���  : ��
**  �ж���Դ  : ��
**  ��    ע  :
*******************************************************************************/
FIFOQueue *Get_Uart1_Rx_Fifo(void)
{
	return &g_Uart1RxFifo;
}

/*******************************************************************************
**  ��������  : FIFOQueue *Get_Uart2_Rx_Fifo(void)
**  ��������  : ����ָ�򴮿�2�����е�ָ��
**  ��    ��  : ��
**  ��    ��  :  ����ָ�򴮿�2�����е�ָ��
**  ȫ�ֱ���  : ��
**  ���ú���  : ��
**  �ж���Դ  : ��
**  ��    ע  :
*******************************************************************************/
FIFOQueue *Get_Uart2_Rx_Fifo(void)
{
	return &g_Uart2RxFifo;
}
/*******************************************************************************
**  ��������  : FIFOQueue *Get_Uart3_Rx_Fifo(void)
**  ��������  : ����ָ�򴮿�3�����е�ָ��
**  ��    ��  : ��
**  ��    ��  :  ����ָ�򴮿�3�����е�ָ��
**  ȫ�ֱ���  : ��
**  ���ú���  : ��
**  �ж���Դ  : ��
**  ��    ע  :
*******************************************************************************/
FIFOQueue *Get_Uart3_Rx_Fifo(void)
{
	return &g_Uart3RxFifo;
}

/*******************************************************************************
**  ��������  : FIFOQueue *Get_Uart4_Rx_Fifo(void)
**  ��������  : ����ָ�򴮿�4�����е�ָ��
**  ��    ��  : ��
**  ��    ��  :  ����ָ�򴮿�4�����е�ָ��
**  ȫ�ֱ���  : ��
**  ���ú���  : ��
**  �ж���Դ  : ��
**  ��    ע  :
*******************************************************************************/
FIFOQueue *Get_Uart4_Rx_Fifo(void)
{
	return &g_Uart4RxFifo;
}
/*******************************************************************************
**  ��������  : FIFOQueue *Get_Uart5_Rx_Fifo(void)
**  ��������  : ����ָ�򴮿�5�����е�ָ��
**  ��    ��  : ��
**  ��    ��  :  ����ָ�򴮿�5�����е�ָ��
**  ȫ�ֱ���  : ��
**  ���ú���  : ��
**  �ж���Դ  : ��
**  ��    ע  :
*******************************************************************************/
FIFOQueue *Get_Uart5_Rx_Fifo(void)
{
	return &g_Uart5RxFifo;
}

/*******************************************************************************
**  ��������  : FIFOQueue *Get_Uart6_Rx_Fifo(void)
**  ��������  : ����ָ�򴮿�6�����е�ָ��
**  ��    ��  : ��
**  ��    ��  :  ����ָ�򴮿�6�����е�ָ��
**  ȫ�ֱ���  : ��
**  ���ú���  : ��
**  �ж���Դ  : ��
**  ��    ע  :
*******************************************************************************/
FIFOQueue *Get_Uart6_Rx_Fifo(void)
{
	return &g_Uart6RxFifo;
}

/*******************************************************************************
**  ��������  : FIFOQueue *Get_Can1_Rx_Fifo(void)
**  ��������  : ����ָ��CAN1�����е�ָ��
**  ��    ��  : ��
**  ��    ��  :  ����ָ��CAN1�����е�ָ��
**  ȫ�ֱ���  : ��
**  ���ú���  : ��
**  �ж���Դ  : ��
**  ��    ע  :
*******************************************************************************/
FIFOQueue *Get_Can1_Rx_Fifo(void)
{
	return &g_Can1RxFifo;
}

/*******************************************************************************
**  ��������  : FIFOQueue *Get_Can2_Rx_Fifo(void)
**  ��������  : ����ָ��CAN2�����е�ָ��
**  ��    ��  : ��
**  ��    ��  :  ����ָ��CAN2�����е�ָ��
**  ȫ�ֱ���  : ��
**  ���ú���  : ��
**  �ж���Դ  : ��
**  ��    ע  :
*******************************************************************************/
FIFOQueue *Get_Can2_Rx_Fifo(void)
{
	return &g_Can2RxFifo;
}

/*************************************************************
** ��������: void FIFO_Queue_Init(void)
** ��������:  ���ж��г�ʼ��
** �������:  ��
** �������:  ��
** ����:  oueh 2013.06.22
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

