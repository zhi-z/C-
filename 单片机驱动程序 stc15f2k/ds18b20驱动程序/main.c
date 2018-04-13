/*******************************************************************************
* 文件名 : main.c
* 实现功能:ds18b20测试程序
* 版本     作者            日期            说明
* V1.0     zhi-z          2015/08/30       初始版本

* 描述   : MCU: STC15F2K60S2     晶振: 22.1184   MHz
           
*******************************************************************************/


#include"DS18B20.H"
#include "uart.h"

void main()
{
	  UartInit();
	  while(1)
	  {
	  	DisplayTemp();
	  }


}