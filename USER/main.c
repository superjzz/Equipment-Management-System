/*******************************************************************************
*本程序只供学习使用，未经作者许可，不得用于其它任何用途
*开发环境：ALIENTEK miniSTM32开发板
*功能摘要：设备管理系统
*版本：V1.0
*作者：江智忠
*完成时间：2017年2月25日
*备注：
Copyright(C) 2017 肇庆学院_jzz.
All rights reserved
********************************************************************************/
#include "stm32f10x.h"
#include "sys.h"
#include "usart.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "rc522.h"
#include "522Init.h"
#include "OLED_I2C.h"
#include "key44.h"
#include "rtc.h" 
#include "door.h" 
#include "beep.h" 
#include "sim900a.h" 
#include "usart2.h" 
/*******************************
*连线说明：
RFID
*1--SDA <----->PA11   0
*2--SCK <----->PA5  PB3
*3--MOSI<----->PA7    5
*4--MISO<----->PA6    4
*5--悬空
*6--GND <----->GND
*7--RST <----->PA12   1
*8--VCC <----->VCC
OLED
*1--SCL<----->PB6
*2--SDA<----->PB7
KEY44
 
********************************/
 

int main(void)
{	
	u8 key=0,i;
	u8 key44=0,pcd=0,Thing_Num=0;
	u32 Thing_Time=0;
	u32 Thing[9][3]={{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0}};
	Stm32_Clock_Init(9);
  uart_init(9600);//串口初始化为9600
//	usart2_init(115200);		//初始化串口3 
	usart2_init(115200);
	delay_init();	//延时函数初始化		
	DOOR_Init();
	//BEEP_Init();
	KEY_Init();//按键初始化
	KEY44_Init();//按键初始化
	RC522_Init();//射频卡初始化
	I2C_Configuration();
	OLED_Init();
  OLED_Fill(0xFF);//全X屏点亮
	delay_ms(2);
	while(RTC_Init())
	{
		OLED_ShowStr(70,4,"RTC ERROR",2);
		delay_ms(800);
		OLED_ShowStr(70,4,"RTC Trying...",2);
	}

//	Beep();
	while(sim900a_send_cmd("AT","OK",100))//检测是否应答AT指令 
	{
		
		printf("未检测到模块!!!");
		delay_ms(800);
		printf("尝试连接模块...");
		delay_ms(400);  
	} 
		L1:	Welcome_ui();//欢迎界面
	while(1)
	{

		pcd=RC522_Operation();//RC522读卡写卡，并显示ui
	/***************************借物部分*****************************/	
		while(pcd!=0)//读到卡了
		{
			key44=KEY44_Scan( 0,0);
				while((key44!=0)&&(key44!=44)) //当按键按下的不是确定
				{
					Thing_Num=key44-1;
					OLED_ShowNum(100,0,key44,2,2);
					delay_ms(10);
					if(KEY44_Scan(0,0)==11  )//按下确定
					{ 				
						if(Thing[Thing_Num][2]==0)
						{
								Thing_Time=RTC_Get_Timecount();
								Thing[Thing_Num][0]=pcd;                 //卡号
								Thing[Thing_Num][1]=Thing_Time;          //时间
								Thing[Thing_Num][2]=1;                   //被借出
								printf("[%d] %d\n",Thing_Num,Thing[Thing_Num][0]);
								printf(" %d\n",Thing[Thing_Num][1]);
								printf(" %d\n\n  ",Thing[Thing_Num][2]);
								QingQuSheBei_ui();
								Door(key44);//开门
								thingout_sms();
								goto L1;
						}
						else
						{
							YiBeiJieChu_ui();
							delay_ms(5000);
							delay_ms(5000);
							goto L1;
						}
					}	
				}
		}	
		
		/****************************还物部分*****************************/		
		  Find_Pcd();	
			PcdAnticoll(SN);
			delay_ms(2);	  
		  Thing_Num=Find_Thing();
		  if( Thing_Num!=0)//刷到物了
			{
				printf("Thing_Num %d\r",Thing_Num);
	
						if(Thing_Num==1)//是物品1
						{
							if(Thing[0][2]==1)//被借了
							{
								YiShiBie_ui();
								Thing[0][0]=0;
								Thing[0][1]=0;
								Thing[0][2]=0;
							}else
							{
								RuKu_ui();
								Door(Thing_Num);
								delay_ms(5000);
								delay_ms(5000);
								goto L1;
							}
						}
						if(Thing_Num==3)
						{
							if((Thing[1][2]==1)||(Thing[2][2]==1)||(Thing[3][2]==1)||(Thing[4][2]==1)||(Thing[5][2]==1)||(Thing[6][2]==1)||(Thing[7][2]==1)||(Thing[8][2]==1))
							{
								YiShiBie_ui();
									for(i=2;i<=9;i++)
								{
									Thing[i-1][0]=0;
									Thing[i-1][1]=0;
									Thing[i-1][2]=0;
								}
							}else
							{
								RuKu_ui();
								Door(Thing_Num);
								delay_ms(5000);
								delay_ms(5000);
								goto L1;
							}

						}
						Door(Thing_Num);
						thingin_sms();
					  goto L1;
			
		}

		/****************************逾期未还部分*****************************/	
		for(i=0;i<=8;i++)
		 {
			 if(Thing[i][2]!=0)
			 {
				 if((RTC_Get_Timecount()-Thing[i][1])>=20)   //设置逾期时间
				 {
					 printf("Thing_Num %d超时%d\r",i+1,RTC_Get_Timecount()-Thing[i][1]);
					 Thing[i][1]=RTC_Get_Timecount();
					 chaoshi_sms();
				 }
			 }
		 }
	 
	/*******************************************************/		 
		 key = KEY_Scan(0);
			 if(key==KEY0_PRES)
		{   
			for(i=0;i<=8;i++)
			{
			printf("[%d] %d\n",i,Thing[i][0]);
			printf(" %d\n",Thing[i][1]);
			printf(" %d\n\n  ",Thing[i][2]);
			}
		}
		 
		 //测试短信功能
//		 if(key==KEY1_PRES)
//		{   printf("in1");	
//			thingout_sms();
//	   printf("out2");	
//		}
		
	//测试模块sim900a是否工作正常	
//		if(key==KEY0_PRES)
//		{   	printf("in");
//			if(sim900a_send_cmd("AT","OK",200)) printf("AT");//??AT??,???????OK?

//  if(sim900a_send_cmd("AT+CPIN?","OK",200)) printf("SIM");// ??SIM??????????SIM??
//	
//  if(sim900a_send_cmd("AT+CGMI","OK",200))printf("YYS");//???????;
//	   printf("out");
//		}
	//////////////////////////////////////////////////	
		
	}




}




