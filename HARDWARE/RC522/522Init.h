#ifndef _522_INIT_H_
#define _522_INIT_H_
#include "sys.h"

//M1卡的某一块写为如下格式，则该块为钱包，可接收扣款和充值命令
//4字节金额（低字节在前）＋4字节金额取反＋4字节金额＋1字节块地址＋1字节块地址取反＋1字节块地址＋1字节块地址取反 
extern u8 data2[4];
extern u8 DefaultKey[6]; 
extern u8 g_ucTempbuf[20];    
extern u8 MLastSelectedSnr[4];
extern u8 RevBuffer[30];  
extern u8 SerBuffer[20]; 
extern u8 CmdValid;
extern u8 CT[2]; //卡类型
extern u8 SN[4]; //卡号
extern u8 RFID[16];
extern u8 RFIDO1[16];
extern u8 RFIDO2[16]; 
extern u8 RFIDO3[16]; //用于存取PcdRead
extern u8 KEY[6];
extern u8 RFID0[16];
extern u8 RFID0_[16];
extern u8 RFID1[16];
extern u8 Init_Value[16];
//Init_Value[0]金额百位，Init_Value[1]金额十位个位，Init_Value[2]金额小数部分，

void Main_Menu_UI(void);
void Information_UI(void);//信息查询界面
void iccardcode(void);
u8 VerficationID(u8 s,u8 *RFID);


#endif





