#include "sim900a.h"
#include "usart2.h"		
#include "delay.h"	
#include "led.h"   	 
#include "key.h"	 	 	 	 	 
#include "string.h"    
#include "usart.h"
//////////////////////////////////////////////////////////////////////////////////	   
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F103������ 
//ATK-SIM900A GSM/GPRSģ������	  
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/4/12
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved
//******************************************************************************** 
//��
/////////////////////////////////////////////////////////////////////////////////// 	
   
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//usmart֧�ֲ���
//���յ���ATָ��Ӧ�����ݷ��ظ����Դ���
//mode:0,������USART3_RX_STA;
//     1,����USART3_RX_STA;
void sim_at_response(u8 mode)
{
	if(USART2_RX_STA&0X8000)		//���յ�һ��������
	{ 
		USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;//��ӽ�����
		printf("%s",USART2_RX_BUF);	//���͵�����
		if(mode)USART2_RX_STA=0;
	} 
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//ATK-SIM900A �������(���Ų��ԡ����Ų��ԡ�GPRS����)���ô���

//sim900a���������,�����յ���Ӧ��
//str:�ڴ���Ӧ����
//����ֵ:0,û�еõ��ڴ���Ӧ����
//    ����,�ڴ�Ӧ������λ��(str��λ��)
u8* sim900a_check_cmd(u8 *str)
{
	char *strx=0;
	if(USART2_RX_STA&0X8000)		//���յ�һ��������
	{ 
		USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;//��ӽ�����
		strx=strstr((const char*)USART2_RX_BUF,(const char*)str);
	} 
	return (u8*)strx;
}
//��sim900a��������
//cmd:���͵������ַ���(����Ҫ��ӻس���),��cmd<0XFF��ʱ��,��������(���緢��0X1A),���ڵ�ʱ�����ַ���.
//ack:�ڴ���Ӧ����,���Ϊ��,���ʾ����Ҫ�ȴ�Ӧ��
//waittime:�ȴ�ʱ��(��λ:10ms)
//����ֵ:0,���ͳɹ�(�õ����ڴ���Ӧ����)
//       1,����ʧ��
u8 sim900a_send_cmd(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART2_RX_STA=0;
	if((u32)cmd<=0XFF)
	{
		while((USART2->SR&0X40)==0);//�ȴ���һ�����ݷ������  
		USART2->DR=(u32)cmd;
	}else u2_printf("%s\r\n",cmd);//��������
	if(ack&&waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	//�ȴ�����ʱ
		{
			delay_us(10000);
			if(USART2_RX_STA&0X8000)//���յ��ڴ���Ӧ����
			{
				if(sim900a_check_cmd(ack))break;//�õ���Ч���� 
				USART2_RX_STA=0;
			} 
		}
		if(waittime==0)res=1; 
	}
	return res;
} 
//sim900a�����Գ���
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
void thingout_sms(void)//�������ɹ�������һ��ʱ���ڹ黹��лл��
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
void thingin_sms(void)//����黹�ɹ�����лʹ�ã�
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
void chaoshi_sms(void)//��������ʱ�������Ϲ黹��
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








