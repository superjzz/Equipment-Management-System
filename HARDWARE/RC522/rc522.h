#ifndef _RC552_H_
#define _RC552_H_

#include <stm32f10x.h>
#include "sys.h"

/////////////////////////////////////////////////////////////////////
//MF522命令字
/////////////////////////////////////////////////////////////////////
#define PCD_IDLE              0x00               //取消当前命令
#define PCD_AUTHENT           0x0E               //验证密钥
#define PCD_RECEIVE           0x08               //接收数据
#define PCD_TRANSMIT          0x04               //发送数据
#define PCD_TRANSCEIVE        0x0C               //发送并接收数据
#define PCD_RESETPHASE        0x0F               //复位
#define PCD_CALCCRC           0x03               //CRC计算

/////////////////////////////////////////////////////////////////////
//Mifare_One卡片命令字
/////////////////////////////////////////////////////////////////////
#define PICC_REQIDL           0x26               //寻天线区内未进入休眠状态
#define PICC_REQALL           0x52               //寻天线区内全部卡
#define PICC_ANTICOLL1        0x93               //防冲撞
#define PICC_ANTICOLL2        0x95               //防冲撞
#define PICC_AUTHENT1A        0x60               //验证A密钥
#define PICC_AUTHENT1B        0x61               //验证B密钥
#define PICC_READ             0x30               //读块
#define PICC_WRITE            0xA0               //写块
#define PICC_DECREMENT        0xC0               //扣款
#define PICC_INCREMENT        0xC1               //充值
#define PICC_RESTORE          0xC2               //调块数据到缓冲区
#define PICC_TRANSFER         0xB0               //保存缓冲区中数据
#define PICC_HALT             0x50               //休眠

/////////////////////////////////////////////////////////////////////
//MF522 FIFO长度定义
/////////////////////////////////////////////////////////////////////
#define DEF_FIFO_LENGTH       64                 //FIFO size=64byte
#define MAXRLEN  18

/////////////////////////////////////////////////////////////////////
//MF522寄存器定义
/////////////////////////////////////////////////////////////////////
// PAGE 0
#define     RFU00                 0x00    
#define     CommandReg            0x01    
#define     ComIEnReg             0x02    
#define     DivlEnReg             0x03    
#define     ComIrqReg             0x04    
#define     DivIrqReg             0x05
#define     ErrorReg              0x06    
#define     Status1Reg            0x07    
#define     Status2Reg            0x08    
#define     FIFODataReg           0x09
#define     FIFOLevelReg          0x0A
#define     WaterLevelReg         0x0B
#define     ControlReg            0x0C
#define     BitFramingReg         0x0D
#define     CollReg               0x0E
#define     RFU0F                 0x0F
// PAGE 1     
#define     RFU10                 0x10
#define     ModeReg               0x11
#define     TxModeReg             0x12
#define     RxModeReg             0x13
#define     TxControlReg          0x14
#define     TxAutoReg             0x15
#define     TxSelReg              0x16
#define     RxSelReg              0x17
#define     RxThresholdReg        0x18
#define     DemodReg              0x19
#define     RFU1A                 0x1A
#define     RFU1B                 0x1B
#define     MifareReg             0x1C
#define     RFU1D                 0x1D
#define     RFU1E                 0x1E
#define     SerialSpeedReg        0x1F
// PAGE 2    
#define     RFU20                 0x20  
#define     CRCResultRegM         0x21
#define     CRCResultRegL         0x22
#define     RFU23                 0x23
#define     ModWidthReg           0x24
#define     RFU25                 0x25
#define     RFCfgReg              0x26
#define     GsNReg                0x27
#define     CWGsCfgReg            0x28
#define     ModGsCfgReg           0x29
#define     TModeReg              0x2A
#define     TPrescalerReg         0x2B
#define     TReloadRegH           0x2C
#define     TReloadRegL           0x2D
#define     TCounterValueRegH     0x2E
#define     TCounterValueRegL     0x2F
// PAGE 3      
#define     RFU30                 0x30
#define     TestSel1Reg           0x31
#define     TestSel2Reg           0x32
#define     TestPinEnReg          0x33
#define     TestPinValueReg       0x34
#define     TestBusReg            0x35
#define     AutoTestReg           0x36
#define     VersionReg            0x37
#define     AnalogTestReg         0x38
#define     TestDAC1Reg           0x39  
#define     TestDAC2Reg           0x3A   
#define     TestADCReg            0x3B   
#define     RFU3C                 0x3C   
#define     RFU3D                 0x3D   
#define     RFU3E                 0x3E   
#define     RFU3F		  		  0x3F

/////////////////////////////////////////////////////////////////////
//和MF522通讯时返回的错误代码
/////////////////////////////////////////////////////////////////////
#define 	MI_OK                 0
#define 	MI_NOTAGERR           (1)
#define 	MI_ERR                (2)

#define	SHAQU1	0X01
#define	KUAI4	0X04
#define	KUAI7	0X07
#define	REGCARD	0xa1
#define	CONSUME	0xa2
#define READCARD	0xa3
#define ADDMONEY	0xa4

#define RC522_IORCC		RCC_APB2Periph_GPIOB
#define RC522_IOPORT	GPIOB
#define RC522_SDA		GPIO_Pin_0
#define RC522_RST		GPIO_Pin_1
#define SET_SPI_CS  	(RC522_IOPORT->BSRR|=1<<0)
#define CLR_SPI_CS  	(RC522_IOPORT->BRR|=1<<0)
#define SET_RC522RST  	(RC522_IOPORT->BSRR|=1<<1)
#define CLR_RC522RST  	(RC522_IOPORT->BRR|=1<<1)
#define SPI3ReadByte()	SPI3WriteByte(0)
u8 Find_Pcd(void);
u8 Find_Thing(void);
int RC522_Operation(void);
void PcdWrite2(u8 *RFID0,u8 *RFID1);
/***************************************************************
*函数：u8 SPIWriteByte(u8 Byte)
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
u8 SPI3WriteByte(u8 Byte);
/***************************************************************
*函数：u8 SPI2_ReadWriteByte(u8 TxData)
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
u8 SPI3_ReadWriteByte(u8 TxData);
/***************************************************************
*函数：void SPI2_SetSpeed(u8 SPI_BaudRatePrescaler)
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
void SPI3_SetSpeed(u8 SPI_BaudRatePrescaler);
/***************************************************************
*函数：void SPI2_Init(void)
*功能：SPI2初始化
*入口：无
*出口：无
*作者：吴晓杰
*完成时间：2016年3月23日_21:28
*备注：
*修改日志：
*修改人：
*修改日期：
***************************************************************/
void SPI3_Init(void);
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
void RC522_Init(void);
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
void RC522_Reset(void);
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
char PcdRequest(u8 req_code,u8 *pTagType);
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
char PcdAnticoll(u8 *pSnr);
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
char PcdSelect(u8 *pSnr);
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
char PcdAuthState(u8 auth_mode,u8 addr,u8 *pKey,u8 *pSnr);
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
char PcdRead(u8 addr,u8 *pData);
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
char PcdWrite(u8 addr,u8 *pData);
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
char PcdHalt(void);
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
void CalulateCRC(u8 *pIn,u8 len,u8 *pOut );
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
char PcdReset(void);
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
char M500PcdConfigISOType(u8 type);
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
u8 ReadRawRC(u8 Address);
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
void WriteRawRC(u8 Address,u8 value);
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
void SetBitMask(u8 reg,u8 mask);
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
void ClearBitMask(u8 reg,u8 mask);
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
				u8 *pOutLenBit);
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
void PcdAntennaOn(void);
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
void PcdAntennaOff(void);
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
char PcdValue(u8 dd_mode,u8 addr,u8 *pValue);
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
char PcdBakValue(u8 sourceaddr,u8 goaladdr);

#endif


