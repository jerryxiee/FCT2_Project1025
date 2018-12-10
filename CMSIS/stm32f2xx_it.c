/**
  ******************************************************************************
  * @file    Project/STM32F2xx_StdPeriph_Template/stm32f2xx_it.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    18-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f2xx_it.h"
#include "FIFOQueue.h"
#include "Test.h"

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	TimingDelay_Decrement();
}

/******************************************************************************/
/*                 STM32F2xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f2xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 
void USART1_IRQHandler(void)
{
	//防止溢出(ORE)时候程序卡死,数据依然会丢失
	if (USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET)
	{
		USART_ReceiveData(USART1);	
	}
	
	//发送中断
	if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET) 			  
	{ 
		USART_ITConfig(USART1, USART_IT_TXE, DISABLE);	
	}	

	//接收中断
	if(USART_GetITStatus(USART1,USART_IT_RXNE) != RESET)
	{
		ElemType eDat;

		g_TestInfo.nUart1RcvCnt++;
		eDat = USART_ReceiveData(USART1) & 0xFF; 	
		QueueIn(Get_Uart1_Rx_Fifo(),eDat);
		if(FCT_FRAME_TAIL == eDat)
		{
			g_UartFlagGroup |= FLAG_GRP_UART1;
		}
		USART_ClearITPendingBit(USART1, USART_IT_RXNE); 
	}
	
}

void USART2_IRQHandler(void)
{
	//防止溢出(ORE)时候程序卡死,数据依然会丢失
	if (USART_GetFlagStatus(USART2, USART_FLAG_ORE) != RESET)
	{
		USART_ReceiveData(USART2);	
	}
	
	//发送中断
	if(USART_GetITStatus(USART2, USART_IT_TXE) != RESET)			  
	{ 
		USART_ITConfig(USART2, USART_IT_TXE, DISABLE);	
	}	

	//接收中断
	if(USART_GetITStatus(USART2,USART_IT_RXNE) != RESET)
	{
		ElemType eDat;

		g_TestInfo.nUart2RcvCnt++;
		eDat = USART_ReceiveData(USART2) & 0xFF;
			
		QueueIn(Get_Uart2_Rx_Fifo(),eDat);
		if(FCT_FRAME_TAIL == eDat)
		{
			g_UartFlagGroup |= FLAG_GRP_UART2;
		}
		USART_ClearITPendingBit(USART2, USART_IT_RXNE); 
	}
	
}


void USART3_IRQHandler(void)
{
	//防止溢出(ORE)时候程序卡死,数据依然会丢失
	if (USART_GetFlagStatus(USART3, USART_FLAG_ORE) != RESET)
	{
		USART_ReceiveData(USART3);	
	}
	
	//发送中断
	if(USART_GetITStatus(USART3, USART_IT_TXE) != RESET) 			  
	{ 
		USART_ITConfig(USART3, USART_IT_TXE, DISABLE);	
	}	

	//接收中断
	if(USART_GetITStatus(USART3,USART_IT_RXNE) != RESET)
	{
		ElemType eDat;

		g_TestInfo.nUart3RcvCnt++;
		eDat = USART_ReceiveData(USART3) & 0xFF; 	
		QueueIn(Get_Uart3_Rx_Fifo(),eDat);
		if(FCT_FRAME_TAIL == eDat)
		{
			g_UartFlagGroup |= FLAG_GRP_UART3;
		}
		USART_ClearITPendingBit(USART3, USART_IT_RXNE); 
	}
	
}


void UART4_IRQHandler(void)
{
	//防止溢出(ORE)时候程序卡死,数据依然会丢失
	if (USART_GetFlagStatus(UART4, USART_FLAG_ORE) != RESET)
	{
		USART_ReceiveData(UART4);	
	}
	
	//发送中断
	if(USART_GetITStatus(UART4, USART_IT_TXE) != RESET)				  
	{ 
		USART_ITConfig(UART4, USART_IT_TXE, DISABLE);	
	}	

	//接收中断
	if(USART_GetITStatus(UART4,USART_IT_RXNE) != RESET)
	{
		ElemType eDat;

		USART_ClearITPendingBit(UART4, USART_IT_RXNE); 
		g_TestInfo.nUart4RcvCnt++;
		eDat = USART_ReceiveData(UART4) & 0xFF;	
		g_TestInfo.nUart4Sum += eDat;
		QueueIn(Get_Uart4_Rx_Fifo(),eDat);
		if(FCT_FRAME_TAIL == eDat)
		{
			g_UartFlagGroup |= FLAG_GRP_UART4;
		}
		
	}
	
}
void UART5_IRQHandler(void)
{
	//防止溢出(ORE)时候程序卡死,数据依然会丢失
	if (USART_GetFlagStatus(UART5, USART_FLAG_ORE) != RESET)
	{
		USART_ReceiveData(UART5);	
	}

	//发送中断
	if(USART_GetITStatus(UART5, USART_IT_TXE) != RESET)				  
	{ 
		USART_ITConfig(UART5, USART_IT_TXE, DISABLE);	
	}	

	//接收中断
	if(USART_GetITStatus(UART5,USART_IT_RXNE) != RESET)
	{
		ElemType eDat;
		
		USART_ClearITPendingBit(UART5, USART_IT_RXNE); 
		g_TestInfo.nUart5RcvCnt++;
		eDat = USART_ReceiveData(UART5) & 0xFF;
		QueueIn(Get_Uart5_Rx_Fifo(),eDat);
		if(FCT_FRAME_TAIL == eDat)
		{
			g_UartFlagGroup |= FLAG_GRP_UART5;
		}
		
	}

}
void USART6_IRQHandler(void)
{
	//防止溢出(ORE)时候程序卡死,数据依然会丢失
	if (USART_GetFlagStatus(USART6, USART_FLAG_ORE) != RESET)
	{
		USART_ReceiveData(USART6);	
	}

	//发送中断
	if(USART_GetITStatus(USART6, USART_IT_TXE) != RESET) 			  
	{ 
		USART_ITConfig(USART6, USART_IT_TXE, DISABLE);	
	}	

	//接收中断
	if(USART_GetITStatus(USART6,USART_IT_RXNE) != RESET)
	{
		ElemType eDat;

		g_TestInfo.nUart6RcvCnt++;
		eDat = USART_ReceiveData(USART6) & 0xFF; 	
		QueueIn(Get_Uart6_Rx_Fifo(),eDat);
		if(FCT_FRAME_TAIL == eDat)
		{
			g_UartFlagGroup |= FLAG_GRP_UART6;
		}
		USART_ClearITPendingBit(USART6, USART_IT_RXNE); 
	}

}

void CAN1_RX0_IRQHandler()
{
	CanRxMsg RxMessage;
	FIFOQueue *pFifo = Get_Can1_Rx_Fifo();
	u8 i = 0;
	
	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
	for(i = 0;i<8;i++)
	{
		QueueIn(pFifo,RxMessage.Data[i]);
	}

}
void CAN2_RX0_IRQHandler()
{
	CanRxMsg RxMessage;
	FIFOQueue *pFifo = Get_Can2_Rx_Fifo();
	u8 i = 0;
	
	CAN_Receive(CAN2, CAN_FIFO0, &RxMessage);
	for(i = 0;i<8;i++)
	{
		QueueIn(pFifo,RxMessage.Data[i]);
	}

}
void SPI2_IRQHandler(void)
{

}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
