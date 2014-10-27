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
/*  Функции работы с микросхемой AT24C512                               */
/************************************************************************/
#define NDIAP 5		//количество диапазонов
#define DATASIZE 6	//количество байт в записываемых данных
#define DETCAL_START_ADDR (0xFF00) //начальный адрес хранения коэффициентов калибровки
	
#define DEVICE_ADDR 0xA0 //адрес устройства
#define WRITE 0x00
#define READ  0x01
void ErrorFlash();
void WriteData2FlashIIC(unsigned int startAddr,uchar *data,uchar numOfDataBytes);
void ReadDataInFlashIIC(unsigned int* startAddr,uchar *data,uchar numOfDataBytes);
void ReadLCAoutFromFlash(unsigned int addr,uchar *data);
void WriteLCAout2Flash(unsigned int addr,uchar *data);
void ReadDetCalInFlash(unsigned int addr,uchar* data);
void WriteDetCal2Flash(unsigned int addr,uchar* data);
unsigned int IndOfF(unsigned int f);
unsigned int CharFtoInd(unsigned char* f);

#endif /* FLASHIIC_H_ */