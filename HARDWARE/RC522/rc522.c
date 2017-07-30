#include "sys.h"
#include "delay.h"
#include "string.h"
#include "rc522.h"
#include "usart.h"
#include "522Init.h"
#include "key.h"
#include "OLED_I2C.h"
unsigned char thing1[4]={136,83,162,252};  //��Ƭ���к�
unsigned char thing2[4]={136,4,190,201};			//��Ƭ���к�
unsigned char thing3[4]={136,83,162,216};				//��Ƭ���к�

u8 Find_Pcd(void)			//����Ѱ������
{
  u8 status;
  PcdReset();//��λRC522
  status=PcdRequest(PICC_REQALL,CT);//Ѱ��������δ��������״̬�Ŀ������ؿ�Ƭ���� 2�ֽ�
	return status;
}
u8 Find_Thing(void)
{
	u8 num=0;
 	u8 SN[4];
       PcdAnticoll(SN);
			if((SN[0]==thing1[0])&&(SN[1]==thing1[1])&&(SN[2]==thing1[2])&&(SN[3]==thing1[3])) //�����Ϊ��1
				{
					num=1;

				}
			if((SN[0]==thing2[0])&&(SN[1]==thing2[1])&&(SN[2]==thing2[2])&&(SN[3]==thing2[3]))   //�����Ϊ��2
				{
					 num=3;;

				}  
			if((SN[0]==thing3[0])&&(SN[1]==thing3[1])&&(SN[2]==thing3[2])&&(SN[3]==thing3[3]))    //�����Ϊ��3
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
	u8  RFID0[16]={"�¶�"};  //д����Ϣ
  u8  RFID1[16]={"2"};  //д����Ϣ
	key = KEY_Scan(0);

	/**********д��ģʽ��key1����ʱ��*********/	
	if(key==KEY1_PRES)
	{   
		PcdWrite2(RFID0,RFID1);
		return 0;
	}
	/**********����ģʽ*********/	
	if(VerficationID(s,RFIDO1)==MI_OK)
	{
        PcdRead(s+1,RFIDO2);
	    	num=(int)RFIDO2[0]-48;
		OLED_Fill(0x00);//ȫ����
	  delay_ms(2);	
		//��������oled����ʾ�����˵���������롣
    QingShuaKa_ui(num);	
     return num;//�������ͷ���1,2,3,4		���򷵻�0
		}
return 0;
	}  
/***************************************************************
*������void PcdWrite2(u8 *RFID0,u8 *RFID1)
*����:д��
*��ڣ���Ҫд�������������
*���ڣ���
*���ߣ�������
*���ʱ�䣺2017��2��27��10:22:39
*��ע��
*�޸���־��
*�޸��ˣ�
*�޸����ڣ�
***************************************************************/
void PcdWrite2(u8 *RFID0,u8 *RFID1)
{
		u8 s=0x08;
	u8 Mkey_Flag=1;
	printf("����д��ģʽ����ſ�\n"); 
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
		printf("д���ɹ�\n");		
}

/***************************************************************
*������u8 SPI3WriteByte(u8 Byte)
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
u8 SPI3WriteByte(u8 Byte)
{
	while((SPI3->SR&0X02)==0);		//�ȴ���������	  
	SPI3->DR = Byte;	 	        //����һ��byte   
	while((SPI3->SR&0X01)==0);      //�ȴ�������һ��byte  
	return SPI3->DR;          	    //�����յ�������			
}
/***************************************************************
*������u8 SPI3_ReadWriteByte(u8 TxData)
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
u8 SPI3_ReadWriteByte(u8 TxData)
{		
	u8 retry=0;				 	
	while(SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET) //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
	{
		retry++;
		if(retry>200)return 0;
	}			  
	SPI_I2S_SendData(SPI3,TxData); //ͨ������SPIx����һ������
	retry=0;
	while(SPI_I2S_GetFlagStatus(SPI3,SPI_I2S_FLAG_RXNE) == RESET) //���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
	{
		retry++;
		if(retry>200)return 0;
	}	  						    
	return SPI_I2S_ReceiveData(SPI3); //����ͨ��SPIx������յ�����					    
}
/***************************************************************
*������void SPI3_SetSpeed(u8 SPI_BaudRatePrescaler)
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
void SPI3_SetSpeed(u8 SPI_BaudRatePrescaler)
{
  	assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
	SPI3->CR1&=0XFFC7;
	SPI3->CR1|=SPI_BaudRatePrescaler;	//����SPI3�ٶ� 
	SPI_Cmd(SPI3,ENABLE); 
} 
/***************************************************************
*������void SPI3_Init(void)
*���ܣ�SPI3��ʼ��
*��ڣ���
*���ڣ���
*���ߣ�������
*���ʱ�䣺2016��3��23��_21:28
*��ע��
*�޸���־��
*�޸��ˣ�
*�޸����ڣ�
***************************************************************/
void SPI3_Init(void)	
{
	GPIO_InitTypeDef GPIO_InitStructure;
  	SPI_InitTypeDef  SPI_InitStructure;
 	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE );	
		RCC_APB1PeriphClockCmd(	RCC_APB1Periph_SPI3, ENABLE );
   GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);	
	GPIO_InitStructure.GPIO_Pin = RC522_SDA|RC522_RST;	 //IO-->
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
    GPIO_Init(RC522_IOPORT, &GPIO_InitStructure);					 //�����趨������ʼ��
    GPIO_ResetBits(RC522_IOPORT,GPIO_Pin_1);			             //
    //GPIO_SetBits(GPIOF,GPIO_Pin_0);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //PB13/14/15����������� 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB
 	GPIO_SetBits(GPIOB,GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5);  //PB13/14/15����
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;	//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		//����ͬ��ʱ�ӵĿ���״̬Ϊ�͵�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;	//����ͬ��ʱ�ӵĵ�һ�������أ��������½������ݱ�����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
	SPI_Init(SPI3, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
	SPI_Cmd(SPI3, ENABLE); //ʹ��SPI����
	//SPI2_ReadWriteByte(0xff);//��������	
}
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
void RC522_Reset(void)
{
	PcdReset();
	PcdAntennaOff();
	delay_ms(2);  
	PcdAntennaOn();
} 
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
char PcdHalt(void)
{
//    char status;
//    u8 unLen;
    u8  ucComMF522Buf[MAXRLEN]; 
    ucComMF522Buf[0] = PICC_HALT;//����
    ucComMF522Buf[1] = 0;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
//    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);
    return MI_OK;
}
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
char PcdReset(void)
{
	SET_RC522RST;
    delay_us(100);
	CLR_RC522RST;
    delay_us(100);
	SET_RC522RST;
    delay_us(100);
    WriteRawRC(CommandReg,PCD_RESETPHASE);	//��λRC522
    delay_us(100);  
    WriteRawRC(ModeReg,0x3D);            //��Mifare��ͨѶ��CRC��ʼֵ0x6363
    WriteRawRC(TReloadRegL,30);           
    WriteRawRC(TReloadRegH,0);
    WriteRawRC(TModeReg,0x8D);
    WriteRawRC(TPrescalerReg,0x3E);	
	WriteRawRC(TxAutoReg,0x40);//����Ҫ   
    return MI_OK;
}
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
void SetBitMask(u8 reg,u8 mask)  
{
    char tmp = 0x0;
    tmp = ReadRawRC(reg);
    WriteRawRC(reg,tmp|mask); //set bit mask
}
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
void ClearBitMask(u8 reg,u8 mask)  
{
    char tmp = 0x0;
    tmp = ReadRawRC(reg);
    WriteRawRC(reg, tmp & ~mask);  // clear bit mask
} 
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
    WriteRawRC(ComIEnReg,irqEn|0x80);  //ʹ�ܽ��ܺͷ����ж�����
    ClearBitMask(ComIrqReg,0x80);     //��ComIrqRegΪ0xff,		
    WriteRawRC(CommandReg,PCD_IDLE); //ȡ����ǰ����
    SetBitMask(FIFOLevelReg,0x80);	
    for (i=0; i<InLenByte; i++)
    {   WriteRawRC(FIFODataReg, pInData[i]);  }
    WriteRawRC(CommandReg, Command);
    if (Command == PCD_TRANSCEIVE)
    {    SetBitMask(BitFramingReg,0x80);  }	  //��ʼ����
	i = 700;//����ʱ��Ƶ�ʵ���������M1�����ȴ�ʱ��25ms
    do 
    {
        n = ReadRawRC(ComIrqReg);
        i--;
    }
    while ((i!=0) && !(n&0x01) && !(n&waitFor));
    ClearBitMask(BitFramingReg,0x80);  //���ͽ���
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
              	lastBits = ReadRawRC(ControlReg) & 0x07;   //�ó������ֽ��е���Чλ�����Ϊ0��ȫ��λ����Ч
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
void PcdAntennaOff(void)
{
	ClearBitMask(TxControlReg, 0x03);
}
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

