#include "sys.h"   
#include "door.h"
#include "delay.h"
#include "usart.h"
//初始化PC6 7 8 9为输出口.并使能这两个口的时钟		    
//DOOR IO初始化
void DOOR_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //使能PC端口时钟   	   	 
	 
 
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;				 //DOOR1-->PC.6 端口配置
	GPIO_Init(GPIOC, &GPIO_InitStructure);					 //根据设定参数初始化GPIOA.6
 GPIO_SetBits(GPIOC,GPIO_Pin_6);						 //PC.6 输出高
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;				 //DOOR1-->PC.7 端口配置
	GPIO_Init(GPIOC, &GPIO_InitStructure);					 //根据设定参数初始化GPIOA.7
 GPIO_SetBits(GPIOC,GPIO_Pin_7);						 //PC.7 输出高
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				 //DOOR1-->PC.8 端口配置
	GPIO_Init(GPIOC, &GPIO_InitStructure);					 //根据设定参数初始化GPIOA.8
 GPIO_SetBits(GPIOC,GPIO_Pin_8);						 //PC.8 输出高
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;				 //DOOR1-->PC.9 端口配置
	GPIO_Init(GPIOC, &GPIO_InitStructure);					 //根据设定参数初始化GPIOA.9
 GPIO_SetBits(GPIOC,GPIO_Pin_9);						 //PC.9 输出高

}
void Door(u8 d)
{
	if(d<=4)
	{
	d=d+5;
	PCout(d)=0;
	delay_ms(10000);
	PCout(d)=1;
	d=0;
	}
}
