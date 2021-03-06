
/*******************************************************************************
* 文件名 : UART.C

* 版本     作者            日期            说明
* V1.1     zhi-z       2014/08/03       初始版本

* 描述   : MCU:      晶振:    MHz
           
*******************************************************************************/

#include "uart.h"
#include <STC15F2K60S2.H>

//unsigned char UART_buf[4]; 

/*******************************************************************************
* 功能描述 : 51单片机的串口初始化
* 函数属性 : 外部
* 输入参数 : 无
* 返回参数 : 无
* 函数详解 : 
      
*******************************************************************************/

void UartInit(void)		//115200bps@22.1184MHz
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x40;		//定时器1时钟为Fosc,即1T
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//设定定时器1为16位自动重装方式
	TL1 = 0xD0;		//设定定时初值
	TH1 = 0xFF;		//设定定时初值
	ET1 = 0;		//禁止定时器1中断
	
	EA = 1;
	ES  = 1;        //打开接收中断
	
	TR1 = 1;		//启动定时器1
	UART_Send_Str("串口初始化完毕!\n");
	UART_Send_Str("已启用热启动（自动下载），请注意程序是否异常！！!\n");
	UART_Send_Str("下面将测试Printf( ),请注意提示！！!\n");
	printf("printf( ) is ok!\n\n");
}

/*******************************************************************************
* 功能描述 : 51单片机的串口中断处理函数
* 函数属性 : 外部
* 输入参数 : 无
* 返回参数 : 无
* 函数详解 : 
      
*******************************************************************************/

void UART_inter() interrupt 4    //串口中断函数
{
   	ES=0;
    //中断接收处理
    if(RI)
    {
		RI=0;
//		UART_buf[counter++] = SBUF;
//		if(counter == 4){counter = 0;}
		IAP_CONTR = 0X60;  //STC15软件复位进入ISP下载
    }
 
    ES=1;  
}

/*******************************************************************************
* 功能描述 : 51单片机的串口发送一个字节的函数
* 函数属性 : 外部
* 输入参数 : unsigned char mydata,要发送的一个字节内容
* 返回参数 : 无
* 函数详解 : 
      
*******************************************************************************/

void UART_Send_Byte(unsigned char mydata)	
{
    ES=0;
    TI=0;
    SBUF=mydata;
    while(!TI);
    TI=0;
    ES=1;
}
 
/*******************************************************************************
* 功能描述 : 串口发送0d和0a两个字节,即回车换行,在串口助手上有回车换行的效果
* 函数属性 : 外部
* 输入参数 : 无
* 返回参数 : 无
* 函数详解 : 
      
*******************************************************************************/

void UART_Send_Enter()
{
    UART_Send_Byte(0x0d);  
    UART_Send_Byte(0x0a);  
}

/*******************************************************************************
* 功能描述 : 单片机的串口发送字符串
* 函数属性 : 外部
* 输入参数 : char *s ,指向字符串的指针
* 返回参数 : 无
* 函数详解 : 
      
*******************************************************************************/

void UART_Send_Str(char *s)
{
    int idata len=strlen(s)-1;
    int idata i;

    for(i=0;i<len;i++)
    {
        UART_Send_Byte(s[i]);
    }
    
    if(s[i]=='\n') 
    {
        UART_Send_Enter();
    }
    else
    {
        UART_Send_Byte(s[i]);
    }
}

/*******************************************************************************
* 功能描述 : 串口发送数值,函数会将数值转为相应的字符串,如 45 转为 "45"再发送出去
* 函数属性 : 外部
* 输入参数 : unsigned long dat,要发送的数值
* 返回参数 : 无
* 函数详解 : 
      
*******************************************************************************/

void UART_Put_Num(unsigned long dat)
{
    char idata temp[20];
    u32_to_str(dat,temp); //把数值转为字符
    UART_Send_Str(temp);
}

/*******************************************************************************
* 功能描述 : 单片机的串口发送调试信息
* 函数属性 : 外部
* 输入参数 : inf:指向提示信息字符串的指针,dat:数值,提示信息就是这个数值的描述
* 返回参数 : 无
* 函数详解 :  
      
*******************************************************************************/

//void UART_Put_Inf(char *inf,unsigned long dat)
//{
//    UART_Send_Str(inf);     //输出的提示信息,用来描述数值
//    UART_Put_Num(dat);      //输出数值
//    UART_Send_Str("\r\n");    //换行
//}

/*******************************************************************************
* 功能描述 : 将一个32位的变量dat转为字符串，比如把1234转为"1234"
* 函数属性 : 外部
* 输入参数 : dat:待转换的变量,str:指向字符数组的指针，转换后的字节串放在其中
* 返回参数 : 无
* 函数详解 : 
      
*******************************************************************************/

void u32_to_str(unsigned long dat,char *str) 
{
    char idata temp[20];
    unsigned char idata i=0,j=0;

    i=0;
    while(dat)
    {
        temp[i]=dat%10+0x30;
        i++;
        dat/=10;
    }
    
    j=i;
    for(i=0;i<j;i++)
    {
        str[i]=temp[j-i-1];
    }
    
    if(!i) {str[i++]='0';}
    str[i]=0;
}

/*******************************************************************************
* 功能描述 : 将一个字符串转为32位的变量，比如"1234"转为1234
* 函数属性 : 外部
* 输入参数 : char *str,指向待转换的字符串的指针
* 返回参数 : unsigned long temp ,转换后的数值
* 函数详解 : 
      
*******************************************************************************/

//unsigned long str_to_u32(char *str) 
//{
//    unsigned long idata temp=0;
//    unsigned long idata fact=1;
//    unsigned char idata len=strlen(str);
//    unsigned char idata i;

//    for(i=len;i>0;i--)
//    {
//        temp += ((str[i-1]-0x30)*fact);
//        fact *= 10;
//    }
//    return temp;
//}


/*******************************************************************************
* 功能描述 : 将数值转换为字符串,如 15 转为 "000F"
* 函数属性 : 外部
* 输入参数 : hex 要转换的数值, str 指向用来保存结果的数组的指针
* 返回参数 : unsigned char 0 
* 函数详解 : 
      
*******************************************************************************/


//unsigned char Hex2Str_16b(unsigned int hex,char *str)
//{
//    unsigned char temp=0;

//    temp=((hex&0xf000)>>12);
//    str[0]=(temp>=10)?(temp-10+'A'):(temp+0x30);

//    temp=((hex&0x0f00)>>8);
//    str[1]=(temp>=10)?(temp-10+'A'):(temp+0x30);

//    temp=((hex&0x00f0)>>4);
//    str[2]=(temp>=10)?(temp-10+'A'):(temp+0x30);

//    temp=((hex&0x000f)>>0);
//    str[3]=(temp>=10)?(temp-10+'A'):(temp+0x30);

//    str[4]=0;

//    return 0;
//}

/*******************************************************************************
* 功能描述  : 重定向这个C库（stdio）printf函数  文件流——》串口USART1
* 函数属性  : 内部
* 输入参数  : ch,*f
* 返回参数  : 无
* 函数详解  : 

*******************************************************************************/

char putchar(char c)
{
	if (c == '\n')	            //判断换行符'\n'，若是换行符，需要插入回车符'\r'
	{
		ES = 0;                 //关闭串口中断
		SBUF = '\r';            //发送回车符
		while(!TI);	            //因为串口速度较慢，等待发送完成
		TI = 0;                 //清除发送标记
		ES = 1;                 //发送完毕，开启串口中断，以响应串口接收
	}

	ES = 0;                     //关闭串口中断
	SBUF = c;                   //发送printf传递过来的字符
	while(!TI);
	TI = 0;
	ES = 1;
	return(1);               //必须要返回true
}

/*******************************************************************************
* 功能描述 : 将数值以16进制输出到串口,能显示的最大值为 65535
* 函数属性 : 外部
* 输入参数 : 要输出显示的数值
* 返回参数 : 无
* 函数详解 : 
      
*******************************************************************************/


//void UART_Put_Hex(unsigned int hex)
//{
//    char temp[20];
//    Hex2Str_16b(hex,temp);
//    UART_Send_Str(temp);
//}

/*******************************************************************************
* 功能描述 : 主函数
* 函数属性 : 
* 输入参数 : 
* 返回参数 : 
* 函数详解 : 
      
*******************************************************************************/

//void main()
//{
//	unsigned char i = 7 ;
//	UartInit();       //串口初始化
//	printf("printf()         ok\n");
//	UART_Send_Str("UART_Send_Str()  ok\n"); //发送字符串
//	UART_Put_Num(i);    //以10进制形式显示数值大小
//	UART_Send_Enter();			//回车换行
//	UART_Put_Hex(15);	//以16进制形式显示数值大小
//	UART_Send_Enter();			//回车换行
//	UART_Put_Inf("i= ",i);	// 本语句在这里的输出结果为  i=7
//	UART_Send_Byte(48);		//输出一个48,48对应的ascll码字符为'0',所以本语句显示内容为  0
//	while(1);
//}
