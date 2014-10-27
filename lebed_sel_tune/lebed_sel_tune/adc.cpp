/*
 * adc.cpp
 *
 * Created: 02.10.2014 15:04:10
 *  Author: USER
 */ 
#include "adc.h"

char read_adc(unsigned char pin) //����������� ���� ���������� Vdet c ������� ADC
{
	DDRB |= _BV(0);
	PORTB |= _BV(0);
	ADMUX = (1 << REFS1)|(1<< REFS0)|	//���������� ������� � ����������� ���������
	(1 << ADLAR)|				//������������ ������ ������ �����
	(pin & 0x07);				//� ������������� � ������ ���� ������
	
	ADCSRA = (1 << ADEN)|		//��������� ������ ���
			(1 << ADSC)|				//��������� ��������������
			(0 << ADFR)|			//����������� ������
			(0 << ADIF)|			//���� ����������
	
			(0 << ADIE)| //��������� ���������� �� ���
			(1 << ADPS2)|(1 << ADPS1)|(0 << ADPS0);	//�������� ������� F_CPU/64 = 172.8 kHz
			
	while(ADCSRA & _BV(ADSC)){_delay_us(2);};			//���� ��������� ��������������
	PORTB &= ~_BV(0);	
	return ADCH;
}

char read_steady_adc(unsigned char pin){//����������� ������ �� ��������� ���������� ���������
	char adc_ampl[3]={0,1,2};
	while( !((adc_ampl[0] == adc_ampl[1]) && (adc_ampl[1] == adc_ampl[2])) ){
	_delay_us(50)	;
	adc_ampl[2] = adc_ampl[1];
	adc_ampl[1] = adc_ampl[0];	
	adc_ampl[0] = read_adc(pin);
	}
	
}