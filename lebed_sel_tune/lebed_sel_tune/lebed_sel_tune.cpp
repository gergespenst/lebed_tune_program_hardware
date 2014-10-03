/*
 * lebed_sel_tune.cpp
 *
 * Created: 29.09.2014 12:10:40
 *  Author: USER
 */ 
#include "main.h"
#include "menu_impl.h"
#include "uart.h"
//////////////////////////////////////////////////////////////////////////
void KeySelect(unsigned char a){//эту функцию следует перенести в класс меню
	switch (a){
		case 0x01:g_currentItem->escFunction(g_currentItem);break;
		case 0x02:g_currentItem->okFunction(g_currentItem);break;
		case 0x03:g_currentItem->upFunction(g_currentItem);break;
		case 0x04:g_currentItem->downFunction(g_currentItem);break;
		case 0x05:g_currentItem->leftFunction(g_currentItem);break;
		case 0x06:g_currentItem->rightFunction(g_currentItem);break;
	}
}
//////////////////////////////////////////////////////////////////////////
void WriteFreqToPrk(uchar* freq){
	uchar datatmp[13]={0x00,0x00,0x00,0x00,
						freq[0]*16 + freq[1],
						freq[2]*16 +  freq[3],
						freq[4]*16 +  freq[5],
						freq[6]*16,
						0x00,0x00,0x00,0x00,0x00};
	
	Write_Controll(0x07F7, &datatmp[0]);
	Write_Controll(0x07F6, &datatmp[4]);
	Write_Controll(0x07F5, &datatmp[8]);

	uchar Instr[4] = {0x04,0,0,0};

	Write_Controll(0x07F4, &Instr[0]);
	delay_us(100);
	while(!GET_ALTERA_BUSY);
}
//////////////////////////////////////////////////////////////////////////
void ChangeModePrk(STATEOFPLATE* state){
		Read_Controll(0x0FFE, state->C_R[1]);
		if (state->mode == 0)
				state->C_R[1][3] = ((state->C_R[1][3])& 0xF8) + 4;
			else
				state->C_R[1][3] = ((state->C_R[1][3])& 0xF8) + 6;
		Write_Controll(0x07FE, state->C_R[1]);	
}
//////////////////////////////////////////////////////////////////////////
void InitPrk(){
	ReadStateFromEEPROM(&g_plateState,(void*)&g_eepromPlateState);
	InitAltera();
	
	Write_Controll(0x07FF, g_plateState.C_R[0]);
	Write_Controll(0x07FE, g_plateState.C_R[1]);
	Write_Controll(0x07FD, g_plateState.C_R[2]);
	Write_Controll(0x07FC, g_plateState.C_R[3]);
	
	delay_ms(10);
	SET_ALTERA_RESET(0);
	delay_ms(5);
	SET_ALTERA_ENABLE(1);
	_delay_ms(10);
	WriteFreqToPrk(g_plateState.freq);
	ChangeModePrk(&g_plateState);
}
//////////////////////////////////////////////////////////////////////////
void SetnOTP(char val){
		if (val == 0)
		{
			PORTB &= ~_BV(7);
		}else
			PORTB |= _BV(7);
			
		SET_SEL_nOTP(val);
}
//////////////////////////////////////////////////////////////////////////
void SetOTPLines(){
// 	if (g_plateState.output)
// 	{
// 		PORTB |= _BV(7);
// 	}else
// 		PORTB &= ~_BV(7);
	
}
//////////////////////////////////////////////////////////////////////////
int main(void)
{
	Init_LCD();
	LCD_Clear();
	LCD_DisplayStringRus(0,0,"      «ј√–”« ј      ");
	InitKeyboard();
	InitPrk();
	InitSelPort();
	IICInit();
	InitUart();
	g_currentItem = &g_mainMenuItem;
	g_currentItem->draw();
	
	SendCOMBytes((uchar*)"REBOOT",7);
	DDRB |= _BV(7);
	
    while(1)
    {
      ScanKeyboard(KeySelect);
	
    }
}