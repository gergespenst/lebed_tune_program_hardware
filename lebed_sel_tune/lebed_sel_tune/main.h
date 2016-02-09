/*
 * main.h
 *
 * Created: 29.09.2014 12:14:55
 *  Author: USER
 */ 


#ifndef MAIN_H_
#define MAIN_H_

#define F_CPU 11059200L //частота кристалла

#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"
// #define port_lcd            PORTA
// #define ddr_lcd             DDRA
// #define pin_lcd             PINA
// #include "lcd_knk.h"

#define KEY_PORT	PORTF	//порт клавиатуры
#define KEY_REG		DDRF	//регистр порта клавиатуры
#define KEY_IN_PORT PINF

#define BLINK_PIN	PF7		//пин светодиода
#define EDR_PIN		PF1		//пин вывода для кнопок Enter, Down, Right
#define EUL_PIN		PF0		//пин вывода для кнопок Esc, Up, Left
#define EE_IN_PIN	PINF2		//пин ввода для кнопок Enter/Esc
#define DU_IN_PIN	PINF3		//пин ввода для кнопок Down/Up
#define LR_IN_PIN	PINF4		//пин ввода для кнопок Left/Right
#include "keyboard.h"



#ifndef DDC_BOARD//настройка порта IIC
	#define IICDDR                DDRB					/*порт управления I2C*/
	#define IICPORT               PORTB					/*порт вывода данных на линию I2C*/
	#define IICPIN                PINB					/*порт ввода данных с линий I2C*/
	#define IICDataPin			  6
	#define IICCcPip              5
	
	#define SEL_PORT PORTE
	#define SEL_PORT_DDR DDRE
	#define CLKK_PIN		6
	#define CLKS_PIN		5
	#define SEL_DATA_PIN	7
	#define SEL_nOTP_PIN	4
	
	#define ADC_UPAS_OUT_PIN 6
#endif

#ifdef DDC_BOARD

		#define IICDDR                DDRB					/*порт управления I2C*/
		#define IICPORT               PORTB					/*порт вывода данных на линию I2C*/
		#define IICPIN                PINB					/*порт ввода данных с линий I2C*/
		#define IICDataPin			  3
		#define IICCcPip              4
		
	#define SEL_PORT PORTB
	#define SEL_PORT_DDR DDRB
	#define CLKK_PIN		1
	#define CLKS_PIN		0
	#define SEL_DATA_PIN	2
	#define SEL_nOTP_PIN	7
	
	#define ADC_UPAS_OUT_PIN 5
#endif

#include "i2c.h"
#include "flashiic.h"
#include "selector_control.h"

#include "altera.h"


//#define ADC_UPAS_IN_PIN 7
#include "adc.h"

#include "ZSelControl.h"

#include <stdlib.h>

#endif /* MAIN_H_ */