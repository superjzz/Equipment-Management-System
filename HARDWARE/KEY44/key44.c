#include "key44.h"
#include "delay.h"
#include "usart.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//4*4按键扫面驱动代码
//修改日期:2015年11月21日14:04:49
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 江智忠
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
 	    
//按键初始化函数 

void KEY44_Init(void)
{

	RCC->APB2ENR|=1<<3;     //使能PORTB时钟

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//关闭jtag，使能SWD，可以用SWD模式调试
	
	GPIOB->CRH&=0X00000000;	//PB15-12设置成输出 11-8设置成输入
	GPIOB->CRH|=0X33338888;  
  PBout(12)=1;
	PBout(13)=1;
	PBout(14)=1;
	PBout(15)=1;              //PB15-12设置成输出高
  PBout(9)=0;
	PBout(10)=0;
	PBout(11)=0;
	PBout(8)=0; 
} 

u8 KEY44_Scan( u16 Hang,u16 Lie)
{
u8 i;
	u8 j;
u8 HL=0;
	u8 key=0;
  	KEY44_Init();
    if(PBin(11)|PBin(10)|PBin(9)|PBin(8)==1)   //判断是否有按键按下，如果有 进入if
	{
		for(i=8;i<12;i++)
		{
			if(PBin(i)==1)
	    Hang= i-7;
		} 
		//取反 进行行扫描
		GPIOB->CRH&=0X00000000;	//PB15-12设置成输出 12-8设置成输入
		GPIOB->CRH|=0X88883333;  
    PBout(8)=1;
	  PBout(9)=1;
	  PBout(10)=1;
	  PBout(11)=1;  		//PB8-11设置成输出高
		PBout(12)=0;
	  PBout(13)=0;
	  PBout(14)=0;
	  PBout(15)=0; 
		for(j=12;j<16;j++)
		{ 
			if(PBin(j)==1)
	    Lie= j-11;
		}	
		HL=Hang*10+Lie;
		Lie=0;
		Hang=0;
		switch(HL)
			{
//				case 11: key=1;break;
//				case 12: key=2;break;
//				case 13: key=3;break;
//				case 21: key=4;break;
//				case 22: key=5;break;
//				case 23: key=6;break;
//				case 31: key=7;break;
//				case 32: key=8;break;
//				case 33: key=9;break;
				case 44: key=1;break;
				case 34: key=2;break;
				case 24: key=3;break;
				case 43: key=4;break;
				case 33: key=5;break;
				case 23: key=6;break;
				case 42: key=7;break;
				case 32: key=8;break;
				case 22: key=9;break;
			default:key=HL;
			}
	//	 printf("key44 %d\n",key);
			KEY44_Init();
			return key;
	}

	return 0;                 // 无按键按下

  }
