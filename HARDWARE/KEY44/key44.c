#include "key44.h"
#include "delay.h"
#include "usart.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//4*4����ɨ����������
//�޸�����:2015��11��21��14:04:49
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
 	    
//������ʼ������ 

void KEY44_Init(void)
{

	RCC->APB2ENR|=1<<3;     //ʹ��PORTBʱ��

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//�ر�jtag��ʹ��SWD��������SWDģʽ����
	
	GPIOB->CRH&=0X00000000;	//PB15-12���ó���� 11-8���ó�����
	GPIOB->CRH|=0X33338888;  
  PBout(12)=1;
	PBout(13)=1;
	PBout(14)=1;
	PBout(15)=1;              //PB15-12���ó������
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
    if(PBin(11)|PBin(10)|PBin(9)|PBin(8)==1)   //�ж��Ƿ��а������£������ ����if
	{
		for(i=8;i<12;i++)
		{
			if(PBin(i)==1)
	    Hang= i-7;
		} 
		//ȡ�� ������ɨ��
		GPIOB->CRH&=0X00000000;	//PB15-12���ó���� 12-8���ó�����
		GPIOB->CRH|=0X88883333;  
    PBout(8)=1;
	  PBout(9)=1;
	  PBout(10)=1;
	  PBout(11)=1;  		//PB8-11���ó������
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

	return 0;                 // �ް�������

  }
