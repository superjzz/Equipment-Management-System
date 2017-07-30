#include "sys.h"   
#include "door.h"
#include "delay.h"
#include "usart.h"
//��ʼ��PC6 7 8 9Ϊ�����.��ʹ���������ڵ�ʱ��		    
//DOOR IO��ʼ��
void DOOR_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //ʹ��PC�˿�ʱ��   	   	 
	 
 
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;				 //DOOR1-->PC.6 �˿�����
	GPIO_Init(GPIOC, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOA.6
 GPIO_SetBits(GPIOC,GPIO_Pin_6);						 //PC.6 �����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;				 //DOOR1-->PC.7 �˿�����
	GPIO_Init(GPIOC, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOA.7
 GPIO_SetBits(GPIOC,GPIO_Pin_7);						 //PC.7 �����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				 //DOOR1-->PC.8 �˿�����
	GPIO_Init(GPIOC, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOA.8
 GPIO_SetBits(GPIOC,GPIO_Pin_8);						 //PC.8 �����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;				 //DOOR1-->PC.9 �˿�����
	GPIO_Init(GPIOC, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOA.9
 GPIO_SetBits(GPIOC,GPIO_Pin_9);						 //PC.9 �����

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
