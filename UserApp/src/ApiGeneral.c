/*******************(C)�������������¼����������޹�˾��Ȩ����****************************
**  �ļ�����:   ApiGeneral.c
**  ��������:   ŷ����
**  ��ǰ�汾:   1.0.0.0
**  ��������:   20130621
**  ����˵��:   ͨ�ú���
**  �޸ļ�¼:   2013.10.25 ¬�����������ط����ܡ�  
				2013.10.26 ¬���������ָ������ȴ���255ʱ�����Bug��
**  ��    ע:   
**********************************(C)copyright 2013**************************************/
#include "ApiGeneral.h"

static __IO u32 TimingDelay;//��ʱ���ӳ�
static u8 s_EyeCmdBuf[UART2_FIFO_SIZE] = "";//���EYEָ��ص�����
static u16 nEyeCmdBufLen = 0;//EYEָ��ص����ݵĳ���
static __IO u32 IoInputState = 0;//IO����״̬��־
static __IO u32 IoFeedBackState = 0;//IO�������״̬��־
GnssCmdInfo g_GnssCmdInfo;

//EYEָ��ص�����
PC_FUNC_PTR Pc_Func_Tbl[PC_FUNC_NUM] = {Eye_Cmd,Eye_Debug,Eye_Sin,Eye_Sout,
	Eye_Vin,Eye_Vout,Eye_Com,Eye_GnssAnte,Eye_Mbcom,Eye_CheckSelf,Eye_Gnss
	,Eye_Power,Eye_Battery,Eye_Count,Eye_Pwm};


/*************************************************************
** ��������:  bool XBAT_Volt_Set(u8 nVolt)
** ��������:  ����ģ���ص�ѹ
** �������:  nVolt:XBAT��ѹֵ*10
** �������:  true:�ɹ� false:ʧ��
** ����:  oueh 2013.7.08
** Note:	
XBAT     XBAT_EN	 XBAT_FB1	 XBAT_FB2
3.3V		1			0			0
4.05V		1			1			0
4.5V		1			0			1
***************************************************************/
bool XBAT_Volt_Set(u8 nVolt)
{
	XBAT_ENABLE;
	switch(nVolt)
	{
		case SET_XBAT_0V:
			XBAT_DISABLE;
			break;
		case SET_XBAT_33V:
			XBAT_FB1_LOW;
			XBAT_FB2_LOW;
			break;
		case SET_XBAT_40V:
			XBAT_FB1_HIGH;
			XBAT_FB2_LOW;
			break;
		case SET_XBAT_45V:
			XBAT_FB1_LOW;
			XBAT_FB2_HIGH;
			break;
		default:
			printf("%s:Err Volt %d\r\n",__FUNCTION__,nVolt);
			return false;
			break;
	}
	return true;
}

/*******************************************************************************
**  ��������  : bool GnssAnt_Set_State(GnssAntStatus Status)
**  ��������  : ��������״̬
**  ��    ��  : 	Status:���õ�����״̬
**  ��    ��  : 	true:�ɹ�	false:ʧ��
**  ȫ�ֱ���  : ��
**  ���ú���  : ��
**  �ж���Դ  : ��
**  ��    ע  :
			ANT1			ANT2
��·		   0				   0	
��·		   X				   1
����		   1				   0
*******************************************************************************/
bool GnssAnt_Set_State(GnssAntStatus Status)
{
	switch(Status)
	{
		//��·
		case OPEN_CIRCUIT:
			GNSS_ANT1_LOW;
			GNSS_ANT2_LOW;
			break;
		//��·
		case SHORT_CIRCUIT:
			GNSS_ANT2_HIGH;
			GNSS_ANT1_HIGH;
			break;
		//����
		case GNSSANT_NORMAL:
			GNSS_ANT1_HIGH;
			GNSS_ANT2_LOW;
			break;
		default:
			printf("%s:Err Status:%d\r\n",__FUNCTION__,Status);
			return false;
			break;
	}

	return true;
}

/*******************************************************************************
**  ��������  : bool EYE_COMMAND(u8 *pCmd,u16 nCmdLen,u32 TimeOut,u8 WaitChar,u8 *pRet,u16 *pRetLen)
**  ��������  : ִ��EYE����
**  ��    ��  : 	pCmd:ָ������
**					nCmdLen:�����
**					TimeOut: ��ʱ�ȴ�ʱ��(����)
**					WartChar:�ȴ����ص��ַ�
**					pRet:ָ�򷵻ص�����
**					nRetLen:���ص����ݵĳ���
**  ��    ��  : ��
**  ȫ�ֱ���  : ��
**  ���ú���  : ��
**  �ж���Դ  : ��
**  ��    ע  :
*******************************************************************************/
bool EYE_COMMAND(u8 *pCmd,u16 nCmdLen,u32 nTimeOut,u8 cWaitChar,u8 *pRet,u16 *pRetLen)
{
	ElemType ch;
	u16 nPos = 0;
	FIFOQueue *pFifo = Get_Uart2_Rx_Fifo();
	
	
	if(strstr((char *)pCmd,"EYE+DEBUG") == NULL)
	{
		QueueInit(pFifo);     //������ڻ���,��������ԭ����Ϊ����������ط�����
	}
	memset(s_EyeCmdBuf,0,sizeof(s_EyeCmdBuf));
	//����ָ������
	Uartx_SendMsg(COM2,pCmd,nCmdLen);
	//�趨��ʱ��ʱ��
	TimingDelay_Set(nTimeOut);
	while(TimingDelay_TimeOut() == false)
	{
		if(QueueFindChar(pFifo,cWaitChar) != NULL)//�ҵ��ȴ����ַ���
		{
	        //�������Ҫ����ֱ�ӷ���
	        if((pRet == NULL) 
	        || (pRetLen == NULL))
	        {
	        	printf("%s:Find ,Not Need Data!\r\n",__FUNCTION__);
	          return true;
	        }		
			
			while(QueueOut(pFifo,&ch) == true)//��ý�������
			{

				pRet[nPos++] = ch;	
			}
			*pRetLen = nPos;
			printf("%s:Find!\r\n",__FUNCTION__);
			return true;
		}
		//ִ�и����ȼ�������
		High_Prio_Task();
	}
	
	if(cWaitChar != 0)//�ȴ�ָ���ַ�����ʱ
	{
		printf("%s:Wait TimeOut!\r\n",__FUNCTION__);
		return false;
	}
	else//����ʱʱ�����յ������ݷ���
	{
		while(QueueOut(pFifo,&ch) == true)//��ý�������
		{
			
			pRet[nPos++] = ch;	
		}
		*pRetLen = nPos;
		printf("%s:TimeOut!\r\n",__FUNCTION__);
		return true;		
	}
}

/*******************************************************************************
**  ��������  : void Eye_Debug(ElemType *pFrame,u16 nLen)
**  ��������  : ����
**  ��    ��  : 	pFrame:ָ��PC�·�������
					len:���ݵĳ���
**  ��    ��  : ��
**  ȫ�ֱ���  : ��
**  ���ú���  : ��
**  �ж���Դ  : ��
**  ��    ע  :
*******************************************************************************/
/*void Eye_Debug(ElemType *pFrame,u16 nLen)
{
	printf("%s: %d\r\n",__FUNCTION__,nLen);

	XHV_Volt_Set(24);
	XVO_ENABLE;
	Delay(1000);
	//����ָ��������ն�
	if(EYE_COMMAND(pFrame,nLen,5*1000,FCT_FRAME_TAIL,s_EyeCmdBuf,&nEyeCmdBufLen) == false)
	{
		Pc_Ack(FCT_ERROR,EXECUTE_TIMEOUT,1);
		return;
	}
	printf("%s:%s %d\r\n",__FUNCTION__,s_EyeCmdBuf,nEyeCmdBufLen);
	Pc_Ack(FCT_MSG_UP,s_EyeCmdBuf,nEyeCmdBufLen);
	return;
}*/


void Eye_Debug(ElemType *pFrame,u16 nLen)
{
	u8 Retry_Times = 0;
	FIFOQueue *pFifo = Get_Uart2_Rx_Fifo();
	printf("%s: %d\r\n",__FUNCTION__,nLen);

	//XVO_DISABLE;
	//Delay(400);
		
	//XVO_ENABLE;
	Delay(1000);
	QueueInit(pFifo);  //������ڻ���
	
	//����ָ��������ն�
	for(Retry_Times = 0; Retry_Times < 5; Retry_Times++)
	{
		if(EYE_COMMAND(pFrame,nLen,1000,FCT_FRAME_TAIL,s_EyeCmdBuf,&nEyeCmdBufLen) == true)
		{
			break;  //���ֳɹ���
		}
		if(Retry_Times == 4)
		{
			Pc_Ack(FCT_ERROR,EXECUTE_TIMEOUT,1);
			return;
		}
	}
	
	printf("%s:%s %d\r\n",__FUNCTION__,s_EyeCmdBuf,nEyeCmdBufLen);
	Pc_Ack(FCT_MSG_UP,s_EyeCmdBuf,nEyeCmdBufLen);
	return;
}



/*******************************************************************************
**  ��������  : void Eye_Sin(ElemType *pFrame,u16 nLen)
**  ��������  : �������������
**  ��    ��  : 	pFrame:ָ��PC�·�������
					len:���ݵĳ���
**  ��    ��  : ��
**  ȫ�ֱ���  : ��
**  ���ú���  : ��
**  �ж���Դ  : ��
**  ��    ע  :
*******************************************************************************/
void Eye_Sin(ElemType *pFrame,u16 nLen)
{
	int nChannel,nTimeOut;
	char nLevel;
	u16 nRetLen = 0;
	printf("%s: %d\r\n",__FUNCTION__,nLen);	
	//��ȡ��š���ƽ����ʱʱ��
	sscanf((char *)&pFrame[8],"%d,%c,%d",&nChannel,&nLevel,&nTimeOut);
	printf("%d %d %d \r\n",nChannel,nLevel,nTimeOut);
	//�������
	if((nChannel > 32 || nChannel < 1)
		|| (nLevel != '+' && nLevel != '-')
		|| (nTimeOut > 127))
	{
		Pc_Ack(FCT_ERROR,COMMAND_ERR,1);
		printf("%s:Para Err!\r\n",__FUNCTION__);
		return;	
	}
	//FCTָ��ͨ�����ָ����ƽ
	if(nLevel == '-')
	{
		g_HC595State &=~(1 << (nChannel - 1));
	}
	else
	{
		g_HC595State |= 1 << (nChannel - 1);
	}
	HC595_Renew();
	XHV_Volt_Set(10);
	Delay(100);
	//����ָ��������ն�
	if(EYE_COMMAND(pFrame,nLen,nTimeOut*1000,FCT_FRAME_TAIL,s_EyeCmdBuf,&nRetLen)==false)
	{
		Pc_Ack(FCT_ERROR,EXECUTE_TIMEOUT,1);
		g_HC595State &=~(1 << (nChannel - 1));
		HC595_Renew();
		return;
	}
	printf("%s:%s\r\n",__FUNCTION__,s_EyeCmdBuf);
	if(strstr((char *)s_EyeCmdBuf,"OK") != NULL)
	{
		if(strchr((char *)s_EyeCmdBuf,nLevel) != NULL)//�����ն���������
		{
			Pc_Ack(FCT_SUCCESS,NULL,0);	
			g_HC595State &=~(1 << (nChannel - 1));
			HC595_Renew();
			XHV_Volt_Set(0);
			return;
		}
		else
		{
			Pc_Ack(FCT_ERROR,EXECUTE_FAIL,1);
			g_HC595State &=~(1 << (nChannel - 1));
			HC595_Renew();
			XHV_Volt_Set(0);
			return;
		}
	}
	else
	{
		Pc_Ack(FCT_MSG_UP,s_EyeCmdBuf,nRetLen);
		g_HC595State &=~(1 << (nChannel - 1));
		HC595_Renew();
		XHV_Volt_Set(0);
		return;
	}
	
	
	
}

/*******************************************************************************
**  ��������  : void Eye_Sout(ElemType *pFrame,u16 nLen)
**  ��������  : �������������
**  ��    ��  : 	pFrame:ָ��PC�·�������
					len:���ݵĳ���
**  ��    ��  : ��
**  ȫ�ֱ���  : ��
**  ���ú���  : ��
**  �ж���Դ  : ��
**  ��    ע  :
*******************************************************************************/
void Eye_Sout(ElemType *pFrame,u16 nLen)
{
	int nChannel,nTimeOut;
	char nLevel;
	u16 nRetLen = 0;

	printf("%s: %d\r\n",__FUNCTION__,nLen);
	//��ȡ��š���ƽ����ʱʱ��
	sscanf((char *)&pFrame[9],"%d,%c,%d",&nChannel,&nLevel,&nTimeOut);
	printf("%d %d %d \r\n",nChannel,nLevel,nTimeOut);
	g_HC595State |= 1 << (nChannel - 1+20);
	XHV_Volt_Set(10);
	HC595_Renew();
	Delay(100);
	//�������
	if((nChannel > 32 || nChannel < 1)
		|| (nLevel != '+' && nLevel != '-')
		|| (nTimeOut > 127))
	{
		Pc_Ack(FCT_ERROR,COMMAND_ERR,1);
		printf("%s:Para Err!\r\n",__FUNCTION__);
		return;	
	}
	//����ָ��������ն�
	if(EYE_COMMAND(pFrame,nLen,nTimeOut*1000,FCT_FRAME_TAIL,s_EyeCmdBuf,&nRetLen)==false)
	{
		Pc_Ack(FCT_ERROR,EXECUTE_TIMEOUT,1);
		g_HC595State &=~(1 << (nChannel - 1+20));
		XHV_Volt_Set(0);
		return;
	}
	printf("%s:%s\r\n",__FUNCTION__,s_EyeCmdBuf);
	Delay(100);
	if(strstr((char *)s_EyeCmdBuf,"OK") != NULL)
	{
		u8 state;
		Get_IoInput_State();//��ȡIO״̬
		state = ((IoInputState >> (nChannel - 1))) & 1;//��ȡָ��IO��ƽ
		if((state + nLevel ) == (0 + '-')
			|| (state + nLevel) == (1 + '+'))//�ж�GNSS�Ƿ������ȷ
		{
				Pc_Ack(FCT_SUCCESS,NULL,0);	
				g_HC595State &=~(1 << (nChannel - 1+20));
				XHV_Volt_Set(0);
				return;				
		}
		else
		{
			Pc_Ack(FCT_ERROR,EXECUTE_FAIL,1);
			g_HC595State &=~(1 << (nChannel - 1+20));
			XHV_Volt_Set(0);
			return;
		}
	}
	else
	{
		Pc_Ack(FCT_MSG_UP,s_EyeCmdBuf,nRetLen);
		g_HC595State &=~(1 << (nChannel - 1+20));
		XHV_Volt_Set(0);
		return;
	}	

}

/*******************************************************************************
**  ��������  : void Eye_Vin(ElemType *pFrame,u16 nLen)
**  ��������  : ģ��������
**  ��    ��  : 	pFrame:ָ��PC�·�������
					len:���ݵĳ���
**  ��    ��  : ��
**  ȫ�ֱ���  : ��
**  ���ú���  : ��
**  �ж���Դ  : ��
**  ��    ע  :
*******************************************************************************/
void Eye_Vin(ElemType *pFrame,u16 nLen)
{
	int nChannel,nVolt,nTimeOut;
	u16 nRetLen = 0;

	printf("%s: %d\r\n",__FUNCTION__,nLen);
	//��ȡ��š���ѹ����ʱʱ��
	sscanf((char *)&pFrame[8],"%d,%d,%d",&nChannel,&nVolt,&nTimeOut);
	printf("%d %d %d \r\n",nChannel,nVolt,nTimeOut);
	//�������
	if((nChannel > 2 || nChannel < 1)
		|| (nVolt >127 && nVolt < 1)
		|| (nTimeOut > 127))
	{
		Pc_Ack(FCT_ERROR,COMMAND_ERR,1);
		printf("%s:Para Err!\r\n",__FUNCTION__);
		return; 
	}
	
	//FCTָ��ͨ�����ָ����ƽ
	g_HC595State |= (1 << (17 + nChannel));
	HC595_Renew();		
	//Ӳ���Ƿ�֧�ָõ�ѹֵ
	if(XHV_Volt_Set(nVolt) == false)
	{
		Pc_Ack(FCT_ERROR,HARDWARE_ERR,1);
		XHV_Volt_Set(0);
		return;
	}
	//����ָ��������ն�
	if(EYE_COMMAND(pFrame,nLen,nTimeOut*1000,FCT_FRAME_TAIL,s_EyeCmdBuf,&nRetLen)==false)
	{
		Pc_Ack(FCT_ERROR,EXECUTE_TIMEOUT,1);
		XHV_Volt_Set(0);
		g_HC595State &= ~(1 << (17 + nChannel));
		HC595_Renew();				
		return;
	}
	printf("%s:%s\r\n",__FUNCTION__,s_EyeCmdBuf);
	XHV_Volt_Set(0);
	if(strstr((char *)s_EyeCmdBuf,"OK") != NULL)//���������Ƿ���ȷ
	{
		int nRetVolt;
		sscanf((char *)&s_EyeCmdBuf[3],"%d",&nRetVolt);
		if(nVolt == nRetVolt)//���������ѹ�Ƿ�һ��
		{
			Pc_Ack(FCT_SUCCESS,NULL,0);	
			g_HC595State &= ~(1 << (17 + nChannel));
			HC595_Renew();			
			return;
		}
		else
		{
			Pc_Ack(FCT_ERROR,EXECUTE_FAIL,1);
			g_HC595State &= ~(1 << (17 + nChannel));
			HC595_Renew();			
			return;
		}
	}
	else
	{
		Pc_Ack(FCT_MSG_UP,s_EyeCmdBuf,nRetLen);
		g_HC595State &= ~(1 << (17 + nChannel));
		HC595_Renew();			
		return;
	}	
}

/*******************************************************************************
**  ��������  : void Eye_Vout(ElemType *pFrame,u16 nLen)
**  ��������  : ģ�������
**  ��    ��  : 	pFrame:ָ��PC�·�������
					len:���ݵĳ���
**  ��    ��  : ��
**  ȫ�ֱ���  : ��
**  ���ú���  : ��
**  �ж���Դ  : ��
**  ��    ע  :
*******************************************************************************/
void Eye_Vout(ElemType *pFrame,u16 nLen)
{
	int nChannel,nVolt,nTimeOut;
	u16 nRetLen = 0;

	printf("%s: %d\r\n",__FUNCTION__,nLen);
	//��ȡ��š���ѹ����ʱʱ��
	sscanf((char *)&pFrame[9],"%d,%d,%d",&nChannel,&nVolt,&nTimeOut);
	printf("%d %d %d \r\n",nChannel,nVolt,nTimeOut);
	//�������
	if((nChannel > 32 || nChannel < 1)
		|| (nVolt >50 && nVolt < 1)
		|| (nTimeOut > 127))
	{
		Pc_Ack(FCT_ERROR,COMMAND_ERR,1);
		printf("%s:Para Err!\r\n",__FUNCTION__);
		return; 
	}
	//����ָ��������ն�
	if(EYE_COMMAND(pFrame,nLen,nTimeOut*1000,FCT_FRAME_TAIL,s_EyeCmdBuf,&nRetLen)==false)
	{
		Pc_Ack(FCT_ERROR,EXECUTE_TIMEOUT,1);
		return;
	}
	//ADC3ConvertedValue
	printf("%s:%s\r\n",__FUNCTION__,s_EyeCmdBuf);
	if(strstr((char *)s_EyeCmdBuf,"OK") != NULL)//���������Ƿ���ȷ
	{
		int nGetVolt;
		
		nGetVolt = (int)((double)ADC3ConvertedValue[1+nChannel] /4096*3.3*9+0.5);
		printf("--GetVolt:V:%d A:%d\r\n",nGetVolt,ADC3ConvertedValue[1+nChannel]);
		if(nVolt == nGetVolt)//���������ѹ�Ƿ�һ��
		{
			Pc_Ack(FCT_SUCCESS,NULL,0);	
			return;
		}
		else
		{
			Pc_Ack(FCT_ERROR,EXECUTE_FAIL,1);
			return;
		}
	}
	else
	{
		Pc_Ack(FCT_MSG_UP,s_EyeCmdBuf,nRetLen);
		return;
	}		
	
}

/*******************************************************************************
**  ��������  : void Eye_Com(ElemType *pFrame,u16 nLen)
**  ��������  : ͨѶ�ӿڲ���
**  ��    ��  : 	pFrame:ָ��PC�·�������
					len:���ݵĳ���
**  ��    ��  : ��
**  ȫ�ֱ���  : ��
**  ���ú���  : ��
**  �ж���Դ  : ��
**  ��    ע  :
*******************************************************************************/
void Eye_Com(ElemType *pFrame,u16 nLen)
{
	int nChannel,nTimeOut;
	char sComMsg[20] = "";
	u16 nRetLen = 0,nComLen = 0;
	FIFOQueue *pFifo = NULL;


	//  2·232 2·485 2·CAN 2·
	//���1:CAN1 2:CAN2 3:232-1 4:232-2 5:485-1 6:485-2
	printf("%s: %d\r\n",__FUNCTION__,nLen);
	//��ȡ�ַ�����ͨ������ʱʱ��
	{
		char *p1=NULL,*p2=NULL,*p3=NULL,*p4=NULL;
		p1 = strchr((char *)pFrame,0x22);
		p2 = strchr((char *)p1+1,0x22);
		p3 = strchr((char *)p2+1,0x2C);
		p4 = strchr((char *)p3+1,0x2C);
		if(p1 == NULL || p2 == NULL || p3 == NULL || p4 == NULL)
		{
			Pc_Ack(FCT_ERROR,COMMAND_ERR,1);
			printf("%s:Para Err!\r\n",__FUNCTION__);				
		}
		memcpy(sComMsg,p1+1,p2-p1-1);
		nComLen = p2-p1-1;
		sscanf(p3+1,"%d",&nChannel);
		sscanf(p4+1,"%d",&nTimeOut);
	}
	printf("%s %d %d \r\n",sComMsg,nChannel,nTimeOut);
	//�������
	if((nChannel > 32 || nChannel < 1)
		|| (nTimeOut > 127))
	{
		Pc_Ack(FCT_ERROR,COMMAND_ERR,1);
		printf("%s:Para Err!\r\n",__FUNCTION__);
		return; 
	}
	//����ָ��������ն�
	if(EYE_COMMAND(pFrame,nLen,nTimeOut*1000,FCT_FRAME_TAIL,s_EyeCmdBuf,&nRetLen)==false)
	{
		Pc_Ack(FCT_ERROR,EXECUTE_TIMEOUT,1);
		return;
	}	
	switch(nChannel)
	{
	//���1:CAN1 2:CAN2 3:232-1 4:232-2 5:485-1 6:485-2
		case 1:
			{	
				u8 i = 0;
				CanTxMsg TxMessage;
				TxMessage.StdId = 0x11;
				TxMessage.RTR = CAN_RTR_DATA;
				TxMessage.IDE = CAN_ID_STD;
				TxMessage.DLC = 8;			
				for(i = 0;i<8;i++)
				{
					TxMessage.Data[i] = sComMsg[i];
				}
				CAN_Transmit(CAN1, &TxMessage);
				pFifo = Get_Can1_Rx_Fifo();
					
			}
			break;
		case 2:			
			{	
				u8 i = 0;
				CanTxMsg TxMessage;
				TxMessage.StdId = 0x11;
				TxMessage.RTR = CAN_RTR_DATA;
				TxMessage.IDE = CAN_ID_STD;
				TxMessage.DLC = 8;			
				for(i = 0;i<8;i++)
				{
					TxMessage.Data[i] = sComMsg[i];
				}
				CAN_Transmit(CAN2, &TxMessage);
				pFifo = Get_Can2_Rx_Fifo();

			}
			break;
		case 3:
			Uartx_SendMsg(COM5,(u8 *)sComMsg,sizeof(sComMsg));
			pFifo = Get_Uart5_Rx_Fifo();
			break;
		case 4:
			Uartx_SendMsg(COM6,(u8 *)sComMsg,sizeof(sComMsg));
			pFifo = Get_Uart6_Rx_Fifo();
			break;
		case 5:
			Uartx_SendMsg(COM1,(u8 *)sComMsg,sizeof(sComMsg));
			pFifo = Get_Uart1_Rx_Fifo();
			break;
		case 6:
			Uartx_SendMsg(COM3,(u8 *)sComMsg,sizeof(sComMsg));
			pFifo = Get_Uart3_Rx_Fifo();
			break;
		default:
			Pc_Ack(FCT_ERROR,COMMAND_ERR,1);
			printf("%s:nChannel %d Err!\r\n",__FUNCTION__,nChannel);			
			break;
	}
	QueueInit(pFifo);//��ն���
	printf("%s:%s\r\n",__FUNCTION__,s_EyeCmdBuf);
	Delay(1000);
	if(strstr((char *)s_EyeCmdBuf,"OK") != NULL)//���������Ƿ���ȷ
	{
		u8 sRetMsg[20] = "",i = 0;
		while(isQueueEmpty(pFifo) == QUEUE_NOT_EMPTY && i<20)
		{
			QueueOut(pFifo,&sRetMsg[i++]);
		}
		if(memcmp(sComMsg,sRetMsg,nComLen) == 0)//��������Ƿ�һ��
		{
			Pc_Ack(FCT_SUCCESS,NULL,0);	
			return;
		}
		else
		{
			Pc_Ack(FCT_ERROR,EXECUTE_FAIL,1);
			return;
		}
	}
	else
	{
		Pc_Ack(FCT_MSG_UP,s_EyeCmdBuf,nRetLen);
		return;
	}		

}

/*******************************************************************************
**  ��������  : void Eye_GnssAnte(ElemType *pFrame,u16 nLen)
**  ��������  : GNSS���߲���
**  ��    ��  : 	pFrame:ָ��PC�·�������
					len:���ݵĳ���
**  ��    ��  : ��
**  ȫ�ֱ���  : ��
**  ���ú���  : ��
**  �ж���Դ  : ��
**  ��    ע  :
*******************************************************************************/
void Eye_GnssAnte(ElemType *pFrame,u16 nLen)
{
	int GnssState,nTimeOut;
	u16 nRetLen = 0;

	printf("%s: %d\r\n",__FUNCTION__,nLen);
	//��ȡ����״̬�ͳ�ʱʱ��
	sscanf((char *)&pFrame[13],"%d,%d",&GnssState,&nTimeOut);
	printf("%d %d \r\n",GnssState,nTimeOut);
	//�������
	if((GnssState > 2 || GnssState < 0)
		|| (nTimeOut > 127))
	{
		Pc_Ack(FCT_ERROR,COMMAND_ERR,1);
		printf("%s:Para Err!\r\n",__FUNCTION__);
		return; 
	}
	//Delay(1000);
	//ģ������״̬
	GnssAnt_Set_State((GnssAntStatus)GnssState);
    Delay(100);
	//����ָ��������ն�
	if(EYE_COMMAND(pFrame,nLen,nTimeOut*1000,FCT_FRAME_TAIL,s_EyeCmdBuf,&nRetLen)==false)
	{
		Pc_Ack(FCT_ERROR,EXECUTE_TIMEOUT,1);
		return;
	}
	printf("%s:%s\r\n",__FUNCTION__,s_EyeCmdBuf);
	if(strstr((char *)s_EyeCmdBuf,"OK") != NULL)//���������Ƿ���ȷ
	{
		int nRetState;
		sscanf((char *)&s_EyeCmdBuf[3],"%d",&nRetState);
		if(GnssState == nRetState)//��������Ƿ�һ��
		{
			Pc_Ack(FCT_SUCCESS,NULL,0); 
			return;
		}
		else
		{
			Pc_Ack(FCT_ERROR,EXECUTE_FAIL,1);
			return;
		}
	}
	else
	{
		Pc_Ack(FCT_MSG_UP,s_EyeCmdBuf,nRetLen);
		return;
	}	

}

/*******************************************************************************
**  ��������  : void Eye_Mbcom(ElemType *pFrame,u16 nLen)
**  ��������  : �ƶ�ͨѶ����
**  ��    ��  : 	pFrame:ָ��PC�·�������
					len:���ݵĳ���
**  ��    ��  : ��
**  ȫ�ֱ���  : ��
**  ���ú���  : ��
**  �ж���Դ  : ��
**  ��    ע  :
*******************************************************************************/
void Eye_Mbcom(ElemType *pFrame,u16 nLen)
{
	int nTimeOut;
    u8 sAt[50] = "";
	u16 nRetLen = 0; 
 
	printf("%s: %d\r\n",__FUNCTION__,nLen);
	//��ȡATָ��ͳ�ʱʱ��
	{
		char *p1=NULL,*p2=NULL,*p3=NULL;	
		p1 = strchr((char *)pFrame,0x22);
		p2 = strchr((char *)p1 + 1,0x22);
		p3 = strchr((char *)p2 + 1,0x2C);
		memcpy(sAt,p1+1,p2-p1-1);
		sscanf(p3+1,"%d",&nTimeOut);
	}
	printf("%s %d\r\n",sAt,nTimeOut);
	//�������
	if((nTimeOut > 127 )
		|| (strlen((char *)sAt) < 2))
	{
		Pc_Ack(FCT_ERROR,COMMAND_ERR,1);
		printf("%s:Para Err!\r\n",__FUNCTION__);
		return; 
	}
	//����ָ��������ն�
	if(EYE_COMMAND(pFrame,nLen,nTimeOut*1000,FCT_FRAME_TAIL,s_EyeCmdBuf,&nRetLen)==false)
	{
		Pc_Ack(FCT_ERROR,EXECUTE_TIMEOUT,1);
		return;
	}
	printf("%s:%s\r\n",__FUNCTION__,s_EyeCmdBuf);
	Pc_Ack(FCT_MSG_UP,s_EyeCmdBuf,nRetLen); 
}

/*******************************************************************************
**  ��������  : void Eye_CheckSelf(ElemType *pFrame,u16 nLen)
**  ��������  : �Լ����
**  ��    ��  : 	pFrame:ָ��PC�·�������
					len:���ݵĳ���
**  ��    ��  : ��
**  ȫ�ֱ���  : ��
**  ���ú���  : ��
**  �ж���Դ  : ��
**  ��    ע  :
*******************************************************************************/
void Eye_CheckSelf(ElemType *pFrame,u16 nLen)
{
	int nTimeOut;
	u16 nRetLen = 0;

	printf("%s: %d\r\n",__FUNCTION__,nLen);
	//��ȡ��ʱʱ��
	sscanf((char *)&pFrame[14],"%d",&nTimeOut);
	printf("%d\r\n",nTimeOut);
	//�������
	if(nTimeOut > 127)
	{
		Pc_Ack(FCT_ERROR,COMMAND_ERR,1);
		printf("%s:Para Err!\r\n",__FUNCTION__);
		return; 
	}
	//����ָ��������ն�
	if(EYE_COMMAND(pFrame,nLen,nTimeOut*1000,FCT_FRAME_TAIL,s_EyeCmdBuf,&nRetLen)==false)
	{
		Pc_Ack(FCT_ERROR,EXECUTE_TIMEOUT,1);
		return;
	}
	printf("%s:%s\r\n",__FUNCTION__,s_EyeCmdBuf);
	Pc_Ack(FCT_MSG_UP,s_EyeCmdBuf,nRetLen);
	return;
}

/*******************************************************************************
**  ��������  : void Eye_Gnss(ElemType *pFrame,u16 nLen)
**  ��������  : GNSS����
**  ��    ��  : 	pFrame:ָ��PC�·�������
					len:���ݵĳ���
**  ��    ��  : ��
**  ȫ�ֱ���  : ��
**  ���ú���  : ��
**  �ж���Դ  : ��
**  ��    ע  :
*******************************************************************************/
void Eye_Gnss(ElemType *pFrame,u16 nLen)
{
	int nTimeOut;
	u16 nRetLen = 0;

	printf("%s: %d\r\n",__FUNCTION__,nLen);
	//��ȡ��ʱʱ��
	sscanf((char *)&pFrame[9],"%d",&nTimeOut);
	printf("%d\r\n",nTimeOut);
	//�������
	if(nTimeOut > 127)
	{
		Pc_Ack(FCT_ERROR,COMMAND_ERR,1);
		printf("%s:Para Err!\r\n",__FUNCTION__);
		return; 
	}
	//����ָ��������ն�
	if(EYE_COMMAND(pFrame,nLen,nTimeOut*1000,FCT_FRAME_TAIL,s_EyeCmdBuf,&nRetLen)==false)
	{
		Pc_Ack(FCT_ERROR,EXECUTE_TIMEOUT,1);
		return;
	}
	printf("%s:%s\r\n",__FUNCTION__,s_EyeCmdBuf);
	if(strstr((char *)s_EyeCmdBuf,"OK") != NULL)//���������Ƿ���ȷ
	{
		Pc_Ack(FCT_SUCCESS,NULL,0); 
	}
	else
	{
		Pc_Ack(FCT_MSG_UP,s_EyeCmdBuf,nRetLen);
		return;
	}	
}

/*******************************************************************************
**  ��������  : void Eye_Power(ElemType *pFrame,u16 nLen)
**  ��������  : ��/��ѹ����
**  ��    ��  : 	pFrame:ָ��PC�·�������
					len:���ݵĳ���
**  ��    ��  : ��
**  ȫ�ֱ���  : ��
**  ���ú���  : ��
**  �ж���Դ  : ��
**  ��    ע  :
*******************************************************************************/
void Eye_Power(ElemType *pFrame,u16 nLen)
{
	int nVolt,nLastTime,nTimeOut;
	u16 nRetLen = 0;
	printf("%s: %d\r\n",__FUNCTION__,nLen);
	//��ȡ��ѹ������ʱ�䡢��ʱʱ��
	sscanf((char *)&pFrame[10],"%d,%d,%d",&nVolt,&nLastTime,&nTimeOut);
	printf("%d %d %d \r\n",nVolt,nLastTime,nTimeOut);
	//�������
	if((nLastTime >127)
		|| (nTimeOut > 127))
	{
		Pc_Ack(FCT_ERROR,COMMAND_ERR,1);
		printf("%s:Para Err!\r\n",__FUNCTION__);
		return; 
	}

	//�ȿ�������
	XBAT_Volt_Set(45);	
	XBATO_ENABLE;	
	//�ر�18·IO��������������ѹֵƫ��
	g_HC595State &= ~((u32)0x000FFFFF);
	HC595_Renew();		
	//Ӳ���Ƿ�֧�ָõ�ѹֵ
	if(XHV_Volt_Set(nVolt) == false)
	{
		Pc_Ack(FCT_ERROR,HARDWARE_ERR,1);
		return;
	}
	Delay(100);
	//XVO_DISABLE;
	Delay(nLastTime*1000);
	//����ָ��������ն�
	if(EYE_COMMAND(pFrame,nLen,nTimeOut*1000,FCT_FRAME_TAIL,s_EyeCmdBuf,&nRetLen)==false)
	{
		Pc_Ack(FCT_ERROR,EXECUTE_TIMEOUT,1);
		XVO_ENABLE;	
		Delay(500);
		XHV_Volt_Set(0);
		XBAT_Volt_Set(0);
		XBATO_DISABLE;
		return;
	}		
	XVO_ENABLE;	
	XHV_Volt_Set(0);
	Delay(500);
	XBAT_Volt_Set(0);
	XBATO_DISABLE;
	Pc_Ack(FCT_MSG_UP,s_EyeCmdBuf,nRetLen);
}

/*******************************************************************************
**  ��������  : void Eye_Battery(ElemType *pFrame,u16 nLen)
**  ��������  : ��س�/�ŵ����
**  ��    ��  : 	pFrame:ָ��PC�·�������
					len:���ݵĳ���
**  ��    ��  : ��
**  ȫ�ֱ���  : ��
**  ���ú���  : ��
**  �ж���Դ  : ��
**  ��    ע  :
*******************************************************************************/
void Eye_Battery(ElemType *pFrame,u16 nLen)
{
	int nTimeOut,nLastTime,bState;
	u16 nRetLen = 0;

	printf("%s: %d\r\n",__FUNCTION__,nLen);
	//��ȡ״̬(1���0�ŵ�)������ʱ�䡢��ʱʱ��
	sscanf((char *)&pFrame[12],"%d,%d,%d",&bState,&nLastTime,&nTimeOut);
	printf("%d %d %d\r\n",bState,nLastTime,nTimeOut);
	//�������
	if((nTimeOut > 127)
		|| (nLastTime >127)
		|| (bState < 0 || bState > 1))
	{
		Pc_Ack(FCT_ERROR,COMMAND_ERR,1);
		printf("%s:Para Err!\r\n",__FUNCTION__);
		return; 
	}
	if(bState == 1)//���
	{
		CHARGE_ENABLE;
		XBATO_DISABLE;
	}
	else
	{	
		//�������õ�ع���
		XBAT_Volt_Set(45);	
		XBATO_ENABLE;
		Delay(100);
		//�ر�����
		//XVO_DISABLE;
		X5V_DISABLE;
		Delay(nLastTime*1000);	
	}
	//����ָ��������ն�
	if(EYE_COMMAND(pFrame,nLen,3*1000,FCT_FRAME_TAIL,s_EyeCmdBuf,&nRetLen)==false)
	{
		Pc_Ack(FCT_ERROR,EXECUTE_TIMEOUT,1);
		if(bState == 1)
		{
			CHARGE_DISABLE;
		}
		else
		{
			XVO_ENABLE;
			X5V_ENABLE;
			Delay(100);
			XBAT_Volt_Set(0);
			XBATO_DISABLE;			
		}
		return;
	}
	
	if(bState == 1)
	{
		Delay(nLastTime*1000);
		CHARGE_DISABLE;
	}
	else
	{
		XVO_ENABLE;
		X5V_ENABLE;
		Delay(100);
		XBAT_Volt_Set(0);
		XBATO_DISABLE;			
	}
	
	printf("%s:%s\r\n",__FUNCTION__,s_EyeCmdBuf);
	Pc_Ack(FCT_MSG_UP,s_EyeCmdBuf,nRetLen);
	return;
}

/*******************************************************************************
**  ��������  : void Eye_Count(ElemType *pFrame,u16 nLen)
**  ��������  : ��������
**  ��    ��  : 	pFrame:ָ��PC�·�������
					len:���ݵĳ���
**  ��    ��  : ��
**  ȫ�ֱ���  : ��
**  ���ú���  : ��
**  �ж���Դ  : ��
**  ��    ע  :
*******************************************************************************/
void Eye_Count(ElemType *pFrame,u16 nLen)
{
	int nTimeOut;
	u16 nRetLen = 0;

	printf("%s: %d\r\n",__FUNCTION__,nLen);
	//��ȡ��ʱʱ��
	sscanf((char *)&pFrame[10],"%d",&nTimeOut);
	printf("%d\r\n",nTimeOut);
	//�������
	if(nTimeOut > 127)
	{
		Pc_Ack(FCT_ERROR,COMMAND_ERR,1);
		printf("%s:Para Err!\r\n",__FUNCTION__);
		Delay(1000);	
		//XVO_DISABLE;
		return; 
	}
	//����ָ��������ն�
	if(EYE_COMMAND(pFrame,nLen,nTimeOut*1000,FCT_FRAME_TAIL,s_EyeCmdBuf,&nRetLen)==false)
	{
		Pc_Ack(FCT_ERROR,EXECUTE_TIMEOUT,1);
		Delay(1000);	
		//XVO_DISABLE;
		return;
	}
	printf("%s:%s\r\n",__FUNCTION__,s_EyeCmdBuf);
	Pc_Ack(FCT_MSG_UP,s_EyeCmdBuf,nRetLen);

	Delay(1000);	
	//XVO_DISABLE;

}

/*******************************************************************************
**  ��������  : void Eye_Pwm(ElemType *pFrame,u16 nLen)
**  ��������  : PWM����
**  ��    ��  : 	pFrame:ָ��PC�·�������
					len:���ݵĳ���
**  ��    ��  : ��
**  ȫ�ֱ���  : ��
**  ���ú���  : ��
**  �ж���Դ  : ��
**  ��    ע  :
*******************************************************************************/
void Eye_Pwm(ElemType *pFrame,u16 nLen)
{
	int nFre,nDuty,nLastTime,nTimeOut;
	u16 nRetLen = 0;

	printf("%s: %d\r\n",__FUNCTION__,nLen);
	//��ȡ��ʱʱ��
	sscanf((char *)&pFrame[8],"%d,%d,%d,%d",&nFre,&nDuty,&nLastTime,&nTimeOut);
	printf("%d %d %d %d\r\n",nFre,nDuty,nLastTime,nTimeOut);
	//�������
	if((nFre <1 || nFre > 1000)
		|| (nDuty < 1 || nDuty > 1000)
		|| (nLastTime < 1 || nLastTime > 1000)
		|| (nTimeOut < 1 || nTimeOut > 1000))
	{
		Pc_Ack(FCT_ERROR,COMMAND_ERR,1);
		printf("%s:Para Err!\r\n",__FUNCTION__);
		return; 
	}

	Tim3_Pwm_Configuration((u32)nFre,(u16)nDuty);
	Delay(nLastTime*1000);
	//����ָ��������ն�
	if(EYE_COMMAND(pFrame,nLen,nTimeOut*1000,FCT_FRAME_TAIL,s_EyeCmdBuf,&nRetLen)==false)
	{
		Pc_Ack(FCT_ERROR,EXECUTE_TIMEOUT,1);
        TIM_Cmd(TIM3, DISABLE);
		return;
	}
	printf("%s:%s\r\n",__FUNCTION__,s_EyeCmdBuf);
	if(strstr((char *)s_EyeCmdBuf,"OK") != NULL)
	{
		int nRetFre,nRetDuty;
		sscanf((char *)&s_EyeCmdBuf[3],"%d,%d",&nRetFre,&nRetDuty);
		if((nRetFre == nFre)&&(nRetDuty == nDuty))//�ж�PWM�����Ƿ���ȷ
		{
				Pc_Ack(FCT_SUCCESS,NULL,0);
				TIM_Cmd(TIM3, DISABLE);
				return;				
		}
		else
		{
			Pc_Ack(FCT_ERROR,EXECUTE_FAIL,1);
			TIM_Cmd(TIM3, DISABLE);
			return;
		}
	}
	else
	{
		Pc_Ack(FCT_MSG_UP,s_EyeCmdBuf,nRetLen);
		TIM_Cmd(TIM3, DISABLE);
		return;
	}		
}

/*******************************************************************************
**  ��������  : void Eye_Cmd(ElemType *pFrame,u16 nLen)
**  ��������  : ����ƽ̨�·��Ĳ���ָ��
**  ��    ��  : 	pFrame:ָ��PC�·�������
					len:���ݵĳ���
**  ��    ��  : ��
**  ȫ�ֱ���  : ��
**  ���ú���  : ��
**  �ж���Դ  : ��
**  ��    ע  :
�洢FLASH�ĸ�ʽ:�ͺ�(16)+��������(1)+�ܰ���(1)+
�ѽ��հ���(1)+�ѷ��Ͱ���(1)+�������(2)+���(#...#)
*******************************************************************************/

void Eye_Cmd(ElemType *pFrame,u16 nLen)
{
	char *pTypeHead = NULL,*pTypeEnd = NULL,*pCmdHead = NULL,*pCmdEnd = NULL;
	char *pPoint = NULL;
	int nTotalItem = 0,nTotalPacket = 0,nCurrentPacket = 0;
	u16 nTypeLen = 0,nCmdLen = 0;
	printf("%s: %d\r\n",__FUNCTION__,nLen);

	//�����е�'.'�滻��'\r'
	pPoint = strchr((char *)pFrame,'.');
	while(pPoint != NULL)
	{
		*pPoint = 0x0D;
		pPoint = strchr(pPoint+1,'.');
	}
	//��ȡ�ͺš��������������ܰ�������ǰ��
	pTypeHead = strchr((char *)pFrame,'\"');
	pTypeEnd = strchr((char *)pTypeHead+1,'\"');
	pCmdHead = strchr((char *)pTypeEnd + 1,'#');
	pCmdEnd = strchr((char *)pCmdHead + 1,'#');
	if(pTypeEnd == NULL || pCmdEnd == NULL
		|| ((pTypeEnd - pTypeHead + 1) > GNSS_TYPE_LEN))
	{
		Pc_Ack(FCT_ERROR,COMMAND_ERR,1);
		return;
	}
	nTypeLen = pTypeEnd - pTypeHead + 1;
	nCmdLen = pCmdEnd - pCmdHead + 1;
	sscanf(pTypeEnd+2,"%d,%d,%d",&nTotalItem,&nTotalPacket,&nCurrentPacket);
	printf("%s:%d,%d,%d\r\n",__FUNCTION__,nTotalItem,nTotalPacket,nCurrentPacket);

	if(nCurrentPacket == 1)//��һ��ʱ���¼�ͺš����������ܰ���
	{
		memset((u8 *)&g_GnssCmdInfo,0,sizeof(GnssCmdInfo));
		memcpy((u8 *)&g_GnssCmdInfo,pTypeHead,nTypeLen);//�ն��ͺ�
		g_GnssCmdInfo.nTotalItem = nTotalItem;//ָ��������
		g_GnssCmdInfo.nTotalPacket = nTotalPacket;//�ܰ���
		g_GnssCmdInfo.nSendPacket = 0;
		g_GnssCmdInfo.nRcvPacket= 0;
		g_GnssCmdInfo.nCmdLen = 0;
		g_GnssCmdInfo.nParamFlag = 0;
	}
	memcpy(&g_GnssCmdInfo.sCmdBuf[g_GnssCmdInfo.nCmdLen],pCmdHead,nCmdLen);
	g_GnssCmdInfo.nCmdLen += nCmdLen;
	g_GnssCmdInfo.nRcvPacket++; 
	//���а���������ϣ������ݴ���FLASH
	if(g_GnssCmdInfo.nRcvPacket == g_GnssCmdInfo.nTotalPacket)
	{
		GnssCmdInfo tempInfo;
		memset((u8 *)&tempInfo,0,sizeof(GnssCmdInfo));
		g_GnssCmdInfo.nParamFlag = PARAM_FLAG;
		sFLASH_EraseSector(GNSS_CMD_FLASH_ADDR);
		sFLASH_WriteBuffer((u8 *)&g_GnssCmdInfo,GNSS_CMD_FLASH_ADDR,sizeof(GnssCmdInfo));
		sFLASH_ReadBuffer((u8 *)&tempInfo,GNSS_CMD_FLASH_ADDR,sizeof(GnssCmdInfo));
		if(memcmp((u8 *)&g_GnssCmdInfo,(u8 *)&tempInfo,sizeof(GnssCmdInfo)) != 0)
		{
			Pc_Ack(FCT_ERROR,EXECUTE_FAIL,1);
			g_GnssCmdInfo.nParamFlag = 0;
			return;				
		}
	}

	Pc_Ack(FCT_SUCCESS,NULL,0);
	return;
}

/*******************************************************************************
**  ��������  : void Get_IoInput_State(void)
**  ��������  : ��ȡ����IO��״̬
**  ��    ��  : ��
**  ��    ��  : ��
**  ȫ�ֱ���  : ��
**  ���ú���  : ��
**  �ж���Դ  : ��
**  ��    ע  :
*******************************************************************************/
void Get_IoInput_State(void)
{	
	u8 nBitIndex = 0;
	//IN21
	if(GPIO_ReadInputDataBit(IO_IN21_PORT,IO_IN21_PIN) == Bit_RESET)
	{
		BIT_CLEAR(IoInputState, nBitIndex);
	}
	else
	{
		BIT_SET(IoInputState, nBitIndex);	
	}
	nBitIndex++;
	//IN22
	if(GPIO_ReadInputDataBit(IO_IN22_PORT,IO_IN22_PIN) == Bit_RESET)
	{
		BIT_CLEAR(IoInputState, nBitIndex);
	}
	else
	{
		BIT_SET(IoInputState, nBitIndex);	
	}
	nBitIndex++;
	//IN23
	if(GPIO_ReadInputDataBit(IO_IN23_PORT,IO_IN23_PIN) == Bit_RESET)
	{
		BIT_CLEAR(IoInputState, nBitIndex);
	}
	else
	{
		BIT_SET(IoInputState, nBitIndex);	
	}
	nBitIndex++;
	//IN24
	if(GPIO_ReadInputDataBit(IO_IN24_PORT,IO_IN24_PIN) == Bit_RESET)
	{
		BIT_CLEAR(IoInputState, nBitIndex);
	}
	else
	{
		BIT_SET(IoInputState, nBitIndex);	
	}
	nBitIndex++;
	//IN25
	if(GPIO_ReadInputDataBit(IO_IN25_PORT,IO_IN25_PIN) == Bit_RESET)
	{
		BIT_CLEAR(IoInputState, nBitIndex);
	}
	else
	{
		BIT_SET(IoInputState, nBitIndex);	
	}
	nBitIndex++;
	//IN26
	if(GPIO_ReadInputDataBit(IO_IN26_PORT,IO_IN26_PIN) == Bit_RESET)
	{
		BIT_CLEAR(IoInputState, nBitIndex);
	}
	else
	{
		BIT_SET(IoInputState, nBitIndex);	
	}
	nBitIndex++;
	//IN27
	if(GPIO_ReadInputDataBit(IO_IN27_PORT,IO_IN27_PIN) == Bit_RESET)
	{
		BIT_CLEAR(IoInputState, nBitIndex);
	}
	else
	{
		BIT_SET(IoInputState, nBitIndex);	
	}
	nBitIndex++;
	//IN28
	if(GPIO_ReadInputDataBit(IO_IN28_PORT,IO_IN28_PIN) == Bit_RESET)
	{
		BIT_CLEAR(IoInputState, nBitIndex);
	}
	else
	{
		BIT_SET(IoInputState, nBitIndex);	
	}
	nBitIndex++;

	printf("IoInputState:0x%08x\r\n",IoInputState);
}

/*******************************************************************************
**  ��������  : void Pc_Ack(FctErrorStatus Status,u8 pMsg,u16 nLen)
**  ��������  : ��PC����Ӧ��
**  ��    ��  : 	Status:�ɹ���ʧ��
					pMsg:��������
					nLen:�������ݳ���
**  ��    ��  : ��
**  ȫ�ֱ���  : ��
**  ���ú���  : ��
**  �ж���Դ  : ��
**  ��    ע  :
*******************************************************************************/
void Pc_Ack(FctErrorStatus Status,u8 *pMsg,u16 nLen)
{
	static u8 sSendMsg[UART4_FIFO_SIZE] = "";
	u16 nPos = 0;

	if(Status == FCT_SUCCESS)
	{
		memcpy(sSendMsg,"OK",2);	
		nPos += 2;
	}
	else if(Status == FCT_ERROR)
	{
		memcpy(sSendMsg,"ERR=",4);	
		nPos += 4;	
	}

	if(strstr((char *)pMsg,"ERR=") != NULL
		|| strstr((char *)sSendMsg,"ERR=") != NULL
		|| strstr((char *)pMsg,"EYE") != NULL)
	{
		Buzzer_Beep(100);
	}
	if(nLen > 0)
	{
		memcpy(&sSendMsg[nPos],pMsg,nLen);
		nPos += nLen;
	}

	if(sSendMsg[nPos-1] != 0x0D)
	{
		sSendMsg[nPos++] = 0x0D;
	}
	//LCD�ѻ�������ʾ
	if(g_LcdMode == LCD_SINGLE_TEST_MODE)
	{
		u16 nLcdLen = 0;
		u8 i = 0;
		
		memset(g_LcdDisplayBuf,0,sizeof(g_LcdDisplayBuf));
		memcpy(g_LcdDisplayBuf,sSendMsg,nPos-1);
		nLcdLen = nPos-1;//ȥ�����һ���ַ�
		//2-4����ʾ����
		for(i = 2;i<5;i++)
		{
			if(nLcdLen <= 0)
			{
				LCD_Display_Ascii(0,i,0,LCD_CLEAR_LINE,LCD_LINE_ASCII_LEN);
				continue;	
			}
			LCD_Display_Ascii(0,i,0,&g_LcdDisplayBuf[i-2][0],16);
			nLcdLen -=16;
			g_LcdDisplayIndex++;
		}
	}
	else if(g_LcdMode == LCD_AUTO_TEST_MODE)
	{
		u8 temp[LCD_LINE_ASCII_LEN+1] = LCD_CLEAR_LINE,tempErr[33] = "",tempIndex;
		char *pCmdHead = (char *)g_GnssCmdInfo.sCmdBuf;
		u16 nItemNum = 0;
		
		nItemNum = g_GnssCmdInfo.nTotalItem;//��ȡ����������
		tempIndex = g_LcdDisplayIndex;
		//�ҵ���ǰ�·���ָ���ȡ�����ַ�����ʾ
		while(tempIndex--)
		{
			pCmdHead += 2;
			pCmdHead = strstr(pCmdHead,"EYE+");
			if(pCmdHead == NULL)
				break;
		}
		if(pCmdHead == NULL)
		{
			memcpy(temp,"CMD ERR!",sizeof("CMD ERR!"));
		}
		else
		{
			char *p1=NULL,*p2=NULL,*p3=NULL,nTypeLen = 0,nParamLen = 0;
			p1 = strchr(pCmdHead,'+');
			p2 = strchr(p1+1,'=');
			p3 = strchr(p2+1,'\r');
			nTypeLen = p2-p1-1;//ָ�����͵ĳ���
			nParamLen = p3-p1-1;//ָ��+�����ĳ���
			//ָ��+��������С��14��ȫ����ʾ������ֵ��ʾ������
			if(nParamLen <= 14)
			{
				memcpy(temp,p1+1,nParamLen);
			}
			else
			{
				memcpy(temp,p1+1,nTypeLen);	
			}
		}
		if(strstr((char *)sSendMsg,"ERR=") != NULL || strstr((char *)sSendMsg,"EYE") != NULL)//ʧ��
		//if(strstr((char *)sSendMsg,"OK") == NULL || strstr((char *)sSendMsg,"EYE") != NULL)//ʧ��
		{
			memcpy(&temp[12],"FAIL",4);
			strcat((char *)g_LcdAutoErrBuf,(char *)temp);
			memset(tempErr,ASCII_SPACE,32);
			if(nPos - 1 > 32)
			{
				memcpy(tempErr,sSendMsg,32);	
			}
			else
			{
				memcpy(tempErr,sSendMsg,nPos - 1);
			}
			strcat((char *)g_LcdAutoErrBuf,(char *)tempErr);
			g_LcdAutoErrIndex++;
		}
		else//�ɹ�
		{
			memcpy(&temp[14],"OK",2);	
		}
		strcat((char *)g_LcdDisplayBuf,(char *)temp);
		g_LcdDisplayIndex++;
		LCD_Show_Auto();
		
		if(g_LcdDisplayIndex >= nItemNum)//���������
		{
			Delay(1000);
			if(g_LcdAutoErrIndex == 0)//���κδ�����
			{
				u8 tempBuf[LCD_LINE_ASCII_LEN] = "Test Count:     ";
				int nCount = 0;

				sscanf((char *)&pMsg[3],"%d",&nCount);
				tempBuf[11] = nCount / 100 + '0';
				tempBuf[12] = (nCount % 100)/10 + '0';
				tempBuf[13] = nCount % 10 + '0';
				LCD_Display_Ascii(0,1,0,LCD_CLEAR_LINE,LCD_LINE_ASCII_LEN);
				LCD_Display_Ascii(0,2,0," Testing  Pass! ",LCD_LINE_ASCII_LEN);
				LCD_Display_Ascii(0,3,0,LCD_CLEAR_LINE,LCD_LINE_ASCII_LEN);
				LCD_Display_Ascii(0,4,0,tempBuf,LCD_LINE_ASCII_LEN);
			}
			else//�д�����
			{
				u8 tempBuf[LCD_LINE_ASCII_LEN] = "Fail Item:   /  ",nDisplayIndex = 0;
				tempBuf[11] = (nDisplayIndex/3 + 1)/10 + '0';
				tempBuf[12] = (nDisplayIndex/3 + 1)%10 + '0';
				tempBuf[14] = g_LcdAutoErrIndex/10 + '0';
				tempBuf[15] = g_LcdAutoErrIndex%10 + '0';
				LCD_Display_Ascii(0,1,0,tempBuf,LCD_LINE_ASCII_LEN);
				LCD_Show_Line(2,4,(u8 *)&g_LcdAutoErrBuf[0][0],strlen((char *)g_LcdAutoErrBuf),nDisplayIndex);			
			}
		}
	}
	
	Uartx_SendMsg(COM4,sSendMsg,nPos);
}

/*******************************************************************************
**  ��������  : void TimingDelay_Decrement(void)
**  ��������  : ���ٶ�ʱ������
**  ��    ��  : ��	
**  ��    ��  : ��
**  ȫ�ֱ���  : ��
**  ���ú���  : ��
**  �ж���Դ  : ��
**  ��    ע  :ÿ��systick����һ�μ���1��ֱ��0Ϊֹ
*******************************************************************************/
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}

/*******************************************************************************
**  ��������  : void TimingDelay_Set(u32 nTime)
**  ��������  : ����systick��ʱ��
**  ��    ��  : ��	
**  ��    ��  : ��
**  ȫ�ֱ���  : ��
**  ���ú���  : ��
**  �ж���Դ  : ��
**  ��    ע  :
*******************************************************************************/
void TimingDelay_Set(u32 nTime)
{
	TimingDelay = nTime;
}

/*******************************************************************************
**  ��������  : bool TimingDelay_TimeOut(void)
**  ��������  : �ж�systick��ʱ���Ƿ�ʱ
**  ��    ��  : ��	
**  ��    ��  : true:�Ѿ���ʱ false:��δ��ʱ
**  ȫ�ֱ���  : ��
**  ���ú���  : ��
**  �ж���Դ  : ��
**  ��    ע  :
*******************************************************************************/
bool TimingDelay_TimeOut(void)
{
	if(TimingDelay == 0)
		return true;
	else
		return false;
}

/*******************************************************************************
**  ��������  : Delay(__IO uint32_t nTime)
**  ��������  : systick��ʱ���ӳ�
**  ��    ��  : nTime:�ӳٵ�ʱ������
**  ��    ��  : ��
**  ȫ�ֱ���  : ��
**  ���ú���  : ��
**  �ж���Դ  : ��
**  ��    ע  :
�ӳٵ�ʱ�䳤����systemʱ�����
��ʱ���ӳ٣�ÿ2��ִ��һ�θ����ȼ�����
*******************************************************************************/
void Delay(__IO uint32_t nTime)
{ 
  TimingDelay = nTime + 1;

   while(TimingDelay != 0)
  {
	
      if((TimingDelay % 2000) == 0)
		High_Prio_Task();
  };
}
/*******************************************************************************
**  ��������  : void Delay_Cnt(u32 cnt)
**  ��������  : �����ķ�ʽ�����ӳ�
**  ��    ��  : cnt:��ָ��ĸ���
**  ��    ��  : ��
**  ȫ�ֱ���  : ��
**  ���ú���  : ��
**  �ж���Դ  : ��
**  ��    ע  :
*******************************************************************************/
void Delay_Cnt(u32 cnt)
{
	while(cnt--);
}

/*************************************************************
** ��������:  void XHV_Volt_Set(u8 nVolt)
** ��������:  ���õ�ѹ
** �������:  nVolt:XHV��ѹֵ
** �������:  true:�ɹ� false:ʧ��
** ����:  oueh 2013.7.08
** Note:
***************************************************************/
bool XHV_Volt_Set(u8 nVolt)
{
	X5V_ENABLE;
	g_HC595State &= XHV_Bit_MASK;
	switch(nVolt)
	{
		case SET_XHV_0V:
			X5V_DISABLE;
			//g_HC595State = 0;
			break;
		case SET_XHV_8V:
			g_HC595State |= XHV_Bit_8V;
			break;
		case SET_XHV_10V:
			g_HC595State |= XHV_Bit_10V;
			break;
		case SET_XHV_12V:
			g_HC595State |= XHV_Bit_12V;
			break;
		case SET_XHV_21V:
			g_HC595State |= XHV_Bit_21V;
			break;
		case SET_XHV_24V:
			g_HC595State |= XHV_Bit_24V;
			break;
		case SET_XHV_40V:
			g_HC595State |= XHV_Bit_40V;
			break;
		default:
			printf("%s:Err Volt %d\r\n",__FUNCTION__,nVolt);
			return false;
			break;
	}
	HC595_Renew();
	return true;
}

/*******************************************************************************
**  ��������  : void Delay_s(u32 Sec)
**  ��������  : �ӳ��롣
**  ��    ��  : ns:�ӳٵ�����	
**  ��    ��  : ��
**  ȫ�ֱ���  : ��
**  ���ú���  : ��
**  �ж���Դ  : ��
**  ��    ע  :�����ڼ���벻������ִ��
*******************************************************************************/
void Delay_s(u32 Sec)
{
	
	while(Sec--)
	{
		Delay_ms(1000);
		//�ڳ�ʱ���ӳ�ʱ����������ȼ�����
		High_Prio_Task();		
	}
}

/*******************************************************************************
**  ��������  : void Delay_ms(u32 nMs)
**  ��������  : �ӳ�N���롣
**  ��    ��  : nMs:�ӳٵĺ�������С��2000
					
**  ��    ��  : ��
**  ȫ�ֱ���  : ��
**  ���ú���  : ��
**  �ж���Դ  : ��
**  ��    ע  :�����ڼ���벻������ִ��
*******************************************************************************/
void Delay_ms(u32 nMs)
{
#if 1
	Delay(nMs);	
#else
	RCC_ClocksTypeDef  rcc_clocks;
	u32 temp;
	u32 nPerMs=0;


	RCC_GetClocksFreq(&rcc_clocks);
	//printf("%d\r\n",rcc_clocks.SYSCLK_Frequency);	
	SysTick->CTRL&=0xfffffffb;					//ѡ���ⲿʱ��
	nPerMs = rcc_clocks.SYSCLK_Frequency/8/1000;//���ÿ����ļ���ֵ			
	SysTick->LOAD=nMs*nPerMs; 	 				//�ӳ�ֵ
	SysTick->VAL =0x00;				 			//��ռ���ֵ
	SysTick->CTRL=0x01 ;						//ʹ��
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));	//�ȴ���ʱʱ��   		
	SysTick->CTRL=0x00;						//�ر�   					
	SysTick->VAL =0X00;	   					//��ռ����� 

#endif
}
/*******************************************************************************
**  ��������  : void Delay_us(u32 nUs)
**  ��������  : �ӳ�N΢�롣
**  ��    ��  : nUs:�ӳٵ�΢������С��2000000
					
**  ��    ��  : ��
**  ȫ�ֱ���  : ��
**  ���ú���  : ��
**  �ж���Դ  : ��
**  ��    ע  :�����ڼ���벻������ִ��
*******************************************************************************/
void Delay_us(u32 nUs)
{
#if 1
	Delay_ms(nUs);
#else
	RCC_ClocksTypeDef  rcc_clocks;
	u32 temp; 
	u32 nPerUs=0;
	
	RCC_GetClocksFreq(&rcc_clocks);
	//printf("%d\r\n",rcc_clocks.SYSCLK_Frequency);	
	SysTick->CTRL&=0xfffffffb;					//ѡ���ⲿʱ��
	nPerUs = rcc_clocks.SYSCLK_Frequency/8/1000000;//���ÿ΢��ļ���ֵ			
	SysTick->LOAD=nUs*nPerUs; 	 				//�ӳ�ֵ
	SysTick->VAL =0x00;				 			//��ռ���ֵ
	SysTick->CTRL=0x01 ;						//ʹ��
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));	//�ȴ���ʱʱ��   		
	SysTick->CTRL=0x00;						//�ر�   					
	SysTick->VAL =0X00;	   					//��ռ����� 	  
#endif
}

/*************************************************************
** ��������:  Print_Debug_Msg
** ��������:  ��ӡ������Ϣ
** �������:  sTitle:��ӡ����
**				sPrintfBuf:��Ϣ
**				nPrintfLen:��Ϣ����
** �������:  ��
** ����:  oueh 2013.05.23
** Note:
***************************************************************/
void Print_Debug_Msg(const u8 *sTitle,const u8 *sPrintfBuf,const u32 nPrintfLen)
{
	u32 i = 0;
	
	printf("%s:**************************:%d\r\n",sTitle,nPrintfLen);
	for(i = 0;i < nPrintfLen;i++)
	{
		printf("%02x ",sPrintfBuf[i]);
	}
	printf("\r\n%s:****************************\r\n",sTitle);
	return;
}

/*******************************************************************************
**  ��������  : void Parse_Fifo4_Msg(ElemType *pFrame,u16 nLen)
**  ��������  : ��������4������֡
**  ��    ��  :	pFrame:ָ�������������֡
					nLen:����֡����
**  ��    ��  : 	��
**  ȫ�ֱ���  : ��
**  ���ú���  : ��
**  �ж���Դ  : ��
**  ��    ע  : 
�ַ���ӳ��ص�����
*******************************************************************************/
void Parse_Fifo4_Msg(ElemType *pFrame,u16 nLen)
{
	u8 i;
	char *StrMapFun[PC_FUNC_NUM] = {"EYE+CMD","EYE+DEBUG","EYE+SIN","EYE+SOUT",
		"EYE+VIN","EYE+VOUT","EYE+COM","EYE+GNSSANTE","EYE+MBCOM",
		"EYE+CHECKSELF","EYE+GNSS","EYE+POWER","EYE+BATTERY",
		"EYE+COUNT","EYE+PWM"};
	
	for(i = 0;i<PC_FUNC_NUM;i++)
	{
		if(strstr((char *)pFrame,StrMapFun[i]) != NULL)
		{
			Pc_Func_Tbl[i](pFrame,nLen);//��ת��Ӧ�Ĵ�����
			return;	
		}
	}
	printf("Parse_Fifo4_Msg:%d\r\n",i);	//�����ʾû��ƥ�䵽
	Pc_Ack(FCT_ERROR,COMMAND_ERR,1);
}

/*******************************************************************************
**  ��������  : void RcvFifo4Msg(void)
**  ��������  : ��ȡЭ��֡����
**  ��    ��  : ��
**  ��    ��  : ��
**  ȫ�ֱ���  : ��
**  ���ú���  : ��
**  �ж���Դ  : ��
**  ��    ע  : 
�ȴӴ��ڻ����л�ȡ����֡ͷ֡β�����ݺ�����������ݡ�

*******************************************************************************/
void RcvFifo4Msg(void)
{
	ElemType *pFrameHead = NULL,*pFrameTail = NULL;
	FIFOQueue *pFifo = Get_Uart4_Rx_Fifo();
	ElemType sFrameBuf[UART4_FIFO_SIZE] = "";
	
	if((g_UartFlagGroup & FLAG_GRP_UART4) == 0)//û��֡��Ҫ����
	{
		return;
	}
	pFrameHead = QueueFindStr(pFifo,FCT_FRAME_HEAD);	//֡ͷ
	if(pFrameHead != NULL)
	{
		u16 nDelLen = 0;
		//ɾ��֡ͷǰ����������
		nDelLen = (pFrameHead - &pFifo->dat[pFifo->front] + pFifo->size) % pFifo->size;
		while(nDelLen--)
		{
			ElemType ch;
			QueueOut(pFifo,&ch);
		}
		pFrameTail = QueueFindChar(pFifo,FCT_FRAME_TAIL);//֡β
		if(pFrameTail != NULL)
		{
			u16 nFrameLen,nPos;
			
			nFrameLen = (pFrameTail - pFrameHead + 1 + pFifo->size) % pFifo->size;
			while(nFrameLen-- > 0)//��FIFO��ȡ��һ֡���ݷ���sFrameBuf
			{
				ElemType ch;
				QueueOut(pFifo,&ch);
				sFrameBuf[nPos++] = ch;
			}			
			Parse_Fifo4_Msg(sFrameBuf,nPos);//������������
			//����֡β����Ϊ������û��֡����Ҫ����
			if(QueueFindChar(pFifo,FCT_FRAME_TAIL) == NULL)
			{
				g_UartFlagGroup &=~FLAG_GRP_UART4;
			}
			return;
		}
	}
	//����֡β����Ϊ������û��֡����Ҫ����
	if(QueueFindChar(pFifo,FCT_FRAME_TAIL) == NULL)
	{
		g_UartFlagGroup &=~FLAG_GRP_UART4;
	}	
}

/*******************************************************************************
**  ��������  : void High_Prio_Task(void)
**  ��������  : ִ�и����ȼ�������
**  ��    ��  : ��
**  ��    ��  : ��
**  ȫ�ֱ���  : ��
**  ���ú���  : ��
**  �ж���Դ  : ��
**  ��    ע  :
1.ι��
*******************************************************************************/
void High_Prio_Task()
{
	IWDG_ReloadCounter();
}

/*************************************************************
** ��������:  CheckSum
** ��������:  ����8λ���У��
** �������:  pMsg:ָ��Ҫ�������Ϣ
**				nLen:��Ϣ�ĳ���
** �������:  8λ���У��
** ����:  oueh 2013.05.23
** Note:
***************************************************************/
u8  CheckSum(u8 *pMsg,u32 nLen)
{
	u8 nChecksum=0;
	u32 i;
    for (i=0;i<nLen;i++)
	{
		nChecksum ^=*pMsg;
		pMsg++;
	}
	return nChecksum;
}

/*************************************************************
** ��������:  TwoASCToHex
** ��������:  ASCIIת������ֵ
** �������:  ASC:ASCII��ֵ
** �������:  ������ֵ
** ����:  oueh 2013.05.23
** Note:ASCII����Ϊ���ֻ�16���Ƶ���ĸ!
***************************************************************/
u8 TwoASCToHex(u8 ASC)
{
	u8	Hex;

	if(IsDigit(ASC))
	{
		Hex = ASC - '0';	
	}
	else if(IsUpper(ASC))
	{
		Hex = ASC - 'A' + 10;	
	}
	else if(IsLower(ASC))
	{
		Hex = ASC - 'a' + 10;	
	}
	else
	{
		//����	
	}

	return Hex;

}

/*************************************************************
** ��������:  HexToTwoASC
** ��������:  HEXת����2��ASCII
** �������:  Hex:��ת����HEX
** �������:  ASCHi���ظ�λ��ASC��ASCLo���ص�λ��ASC
** ����:  oueh 2013.05.23
** Note:
***************************************************************/
void HexToTwoASC(u8 Hex,u8 *ASCHi,u8 *ASCLo)
{
	u8 Hi,Lo;
	Hi = Hex >> 4 &0x0F;
	Lo = Hex &0x0F;
	*ASCHi = Hex2Ascii(Hi);
	*ASCLo = Hex2Ascii(Lo);
}

/*************************************************************
** ��������:  void App_Init()
** ��������:  ��ʼ��Ӧ�ó���
** �������:  ��
** �������:  ��
** ����:  oueh 2013.07.23
** Note:
***************************************************************/
void App_Init()
{
	//LCD������ʼ��
	g_LcdMode = LCD_MENU_MODE;
	memset(g_LcdDisplayBuf,0,sizeof(g_LcdDisplayBuf));
	g_LcdDisplayIndex = 0;	
	g_MenuIndex = 1;			//����������
	//�ر�IO
	g_HC595State &= ~0x0003FFFF;
	HC595_Renew();
	XHV_Volt_Set(0);	
	//��ʼ������ָ�
	memset(&g_GnssCmdInfo,0,sizeof(g_GnssCmdInfo));
	sFLASH_ReadBuffer((u8 *)&g_GnssCmdInfo,GNSS_CMD_FLASH_ADDR,sizeof(g_GnssCmdInfo));
	if(g_GnssCmdInfo.nParamFlag != PARAM_FLAG)
	{
		printf("%s:Get GnssCmdInfo Err\r\n",__FUNCTION__);
		memset(&g_GnssCmdInfo,0,sizeof(g_GnssCmdInfo));		
	}
	LCD_Configuration();	
	LCD_Show_Menu();
}

/*************************************************************
** ��������:  void AutoTest_Packet_Send()
** ��������:  �Զ����Էְ�����
** �������:  ��
** �������:  ��
** ����:  oueh 2013.08.23
** Note:
***************************************************************/
void AutoTest_Packet_Send()
{
	//�в���ָ���δ�����꣬��������
	if(g_GnssCmdInfo.nSendPacket > 0 && g_GnssCmdInfo.nSendPacket < g_GnssCmdInfo.nTotalPacket)
	{
		u8 nPackIndex = 0,nFindCnt = 0;
		char *pCmdHead = NULL,*pCmdEnd = NULL;
		u16 nCmdLen = 0;

		nPackIndex = g_GnssCmdInfo.nSendPacket + 1;
		nFindCnt= (nPackIndex*2 - 1);
		pCmdHead = (char *)g_GnssCmdInfo.sCmdBuf;
		while(nFindCnt--)
		{
			pCmdHead = strchr(pCmdHead,'#');
			pCmdEnd = strchr(pCmdHead + 1,'#');
			if(pCmdEnd == NULL)
			{
				printf("%s:Cmd Format Err\r\n",__FUNCTION__);
				return;
			}
			pCmdHead = pCmdHead + 1;
		}
		nCmdLen = pCmdEnd - pCmdHead;
		QueueInStr(Get_Uart4_Rx_Fifo(),(u8 *)pCmdHead,nCmdLen);
		g_GnssCmdInfo.nSendPacket++;
		g_UartFlagGroup |= FLAG_GRP_UART4;
	}
}

