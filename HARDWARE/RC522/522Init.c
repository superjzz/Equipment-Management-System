#include "sys.h"
#include "delay.h"
#include "522Init.h"
#include "rc522.h"
#include "string.h"

u8 data2[4]  = {0x12,0,0,0};
u8 DefaultKey[6] ={0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; 
u8 g_ucTempbuf[20];    
u8 MLastSelectedSnr[4];
u8 RevBuffer[30];  
u8 SerBuffer[20]; 
u8 CmdValid;
u8 CT[2]; //������
u8 SN[4]; //����
u8 RFID[16];
u8 RFIDO1[16];
u8 RFIDO2[16]; 
u8 RFIDO3[16];//���ڴ�ȡPcdRead
u8 KEY[6]={0xff,0xff,0xff,0xff,0xff,0xff};
//extern u8 RFID0[16]={"����"};                        //д��
//extern u8 RFID1[16]={"13808819022"};
u8 Init_Value[16]={2,00,13,~2,~00,~13,19};


u8 VerficationID(u8 s,u8 *RFID)
{
	u8 status;
//	u8 status1;
	status = PcdRequest(PICC_REQALL,CT);/*Ѱ��*/
	if(status==MI_OK)//Ѱ���ɹ�
	{
		status=MI_ERR;
		status = PcdAnticoll(SN);/*����ײ*/ 
	}	
	if (status==MI_OK)//����ײ�ɹ�
	{
		status=MI_ERR;		
		status =PcdSelect(SN);
	}
	if(status==MI_OK)//ѡ���ɹ�
	{
		status=MI_ERR;
		status =PcdAuthState(0x60,0x09,KEY,SN);
	}
	
	if(status==MI_OK)//��֤�ɹ�
	{
		status=MI_ERR;
		status=PcdRead(s,RFID);
	}
	
	return status;
}








