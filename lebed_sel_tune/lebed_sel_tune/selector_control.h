/*
 * selector_control.h
 *
 * Created: 05.12.2013 15:34:13
 *  Author: 123
 * Файл с функциями управления селектором
 */ 


#ifndef SELECTOR_CONTROL_H_
#define SELECTOR_CONTROL_H_
//дефайны порта должны быть в main.h
// #define SEL_PORT PORTB
// #define SEL_PORT_DDR DDRB
// #define CLKK_PIN		6
// #define CLKS_PIN		7
// #define SEL_DATA_PIN	5
//#define SEL_nOTP_PIN		4
	#ifndef uchar
		typedef unsigned char uchar;

	#endif
#include "main.h"
#include <avr/interrupt.h>

#ifndef SET_LINE
#define SET_LINE(port,line,val) (val == 0)?(port &= ~_BV(line)):(port |= _BV(line))
#endif

#define SETCLKK(a) SET_LINE(SEL_PORT,CLKK_PIN,a)
#define SETCLKS(a) SET_LINE(SEL_PORT,CLKS_PIN,a)
#define SETDATA(a) SET_LINE(SEL_PORT,SEL_DATA_PIN,a)
#define SET_SEL_nOTP(a)		SET_LINE(SEL_nOTP_PORT,SEL_nOTP_PIN,a)

void SendLCA2Sel(unsigned char* LCAout);
void SendA(unsigned int* LCAout);
void InitSelPort();

extern void SetnOTP(char val);
#endif /* SELECTOR_CONTROL_H_ */