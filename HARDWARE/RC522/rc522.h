#ifndef _RC552_H_
#define _RC552_H_

#include <stm32f10x.h>
#include "sys.h"

/////////////////////////////////////////////////////////////////////
//MF522������
/////////////////////////////////////////////////////////////////////
#define PCD_IDLE              0x00               //ȡ����ǰ����
#define PCD_AUTHENT           0x0E               //��֤��Կ
#define PCD_RECEIVE           0x08               //��������
#define PCD_TRANSMIT          0x04               //��������
#define PCD_TRANSCEIVE        0x0C               //���Ͳ���������
#define PCD_RESETPHASE        0x0F               //��λ
#define PCD_CALCCRC           0x03               //CRC����

/////////////////////////////////////////////////////////////////////
//Mifare_One��Ƭ������
/////////////////////////////////////////////////////////////////////
#define PICC_REQIDL           0x26               //Ѱ��������δ��������״̬
#define PICC_REQALL           0x52               //Ѱ��������ȫ����
#define PICC_ANTICOLL1        0x93               //����ײ
#define PICC_ANTICOLL2        0x95               //����ײ
#define PICC_AUTHENT1A        0x60               //��֤A��Կ
#define PICC_AUTHENT1B        0x61               //��֤B��Կ
#define PICC_READ             0x30               //����
#define PICC_WRITE            0xA0               //д��
#define PICC_DECREMENT        0xC0               //�ۿ�
#define PICC_INCREMENT        0xC1               //��ֵ
#define PICC_RESTORE          0xC2               //�������ݵ�������
#define PICC_TRANSFER         0xB0               //���滺����������
#define PICC_HALT             0x50               //����

/////////////////////////////////////////////////////////////////////
//MF522 FIFO���ȶ���
/////////////////////////////////////////////////////////////////////
#define DEF_FIFO_LENGTH       64                 //FIFO size=64byte
#define MAXRLEN  18

/////////////////////////////////////////////////////////////////////
//MF522�Ĵ�������
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
//��MF522ͨѶʱ���صĴ������
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
*������u8 SPIWriteByte(u8 Byte)
*���ܣ�SPI����һ�ֽ�
*��ڣ�SPI���͵��ֽ�
*���ڣ��յ�������
*���ߣ�������
*���ʱ�䣺2016��3��23��_21:36
*��ע��
*�޸���־��
*�޸��ˣ�
*�޸����ڣ�
***************************************************************/
u8 SPI3WriteByte(u8 Byte);
/***************************************************************
*������u8 SPI2_ReadWriteByte(u8 TxData)
*���ܣ�SPI��дһ���ֽ�
*��ڣ�SPIҪд����ֽ�
*���ڣ���ȡ�����ֽ�
*���ߣ�������
*���ʱ�䣺2016��3��23��_21:36
*��ע��
*�޸���־��
*�޸��ˣ�
*�޸����ڣ�
***************************************************************/
u8 SPI3_ReadWriteByte(u8 TxData);
/***************************************************************
*������void SPI2_SetSpeed(u8 SPI_BaudRatePrescaler)
*���ܣ�SPI �ٶ�����
*��ڣ�SPI���ٶȷ�Ƶ
*���ڣ���
*���ߣ�������
*���ʱ�䣺2016��3��23��_21:30
*��ע��1.SPI_BaudRatePrescaler_2   2��Ƶ   
	   2.SPI_BaudRatePrescaler_8   8��Ƶ   
	   3.SPI_BaudRatePrescaler_16  16��Ƶ  
	   4.SPI_BaudRatePrescaler_256 256��Ƶ
*�޸���־��
*�޸��ˣ�
*�޸����ڣ�
***************************************************************/
void SPI3_SetSpeed(u8 SPI_BaudRatePrescaler);
/***************************************************************
*������void SPI2_Init(void)
*���ܣ�SPI2��ʼ��
*��ڣ���
*���ڣ���
*���ߣ�������
*���ʱ�䣺2016��3��23��_21:28
*��ע��
*�޸���־��
*�޸��ˣ�
*�޸����ڣ�
***************************************************************/
void SPI3_Init(void);
/***************************************************************
*������void RC522_Init(void)
*���ܣ�RC522��ʼ��
*��ڣ���
*���ڣ���
*���ߣ�������
*���ʱ�䣺2016��3��23��_21:40
*��ע��
*�޸���־��
*�޸��ˣ�
*�޸����ڣ�
***************************************************************/
void RC522_Init(void);
/***************************************************************
*������void RC522_Reset(void)
*���ܣ�RC522��λ
*��ڣ���
*���ڣ���
*���ߣ�������
*���ʱ�䣺2016��3��23��_21:40
*��ע��
*�޸���־��
*�޸��ˣ�
*�޸����ڣ�
***************************************************************/
void RC522_Reset(void);
/***************************************************************
*������char PcdRequest(u8 req_code,u8 *pTagType)
*���ܣ�Ѱ��
*��ڣ�Ѱ����ʽ�Ϳ�Ƭ���ʹ���
*���ڣ�MI_OK/MI_ERR
*���ߣ�������
*���ʱ�䣺2016��3��23��_21:47
*��ע��req_code[IN]:Ѱ����ʽ
	   0x52 = Ѱ��Ӧ�������з���14443A��׼�Ŀ�
       0x26 = Ѱδ��������״̬�Ŀ�
       pTagType[OUT]����Ƭ���ʹ���
       0x4400 = Mifare_UltraLight
       0x0400 = Mifare_One(S50)
       0x0200 = Mifare_One(S70)
       0x0800 = Mifare_Pro(X)
       0x4403 = Mifare_DESFire
*�޸���־��
*�޸��ˣ�
*�޸����ڣ�
***************************************************************/
char PcdRequest(u8 req_code,u8 *pTagType);
/***************************************************************
*������char PcdAnticoll(u8 *pSnr)
*���ܣ�����ײ
*��ڣ���Ƭ���кţ�4�ֽڣ�
*���ڣ�MI_OK/MI_ERR
*���ߣ�������
*���ʱ�䣺2016��3��23��_21:50
*��ע��
*�޸���־��
*�޸��ˣ�
*�޸����ڣ�
***************************************************************/
char PcdAnticoll(u8 *pSnr);
/***************************************************************
*������char PcdSelect(u8 *pSnr)
*���ܣ�ѡ����Ƭ
*��ڣ���Ƭ���кţ�4�ֽڣ�
*���ڣ�MI_OK/MI_ERR
*���ߣ�������
*���ʱ�䣺2016��3��23��_21:50
*��ע��
*�޸���־��
*�޸��ˣ�
*�޸����ڣ�
***************************************************************/
char PcdSelect(u8 *pSnr);
/***************************************************************
*������char PcdAuthState(u8 auth_mode,u8 addr,u8 *pKey,u8 *pSnr)
*���ܣ���֤������
*��ڣ�������֤ģʽ�����ַ�����롢��Ƭ���кţ�4�ֽ�
*���ڣ�MI_OK/MI_ERR
*���ߣ�������
*���ʱ�䣺2016��3��23��_22:21
*��ע��auth_mode[IN]: ������֤ģʽ
       0x60 = ��֤A��Կ
       0x61 = ��֤B��Կ 
       addr[IN]�����ַ
       pKey[IN]������
       pSnr[IN]����Ƭ���кţ�4�ֽ�
*�޸���־��
*�޸��ˣ�
*�޸����ڣ�
***************************************************************/
char PcdAuthState(u8 auth_mode,u8 addr,u8 *pKey,u8 *pSnr);
/***************************************************************
*������char PcdRead(u8 addr,u8 *pData)
*���ܣ���ȡM1��һ������
*��ڣ����ַ�����������ݣ�16�ֽ�
*���ڣ�MI_OK/MI_ERR
*���ߣ�������
*���ʱ�䣺2016��3��23��_22:22
*��ע��addr[IN]�����ַ
       pData[OUT]�����������ݣ�16�ֽ�
*�޸���־��
*�޸��ˣ�
*�޸����ڣ�
***************************************************************/
char PcdRead(u8 addr,u8 *pData);
/***************************************************************
*������char PcdWrite(u8 addr,u8 *pData)
*���ܣ�д���ݵ�M1��һ��
*��ڣ����ַ��д������ݣ�16�ֽ�
*���ڣ�MI_OK/MI_ERR
*���ߣ�������
*���ʱ�䣺2016��3��23��_22:24
*��ע��addr[IN]�����ַ
       pData[IN]��д������ݣ�16�ֽ�
*�޸���־��
*�޸��ˣ�
*�޸����ڣ�
***************************************************************/
char PcdWrite(u8 addr,u8 *pData);
/***************************************************************
*������char PcdHalt(void)
*���ܣ����Ƭ��������״̬
*��ڣ���
*���ڣ�MI_OK
*���ߣ�������
*���ʱ�䣺2016��3��23��_22:34
*��ע��
*�޸���־��
*�޸��ˣ�
*�޸����ڣ�
***************************************************************/
char PcdHalt(void);
/***************************************************************
*������void CalulateCRC(u8 *pIn ,u8 len,u8 *pOut )
*���ܣ���MF522����CRC16����
*��ڣ�д�����ݡ��������ݡ�����
*���ڣ���
*���ߣ�������
*���ʱ�䣺2016��3��23��_22:39
*��ע��
*�޸���־��
*�޸��ˣ�
*�޸����ڣ�
***************************************************************/
void CalulateCRC(u8 *pIn,u8 len,u8 *pOut );
/***************************************************************
*������char PcdReset(void)
*���ܣ���λRC522
*��ڣ���
*���ڣ�MI_OK
*���ߣ�������
*���ʱ�䣺2016��3��23��_22:40
*��ע��
*�޸���־��
*�޸��ˣ�
*�޸����ڣ�
***************************************************************/
char PcdReset(void);
/***************************************************************
*������char M500PcdConfigISOType(u8 type)
*���ܣ�����RC522�Ĺ�����ʽ 
*��ڣ�������ʽ
*���ڣ�MI_OK
*���ߣ�������
*���ʱ�䣺2016��3��23��_22:42
*��ע��
*�޸���־��
*�޸��ˣ�
*�޸����ڣ�
***************************************************************/
char M500PcdConfigISOType(u8 type);
/***************************************************************
*������u8 ReadRawRC(u8 Address)
*���ܣ���RC632�Ĵ���
*��ڣ��Ĵ�����ַ
*���ڣ�������ֵ
*���ߣ�������
*���ʱ�䣺2016��3��23��_22:44
*��ע��Address[IN]:�Ĵ�����ַ
*�޸���־��
*�޸��ˣ�
*�޸����ڣ�
***************************************************************/
u8 ReadRawRC(u8 Address);
/***************************************************************
*������void WriteRawRC(u8 Address, u8 value)
*���ܣ�дRC632�Ĵ���
*��ڣ��Ĵ�����ַ��д���ֵ
*���ڣ���
*���ߣ�������
*���ʱ�䣺2016��3��23��_22:44
*��ע��Address[IN]:�Ĵ�����ַ
       value[IN]:д���ֵ
*�޸���־��
*�޸��ˣ�
*�޸����ڣ�
***************************************************************/
void WriteRawRC(u8 Address,u8 value);
/***************************************************************
*������void SetBitMask(u8 reg,u8 mask) 
*���ܣ���RC522�Ĵ���λ
*��ڣ��Ĵ�����ַ����Ϊֵ
*���ڣ���
*���ߣ�������
*���ʱ�䣺2016��3��23��_22:47
*��ע��reg[IN]:�Ĵ�����ַ
       mask[IN]:��λֵ
*�޸���־��
*�޸��ˣ�
*�޸����ڣ�
***************************************************************/
void SetBitMask(u8 reg,u8 mask);
/***************************************************************
*������void ClearBitMask(u8 reg,u8 mask)  
*���ܣ���RC522�Ĵ���λ
*��ڣ��Ĵ�����ַ����λֵ
*���ڣ���
*���ߣ�������
*���ʱ�䣺2016��3��23��_22:49
*��ע��reg[IN]:�Ĵ�����ַ
	   mask[IN]:��λֵ
*�޸���־��
*�޸��ˣ�
*�޸����ڣ�
***************************************************************/
void ClearBitMask(u8 reg,u8 mask);
/***************************************************************
*������char PcdComMF522(u8 Command, 
				u8 *pInData, 
				u8 InLenByte,
				u8 *pOutData, 
				u8 *pOutLenBit)
*���ܣ�ͨ��RC522��ISO14443��ͨѶ
*��ڣ�RC522�����֡�ͨ��RC522���͵���Ƭ�����ݡ��������ݵ��ֽڳ���
	   ���յ��Ŀ�Ƭ�������ݡ��������ݵ�λ����
*���ڣ�MI_OK/MI_ERR
*���ߣ�������
*���ʱ�䣺2016��3��23��_22:54
*��ע��Command[IN]:RC522������
       pIn [IN]:ͨ��RC522���͵���Ƭ������
       InLenByte[IN]:�������ݵ��ֽڳ���
       pOut [OUT]:���յ��Ŀ�Ƭ��������
       *pOutLenBit[OUT]:�������ݵ�λ����
*�޸���־��
*�޸��ˣ�
*�޸����ڣ�
***************************************************************/
char PcdComMF522(u8 Command, 
				u8 *pInData, 
				u8 InLenByte,
				u8 *pOutData, 
				u8 *pOutLenBit);
/***************************************************************
*������void PcdAntennaOn(void)
*���ܣ���������
*��ڣ���
*���ڣ���
*���ߣ�������
*���ʱ�䣺2016��3��23��_22:59
*��ע��ÿ��������ر����շ���֮��Ӧ������1ms�ļ��
*�޸���־��
*�޸��ˣ�
*�޸����ڣ�
***************************************************************/
void PcdAntennaOn(void);
/***************************************************************
*������void PcdAntennaOff(void)
*���ܣ��ر�����
*��ڣ���
*���ڣ���
*���ߣ�������
*���ʱ�䣺2016��3��23��_23:01
*��ע��
*�޸���־��
*�޸��ˣ�
*�޸����ڣ�
***************************************************************/
void PcdAntennaOff(void);
/***************************************************************
*������char PcdValue(u8 dd_mode,u8 addr,u8 *pValue)
*���ܣ��ۿ�ͳ�ֵ
*��ڣ������֡�Ǯ����ַ��4�ֽ���(��)ֵ����λ��ǰ
*���ڣ�MI_OK/MI_ERR
*���ߣ�������
*���ʱ�䣺2016��3��24��_9:43
*��ע��dd_mode[IN]��������
       0xC0 = �ۿ�
       0xC1 = ��ֵ
       addr[IN]��Ǯ����ַ
       pValue[IN]��4�ֽ���(��)ֵ����λ��ǰ
*�޸���־��
*�޸��ˣ�
*�޸����ڣ�
***************************************************************/
char PcdValue(u8 dd_mode,u8 addr,u8 *pValue);
/***************************************************************
*������char PcdBakValue(u8 sourceaddr,u8 goaladdr)
*���ܣ�����Ǯ��
*��ڣ�Դ��ַ��Ŀ���ַ
*���ڣ�MI_OK/MI_ERR
*���ߣ�������
*���ʱ�䣺2016��3��24��_9:43
*��ע��sourceaddr[IN]��Դ��ַ
       goaladdr[IN]��Ŀ���ַ
*�޸���־��
*�޸��ˣ�
*�޸����ڣ�
***************************************************************/
char PcdBakValue(u8 sourceaddr,u8 goaladdr);

#endif


