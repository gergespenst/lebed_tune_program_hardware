/*
 * flashiic.c
 *
 * Created: 19.09.2014 13:48:43
 *  Author: USER
 */ 
#include "flashiic.h"
//////////////////////////////////////////////////////////////////////////
void WriteData2FlashIIC(unsigned int startAddr,uchar *data,uchar numOfDataBytes){//запись в память n байт по адресу
	// реализация постраничной записи. возникают проблемы на границе страниц
	//IICStart();
	//if(IICByteWrite( (DEVICE_ADDR + WRITE) ) == 0x01)				//запись адреса устройства
	//SendCOMBytes((uchar*)"No ask",6);
	//if(IICByteWrite( (uchar)((startAddr&0xFF00) >> 8)) == 0x01)		//старшая часть адреса ячейки
	//SendCOMBytes((uchar*)"No ask",6);
	//if(IICByteWrite( (uchar)((startAddr&0x00FF))) == 0x01)			//младшая часть адреса ячейки
	//SendCOMBytes((uchar*)"No ask",6);
	//for (uchar i = 0; i < numOfDataBytes; i++)									//байты данных
	//{
	//if(IICByteWrite( data[i] ) == 0x01)
	//SendCOMBytes((uchar*)"No ask",6);
	////SendCOMBytes(&data[i],1);
	//}
	//IICStop();	//конец передачи
	char i=0;
	//реализация побайтной записи
	for ( i = 0; i < numOfDataBytes; i++)									//байты данных
	{
		IICStart();
		if(IICByteWrite( (DEVICE_ADDR + WRITE) ) == 0x01)				//запись адреса устройства
		ErrorFlash();
		if(IICByteWrite( (uchar)(((startAddr+i)&0xFF00) >> 8)) == 0x01)		//старшая часть адреса ячейки
		ErrorFlash();
		if(IICByteWrite( (uchar)(((startAddr+i)&0x00FF))) == 0x01)			//младшая часть адреса ячейки
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
{//используется произвольный доступ к памяти
	char i=0;
	IICStart();
	if(IICByteWrite( (DEVICE_ADDR + WRITE) ) == 0x01)				//запись адреса устройства
		i++;
	if(IICByteWrite( (uchar)((*startAddr&0xFF00) >> 8)) == 0x01)		//старшая часть адреса ячейки
		i++;
	if(IICByteWrite( (uchar)((*startAddr&0x00FF))) == 0x01)			//младшая часть адреса ячейки
		i++;
	IICStart();
	if(IICByteWrite( (DEVICE_ADDR + READ) ) == 0x01)				//запись адреса устройства
		i++;
	if (i > 0) ErrorFlash();
	for ( i = 0; i < numOfDataBytes; i++)									//побайтное чтение из памяти. у предпоследнего символа нет ACK
	{
		data[i] = IICByteRead((uchar) (i == (numOfDataBytes - 1)));
	//	uart_putchar(i);
	}
	//SendCOMBytes(data,6);
	IICStop();
}
//////////////////////////////////////////////////////////////////////////
void ReadLCAoutFromFlash(unsigned int addr,uchar *data)
{//обертка для записи данных из EEPROM контроллера во flash
	//uchar temp_data[DATASIZE];
	//uart_putchar('a');
	ReadDataInFlashIIC(&addr,data,DATASIZE);
	//SendCOMBytes((uchar*)data,6);
	//eeprom_update_block(temp_data,data,DATASIZE);
	
}
//////////////////////////////////////////////////////////////////////////
void WriteLCAout2Flash(unsigned int addr,uchar *data){//обертка для записи данных из EEPROM контроллера во flash
	//	uchar temp_data[6];
	//	eeprom_read_block(temp_data,data,6);
	WriteData2FlashIIC(addr,data,DATASIZE);
	//SendCOMBytes((uchar*)data,6);
}
//////////////////////////////////////////////////////////////////////////
//Функция записи калибровочного коэффициента
void WriteDetCal2Flash( unsigned int addr,uchar* data )
{
	unsigned int shift_addr = addr + DETCAL_START_ADDR;
	WriteData2FlashIIC(shift_addr,data,1);
}
//Функция чтения калибровочного коэффициента
void ReadDetCalInFlash( unsigned int addr,uchar* data )
{
	unsigned int shift_addr = addr + DETCAL_START_ADDR;
	ReadDataInFlashIIC(&shift_addr,data,1);
}


/************************************************************************/
/*  Функции преобразования частоты в адрес								*/
/************************************************************************/
static const unsigned int Fr[NDIAP + 1]  ={150,300,600,1200,2400,3000};//граничные частоты диапазонов
static const unsigned int df[NDIAP + 1]  ={5,  10, 20,  40,  80, 100};//шаги частот в диапазонах
static const unsigned int N[NDIAP + 1]  = { 300,300, 300, 300,     75};//количество точек в диапазонах
//////////////////////////////////////////////////////////////////////////
unsigned int //возвращает позицию частоты во flash возможна ошибка в одну позиции по тому что не учитываются десятые и сотые доли
	IndOfF(unsigned int f){
	unsigned int i = 0, Fc = 0, Nd = 0xFFFF, Npc = 0, Nf = 0;// Fc - частота без двух младших, Nd - номер диапазона, Npc - номер частоты в диапазоне, Nf - номер частоты во флешке
	unsigned int Fcl = 0;// младшие разряды частоты для решения в какую сторону притягниваться
	Fc = f;//приводим частоту к виду Fr, частота в 10 кГц
	for(i = 0; i < NDIAP+1; i++){
		if (Fc > ((unsigned int)(Fr[i]))*10)//определяем в какой диапазон попала наша частота
		if(i != NDIAP)  (Nd = i);
		else {
			unsigned int maxInd = 0;
			for(i = 0; i < NDIAP+1; i++)
			maxInd +=  ((unsigned int)(N[i]));
			return maxInd;
		};//если частота выше самой верхней
	}
	if (Nd == 0xFFFF) return 0;//если частота ниже самой нижней граничной
	//SendCOMBytes((uchar*)&Nd,2);
	if(Fc > (unsigned int)(Fr[Nd])*10)
	Npc = (Fc - (unsigned int)(Fr[Nd])*10)/(unsigned int)(df[Nd]);  //номер в поддиапазоне
	
	Fcl = f - (unsigned int)Npc*(unsigned int)(df[Nd]) - (unsigned int)(Fr[Nd])*10;// разница между получившейся частотой в поддиапазоне и исходной частотой

	for(i = 0; i < Nd; i++){// увеличиваем сдвиг от начала флешки на количество диапазонов
		Nf += (unsigned int)(N[i]);

	}

	if (Fcl >= ((unsigned int)(df[Nd])/2))//Если больше или равно чем пол шага сетки то увеличиваем номер
										// (Fcl >= ((unsigned int)(df[Nd])/2)) - вызывает перескок по частоте на шаге 5 кГц раньше чем нужно
	Nf = Npc + Nf + 1;
	else
	Nf = Npc + Nf;
	
	//SendCOMBytes((uchar*)&Nf,2);
	return Nf*(unsigned int)DATASIZE;
}
//////////////////////////////////////////////////////////////////////////
const unsigned int ves[7] = {10000,1000,100,10,1,0,0};
unsigned int CharFtoInd(unsigned char* f){//зависит от длины частоты
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

