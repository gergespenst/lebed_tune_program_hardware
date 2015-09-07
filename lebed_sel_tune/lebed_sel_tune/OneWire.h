/*
 * _1wire.h
 *
 * Created: 07.09.2015 13:21:44
 *  Author: USER
 */ 


#ifndef ONEWIRE_H_
#define ONEWIRE_H_
/* ���������� ���� ��������� ������ */
#define THERM_PORT PORTE
#define THERM_DDR DDRE
#define THERM_PIN PINE
#define THERM_DQ PE2
/* ������� ��� "�������� ����" � ��������� ������ �����/������ */
#define THERM_INPUT_MODE() THERM_DDR&=~(1<<THERM_DQ)
#define THERM_OUTPUT_MODE() THERM_DDR|=(1<<THERM_DQ)
#define THERM_LOW() THERM_PORT&=~(1<<THERM_DQ)
#define THERM_HIGH() THERM_PORT|=(1<<THERM_DQ)

// ����� �������
uint8_t therm_reset(){
	uint8_t i;
	// �������� ���� ���� �� 480uS
	THERM_LOW();
	THERM_OUTPUT_MODE();
	_delay_us(480);             // �������� ������� �������� �� ����
	// �������� ����� �� 60uS
	THERM_INPUT_MODE();
	_delay_us(60);
	// �������� �������� �� ����� � ������ 480uS
	i=(THERM_PIN & (1<<THERM_DQ));
	_delay_us(420);
	// ���������� �������� (0=OK, 1=������ �� ������)
	return i;
}


// ������� �������� ����
void therm_write_bit(uint8_t bit){
	// �������� �� 1uS
	THERM_LOW();
	THERM_OUTPUT_MODE();
	_delay_us(1);
	// ���� ����� ��������� 1, ��������� ����� (���� ���, ��������� ��� ����)
	if(bit) THERM_INPUT_MODE();
	// ���� 60uS � ��������� �����
	_delay_us(60);
	THERM_INPUT_MODE();
}

// ������ ����
uint8_t therm_read_bit(void){
	uint8_t bit=0;
	// �������� �� 1uS
	THERM_LOW();
	THERM_OUTPUT_MODE();
	_delay_us(1);
	// ��������� �� 14uS
	THERM_INPUT_MODE();
	_delay_us(14);
	// ������ ���������
	if(THERM_PIN&(1<<THERM_DQ)) bit=1;
	// ���� 45 ��� � ���������� ��������
	_delay_us(45);
	return bit;
}

// ������ ����
uint8_t therm_read_byte(void){
	uint8_t i=8, n=0;
	while(i--){
		// �������� � ����� �� 1 � ��������� ��������� ��������
		n>>=1;
		n|=(therm_read_bit()<<7);
	}
	return n;
}

// ���������� ����
void therm_write_byte(uint8_t byte){
	uint8_t i=8;
	while(i--){
		// ���������� ��� � �������� ������ �� 1
		therm_write_bit(byte&1);
		byte>>=1;
	}
}

void send_programming_impulse()
{
	THERM_OUTPUT_MODE();
	THERM_HIGH();
	_delay_ms(60);
	THERM_LOW();
	_delay_ms(5);
	THERM_HIGH();
	_delay_ms(50);
}


#endif /* 1WIRE_H_ */