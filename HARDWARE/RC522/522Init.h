#ifndef _522_INIT_H_
#define _522_INIT_H_
#include "sys.h"

//M1����ĳһ��дΪ���¸�ʽ����ÿ�ΪǮ�����ɽ��տۿ�ͳ�ֵ����
//4�ֽڽ����ֽ���ǰ����4�ֽڽ��ȡ����4�ֽڽ�1�ֽڿ��ַ��1�ֽڿ��ַȡ����1�ֽڿ��ַ��1�ֽڿ��ַȡ�� 
extern u8 data2[4];
extern u8 DefaultKey[6]; 
extern u8 g_ucTempbuf[20];    
extern u8 MLastSelectedSnr[4];
extern u8 RevBuffer[30];  
extern u8 SerBuffer[20]; 
extern u8 CmdValid;
extern u8 CT[2]; //������
extern u8 SN[4]; //����
extern u8 RFID[16];
extern u8 RFIDO1[16];
extern u8 RFIDO2[16]; 
extern u8 RFIDO3[16]; //���ڴ�ȡPcdRead
extern u8 KEY[6];
extern u8 RFID0[16];
extern u8 RFID0_[16];
extern u8 RFID1[16];
extern u8 Init_Value[16];
//Init_Value[0]����λ��Init_Value[1]���ʮλ��λ��Init_Value[2]���С�����֣�

void Main_Menu_UI(void);
void Information_UI(void);//��Ϣ��ѯ����
void iccardcode(void);
u8 VerficationID(u8 s,u8 *RFID);


#endif





