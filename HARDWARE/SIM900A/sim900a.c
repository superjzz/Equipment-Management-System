#include "sim900a.h"
#include "usart2.h"		
#include "delay.h"	
#include "led.h"   	 
#include "key.h"	 	 	 	 	 
#include "string.h"    
#include "usart.h"
//////////////////////////////////////////////////////////////////////////////////	   
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F103开发板 
//ATK-SIM900A GSM/GPRS模块驱动	  
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2015/4/12
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved
//******************************************************************************** 
//无
/////////////////////////////////////////////////////////////////////////////////// 	
   
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//usmart支持部分
//将收到的AT指令应答数据返回给电脑串口
//mode:0,不清零USART3_RX_STA;
//     1,清零USART3_RX_STA;
void sim_at_response(u8 mode)
{
	if(USART2_RX_STA&0X8000)		//接收到一次数据了
	{ 
		USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;//添加结束符
		printf("%s",USART2_RX_BUF);	//发送到串口
		if(mode)USART2_RX_STA=0;
	} 
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//ATK-SIM900A 各项测试(拨号测试、短信测试、GPRS测试)共用代码

//sim900a发送命令后,检测接收到的应答
//str:期待的应答结果
//返回值:0,没有得到期待的应答结果
//    其他,期待应答结果的位置(str的位置)
u8* sim900a_check_cmd(u8 *str)
{
	char *strx=0;
	if(USART2_RX_STA&0X8000)		//接收到一次数据了
	{ 
		USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;//添加结束符
		strx=strstr((const char*)USART2_RX_BUF,(const char*)str);
	} 
	return (u8*)strx;
}
//向sim900a发送命令
//cmd:发送的命令字符串(不需要添加回车了),当cmd<0XFF的时候,发送数字(比如发送0X1A),大于的时候发送字符串.
//ack:期待的应答结果,如果为空,则表示不需要等待应答
//waittime:等待时间(单位:10ms)
//返回值:0,发送成功(得到了期待的应答结果)
//       1,发送失败
u8 sim900a_send_cmd(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART2_RX_STA=0;
	if((u32)cmd<=0XFF)
	{
		while((USART2->SR&0X40)==0);//等待上一次数据发送完成  
		USART2->DR=(u32)cmd;
	}else u2_printf("%s\r\n",cmd);//发送命令
	if(ack&&waittime)		//需要等待应答
	{
		while(--waittime)	//等待倒计时
		{
			delay_us(10000);
			if(USART2_RX_STA&0X8000)//接收到期待的应答结果
			{
				if(sim900a_check_cmd(ack))break;//得到有效数据 
				USART2_RX_STA=0;
			} 
		}
		if(waittime==0)res=1; 
	}
	return res;
} 
//sim900a主测试程序
void sim900a_test(void)
{

		if(sim900a_send_cmd("AT","OK",100)) 
			{	printf("err1");}
					
	  if(sim900a_send_cmd("AT+CMGF=0","OK",20))
			{printf("err2");}
	
	  if(sim900a_send_cmd("AT+CMGS=45",">",20))
			{printf("err3");}
	
		u2_printf("%s","0011000D91685126464175F80008AA1E8BF7572889C45B9A65F695F451855F528FD856687269FF0C8C228C22FF01");
			//0011000D91685126464175F80008AA1E8BF7572889C45B9A65F695F451855F528FD856687269FF0C8C228C22FF01

	 if(sim900a_send_cmd((u8*)0x1A,"+CMGS:",100))
			{printf("err4");}

}
void thingout_sms(void)//器物出库成功，请在一定时间内归还，谢谢！
{
		if(sim900a_send_cmd("AT","OK",100)) 
			{	printf("err1");}
					
	  if(sim900a_send_cmd("AT+CMGF=0","OK",20))
			{printf("err2");}
	
	  if(sim900a_send_cmd("AT+CMGS=55",">",20))
			{printf("err3");}
	
		u2_printf("%s","0011000D91683170471209F20008AA285668726951FA5E936210529FFF0C8BF757284E005B9A65F695F451855F528FD8FF0C8C228C22FF01");
			//0011000D91683170471209F20008AA285668726951FA5E936210529FFF0C8BF757284E005B9A65F695F451855F528FD8FF0C8C228C22FF01
			//0011000D91685126464175F80008AA285668726951FA5E936210529FFF0C8BF757284E005B9A65F695F451855F528FD8FF0C8C228C22FF01
	 if(sim900a_send_cmd((u8*)0x1A,"+CMGS:",1000))
			{printf("err4");}
}
void thingin_sms(void)//器物归还成功，感谢使用！
{
		if(sim900a_send_cmd("AT","OK",100)) 
			{	printf("err1");}
					
	  if(sim900a_send_cmd("AT+CMGF=0","OK",20))
			{printf("err2");}
	
	  if(sim900a_send_cmd("AT+CMGS=39",">",20))
			{printf("err3");}
	
		u2_printf("%s","0011000D91683170471209F20008AA18566872695F528FD86210529FFF0C611F8C224F7F7528FF01");
			//0011000D91683170471209F20008AA18566872695F528FD86210529FFF0C611F8C224F7F7528FF01
			//0011000D91685126464175F80008AA18566872695F528FD86210529FFF0C611F8C224F7F7528FF01
	 if(sim900a_send_cmd((u8*)0x1A,"+CMGS:",300))
			{printf("err4");}
}
void chaoshi_sms(void)//器物借出超时，请马上归还！
{
		if(sim900a_send_cmd("AT","OK",100)) 
			{	printf("err1");}
					
	  if(sim900a_send_cmd("AT+CMGF=0","OK",20))
			{printf("err2");}
	
	  if(sim900a_send_cmd("AT+CMGS=41",">",20))
			{printf("err3");}
	
		u2_printf("%s","0011000D91683170471209F20008AA1A56687269501F51FA8D8565F6FF0C8BF79A6C4E0A5F528FD8FF01");
			//0011000D91683170471209F20008AA1A56687269501F51FA8D8565F6FF0C8BF79A6C4E0A5F528FD8FF01
			//0011000D91685126464175F80008AA1A56687269501F51FA8D8565F6FF0C8BF79A6C4E0A5F528FD8FF01
	 if(sim900a_send_cmd((u8*)0x1A,"+CMGS:",100))
			{printf("err4");}
}








