/*******************(C)深圳市伊爱高新技术开发有限公司版权所有****************************
**  文件名称:   ApiGeneral.c
**  创建作者:   欧恩惠
**  当前版本:   1.0.0.0
**  创建日期:   20130621
**  功能说明:   通用函数
**  修改记录:   2013.10.25 卢洪棚：添加握手重发功能。  
				2013.10.26 卢洪棚：修正当指令包长度大于255时出错的Bug。
**  备    注:   
**********************************(C)copyright 2013**************************************/
#include "ApiGeneral.h"

static __IO u32 TimingDelay;//定时器延迟
static u8 s_EyeCmdBuf[UART2_FIFO_SIZE] = "";//存放EYE指令返回的数据
static u16 nEyeCmdBufLen = 0;//EYE指令返回的数据的长度
static __IO u32 IoInputState = 0;//IO输入状态标志
static __IO u32 IoFeedBackState = 0;//IO输出反馈状态标志
GnssCmdInfo g_GnssCmdInfo;

//EYE指令回调函数
PC_FUNC_PTR Pc_Func_Tbl[PC_FUNC_NUM] = {Eye_Cmd,Eye_Debug,Eye_Sin,Eye_Sout,
	Eye_Vin,Eye_Vout,Eye_Com,Eye_GnssAnte,Eye_Mbcom,Eye_CheckSelf,Eye_Gnss
	,Eye_Power,Eye_Battery,Eye_Count,Eye_Pwm};


/*************************************************************
** 函数名称:  bool XBAT_Volt_Set(u8 nVolt)
** 功能描述:  设置模拟电池电压
** 输入参数:  nVolt:XBAT电压值*10
** 输出参数:  true:成功 false:失败
** 创建:  oueh 2013.7.08
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
**  函数名称  : bool GnssAnt_Set_State(GnssAntStatus Status)
**  函数功能  : 设置天线状态
**  输    入  : 	Status:设置的天线状态
**  输    出  : 	true:成功	false:失败
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
			ANT1			ANT2
开路		   0				   0	
短路		   X				   1
正常		   1				   0
*******************************************************************************/
bool GnssAnt_Set_State(GnssAntStatus Status)
{
	switch(Status)
	{
		//开路
		case OPEN_CIRCUIT:
			GNSS_ANT1_LOW;
			GNSS_ANT2_LOW;
			break;
		//短路
		case SHORT_CIRCUIT:
			GNSS_ANT2_HIGH;
			GNSS_ANT1_HIGH;
			break;
		//正常
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
**  函数名称  : bool EYE_COMMAND(u8 *pCmd,u16 nCmdLen,u32 TimeOut,u8 WaitChar,u8 *pRet,u16 *pRetLen)
**  函数功能  : 执行EYE命令
**  输    入  : 	pCmd:指向命令
**					nCmdLen:命令长度
**					TimeOut: 超时等待时间(毫秒)
**					WartChar:等待返回的字符
**					pRet:指向返回的数据
**					nRetLen:返回的数据的长度
**  输    出  : 无
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
*******************************************************************************/
bool EYE_COMMAND(u8 *pCmd,u16 nCmdLen,u32 nTimeOut,u8 cWaitChar,u8 *pRet,u16 *pRetLen)
{
	ElemType ch;
	u16 nPos = 0;
	FIFOQueue *pFifo = Get_Uart2_Rx_Fifo();
	
	
	if(strstr((char *)pCmd,"EYE+DEBUG") == NULL)
	{
		QueueInit(pFifo);     //清除串口缓存,这样做的原因是为了配合握手重发机制
	}
	memset(s_EyeCmdBuf,0,sizeof(s_EyeCmdBuf));
	//发送指定命令
	Uartx_SendMsg(COM2,pCmd,nCmdLen);
	//设定超时定时器
	TimingDelay_Set(nTimeOut);
	while(TimingDelay_TimeOut() == false)
	{
		if(QueueFindChar(pFifo,cWaitChar) != NULL)//找到等待的字符串
		{
	        //如果不需要数据直接返回
	        if((pRet == NULL) 
	        || (pRetLen == NULL))
	        {
	        	printf("%s:Find ,Not Need Data!\r\n",__FUNCTION__);
	          return true;
	        }		
			
			while(QueueOut(pFifo,&ch) == true)//获得接收数据
			{

				pRet[nPos++] = ch;	
			}
			*pRetLen = nPos;
			printf("%s:Find!\r\n",__FUNCTION__);
			return true;
		}
		//执行高优先级的任务
		High_Prio_Task();
	}
	
	if(cWaitChar != 0)//等待指定字符串超时
	{
		printf("%s:Wait TimeOut!\r\n",__FUNCTION__);
		return false;
	}
	else//将超时时间内收到的数据返回
	{
		while(QueueOut(pFifo,&ch) == true)//获得接收数据
		{
			
			pRet[nPos++] = ch;	
		}
		*pRetLen = nPos;
		printf("%s:TimeOut!\r\n",__FUNCTION__);
		return true;		
	}
}

/*******************************************************************************
**  函数名称  : void Eye_Debug(ElemType *pFrame,u16 nLen)
**  函数功能  : 握手
**  输    入  : 	pFrame:指向PC下发的数据
					len:数据的长度
**  输    出  : 无
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
*******************************************************************************/
/*void Eye_Debug(ElemType *pFrame,u16 nLen)
{
	printf("%s: %d\r\n",__FUNCTION__,nLen);

	XHV_Volt_Set(24);
	XVO_ENABLE;
	Delay(1000);
	//发送指令给被测终端
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
	QueueInit(pFifo);  //清除串口缓存
	
	//发送指令给被测终端
	for(Retry_Times = 0; Retry_Times < 5; Retry_Times++)
	{
		if(EYE_COMMAND(pFrame,nLen,1000,FCT_FRAME_TAIL,s_EyeCmdBuf,&nEyeCmdBufLen) == true)
		{
			break;  //握手成功；
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
**  函数名称  : void Eye_Sin(ElemType *pFrame,u16 nLen)
**  函数功能  : 开关量输入测试
**  输    入  : 	pFrame:指向PC下发的数据
					len:数据的长度
**  输    出  : 无
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
*******************************************************************************/
void Eye_Sin(ElemType *pFrame,u16 nLen)
{
	int nChannel,nTimeOut;
	char nLevel;
	u16 nRetLen = 0;
	printf("%s: %d\r\n",__FUNCTION__,nLen);	
	//获取序号、电平、超时时间
	sscanf((char *)&pFrame[8],"%d,%c,%d",&nChannel,&nLevel,&nTimeOut);
	printf("%d %d %d \r\n",nChannel,nLevel,nTimeOut);
	//参数检测
	if((nChannel > 32 || nChannel < 1)
		|| (nLevel != '+' && nLevel != '-')
		|| (nTimeOut > 127))
	{
		Pc_Ack(FCT_ERROR,COMMAND_ERR,1);
		printf("%s:Para Err!\r\n",__FUNCTION__);
		return;	
	}
	//FCT指定通道输出指定电平
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
	//发送指令给被测终端
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
		if(strchr((char *)s_EyeCmdBuf,nLevel) != NULL)//被测终端输入正常
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
**  函数名称  : void Eye_Sout(ElemType *pFrame,u16 nLen)
**  函数功能  : 开关量输出测试
**  输    入  : 	pFrame:指向PC下发的数据
					len:数据的长度
**  输    出  : 无
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
*******************************************************************************/
void Eye_Sout(ElemType *pFrame,u16 nLen)
{
	int nChannel,nTimeOut;
	char nLevel;
	u16 nRetLen = 0;

	printf("%s: %d\r\n",__FUNCTION__,nLen);
	//获取序号、电平、超时时间
	sscanf((char *)&pFrame[9],"%d,%c,%d",&nChannel,&nLevel,&nTimeOut);
	printf("%d %d %d \r\n",nChannel,nLevel,nTimeOut);
	g_HC595State |= 1 << (nChannel - 1+20);
	XHV_Volt_Set(10);
	HC595_Renew();
	Delay(100);
	//参数检测
	if((nChannel > 32 || nChannel < 1)
		|| (nLevel != '+' && nLevel != '-')
		|| (nTimeOut > 127))
	{
		Pc_Ack(FCT_ERROR,COMMAND_ERR,1);
		printf("%s:Para Err!\r\n",__FUNCTION__);
		return;	
	}
	//发送指令给被测终端
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
		Get_IoInput_State();//读取IO状态
		state = ((IoInputState >> (nChannel - 1))) & 1;//获取指定IO电平
		if((state + nLevel ) == (0 + '-')
			|| (state + nLevel) == (1 + '+'))//判断GNSS是否输出正确
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
**  函数名称  : void Eye_Vin(ElemType *pFrame,u16 nLen)
**  函数功能  : 模拟量输入
**  输    入  : 	pFrame:指向PC下发的数据
					len:数据的长度
**  输    出  : 无
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
*******************************************************************************/
void Eye_Vin(ElemType *pFrame,u16 nLen)
{
	int nChannel,nVolt,nTimeOut;
	u16 nRetLen = 0;

	printf("%s: %d\r\n",__FUNCTION__,nLen);
	//获取序号、电压、超时时间
	sscanf((char *)&pFrame[8],"%d,%d,%d",&nChannel,&nVolt,&nTimeOut);
	printf("%d %d %d \r\n",nChannel,nVolt,nTimeOut);
	//参数检测
	if((nChannel > 2 || nChannel < 1)
		|| (nVolt >127 && nVolt < 1)
		|| (nTimeOut > 127))
	{
		Pc_Ack(FCT_ERROR,COMMAND_ERR,1);
		printf("%s:Para Err!\r\n",__FUNCTION__);
		return; 
	}
	
	//FCT指定通道输出指定电平
	g_HC595State |= (1 << (17 + nChannel));
	HC595_Renew();		
	//硬件是否支持该电压值
	if(XHV_Volt_Set(nVolt) == false)
	{
		Pc_Ack(FCT_ERROR,HARDWARE_ERR,1);
		XHV_Volt_Set(0);
		return;
	}
	//发送指令给被测终端
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
	if(strstr((char *)s_EyeCmdBuf,"OK") != NULL)//返回数据是否正确
	{
		int nRetVolt;
		sscanf((char *)&s_EyeCmdBuf[3],"%d",&nRetVolt);
		if(nVolt == nRetVolt)//输入输出电压是否一致
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
**  函数名称  : void Eye_Vout(ElemType *pFrame,u16 nLen)
**  函数功能  : 模拟量输出
**  输    入  : 	pFrame:指向PC下发的数据
					len:数据的长度
**  输    出  : 无
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
*******************************************************************************/
void Eye_Vout(ElemType *pFrame,u16 nLen)
{
	int nChannel,nVolt,nTimeOut;
	u16 nRetLen = 0;

	printf("%s: %d\r\n",__FUNCTION__,nLen);
	//获取序号、电压、超时时间
	sscanf((char *)&pFrame[9],"%d,%d,%d",&nChannel,&nVolt,&nTimeOut);
	printf("%d %d %d \r\n",nChannel,nVolt,nTimeOut);
	//参数检测
	if((nChannel > 32 || nChannel < 1)
		|| (nVolt >50 && nVolt < 1)
		|| (nTimeOut > 127))
	{
		Pc_Ack(FCT_ERROR,COMMAND_ERR,1);
		printf("%s:Para Err!\r\n",__FUNCTION__);
		return; 
	}
	//发送指令给被测终端
	if(EYE_COMMAND(pFrame,nLen,nTimeOut*1000,FCT_FRAME_TAIL,s_EyeCmdBuf,&nRetLen)==false)
	{
		Pc_Ack(FCT_ERROR,EXECUTE_TIMEOUT,1);
		return;
	}
	//ADC3ConvertedValue
	printf("%s:%s\r\n",__FUNCTION__,s_EyeCmdBuf);
	if(strstr((char *)s_EyeCmdBuf,"OK") != NULL)//返回数据是否正确
	{
		int nGetVolt;
		
		nGetVolt = (int)((double)ADC3ConvertedValue[1+nChannel] /4096*3.3*9+0.5);
		printf("--GetVolt:V:%d A:%d\r\n",nGetVolt,ADC3ConvertedValue[1+nChannel]);
		if(nVolt == nGetVolt)//输入输出电压是否一致
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
**  函数名称  : void Eye_Com(ElemType *pFrame,u16 nLen)
**  函数功能  : 通讯接口测试
**  输    入  : 	pFrame:指向PC下发的数据
					len:数据的长度
**  输    出  : 无
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
*******************************************************************************/
void Eye_Com(ElemType *pFrame,u16 nLen)
{
	int nChannel,nTimeOut;
	char sComMsg[20] = "";
	u16 nRetLen = 0,nComLen = 0;
	FIFOQueue *pFifo = NULL;


	//  2路232 2路485 2路CAN 2路
	//标号1:CAN1 2:CAN2 3:232-1 4:232-2 5:485-1 6:485-2
	printf("%s: %d\r\n",__FUNCTION__,nLen);
	//获取字符串、通道、超时时间
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
	//参数检测
	if((nChannel > 32 || nChannel < 1)
		|| (nTimeOut > 127))
	{
		Pc_Ack(FCT_ERROR,COMMAND_ERR,1);
		printf("%s:Para Err!\r\n",__FUNCTION__);
		return; 
	}
	//发送指令给被测终端
	if(EYE_COMMAND(pFrame,nLen,nTimeOut*1000,FCT_FRAME_TAIL,s_EyeCmdBuf,&nRetLen)==false)
	{
		Pc_Ack(FCT_ERROR,EXECUTE_TIMEOUT,1);
		return;
	}	
	switch(nChannel)
	{
	//标号1:CAN1 2:CAN2 3:232-1 4:232-2 5:485-1 6:485-2
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
	QueueInit(pFifo);//清空队列
	printf("%s:%s\r\n",__FUNCTION__,s_EyeCmdBuf);
	Delay(1000);
	if(strstr((char *)s_EyeCmdBuf,"OK") != NULL)//返回数据是否正确
	{
		u8 sRetMsg[20] = "",i = 0;
		while(isQueueEmpty(pFifo) == QUEUE_NOT_EMPTY && i<20)
		{
			QueueOut(pFifo,&sRetMsg[i++]);
		}
		if(memcmp(sComMsg,sRetMsg,nComLen) == 0)//输入输出是否一致
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
**  函数名称  : void Eye_GnssAnte(ElemType *pFrame,u16 nLen)
**  函数功能  : GNSS天线测试
**  输    入  : 	pFrame:指向PC下发的数据
					len:数据的长度
**  输    出  : 无
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
*******************************************************************************/
void Eye_GnssAnte(ElemType *pFrame,u16 nLen)
{
	int GnssState,nTimeOut;
	u16 nRetLen = 0;

	printf("%s: %d\r\n",__FUNCTION__,nLen);
	//获取天线状态和超时时间
	sscanf((char *)&pFrame[13],"%d,%d",&GnssState,&nTimeOut);
	printf("%d %d \r\n",GnssState,nTimeOut);
	//参数检测
	if((GnssState > 2 || GnssState < 0)
		|| (nTimeOut > 127))
	{
		Pc_Ack(FCT_ERROR,COMMAND_ERR,1);
		printf("%s:Para Err!\r\n",__FUNCTION__);
		return; 
	}
	//Delay(1000);
	//模拟天线状态
	GnssAnt_Set_State((GnssAntStatus)GnssState);
    Delay(100);
	//发送指令给被测终端
	if(EYE_COMMAND(pFrame,nLen,nTimeOut*1000,FCT_FRAME_TAIL,s_EyeCmdBuf,&nRetLen)==false)
	{
		Pc_Ack(FCT_ERROR,EXECUTE_TIMEOUT,1);
		return;
	}
	printf("%s:%s\r\n",__FUNCTION__,s_EyeCmdBuf);
	if(strstr((char *)s_EyeCmdBuf,"OK") != NULL)//返回数据是否正确
	{
		int nRetState;
		sscanf((char *)&s_EyeCmdBuf[3],"%d",&nRetState);
		if(GnssState == nRetState)//输入输出是否一致
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
**  函数名称  : void Eye_Mbcom(ElemType *pFrame,u16 nLen)
**  函数功能  : 移动通讯测试
**  输    入  : 	pFrame:指向PC下发的数据
					len:数据的长度
**  输    出  : 无
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
*******************************************************************************/
void Eye_Mbcom(ElemType *pFrame,u16 nLen)
{
	int nTimeOut;
    u8 sAt[50] = "";
	u16 nRetLen = 0; 
 
	printf("%s: %d\r\n",__FUNCTION__,nLen);
	//获取AT指令和超时时间
	{
		char *p1=NULL,*p2=NULL,*p3=NULL;	
		p1 = strchr((char *)pFrame,0x22);
		p2 = strchr((char *)p1 + 1,0x22);
		p3 = strchr((char *)p2 + 1,0x2C);
		memcpy(sAt,p1+1,p2-p1-1);
		sscanf(p3+1,"%d",&nTimeOut);
	}
	printf("%s %d\r\n",sAt,nTimeOut);
	//参数检测
	if((nTimeOut > 127 )
		|| (strlen((char *)sAt) < 2))
	{
		Pc_Ack(FCT_ERROR,COMMAND_ERR,1);
		printf("%s:Para Err!\r\n",__FUNCTION__);
		return; 
	}
	//发送指令给被测终端
	if(EYE_COMMAND(pFrame,nLen,nTimeOut*1000,FCT_FRAME_TAIL,s_EyeCmdBuf,&nRetLen)==false)
	{
		Pc_Ack(FCT_ERROR,EXECUTE_TIMEOUT,1);
		return;
	}
	printf("%s:%s\r\n",__FUNCTION__,s_EyeCmdBuf);
	Pc_Ack(FCT_MSG_UP,s_EyeCmdBuf,nRetLen); 
}

/*******************************************************************************
**  函数名称  : void Eye_CheckSelf(ElemType *pFrame,u16 nLen)
**  函数功能  : 自检测试
**  输    入  : 	pFrame:指向PC下发的数据
					len:数据的长度
**  输    出  : 无
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
*******************************************************************************/
void Eye_CheckSelf(ElemType *pFrame,u16 nLen)
{
	int nTimeOut;
	u16 nRetLen = 0;

	printf("%s: %d\r\n",__FUNCTION__,nLen);
	//获取超时时间
	sscanf((char *)&pFrame[14],"%d",&nTimeOut);
	printf("%d\r\n",nTimeOut);
	//参数检测
	if(nTimeOut > 127)
	{
		Pc_Ack(FCT_ERROR,COMMAND_ERR,1);
		printf("%s:Para Err!\r\n",__FUNCTION__);
		return; 
	}
	//发送指令给被测终端
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
**  函数名称  : void Eye_Gnss(ElemType *pFrame,u16 nLen)
**  函数功能  : GNSS测试
**  输    入  : 	pFrame:指向PC下发的数据
					len:数据的长度
**  输    出  : 无
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
*******************************************************************************/
void Eye_Gnss(ElemType *pFrame,u16 nLen)
{
	int nTimeOut;
	u16 nRetLen = 0;

	printf("%s: %d\r\n",__FUNCTION__,nLen);
	//获取超时时间
	sscanf((char *)&pFrame[9],"%d",&nTimeOut);
	printf("%d\r\n",nTimeOut);
	//参数检测
	if(nTimeOut > 127)
	{
		Pc_Ack(FCT_ERROR,COMMAND_ERR,1);
		printf("%s:Para Err!\r\n",__FUNCTION__);
		return; 
	}
	//发送指令给被测终端
	if(EYE_COMMAND(pFrame,nLen,nTimeOut*1000,FCT_FRAME_TAIL,s_EyeCmdBuf,&nRetLen)==false)
	{
		Pc_Ack(FCT_ERROR,EXECUTE_TIMEOUT,1);
		return;
	}
	printf("%s:%s\r\n",__FUNCTION__,s_EyeCmdBuf);
	if(strstr((char *)s_EyeCmdBuf,"OK") != NULL)//返回数据是否正确
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
**  函数名称  : void Eye_Power(ElemType *pFrame,u16 nLen)
**  函数功能  : 高/低压测试
**  输    入  : 	pFrame:指向PC下发的数据
					len:数据的长度
**  输    出  : 无
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
*******************************************************************************/
void Eye_Power(ElemType *pFrame,u16 nLen)
{
	int nVolt,nLastTime,nTimeOut;
	u16 nRetLen = 0;
	printf("%s: %d\r\n",__FUNCTION__,nLen);
	//获取电压、持续时间、超时时间
	sscanf((char *)&pFrame[10],"%d,%d,%d",&nVolt,&nLastTime,&nTimeOut);
	printf("%d %d %d \r\n",nVolt,nLastTime,nTimeOut);
	//参数检测
	if((nLastTime >127)
		|| (nTimeOut > 127))
	{
		Pc_Ack(FCT_ERROR,COMMAND_ERR,1);
		printf("%s:Para Err!\r\n",__FUNCTION__);
		return; 
	}

	//先开启备电
	XBAT_Volt_Set(45);	
	XBATO_ENABLE;	
	//关闭18路IO输出，否则输出电压值偏低
	g_HC595State &= ~((u32)0x000FFFFF);
	HC595_Renew();		
	//硬件是否支持该电压值
	if(XHV_Volt_Set(nVolt) == false)
	{
		Pc_Ack(FCT_ERROR,HARDWARE_ERR,1);
		return;
	}
	Delay(100);
	//XVO_DISABLE;
	Delay(nLastTime*1000);
	//发送指令给被测终端
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
**  函数名称  : void Eye_Battery(ElemType *pFrame,u16 nLen)
**  函数功能  : 电池充/放电测试
**  输    入  : 	pFrame:指向PC下发的数据
					len:数据的长度
**  输    出  : 无
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
*******************************************************************************/
void Eye_Battery(ElemType *pFrame,u16 nLen)
{
	int nTimeOut,nLastTime,bState;
	u16 nRetLen = 0;

	printf("%s: %d\r\n",__FUNCTION__,nLen);
	//获取状态(1充电0放电)、持续时间、超时时间
	sscanf((char *)&pFrame[12],"%d,%d,%d",&bState,&nLastTime,&nTimeOut);
	printf("%d %d %d\r\n",bState,nLastTime,nTimeOut);
	//参数检测
	if((nTimeOut > 127)
		|| (nLastTime >127)
		|| (bState < 0 || bState > 1))
	{
		Pc_Ack(FCT_ERROR,COMMAND_ERR,1);
		printf("%s:Para Err!\r\n",__FUNCTION__);
		return; 
	}
	if(bState == 1)//充电
	{
		CHARGE_ENABLE;
		XBATO_DISABLE;
	}
	else
	{	
		//开启备用电池供电
		XBAT_Volt_Set(45);	
		XBATO_ENABLE;
		Delay(100);
		//关闭主电
		//XVO_DISABLE;
		X5V_DISABLE;
		Delay(nLastTime*1000);	
	}
	//发送指令给被测终端
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
**  函数名称  : void Eye_Count(ElemType *pFrame,u16 nLen)
**  函数功能  : 计数测试
**  输    入  : 	pFrame:指向PC下发的数据
					len:数据的长度
**  输    出  : 无
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
*******************************************************************************/
void Eye_Count(ElemType *pFrame,u16 nLen)
{
	int nTimeOut;
	u16 nRetLen = 0;

	printf("%s: %d\r\n",__FUNCTION__,nLen);
	//获取超时时间
	sscanf((char *)&pFrame[10],"%d",&nTimeOut);
	printf("%d\r\n",nTimeOut);
	//参数检测
	if(nTimeOut > 127)
	{
		Pc_Ack(FCT_ERROR,COMMAND_ERR,1);
		printf("%s:Para Err!\r\n",__FUNCTION__);
		Delay(1000);	
		//XVO_DISABLE;
		return; 
	}
	//发送指令给被测终端
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
**  函数名称  : void Eye_Pwm(ElemType *pFrame,u16 nLen)
**  函数功能  : PWM测试
**  输    入  : 	pFrame:指向PC下发的数据
					len:数据的长度
**  输    出  : 无
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
*******************************************************************************/
void Eye_Pwm(ElemType *pFrame,u16 nLen)
{
	int nFre,nDuty,nLastTime,nTimeOut;
	u16 nRetLen = 0;

	printf("%s: %d\r\n",__FUNCTION__,nLen);
	//获取超时时间
	sscanf((char *)&pFrame[8],"%d,%d,%d,%d",&nFre,&nDuty,&nLastTime,&nTimeOut);
	printf("%d %d %d %d\r\n",nFre,nDuty,nLastTime,nTimeOut);
	//参数检测
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
	//发送指令给被测终端
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
		if((nRetFre == nFre)&&(nRetDuty == nDuty))//判断PWM测试是否正确
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
**  函数名称  : void Eye_Cmd(ElemType *pFrame,u16 nLen)
**  函数功能  : 接收平台下发的测试指令
**  输    入  : 	pFrame:指向PC下发的数据
					len:数据的长度
**  输    出  : 无
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
存储FLASH的格式:型号(16)+命令条数(1)+总包数(1)+
已接收包数(1)+已发送包数(1)+命令集长度(2)+命令集(#...#)
*******************************************************************************/

void Eye_Cmd(ElemType *pFrame,u16 nLen)
{
	char *pTypeHead = NULL,*pTypeEnd = NULL,*pCmdHead = NULL,*pCmdEnd = NULL;
	char *pPoint = NULL;
	int nTotalItem = 0,nTotalPacket = 0,nCurrentPacket = 0;
	u16 nTypeLen = 0,nCmdLen = 0;
	printf("%s: %d\r\n",__FUNCTION__,nLen);

	//把所有的'.'替换成'\r'
	pPoint = strchr((char *)pFrame,'.');
	while(pPoint != NULL)
	{
		*pPoint = 0x0D;
		pPoint = strchr(pPoint+1,'.');
	}
	//获取型号、总命令条数、总包数、当前包
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

	if(nCurrentPacket == 1)//第一包时候记录型号、总条数、总包数
	{
		memset((u8 *)&g_GnssCmdInfo,0,sizeof(GnssCmdInfo));
		memcpy((u8 *)&g_GnssCmdInfo,pTypeHead,nTypeLen);//终端型号
		g_GnssCmdInfo.nTotalItem = nTotalItem;//指令总条数
		g_GnssCmdInfo.nTotalPacket = nTotalPacket;//总包数
		g_GnssCmdInfo.nSendPacket = 0;
		g_GnssCmdInfo.nRcvPacket= 0;
		g_GnssCmdInfo.nCmdLen = 0;
		g_GnssCmdInfo.nParamFlag = 0;
	}
	memcpy(&g_GnssCmdInfo.sCmdBuf[g_GnssCmdInfo.nCmdLen],pCmdHead,nCmdLen);
	g_GnssCmdInfo.nCmdLen += nCmdLen;
	g_GnssCmdInfo.nRcvPacket++; 
	//所有包都接收完毕，则将数据存入FLASH
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
**  函数名称  : void Get_IoInput_State(void)
**  函数功能  : 获取输入IO的状态
**  输    入  : 无
**  输    出  : 无
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
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
**  函数名称  : void Pc_Ack(FctErrorStatus Status,u8 pMsg,u16 nLen)
**  函数功能  : 给PC返回应答
**  输    入  : 	Status:成功或失败
					pMsg:附加数据
					nLen:附加数据长度
**  输    出  : 无
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
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
	//LCD脱机测试显示
	if(g_LcdMode == LCD_SINGLE_TEST_MODE)
	{
		u16 nLcdLen = 0;
		u8 i = 0;
		
		memset(g_LcdDisplayBuf,0,sizeof(g_LcdDisplayBuf));
		memcpy(g_LcdDisplayBuf,sSendMsg,nPos-1);
		nLcdLen = nPos-1;//去掉最后一个字符
		//2-4行显示内容
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
		
		nItemNum = g_GnssCmdInfo.nTotalItem;//获取测试项条数
		tempIndex = g_LcdDisplayIndex;
		//找到当前下发的指令，提取命令字符串显示
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
			nTypeLen = p2-p1-1;//指令类型的长度
			nParamLen = p3-p1-1;//指令+参数的长度
			//指令+参数长度小于14则全部显示，否则值显示测试项
			if(nParamLen <= 14)
			{
				memcpy(temp,p1+1,nParamLen);
			}
			else
			{
				memcpy(temp,p1+1,nTypeLen);	
			}
		}
		if(strstr((char *)sSendMsg,"ERR=") != NULL || strstr((char *)sSendMsg,"EYE") != NULL)//失败
		//if(strstr((char *)sSendMsg,"OK") == NULL || strstr((char *)sSendMsg,"EYE") != NULL)//失败
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
		else//成功
		{
			memcpy(&temp[14],"OK",2);	
		}
		strcat((char *)g_LcdDisplayBuf,(char *)temp);
		g_LcdDisplayIndex++;
		LCD_Show_Auto();
		
		if(g_LcdDisplayIndex >= nItemNum)//测试项完毕
		{
			Delay(1000);
			if(g_LcdAutoErrIndex == 0)//无任何错误项
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
			else//有错误项
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
**  函数名称  : void TimingDelay_Decrement(void)
**  函数功能  : 减少定时器计数
**  输    入  : 无	
**  输    出  : 无
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :每次systick到来一次减少1，直到0为止
*******************************************************************************/
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}

/*******************************************************************************
**  函数名称  : void TimingDelay_Set(u32 nTime)
**  函数功能  : 设置systick定时器
**  输    入  : 无	
**  输    出  : 无
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
*******************************************************************************/
void TimingDelay_Set(u32 nTime)
{
	TimingDelay = nTime;
}

/*******************************************************************************
**  函数名称  : bool TimingDelay_TimeOut(void)
**  函数功能  : 判断systick定时器是否超时
**  输    入  : 无	
**  输    出  : true:已经超时 false:还未超时
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
*******************************************************************************/
bool TimingDelay_TimeOut(void)
{
	if(TimingDelay == 0)
		return true;
	else
		return false;
}

/*******************************************************************************
**  函数名称  : Delay(__IO uint32_t nTime)
**  函数功能  : systick定时器延迟
**  输    入  : nTime:延迟的时基个数
**  输    出  : 无
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
延迟的时间长短与system时基相关
长时间延迟，每2秒执行一次高优先级任务
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
**  函数名称  : void Delay_Cnt(u32 cnt)
**  函数功能  : 计数的方式进行延迟
**  输    入  : cnt:空指令的个数
**  输    出  : 无
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
*******************************************************************************/
void Delay_Cnt(u32 cnt)
{
	while(cnt--);
}

/*************************************************************
** 函数名称:  void XHV_Volt_Set(u8 nVolt)
** 功能描述:  设置电压
** 输入参数:  nVolt:XHV电压值
** 输出参数:  true:成功 false:失败
** 创建:  oueh 2013.7.08
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
**  函数名称  : void Delay_s(u32 Sec)
**  函数功能  : 延迟秒。
**  输    入  : ns:延迟的秒数	
**  输    出  : 无
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :在这期间代码不会往下执行
*******************************************************************************/
void Delay_s(u32 Sec)
{
	
	while(Sec--)
	{
		Delay_ms(1000);
		//在长时间延迟时，处理高优先级任务
		High_Prio_Task();		
	}
}

/*******************************************************************************
**  函数名称  : void Delay_ms(u32 nMs)
**  函数功能  : 延迟N毫秒。
**  输    入  : nMs:延迟的毫秒数，小于2000
					
**  输    出  : 无
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :在这期间代码不会往下执行
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
	SysTick->CTRL&=0xfffffffb;					//选择外部时钟
	nPerMs = rcc_clocks.SYSCLK_Frequency/8/1000;//获得每毫秒的计数值			
	SysTick->LOAD=nMs*nPerMs; 	 				//延迟值
	SysTick->VAL =0x00;				 			//清空计数值
	SysTick->CTRL=0x01 ;						//使能
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));	//等待超时时间   		
	SysTick->CTRL=0x00;						//关闭   					
	SysTick->VAL =0X00;	   					//清空计数器 

#endif
}
/*******************************************************************************
**  函数名称  : void Delay_us(u32 nUs)
**  函数功能  : 延迟N微秒。
**  输    入  : nUs:延迟的微秒数，小于2000000
					
**  输    出  : 无
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :在这期间代码不会往下执行
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
	SysTick->CTRL&=0xfffffffb;					//选择外部时钟
	nPerUs = rcc_clocks.SYSCLK_Frequency/8/1000000;//获得每微秒的计数值			
	SysTick->LOAD=nUs*nPerUs; 	 				//延迟值
	SysTick->VAL =0x00;				 			//清空计数值
	SysTick->CTRL=0x01 ;						//使能
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));	//等待超时时间   		
	SysTick->CTRL=0x00;						//关闭   					
	SysTick->VAL =0X00;	   					//清空计数器 	  
#endif
}

/*************************************************************
** 函数名称:  Print_Debug_Msg
** 功能描述:  打印调试信息
** 输入参数:  sTitle:打印标题
**				sPrintfBuf:消息
**				nPrintfLen:消息长度
** 输出参数:  无
** 创建:  oueh 2013.05.23
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
**  函数名称  : void Parse_Fifo4_Msg(ElemType *pFrame,u16 nLen)
**  函数功能  : 解析串口4的数据帧
**  输    入  :	pFrame:指向待解析的数据帧
					nLen:数据帧长度
**  输    出  : 	无
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  : 
字符串映射回调函数
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
			Pc_Func_Tbl[i](pFrame,nLen);//跳转相应的处理函数
			return;	
		}
	}
	printf("Parse_Fifo4_Msg:%d\r\n",i);	//这里表示没有匹配到
	Pc_Ack(FCT_ERROR,COMMAND_ERR,1);
}

/*******************************************************************************
**  函数名称  : void RcvFifo4Msg(void)
**  函数功能  : 获取协议帧数据
**  输    入  : 无
**  输    出  : 无
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  : 
先从串口缓冲中获取待有帧头帧尾的数据后解析数据内容。

*******************************************************************************/
void RcvFifo4Msg(void)
{
	ElemType *pFrameHead = NULL,*pFrameTail = NULL;
	FIFOQueue *pFifo = Get_Uart4_Rx_Fifo();
	ElemType sFrameBuf[UART4_FIFO_SIZE] = "";
	
	if((g_UartFlagGroup & FLAG_GRP_UART4) == 0)//没有帧需要处理
	{
		return;
	}
	pFrameHead = QueueFindStr(pFifo,FCT_FRAME_HEAD);	//帧头
	if(pFrameHead != NULL)
	{
		u16 nDelLen = 0;
		//删除帧头前的无用数据
		nDelLen = (pFrameHead - &pFifo->dat[pFifo->front] + pFifo->size) % pFifo->size;
		while(nDelLen--)
		{
			ElemType ch;
			QueueOut(pFifo,&ch);
		}
		pFrameTail = QueueFindChar(pFifo,FCT_FRAME_TAIL);//帧尾
		if(pFrameTail != NULL)
		{
			u16 nFrameLen,nPos;
			
			nFrameLen = (pFrameTail - pFrameHead + 1 + pFifo->size) % pFifo->size;
			while(nFrameLen-- > 0)//从FIFO中取出一帧数据放入sFrameBuf
			{
				ElemType ch;
				QueueOut(pFifo,&ch);
				sFrameBuf[nPos++] = ch;
			}			
			Parse_Fifo4_Msg(sFrameBuf,nPos);//解析数据内容
			//若无帧尾则认为队列中没有帧数据要处理。
			if(QueueFindChar(pFifo,FCT_FRAME_TAIL) == NULL)
			{
				g_UartFlagGroup &=~FLAG_GRP_UART4;
			}
			return;
		}
	}
	//若无帧尾则认为队列中没有帧数据要处理。
	if(QueueFindChar(pFifo,FCT_FRAME_TAIL) == NULL)
	{
		g_UartFlagGroup &=~FLAG_GRP_UART4;
	}	
}

/*******************************************************************************
**  函数名称  : void High_Prio_Task(void)
**  函数功能  : 执行高优先级的任务
**  输    入  : 无
**  输    出  : 无
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
1.喂狗
*******************************************************************************/
void High_Prio_Task()
{
	IWDG_ReloadCounter();
}

/*************************************************************
** 函数名称:  CheckSum
** 功能描述:  计算8位异或校验
** 输入参数:  pMsg:指向要计算的消息
**				nLen:消息的长度
** 输出参数:  8位异或校验
** 创建:  oueh 2013.05.23
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
** 函数名称:  TwoASCToHex
** 功能描述:  ASCII转换成数值
** 输入参数:  ASC:ASCII数值
** 输出参数:  返回数值
** 创建:  oueh 2013.05.23
** Note:ASCII必须为数字或16进制的字母!
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
		//错误	
	}

	return Hex;

}

/*************************************************************
** 函数名称:  HexToTwoASC
** 功能描述:  HEX转换成2个ASCII
** 输入参数:  Hex:待转换的HEX
** 输出参数:  ASCHi返回高位的ASC，ASCLo返回低位的ASC
** 创建:  oueh 2013.05.23
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
** 函数名称:  void App_Init()
** 功能描述:  初始化应用程序
** 输入参数:  无
** 输出参数:  无
** 创建:  oueh 2013.07.23
** Note:
***************************************************************/
void App_Init()
{
	//LCD参数初始化
	g_LcdMode = LCD_MENU_MODE;
	memset(g_LcdDisplayBuf,0,sizeof(g_LcdDisplayBuf));
	g_LcdDisplayIndex = 0;	
	g_MenuIndex = 1;			//跳过主界面
	//关闭IO
	g_HC595State &= ~0x0003FFFF;
	HC595_Renew();
	XHV_Volt_Set(0);	
	//初始化测试指令集
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
** 函数名称:  void AutoTest_Packet_Send()
** 功能描述:  自动测试分包发送
** 输入参数:  无
** 输出参数:  无
** 创建:  oueh 2013.08.23
** Note:
***************************************************************/
void AutoTest_Packet_Send()
{
	//有测试指令包未发送完，继续发送
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

