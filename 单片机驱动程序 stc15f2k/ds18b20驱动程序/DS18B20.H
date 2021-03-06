/*******************************************************************************
* 文件名 : ds12864.h
* 实现功能:
* 版本     作者            日期            说明
* V1.0     zhi-z            2015/08/30       初始版本

* 描述   : MCU: STC15F2K60S2     晶振: 22.1184   MHz
           
*******************************************************************************/
#ifndef __DS18B20_H__
#define __DS18B20_H__

#include <STC15F2K60S2.H>

sbit DQ=P5^5;

bit InitDs18b20(void);	   //初始化,有返回值。
void Temperature(void);    //读取温度
void DisplayTemp(void);	   // 显示函数(将读取到的温度计算并显示)

#endif


