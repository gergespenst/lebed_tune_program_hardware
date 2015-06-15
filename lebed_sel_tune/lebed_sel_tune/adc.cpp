/*
 * adc.cpp
 *
 * Created: 02.10.2014 15:04:10
 *  Author: USER
 */ 
#include "adc.h"

char read_adc(unsigned char pin) //Определение кода напряжения Vdet c помощью ADC
{
// 	DDRB |= _BV(0);
// 	PORTB |= _BV(0);
	ADMUX = (1 << REFS1)|(1<< REFS0)|	//подключаем опорное к внутреннему источнику
	(1 << ADLAR)|				//левосторнний формат вывода битов
	(pin & 0x07);				//и утсанавливаем с какого пина читать
	
	ADCSRA = (1 << ADEN)|		//разрешаем работу АЦП
			(1 << ADSC)|				//запускаем преобразование
			(0 << ADFR)|			//однократный запуск
			(0 << ADIF)|			//флаг прерывания
	
			(0 << ADIE)| //запрещаем прерывания от АЦП
			(1 << ADPS2)|(1 << ADPS1)|(0 << ADPS0);	//тактовая частота F_CPU/64 = 172.8 kHz
			
	while(ADCSRA & _BV(ADSC)){_delay_us(2);};			//ждем окончания преобразования
//	PORTB &= ~_BV(0);	
	return ADCH;
}

char read_steady_adc(unsigned char pin){//определение уровня по окончании переходных процессов
	char adc_ampl[3]={0,1,2};
	while( !((adc_ampl[0] == adc_ampl[1]) && (adc_ampl[1] == adc_ampl[2])) ){
	_delay_us(50)	;
	adc_ampl[2] = adc_ampl[1];
	adc_ampl[1] = adc_ampl[0];	
	adc_ampl[0] = read_adc(pin);
	}
	
}