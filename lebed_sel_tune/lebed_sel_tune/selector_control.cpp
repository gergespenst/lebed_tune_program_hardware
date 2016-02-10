/*
 * selector_control.c
 *
 * Created: 19.09.2014 14:56:50
 *  Author: USER
 */ 
#include "selector_control.h"


void SetCLKK(char val){//изменение состояния линии CLKK
	if ( val == 0)
	SEL_PORT &= ~_BV(CLKK_PIN);
	else
	SEL_PORT |= _BV(CLKK_PIN);
}

void SetCLKS(char val){//изменение состояния линии CLKS
	if ( val == 0)
	SEL_PORT &= ~_BV(CLKS_PIN);
	else
	SEL_PORT |= _BV(CLKS_PIN);
}

void SetDATA(char val){//изменение состояния линии DATA
	if ( val == 0)
	SEL_PORT &= ~_BV(SEL_DATA_PIN);
	else
	SEL_PORT |= _BV(SEL_DATA_PIN);
}

void InitSelPort(){//начальные установки порта
	SEL_PORT_DDR |= _BV(CLKK_PIN);
	SEL_PORT_DDR |= _BV(CLKS_PIN);
	SEL_PORT_DDR |= _BV(SEL_DATA_PIN);
	SEN_nOTP_DDR |= _BV(SEL_nOTP_PIN);
	
	//SEN_nOTP_DDR |= _BV()
	SETCLKK(0);
	SETCLKS(0);
	SETDATA(0);
	SET_SEL_nOTP(0);

}

void SendA(unsigned int* LCAout){//отправка значения аттенюатора
	char i;
	cli();
	for (i = 8; i > 0; i--)
	{
		SETCLKK(0);
		_delay_us(10);
		//SetDATA( (( ((unsigned int)eeprom_read_word(&LCAout[2])) >> (8 + i - 1)) & 0x0001));
		SETDATA( ((LCAout[2] >> (8 + i - 1)) & 0x0001));
		_delay_us(10);
		SETCLKK(1);
		_delay_us(10);
	}
	SETCLKK(0);
	SETCLKS(0);
	SETDATA(0);
	sei();
}

void SendLCA2Sel(unsigned char* LCAout){//отправка значения настройки контуров
	char i;
	unsigned int* LCA = (unsigned int* ) LCAout;
	cli();

	SetnOTP(1);//запираем предкорретор
	for (  i = 41; i > 0; i--)
	{
		if (i <= 16)
		{
			SETCLKS(0);
			_delay_us(10);
			//SetDATA( (( ((unsigned int)eeprom_read_word(&LCA[0])) >> (i - 1)) & 0x0001));
			SETDATA( ((LCA[0] >> (i - 1)) & 0x0001));
			_delay_us(10);
			SETCLKS(1);
			_delay_us(10);
			
		} else if ((16 < i)&&(i <= 32))
		{
			SETCLKS(0);
			_delay_us(10);
			//SetDATA( (( ((unsigned int)eeprom_read_word(&LCA[1])) >> (i - 16 - 1)) & 0x0001));
			SETDATA( ((LCA[1] >> (i - 16 - 1)) & 0x0001));
			_delay_us(10);
			SETCLKS(1);
			_delay_us(10);
			
		} else if ((32 < i)&&(i <= 40))
		{
			SETCLKS(0);
			_delay_us(10);
			//SetDATA( (( ((unsigned int)eeprom_read_word(&LCA[2])) >> (i - 32 - 1)) & 0x0001));
			SETDATA( ((LCA[2] >> (i - 32 - 1)) & 0x0001));
			_delay_us(10);
			SETCLKS(1);
			_delay_us(10);
			
		}else if (41 == i)
		{
			SendA(LCA);
		}
		
	}
	SETCLKK(0);
	SETCLKS(0);
	SETDATA(0);
	SetnOTP(0);//отпираем предкорретор
	sei();

}

