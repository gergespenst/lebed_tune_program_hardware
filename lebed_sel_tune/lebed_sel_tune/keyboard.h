/*
 * KeyboardDefines.h
 *
 * Created: 07.10.2013 13:21:04
 *  Author: 123
 * Файл описывает функции ддля работы с клавиатурой
 */ 


#ifndef KEYBOARD_H_
#define KEYBOARD_H_
//описания портов перенесены в файл main.h
//#define KEY_IN_PORT	PORTF	//порт клавиатуры
//#define KEY_IN_REG		DDRF	//регистр порта клавиатуры
//#define KEY_IN_PORT PINF

// #define KEY_OUT_PORT PORTD
// #define KEY_OUT_REG  DDRD
// #define KEY_OUT_PIN	 PIND
//#define BLINK_PIN	PF5		//пин светодиода
//#define EDR_PIN		PF0		//пин вывода для кнопок Enter, Down, Right
//#define EUL_PIN		PF4		//пин вывода для кнопок Esc, Up, Left
//#define EE_IN_PIN	PINF1		//пин ввода для кнопок Enter/Esc
//#define DU_IN_PIN	PINF2		//пин ввода для кнопок Down/Up
//#define LR_IN_PIN	PINF3		//пин ввода для кнопок Left/Right
#include "main.h"

#define  EE_NUM 1 //константы значений клавиш
#define  DU_NUM 3
#define  LR_NUM 5
#define	 NOP 0

#ifndef uchar
	typedef unsigned char uchar;

#endif

#define CLK_STAT  ((KEY_OUT_PIN&_BV(EDR_PIN)) == _BV(EDR_PIN))
void InitKeyboard();
void Blink(unsigned long t);
void ScanKeyboard( void (*operation)(unsigned char));
#endif /* KEYBOARDDEFINES_H_ */