#include "sys.h"
#include "delay.h"
#include "string.h"
#include "rc522.h"
#include "usart.h"
#include "522Init.h"
#include "key.h"
#include "OLED_I2C.h"
unsigned char thing1[4]={136,83,162,252};  //卡片序列号
unsigned char thing2[4]={136,4,190,201};			//卡片序列号
unsigned char thing3[4]={136,83,162,216};				//卡片序列号

u8 Find_Pcd(void)			//不断寻卡函数
{
  u8 status;
  PcdReset();//复位RC522
  status=PcdRequest(PICC_REQALL,CT);//寻天线区内未进入休眠状态的卡，返回卡片类型 2字节
	return status;
}
u8 Find_Thing(void)
{
	u8 num=0;
 	u8 SN[4];
       PcdAnticoll(SN);
			if((SN[0]==thing1[0])&&(SN[1]==thing1[1])&&(SN[2]==thing1[2])&&(SN[3]==thing1[3])) //如果卡为卡1
				{
					num=1;

				}
			if((SN[0]==thing2[0])&&(SN[1]==thing2[1])&&(SN[2]==thing2[2])&&(SN[3]==thing2[3]))   //如果卡为卡2
				{
					 num=3;;

				}  
			if((SN[0]==thing3[0])&&(SN[1]==thing3[1])&&(SN[2]==thing3[2])&&(SN[3]==thing3[3]))    //如果卡为卡3
				{
						num=3;

				}

				return num;
		
	
}
int RC522_Operation(void)
{
	int num=0;
	u8 key=0;
	extern const u8 name[];
	u8 s=0x08;
	u8  RFID0[16]={"陈二"};  //写入信息
  u8  RFID1[16]={"2"};  //写入信息
	key = KEY_Scan(0);

	/**********写卡模式（key1按下时）*********/	
	if(key==KEY1_PRES)
	{   
		PcdWrite2(RFID0,RFID1);
		return 0;
	}
	/**********读卡模式*********/	
	if(VerficationID(s,RFIDO1)==MI_OK)
	{
        PcdRead(s+1,RFIDO2);
	    	num=(int)RFIDO2[0]-48;
		OLED_Fill(0x00);//全屏灭
	  delay_ms(2);	
		//读卡后，在oled中显示借用人的姓名与号码。
    QingShuaKa_ui(num);	
     return num;//读到卡就返回1,2,3,4		否则返回0
		}
return 0;
	}  
/***************************************************************
*函数：void PcdWrite2(u8 *RFID0,u8 *RFID1)
*功能:写卡
*入口：需要写入的两个块数据
*出口：无
*作者：江智忠
*完成时间：2017年2月27日10:22:39
*备注：
*修改日志：
*修改人：
*修改日期：
***************************************************************/
void PcdWrite2(u8 *RFID0,u8 *RFID1)
{
		u8 s=0x08;
	u8 Mkey_Flag=1;
	printf("进入写卡模式，请放卡\n"); 
		Mkey_Flag=1;
		delay_ms(50);
		while(Mkey_Flag==1)
		{
		  if(VerficationID(s,RFID)==MI_OK)
		   if(PcdWrite(s,RFID0)==MI_OK)
		     if(PcdWrite(s+1,RFID1)==MI_OK)	
						if(PcdWrite(s+2,Init_Value)==MI_OK)

		       {
				   Mkey_Flag=0;
				}
		}
		printf("写卡成功\n");		
}

/***************************************************************
*函数：u8 SPI3WriteByte(u8 Byte)
*功能：SPI发送一字节
*入口：SPI发送的字节
*出口：收到的数据
*作者：吴晓杰
*完成时间：2016年3月23日_21:36
*备注：
*修改日志：
*修改人：
*修改日期：
***************************************************************/
u8 SPI3WriteByte(u8 Byte)
{
	while((SPI3->SR&0X02)==0);		//等待发送区空	  
	SPI3->DR = Byte;	 	        //发送一个byte   
	while((SPI3->SR&0X01)==0);      //等待接收完一个byte  
	return SPI3->DR;          	    //返回收到的数据			
}
/***************************************************************
*函数：u8 SPI3_ReadWriteByte(u8 TxData)
*功能：SPI读写一个字节
*入口：SPI要写入的字节
*出口：读取到的字节
*作者：吴晓杰
*完成时间：2016年3月23日_21:36
*备注：
*修改日志：
*修改人：
*修改日期：
***************************************************************/
u8 SPI3_ReadWriteByte(u8 TxData)
{		
	u8 retry=0;				 	
	while(SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位
	{
		retry++;
		if(retry>200)return 0;
	}			  
	SPI_I2S_SendData(SPI3,TxData); //通过外设SPIx发送一个数据
	retry=0;
	while(SPI_I2S_GetFlagStatus(SPI3,SPI_I2S_FLAG_RXNE) == RESET) //检查指定的SPI标志位设置与否:接受缓存非空标志位
	{
		retry++;
		if(retry>200)return 0;
	}	  						    
	return SPI_I2S_ReceiveData(SPI3); //返回通过SPIx最近接收的数据					    
}
/***************************************************************
*函数：void SPI3_SetSpeed(u8 SPI_BaudRatePrescaler)
*功能：SPI 速度设置
*入口：SPI的速度分频
*出口：无
*作者：吴晓杰
*完成时间：2016年3月23日_21:30
*备注：1.SPI_BaudRatePrescaler_2   2分频   
	   2.SPI_BaudRatePrescaler_8   8分频   
	   3.SPI_BaudRatePrescaler_16  16分频  
	   4.SPI_BaudRatePrescaler_256 256分频
*修改日志：
*修改人：
*修改日期：
***************************************************************/
void SPI3_SetSpeed(u8 SPI_BaudRatePrescaler)
{
  	assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
	SPI3->CR1&=0XFFC7;
	SPI3->CR1|=SPI_BaudRatePrescaler;	//设置SPI3速度 
	SPI_Cmd(SPI3,ENABLE); 
} 
/***************************************************************
*函数：void SPI3_Init(void)
*功能：SPI3初始化
*入口：无
*出口：无
*作者：吴晓杰
*完成时间：2016年3月23日_21:28
*备注：
*修改日志：
*修改人：
*修改日期：
***************************************************************/
void SPI3_Init(void)	
{
	GPIO_InitTypeDef GPIO_InitStructure;
  	SPI_InitTypeDef  SPI_InitStructure;
 	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE );	
		RCC_APB1PeriphClockCmd(	RCC_APB1Periph_SPI3, ENABLE );
   GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);	
	GPIO_InitStructure.GPIO_Pin = RC522_SDA|RC522_RST;	 //IO-->
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
    GPIO_Init(RC522_IOPORT, &GPIO_InitStructure);					 //根据设定参数初始化
    GPIO_ResetBits(RC522_IOPORT,GPIO_Pin_1);			             //
    //GPIO_SetBits(GPIOF,GPIO_Pin_0);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //PB13/14/15复用推挽输出 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB
 	GPIO_SetBits(GPIOB,GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5);  //PB13/14/15上拉
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;	//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		//串行同步时钟的空闲状态为低电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;	//串行同步时钟的第一个跳变沿（上升或下降）数据被采样
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//定义波特率预分频的值:波特率预分频值为256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	SPI_Init(SPI3, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
	SPI_Cmd(SPI3, ENABLE); //使能SPI外设
	//SPI2_ReadWriteByte(0xff);//启动传输	
}
/***************************************************************
*函数：void RC522_Init(void)
*功能：RC522初始化
*入口：无
*出口：无
*作者：吴晓杰
*完成时间：2016年3月23日_21:40
*备注：
*修改日志：
*修改人：
*修改日期：
***************************************************************/
void RC522_Init(void)
{
	SPI3_Init();
	PcdReset();
	PcdAntennaOff();
	delay_ms(2);  
	PcdAntennaOn();
	M500PcdConfigISOType( 'A' );
}
/***************************************************************
*函数：void RC522_Reset(void)
*功能：RC522复位
*入口：无
*出口：无
*作者：吴晓杰
*完成时间：2016年3月23日_21:40
*备注：
*修改日志：
*修改人：
*修改日期：
***************************************************************/
void RC522_Reset(void)
{
	PcdReset();
	PcdAntennaOff();
	delay_ms(2);  
	PcdAntennaOn();
} 
/***************************************************************
*函数：char PcdRequest(u8 req_code,u8 *pTagType)
*功能：寻卡
*入口：寻卡方式和卡片类型代码
*出口：MI_OK/MI_ERR
*作者：吴晓杰
*完成时间：2016年3月23日_21:47
*备注：req_code[IN]:寻卡方式
	   0x52 = 寻感应区内所有符合14443A标准的卡
       0x26 = 寻未进入休眠状态的卡
       pTagType[OUT]：卡片类型代码
       0x4400 = Mifare_UltraLight
       0x0400 = Mifare_One(S50)
       0x0200 = Mifare_One(S70)
       0x0800 = Mifare_Pro(X)
       0x4403 = Mifare_DESFire
*修改日志：
*修改人：
*修改日期：
***************************************************************/
char PcdRequest(u8 req_code,u8 *pTagType)
{
	char status;  
	u8  unLen;
	u8  ucComMF522Buf[MAXRLEN]; 
	ClearBitMask(Status2Reg,0x08);
	WriteRawRC(BitFramingReg,0x07);
	SetBitMask(TxControlReg,0x03);
	ucComMF522Buf[0] = req_code;
	status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,1,ucComMF522Buf,&unLen);
	if((status == MI_OK) && (unLen == 0x10))
	{    
		*pTagType     = ucComMF522Buf[0];
		*(pTagType+1) = ucComMF522Buf[1];
	}
	else
	{   status = MI_ERR;   }
	return status;
}
/***************************************************************
*函数：char PcdAnticoll(u8 *pSnr)
*功能：防冲撞
*入口：卡片序列号（4字节）
*出口：MI_OK/MI_ERR
*作者：吴晓杰
*完成时间：2016年3月23日_21:50
*备注：
*修改日志：
*修改人：
*修改日期：
***************************************************************/
char PcdAnticoll(u8 *pSnr)
{
    char status;
    u8   i,snr_check=0;
    u8   unLen;
    u8   ucComMF522Buf[MAXRLEN]; 
    ClearBitMask(Status2Reg,0x08);
    WriteRawRC(BitFramingReg,0x00);
    ClearBitMask(CollReg,0x80);
    ucComMF522Buf[0] = PICC_ANTICOLL1;
    ucComMF522Buf[1] = 0x20;
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,2,ucComMF522Buf,&unLen);
    if (status == MI_OK)
    {
    	 for(i=0; i<4; i++)
         {   
             *(pSnr+i)  = ucComMF522Buf[i];
             snr_check ^= ucComMF522Buf[i];
         }
         if(snr_check != ucComMF522Buf[i])
         {   status = MI_ERR;    }
    }
    SetBitMask(CollReg,0x80);
    return status;
}
/***************************************************************
*函数：char PcdSelect(u8 *pSnr)
*功能：选定卡片
*入口：卡片序列号（4字节）
*出口：MI_OK/MI_ERR
*作者：吴晓杰
*完成时间：2016年3月23日_21:50
*备注：
*修改日志：
*修改人：
*修改日期：
***************************************************************/
char PcdSelect(u8 *pSnr)
{
    char status;
    u8   i;
    u8   unLen;
    u8   ucComMF522Buf[MAXRLEN]; 
    ucComMF522Buf[0] = PICC_ANTICOLL1;
    ucComMF522Buf[1] = 0x70;
    ucComMF522Buf[6] = 0;
    for (i=0; i<4; i++)
    {
    	ucComMF522Buf[i+2] = *(pSnr+i);
    	ucComMF522Buf[6]  ^= *(pSnr+i);
    }
    CalulateCRC(ucComMF522Buf,7,&ucComMF522Buf[7]);
    ClearBitMask(Status2Reg,0x08);
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,9,ucComMF522Buf,&unLen);
    if ((status == MI_OK) && (unLen == 0x18))
    {   status = MI_OK;  }
    else
    {   status = MI_ERR;    }
    return status;
}  
/***************************************************************
*函数：char PcdAuthState(u8 auth_mode,u8 addr,u8 *pKey,u8 *pSnr)
*功能：验证卡密码
*入口：密码验证模式、块地址、密码、卡片序列号，4字节
*出口：MI_OK/MI_ERR
*作者：吴晓杰
*完成时间：2016年3月23日_22:21
*备注：auth_mode[IN]: 密码验证模式
       0x60 = 验证A密钥
       0x61 = 验证B密钥 
       addr[IN]：块地址
       pKey[IN]：密码
       pSnr[IN]：卡片序列号，4字节
*修改日志：
*修改人：
*修改日期：
***************************************************************/
char PcdAuthState(u8 auth_mode,u8 addr,u8 *pKey,u8 *pSnr)
{
    char status;
    u8 unLen;
	u8 i,ucComMF522Buf[MAXRLEN]; 
    ucComMF522Buf[0] = auth_mode;
    ucComMF522Buf[1] = addr;
    for (i=0; i<6; i++)
    {    ucComMF522Buf[i+2] = *(pKey+i);   }
    for (i=0; i<6; i++)
    {    ucComMF522Buf[i+8] = *(pSnr+i);   }
    memcpy(&ucComMF522Buf[2], pKey, 6); 
    memcpy(&ucComMF522Buf[8], pSnr, 4); 
    status = PcdComMF522(PCD_AUTHENT,ucComMF522Buf,12,ucComMF522Buf,&unLen);
    if((status != MI_OK) || (!(ReadRawRC(Status2Reg) & 0x08)))
    {   status = MI_ERR;   }
    return status;
}
/***************************************************************
*函数：char PcdRead(u8 addr,u8 *pData)
*功能：读取M1卡一块数据
*入口：块地址、读出的数据，16字节
*出口：MI_OK/MI_ERR
*作者：吴晓杰
*完成时间：2016年3月23日_22:22
*备注：addr[IN]：块地址
       pData[OUT]：读出的数据，16字节
*修改日志：
*修改人：
*修改日期：
***************************************************************/
char PcdRead(u8 addr,u8 *pData)
{
    char status;
    u8 unLen;
    u8 i,ucComMF522Buf[MAXRLEN];
	
    ucComMF522Buf[0] = PICC_READ;
    ucComMF522Buf[1] = addr;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);
    if((status == MI_OK) && (unLen == 0x90))
    {
        for (i=0; i<16; i++)
        {    *(pData +i) = ucComMF522Buf[i];   }
    }
    else
    {   status = MI_ERR;   }
    return status;
}
/***************************************************************
*函数：char PcdWrite(u8 addr,u8 *pData)
*功能：写数据到M1卡一块
*入口：块地址、写入的数据，16字节
*出口：MI_OK/MI_ERR
*作者：吴晓杰
*完成时间：2016年3月23日_22:24
*备注：addr[IN]：块地址
       pData[IN]：写入的数据，16字节
*修改日志：
*修改人：
*修改日期：
***************************************************************/
char PcdWrite(u8 addr,u8 *pData)
{
    char status;
    u8  unLen;
    u8  i,ucComMF522Buf[MAXRLEN]; 
    ucComMF522Buf[0] = PICC_WRITE;
    ucComMF522Buf[1] = addr;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);
    if((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
    {   status = MI_ERR;   }
    if(status == MI_OK)
    {
        for (i=0; i<16; i++)
        {    
        	ucComMF522Buf[i] = *(pData +i);   
        }
        CalulateCRC(ucComMF522Buf,16,&ucComMF522Buf[16]);

        status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,18,ucComMF522Buf,&unLen);
        if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
        {   status = MI_ERR;   }
    }
    return status;
}
/***************************************************************
*函数：char PcdHalt(void)
*功能：命令卡片进入休眠状态
*入口：无
*出口：MI_OK
*作者：吴晓杰
*完成时间：2016年3月23日_22:34
*备注：
*修改日志：
*修改人：
*修改日期：
***************************************************************/
char PcdHalt(void)
{
//    char status;
//    u8 unLen;
    u8  ucComMF522Buf[MAXRLEN]; 
    ucComMF522Buf[0] = PICC_HALT;//休眠
    ucComMF522Buf[1] = 0;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
//    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);
    return MI_OK;
}
/***************************************************************
*函数：void CalulateCRC(u8 *pIn ,u8 len,u8 *pOut )
*功能：用MF522计算CRC16函数
*入口：写入数据、返回数据、长度
*出口：无
*作者：吴晓杰
*完成时间：2016年3月23日_22:39
*备注：
*修改日志：
*修改人：
*修改日期：
***************************************************************/
void CalulateCRC(u8 *pIn,u8 len,u8 *pOut )
{
    u8 i,n;
    ClearBitMask(DivIrqReg,0x04);
    WriteRawRC(CommandReg,PCD_IDLE);
    SetBitMask(FIFOLevelReg,0x80);
    for(i=0; i<len; i++)
    {   WriteRawRC(FIFODataReg, *(pIn +i));   }
    WriteRawRC(CommandReg, PCD_CALCCRC);
    i = 0xFF;
    do 
    {
        n = ReadRawRC(DivIrqReg);
        i--;
    }
    while ((i!=0) && !(n&0x04));
    pOut [0] = ReadRawRC(CRCResultRegL);
    pOut [1] = ReadRawRC(CRCResultRegM);
}
/***************************************************************
*函数：char PcdReset(void)
*功能：复位RC522
*入口：无
*出口：MI_OK
*作者：吴晓杰
*完成时间：2016年3月23日_22:40
*备注：
*修改日志：
*修改人：
*修改日期：
***************************************************************/
char PcdReset(void)
{
	SET_RC522RST;
    delay_us(100);
	CLR_RC522RST;
    delay_us(100);
	SET_RC522RST;
    delay_us(100);
    WriteRawRC(CommandReg,PCD_RESETPHASE);	//软复位RC522
    delay_us(100);  
    WriteRawRC(ModeReg,0x3D);            //和Mifare卡通讯，CRC初始值0x6363
    WriteRawRC(TReloadRegL,30);           
    WriteRawRC(TReloadRegH,0);
    WriteRawRC(TModeReg,0x8D);
    WriteRawRC(TPrescalerReg,0x3E);	
	WriteRawRC(TxAutoReg,0x40);//必须要   
    return MI_OK;
}
/***************************************************************
*函数：char M500PcdConfigISOType(u8 type)
*功能：设置RC522的工作方式 
*入口：工作方式
*出口：MI_OK
*作者：吴晓杰
*完成时间：2016年3月23日_22:42
*备注：
*修改日志：
*修改人：
*修改日期：
***************************************************************/
char M500PcdConfigISOType(u8 type)
{
   if(type == 'A')   //ISO14443_A
   { 
       ClearBitMask(Status2Reg,0x08);
       WriteRawRC(ModeReg,0x3D);//3F
       WriteRawRC(RxSelReg,0x86);//84
       WriteRawRC(RFCfgReg,0x7F);  //4F
   	   WriteRawRC(TReloadRegL,30);//tmoLength);// TReloadVal = 'h6a =tmoLength(dec) 
	   WriteRawRC(TReloadRegH,0);
       WriteRawRC(TModeReg,0x8D);
	   WriteRawRC(TPrescalerReg,0x3E);
	   delay_us(1000);
       PcdAntennaOn();
   }
   else{ return 1; }
   return MI_OK;
}
/***************************************************************
*函数：u8 ReadRawRC(u8 Address)
*功能：读RC632寄存器
*入口：寄存器地址
*出口：读到的值
*作者：吴晓杰
*完成时间：2016年3月23日_22:44
*备注：Address[IN]:寄存器地址
*修改日志：
*修改人：
*修改日期：
***************************************************************/
u8 ReadRawRC(u8 Address)
{
    u8 ucAddr;
    u8 ucResult=0;
	CLR_SPI_CS;
    ucAddr = ((Address<<1)&0x7E)|0x80;
	SPI3WriteByte(ucAddr);
	ucResult=SPI3ReadByte();
	SET_SPI_CS;
    return ucResult;
}
/***************************************************************
*函数：void WriteRawRC(u8 Address, u8 value)
*功能：写RC632寄存器
*入口：寄存器地址、写入的值
*出口：无
*作者：吴晓杰
*完成时间：2016年3月23日_22:44
*备注：Address[IN]:寄存器地址
       value[IN]:写入的值
*修改日志：
*修改人：
*修改日期：
***************************************************************/
void WriteRawRC(u8 Address,u8 value)
{  
    u8   ucAddr;
	CLR_SPI_CS;
    ucAddr = ((Address<<1)&0x7E);
	SPI3WriteByte(ucAddr);
	SPI3WriteByte(value);
	SET_SPI_CS;
}
/***************************************************************
*函数：void SetBitMask(u8 reg,u8 mask) 
*功能：置RC522寄存器位
*入口：寄存器地址、置为值
*出口：无
*作者：吴晓杰
*完成时间：2016年3月23日_22:47
*备注：reg[IN]:寄存器地址
       mask[IN]:置位值
*修改日志：
*修改人：
*修改日期：
***************************************************************/
void SetBitMask(u8 reg,u8 mask)  
{
    char tmp = 0x0;
    tmp = ReadRawRC(reg);
    WriteRawRC(reg,tmp|mask); //set bit mask
}
/***************************************************************
*函数：void ClearBitMask(u8 reg,u8 mask)  
*功能：清RC522寄存器位
*入口：寄存器地址、清位值
*出口：无
*作者：吴晓杰
*完成时间：2016年3月23日_22:49
*备注：reg[IN]:寄存器地址
	   mask[IN]:清位值
*修改日志：
*修改人：
*修改日期：
***************************************************************/
void ClearBitMask(u8 reg,u8 mask)  
{
    char tmp = 0x0;
    tmp = ReadRawRC(reg);
    WriteRawRC(reg, tmp & ~mask);  // clear bit mask
} 
/***************************************************************
*函数：char PcdComMF522(u8 Command, 
				u8 *pInData, 
				u8 InLenByte,
				u8 *pOutData, 
				u8 *pOutLenBit)
*功能：通过RC522和ISO14443卡通讯
*入口：RC522命令字、通过RC522发送到卡片的数据、发送数据的字节长度
	   接收到的卡片返回数据、返回数据的位长度
*出口：MI_OK/MI_ERR
*作者：吴晓杰
*完成时间：2016年3月23日_22:54
*备注：Command[IN]:RC522命令字
       pIn [IN]:通过RC522发送到卡片的数据
       InLenByte[IN]:发送数据的字节长度
       pOut [OUT]:接收到的卡片返回数据
       *pOutLenBit[OUT]:返回数据的位长度
*修改日志：
*修改人：
*修改日期：
***************************************************************/
char PcdComMF522(u8 Command, 
				u8 *pInData, 
				u8 InLenByte,
				u8 *pOutData, 
				u8 *pOutLenBit)
{
    char status = MI_ERR;
    u8  irqEn   = 0x00;
    u8  waitFor = 0x00;
    u8  lastBits;
    u8  n;
    u16 i;
    switch (Command)
    {
        case PCD_AUTHENT:
			irqEn   = 0x12;
			waitFor = 0x10;
			break;
		case PCD_TRANSCEIVE:
			irqEn   = 0x77;
			waitFor = 0x30;
			break;
		default:
			break;
    }
    WriteRawRC(ComIEnReg,irqEn|0x80);  //使能接受和发送中断请求
    ClearBitMask(ComIrqReg,0x80);     //置ComIrqReg为0xff,		
    WriteRawRC(CommandReg,PCD_IDLE); //取消当前命令
    SetBitMask(FIFOLevelReg,0x80);	
    for (i=0; i<InLenByte; i++)
    {   WriteRawRC(FIFODataReg, pInData[i]);  }
    WriteRawRC(CommandReg, Command);
    if (Command == PCD_TRANSCEIVE)
    {    SetBitMask(BitFramingReg,0x80);  }	  //开始发送
	i = 700;//根据时钟频率调整，操作M1卡最大等待时间25ms
    do 
    {
        n = ReadRawRC(ComIrqReg);
        i--;
    }
    while ((i!=0) && !(n&0x01) && !(n&waitFor));
    ClearBitMask(BitFramingReg,0x80);  //发送结束
    if (i!=0)
    {    
        if(!(ReadRawRC(ErrorReg)&0x1B))
        {
            status = MI_OK;
            if (n & irqEn & 0x01)
            {   status = MI_NOTAGERR;   }
            if (Command == PCD_TRANSCEIVE)
            {
               	n = ReadRawRC(FIFOLevelReg); 
              	lastBits = ReadRawRC(ControlReg) & 0x07;   //得出接受字节中的有效位，如果为0，全部位都有效
                if (lastBits)
                {   *pOutLenBit = (n-1)*8 + lastBits;   }
                else
                {   *pOutLenBit = n*8;   }
                if (n == 0)
                {   n = 1;    }
                if (n > MAXRLEN)
                {   n = MAXRLEN;   }
                for (i=0; i<n; i++)
                {   pOutData[i] = ReadRawRC(FIFODataReg);    }
            }
        }
        else
        {   status = MI_ERR;   }  
    }
    SetBitMask(ControlReg,0x80);           // stop timer now
    WriteRawRC(CommandReg,PCD_IDLE); 
    return status;
}
/***************************************************************
*函数：void PcdAntennaOn(void)
*功能：开启天线
*入口：无
*出口：无
*作者：吴晓杰
*完成时间：2016年3月23日_22:59
*备注：每次启动或关闭天险发射之间应至少有1ms的间隔
*修改日志：
*修改人：
*修改日期：
***************************************************************/
void PcdAntennaOn(void)
{
    u8 i;
    i = ReadRawRC(TxControlReg);
    if(!(i & 0x03))
    {
        SetBitMask(TxControlReg, 0x03);
    }
}
/***************************************************************
*函数：void PcdAntennaOff(void)
*功能：关闭天线
*入口：无
*出口：无
*作者：吴晓杰
*完成时间：2016年3月23日_23:01
*备注：
*修改日志：
*修改人：
*修改日期：
***************************************************************/
void PcdAntennaOff(void)
{
	ClearBitMask(TxControlReg, 0x03);
}
/***************************************************************
*函数：char PcdValue(u8 dd_mode,u8 addr,u8 *pValue)
*功能：扣款和充值
*入口：命令字、钱包地址、4字节增(减)值，低位在前
*出口：MI_OK/MI_ERR
*作者：吴晓杰
*完成时间：2016年3月24日_9:43
*备注：dd_mode[IN]：命令字
       0xC0 = 扣款
       0xC1 = 充值
       addr[IN]：钱包地址
       pValue[IN]：4字节增(减)值，低位在前
*修改日志：
*修改人：
*修改日期：
***************************************************************/
char PcdValue(u8 dd_mode,u8 addr,u8 *pValue)
{
    char status;
    u8 unLen;
    u8 ucComMF522Buf[MAXRLEN]; 
    u8 i;
    ucComMF522Buf[0] = dd_mode;
    ucComMF522Buf[1] = addr;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);
    if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
    {   status = MI_ERR;   }   
    if (status == MI_OK)
    {
        memcpy(ucComMF522Buf, pValue, 4);
        for (i=0; i<16; i++)
        {    ucComMF522Buf[i] = *(pValue+i);   }
        CalulateCRC(ucComMF522Buf,4,&ucComMF522Buf[4]);
        unLen = 0;
        status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,6,ucComMF522Buf,&unLen);
		if (status != MI_ERR)
        {    status = MI_OK;    }
    }
    
    if (status == MI_OK)
    {
        ucComMF522Buf[0] = PICC_TRANSFER;
        ucComMF522Buf[1] = addr;
        CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]); 
        status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);
        if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
        {   status = MI_ERR;   }
    }
    return status;
}
/***************************************************************
*函数：char PcdBakValue(u8 sourceaddr,u8 goaladdr)
*功能：备份钱包
*入口：源地址、目标地址
*出口：MI_OK/MI_ERR
*作者：吴晓杰
*完成时间：2016年3月24日_9:43
*备注：sourceaddr[IN]：源地址
       goaladdr[IN]：目标地址
*修改日志：
*修改人：
*修改日期：
***************************************************************/
char PcdBakValue(u8 sourceaddr,u8 goaladdr)
{
    char status;
    u8  unLen;
    u8 ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = PICC_RESTORE;
    ucComMF522Buf[1] = sourceaddr;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

    if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
    {   status = MI_ERR;   }
    
    if (status == MI_OK)
    {
        ucComMF522Buf[0] = 0;
        ucComMF522Buf[1] = 0;
        ucComMF522Buf[2] = 0;
        ucComMF522Buf[3] = 0;
        CalulateCRC(ucComMF522Buf,4,&ucComMF522Buf[4]);
 
        status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,6,ucComMF522Buf,&unLen);
		if (status != MI_ERR)
        {    status = MI_OK;    }
    }
    
    if (status != MI_OK)
    {    return MI_ERR;   }
    
    ucComMF522Buf[0] = PICC_TRANSFER;
    ucComMF522Buf[1] = goaladdr;

    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

    if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
    {   status = MI_ERR;   }

    return status;
}

