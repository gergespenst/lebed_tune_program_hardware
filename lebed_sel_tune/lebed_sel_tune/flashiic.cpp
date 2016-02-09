/*
 * flashiic.c
 *
 * Created: 19.09.2014 13:48:43
 *  Author: USER
 */ 
#include "flashiic.h"
//////////////////////////////////////////////////////////////////////////
void WriteData2FlashIIC(unsigned int startAddr,uchar *data,uchar numOfDataBytes){//������ � ������ n ���� �� ������
	// ���������� ������������ ������. ��������� �������� �� ������� �������
	//IICStart();
	//if(IICByteWrite( (DEVICE_ADDR + WRITE) ) == 0x01)				//������ ������ ����������
	//SendCOMBytes((uchar*)"No ask",6);
	//if(IICByteWrite( (uchar)((startAddr&0xFF00) >> 8)) == 0x01)		//������� ����� ������ ������
	//SendCOMBytes((uchar*)"No ask",6);
	//if(IICByteWrite( (uchar)((startAddr&0x00FF))) == 0x01)			//������� ����� ������ ������
	//SendCOMBytes((uchar*)"No ask",6);
	//for (uchar i = 0; i < numOfDataBytes; i++)									//����� ������
	//{
	//if(IICByteWrite( data[i] ) == 0x01)
	//SendCOMBytes((uchar*)"No ask",6);
	////SendCOMBytes(&data[i],1);
	//}
	//IICStop();	//����� ��������
	char i=0;
	//���������� ��������� ������
	for ( i = 0; i < numOfDataBytes; i++)									//����� ������
	{
		IICStart();
		if(IICByteWrite( (DEVICE_ADDR + WRITE) ) == 0x01)				//������ ������ ����������
		ErrorFlash();
		if(IICByteWrite( (uchar)(((startAddr+i)&0xFF00) >> 8)) == 0x01)		//������� ����� ������ ������
		ErrorFlash();
		if(IICByteWrite( (uchar)(((startAddr+i)&0x00FF))) == 0x01)			//������� ����� ������ ������
		ErrorFlash();

		if(IICByteWrite( data[i] ) == 0x01)
		ErrorFlash();
		//SendCOMBytes(&data[i],1);
		
		IICStop();
		_delay_ms(5);
	}
	
}

//////////////////////////////////////////////////////////////////////////

void ReadDataInFlashIIC(unsigned int* startAddr,uchar *data,uchar numOfDataBytes)
{//������������ ������������ ������ � ������
	char i=0;
	IICStart();
	if(IICByteWrite( (DEVICE_ADDR + WRITE) ) == 0x01)				//������ ������ ����������
		i++;
	if(IICByteWrite( (uchar)((*startAddr&0xFF00) >> 8)) == 0x01)		//������� ����� ������ ������
		i++;
	if(IICByteWrite( (uchar)((*startAddr&0x00FF))) == 0x01)			//������� ����� ������ ������
		i++;
	IICStart();
	if(IICByteWrite( (DEVICE_ADDR + READ) ) == 0x01)				//������ ������ ����������
		i++;
	if (i > 0) ErrorFlash();
	for ( i = 0; i < numOfDataBytes; i++)									//��������� ������ �� ������. � �������������� ������� ��� ACK
	{
		data[i] = IICByteRead((uchar) (i == (numOfDataBytes - 1)));
	//	uart_putchar(i);
	}
	//SendCOMBytes(data,6);
	IICStop();
}
//////////////////////////////////////////////////////////////////////////
void ReadLCAoutFromFlash(unsigned int addr,uchar *data)
{//������� ��� ������ ������ �� EEPROM ����������� �� flash
	//uchar temp_data[DATASIZE];
	//uart_putchar('a');
	ReadDataInFlashIIC(&addr,data,DATASIZE);
	//SendCOMBytes((uchar*)data,6);
	//eeprom_update_block(temp_data,data,DATASIZE);
	
}
//////////////////////////////////////////////////////////////////////////
void WriteLCAout2Flash(unsigned int addr,uchar *data){//������� ��� ������ ������ �� EEPROM ����������� �� flash
	//	uchar temp_data[6];
	//	eeprom_read_block(temp_data,data,6);
	WriteData2FlashIIC(addr,data,DATASIZE);
	//SendCOMBytes((uchar*)data,6);
}
//////////////////////////////////////////////////////////////////////////
//������� ������ �������������� ������������
void WriteDetCal2Flash( unsigned int addr,uchar* data )
{
	unsigned int shift_addr = addr + DETCAL_START_ADDR;
	WriteData2FlashIIC(shift_addr,data,1);
}
//������� ������ �������������� ������������
void ReadDetCalInFlash( unsigned int addr,uchar* data )
{
	unsigned int shift_addr = addr + DETCAL_START_ADDR;
	ReadDataInFlashIIC(&shift_addr,data,1);
}


/************************************************************************/
/*  ������� �������������� ������� � �����								*/
/************************************************************************/
static const unsigned int Fr[NDIAP + 1]  ={150,300,600,1200,2400,3000};//��������� ������� ����������
static const unsigned int df[NDIAP + 1]  ={5,  10, 20,  40,  80, 100};//���� ������ � ����������
static const unsigned int N[NDIAP + 1]  = { 300,300, 300, 300,     75};//���������� ����� � ����������
//////////////////////////////////////////////////////////////////////////
unsigned int //���������� ������� ������� �� flash �������� ������ � ���� ������� �� ���� ��� �� ����������� ������� � ����� ����
	IndOfF(unsigned int f){
	unsigned int i = 0, Fc = 0, Nd = 0xFFFF, Npc = 0, Nf = 0;// Fc - ������� ��� ���� �������, Nd - ����� ���������, Npc - ����� ������� � ���������, Nf - ����� ������� �� ������
	unsigned int Fcl = 0;// ������� ������� ������� ��� ������� � ����� ������� ��������������
	Fc = f;//�������� ������� � ���� Fr, ������� � 10 ���
	for(i = 0; i < NDIAP+1; i++){
		if (Fc > ((unsigned int)(Fr[i]))*10)//���������� � ����� �������� ������ ���� �������
		if(i != NDIAP)  (Nd = i);
		else {
			unsigned int maxInd = 0;
			for(i = 0; i < NDIAP+1; i++)
			maxInd +=  ((unsigned int)(N[i]));
			return maxInd;
		};//���� ������� ���� ����� �������
	}
	if (Nd == 0xFFFF) return 0;//���� ������� ���� ����� ������ ���������
	//SendCOMBytes((uchar*)&Nd,2);
	if(Fc > (unsigned int)(Fr[Nd])*10)
	Npc = (Fc - (unsigned int)(Fr[Nd])*10)/(unsigned int)(df[Nd]);  //����� � ������������
	
	Fcl = f - (unsigned int)Npc*(unsigned int)(df[Nd]) - (unsigned int)(Fr[Nd])*10;// ������� ����� ������������ �������� � ������������ � �������� ��������

	for(i = 0; i < Nd; i++){// ����������� ����� �� ������ ������ �� ���������� ����������
		Nf += (unsigned int)(N[i]);

	}

	if (Fcl >= ((unsigned int)(df[Nd])/2))//���� ������ ��� ����� ��� ��� ���� ����� �� ����������� �����
										// (Fcl >= ((unsigned int)(df[Nd])/2)) - �������� �������� �� ������� �� ���� 5 ��� ������ ��� �����
	Nf = Npc + Nf + 1;
	else
	Nf = Npc + Nf;
	
	//SendCOMBytes((uchar*)&Nf,2);
	return Nf*(unsigned int)DATASIZE;
}
//////////////////////////////////////////////////////////////////////////
const unsigned int ves[7] = {10000,1000,100,10,1,0,0};
unsigned int CharFtoInd(unsigned char* f){//������� �� ����� �������
	unsigned int dF = 0;
	char i;
	for( i = 0; i < 7; i++){
		dF += ((unsigned int)(f[i] & 0x0F))*(ves[i]);
	}
	return IndOfF(dF);
}

void ErrorFlash()
{
	lcd_clrscr();
	lcd_puts("   SEL FLASH ERROR  ");
	_delay_ms(500);
}

