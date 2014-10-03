/*
 * flashiic.h
 *
 * Created: 13.12.2013 16:56:51
 *  Author: 123
 */ 


#ifndef FLASHIIC_H_
#define FLASHIIC_H_
#include "main.h"
/************************************************************************/
/*  ������� ������ � ����������� AT24C512                               */
/************************************************************************/
#define NDIAP 5		//���������� ����������
#define DATASIZE 6	//���������� ���� � ������������ ������

	
#define DEVICE_ADDR 0xA0 //����� ����������
#define WRITE 0x00
#define READ  0x01
void ErrorFlash();
void WriteData2FlashIIC(unsigned int startAddr,uchar *data,uchar numOfDataBytes);
void ReadDataInFlashIIC(unsigned int* startAddr,uchar *data,uchar numOfDataBytes);
void ReadLCAoutFromFlash(unsigned int addr,uchar *data);
void WriteLCAout2Flash(unsigned int addr,uchar *data);
unsigned int IndOfF(unsigned int f);
unsigned int CharFtoInd(unsigned char* f);

#endif /* FLASHIIC_H_ */