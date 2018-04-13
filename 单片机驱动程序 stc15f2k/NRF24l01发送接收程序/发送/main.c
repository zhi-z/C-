/*******************************************************************************
* 文件名 : main.c
* 实现功能:NRF24L01发送测试程序
* 版本     作者            日期            说明
* V1.0     zhi-z      2015/08/05       初始版本

* 描述   : MCU: STC15f2k60s2     晶振: 22.1184  MHz
			
			利用独立按键发送信息
           
*******************************************************************************/ 
#include "NRF24L01.h" 
#include "uart.h"
 
sbit k1 = P3^3;	   //按键发送数据
sbit k2 = P3^5;
sbit k3 = P3^4;
sbit k4 = P3^2;


 
/*******************************************************************************
* 功能描述 : 主函数
* 函数属性 : 
* 输入参数 : 
* 返回参数 : 
* 函数详解 : 
*******************************************************************************/

void main(void)
{
	//-------------------------------------------------------- 发送数据
	//请到头文件中修改数据的宽度，以保证匹配
	uchar a[Tx_Data_Width]=  {'0','0','0'}; 
	
	UartInit();//串口初始化
	init_NRF24L01() ;//无线模块初始化
	
	k1=k2=k3=k4=1;	  
	
	UART_Send_Str("开始发送信息\r\n\r\n\r\n");//串口显示
	
	while(1)//大循环扫描
	{	
	
		if( NRF24L01_TxPacket(a) )	// Transmit Tx buffer data
		{
			//判读Tx_DS,如果模块的连接不正常，将没有串口、LED的提示
			UART_Send_Str("Send: ………… \n");
		}

		if(k1==0)
		{
			a[0]='1'; 
		}
		else if(k3==0)
		{
			a[0]='2'; 
		}				
		else if(k4==0)
		{
			a[0]='3'; 
		}	
		else if(k2==0)
		{
			a[0]='4'; 
		}
		else
		{
			a[0]='0';
		}					
	}
}