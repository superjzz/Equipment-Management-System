#ifndef __KEY44_H
#define __KEY44_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//4*4按键扫面驱动代码
//修改日期:2015年11月21日14:04:49
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 江智忠
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////   	 


#define KEY0_PRES	1		//KEY0  
#define KEY1_PRES	2		//KEY1 
#define WKUP_PRES	3		//WK_UP  

void KEY44_Init(void);//IO初始化
u8 KEY44_Scan(u16 Hang,u16 Lie);  	//按键扫描函数					    
#endif
