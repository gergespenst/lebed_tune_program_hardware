/*
 * keyboard.cpp
 *
 * Created: 29.09.2014 14:16:11
 *  Author: USER
 */ 
#include "keyboard.h"
void InitKeyboard(){
	KEY_REG |= _BV(EDR_PIN);//0x31;
	KEY_REG |= _BV(EUL_PIN);
	KEY_REG |= _BV(BLINK_PIN);
	KEY_PORT = 0x00;
	KEY_IN_PORT = 0x00;
	KEY_PORT |= _BV(BLINK_PIN);
	KEY_PORT |= _BV(EDR_PIN);
}

void Blink(unsigned long t){
	KEY_PORT &= ~_BV(BLINK_PIN);
	while(t > 0){	_delay_us(1);t--;}
	KEY_PORT |= _BV(BLINK_PIN);
}

//void ScanKeyboard(unsigned char *previosBtn, void (*operation)(char)){
void ScanKeyboard( void (*operation)(unsigned char)){
	static unsigned char  previosBtn;
	unsigned char clickedBtn;
	switch (KEY_IN_PORT & ((1 << DU_IN_PIN) | (1 << EE_IN_PIN) | (1 << LR_IN_PIN)))//реакция на нажатие клавиш
	{
		case (1 << DU_IN_PIN):
		clickedBtn = DU_NUM + CLK_STAT;
		break;
		
		case (1 << EE_IN_PIN):
		clickedBtn = EE_NUM + CLK_STAT;
		break;
		
		case (1 << LR_IN_PIN):
		clickedBtn = LR_NUM + CLK_STAT;
		break;
		
		default: clickedBtn = NOP;
		break;
	}
	

	if ((clickedBtn != NOP))//если была нажата клавиша
	{
		if (clickedBtn != previosBtn)
		{
			(*operation)(clickedBtn);
			previosBtn = clickedBtn;//запоминаем нажатую клавишу
		}
	}
	else
	{
		if(CLK_STAT) {KEY_PORT = (KEY_PORT&(~_BV(EDR_PIN)))|_BV(EUL_PIN);}//переключаем линии выбора клавиш
		else {KEY_PORT = (KEY_PORT|_BV(EDR_PIN))&(~_BV(EUL_PIN));}
		previosBtn = NOP;
	}
}