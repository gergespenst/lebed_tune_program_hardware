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

#define KEY_OUT_PORT	PORTC	//порт клавиатуры
#define KEY_OUT_REG		DDRC	//регистр порта клавиатуры
#define KEY_OUT_PIN		PINC

#define KEY_IN_PORT PORTD
#define KEY_IN_REG  DDRD
#define KEY_IN_PIN	 PIND

#define BLINK_PIN	PC7		//пин светодиода
#define EDR_PIN		PD6		//пин вывода для кнопок Enter, Down, Right
#define EUL_PIN		PD7	//пин вывода для кнопок Esc, Up, Left

#define EE_IN_PIN	PINC0		//пин ввода для кнопок Enter/Esc
#define DU_IN_PIN	PINC1		//пин ввода для кнопок Down/Up
#define LR_IN_PIN	PINC2		//пин ввода для кнопок Left/Right
#include "keyboard.h"

#define PortReg(port) (*(port))
#define DirReg(port)  (*((port) - 1))
#define PinReg(port)  (*((port) - 2))

#ifdef PWC_PLATE//настройка порта IIC
	#define IICDDR                DDRB					/*порт управления I2C*/
	#define IICPORT               PORTB					/*порт вывода данных на линию I2C*/
	#define IICPIN                PINB					/*порт ввода данных с линий I2C*/
	#define IICSDAPin			  4
	#define IICSDLPin              7

	#define SEL_PORT		PORTG
	#define SEL_PORT_DDR	DDRG
	#define CLKK_PIN		1
	#define CLKS_PIN		3
	#define SEL_DATA_PIN	0
	
	#define SEL_nOTP_PORT	PORTB
	#define SEN_nOTP_DDR	DirReg(&SEL_nOTP_PORT)
	#define SEL_nOTP_PIN	5

	#define ADC_UPAS_OUT_PIN 3
#endif



#ifdef KNK_PRK_PLATE//настройка порта IIC
	#define IICDDR                DDRB					/*порт управления I2C*/
	#define IICPORT               PORTB					/*порт вывода данных на линию I2C*/
	#define IICPIN                PINB					/*порт ввода данных с линий I2C*/
	#define IICSDAPin			  6
	#define IICSDLPin              5
	
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
		#define IICSDAPin			  3
		#define IICSDLPin              4
		
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