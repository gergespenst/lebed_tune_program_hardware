/*
 * keyboard.cpp
 *
 * Created: 29.09.2014 14:16:11
 *  Author: USER
 */ 
#include "keyboard.h"
void InitKeyboard(){
// 	KEY_REG |= _BV(EDR_PIN);//0x31;
// 	KEY_REG |= _BV(EUL_PIN);
// 	KEY_REG |= _BV(BLINK_PIN);
// 	KEY_PORT = 0x00;
// 	KEY_IN_PORT = 0x00;
// 	KEY_PORT |= _BV(BLINK_PIN);
// 	KEY_PORT |= _BV(EDR_PIN);



}

void Blink(unsigned long t){
// 	KEY_PORT &= ~_BV(BLINK_PIN);
// 	while(t > 0){	_delay_us(1);t--;}
// 	KEY_PORT |= _BV(BLINK_PIN);
}

//void ScanKeyboard(unsigned char *previosBtn, void (*operation)(char)){
#define MAX_STAGE 3
#define OUT_MASK ((1 << DU_IN_PIN) | (1 << EE_IN_PIN) | (1 << LR_IN_PIN))
void ScanKeyboard( void (*operation)(unsigned char)){
	static unsigned char  previosBtn;
	unsigned char clickedBtn;
	

	
// 	switch (KEY_OUT_PIN & ((1 << DU_IN_PIN) | (1 << EE_IN_PIN) | (1 << LR_IN_PIN)))//реакция на нажатие клавиш
// 	{
// 		case (1 << DU_IN_PIN):
// 		clickedBtn = DU_NUM + CLK_STAT;
// 		break;
// 		
// 		case (1 << EE_IN_PIN):
// 		clickedBtn = EE_NUM + CLK_STAT;
// 		break;
// 		
// 		case (1 << LR_IN_PIN):
// 		clickedBtn = LR_NUM + CLK_STAT;
// 		break;
// 		
// 		default: clickedBtn = NOP;
// 		break;
// 	}
// 	
KEY_OUT_PORT |= ((1 << DU_IN_PIN) | (1 << EE_IN_PIN) | (1 << LR_IN_PIN));
KEY_OUT_REG  |= ((1 << DU_IN_PIN) | (1 << EE_IN_PIN) | (1 << LR_IN_PIN));
KEY_IN_REG   &= ~(_BV(EDR_PIN) | _BV(EUL_PIN));
KEY_IN_PORT &= ~(_BV(EDR_PIN) | _BV(EUL_PIN));

char g_hex[] ="0123456789ABCDEF";
for (char stage = 0; stage < MAX_STAGE; stage++)
{
	KEY_OUT_PORT = ~_BV(stage);
	_delay_ms(10);
	clickedBtn = KEY_IN_PIN ;//& (_BV(EDR_PIN) | _BV(EUL_PIN));
	if ( ((clickedBtn & _BV(EDR_PIN))>>EDR_PIN) == 0)
	{
		clickedBtn = (stage) + 0x10;
		break;
	}else
	if (((clickedBtn & _BV(EUL_PIN))>>EUL_PIN ) == 0)
	{
		clickedBtn = (stage) + 0x20;
		break;
	}else
		clickedBtn = NOP;
	_delay_ms(10);
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
		previosBtn = NOP;
// 		if(CLK_STAT) {KEY_IN_PORT = (KEY_IN_PORT&(~_BV(EDR_PIN)))|_BV(EUL_PIN);}//переключаем линии выбора клавиш
// 		else {KEY_IN_PORT = (KEY_IN_PORT|_BV(EDR_PIN))&(~_BV(EUL_PIN));}
		
		
	}
}