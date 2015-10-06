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

#define ONEWIRE_PORT PORTE
#define ONEWIRE_DDR DDRE
#define ONEWIRE_PIN PINE
#define ONEWIRE_PIN_NUM PE2

void onewire_high(){
	ONEWIRE_PORT &=~ (1<<ONEWIRE_PIN_NUM);
	ONEWIRE_DDR  &=~ (1<<ONEWIRE_PIN_NUM);
}

void onewire_low(){
	ONEWIRE_PORT &=~ (1<<ONEWIRE_PIN_NUM);
	ONEWIRE_DDR  |= (1<<ONEWIRE_PIN_NUM);
}

uint8_t onewire_level(){
	return ONEWIRE_PIN & (1<<ONEWIRE_PIN_NUM);
}

char onewire_init(){
	onewire_low();
	_delay_us(480);
	onewire_high();
	_delay_us(2);
	for(uint8_t i= 60;i;i++){
		if(!onewire_level()){
			while(!onewire_level());
			return 1;
		}
		_delay_us(1);
	}
	return 0;
}

void onewire_write_bit(uint8_t bit){
	onewire_low();
	if(bit){
		_delay_us(5);
		onewire_high();
		_delay_us(90);
		}else{
		_delay_us(90);
		onewire_high();
		_delay_us(5);
	}
}

void onewire_send(uint8_t data){
	for(uint8_t i=0;i<8;i++){
		onewire_write_bit(data&0x01);
		data>>= 1;
	}
}


  
	  
uint8_t onewire_read_bit(){
	onewire_low();
	_delay_us(2);
	onewire_high();
	_delay_us(8);
	uint8_t r = onewire_level();
	_delay_us(80);
	return r;
}

uint8_t onewire_read(){
	uint8_t r = 0;
	for (uint8_t p = 8; p; p--) {
		r >>= 1;
		if (onewire_read_bit())
		r |= 0x80;
	}
	return r;
}

void time_slot(uint8_t data){
	onewire_low();
	if(data)
	_delay_us(6);
	else
	_delay_us(60);
	onewire_high();
	_delay_ms(10);
}

void rw1990_write_bit(uint8_t data){
	onewire_low();
	if (data)
	_delay_us(6);
	else
	_delay_us(60);
	onewire_high();
	_delay_ms(2);
}

void rw1990_write(uint8_t data){
	for(uint8_t i=0;i<8;i++){
		rw1990_write_bit(data & 0x01);
		data>>=1;
	}
}

char rw1990_newcode(uint8_t* buf){
	if (onewire_init()){
		onewire_send(0xD1);
		time_slot(0);
	}else return 0;
	if(onewire_init()){
		onewire_send(0xD5);
		for(uint8_t i=0;i<8;i++){
			rw1990_write(~buf[i]);
		}
	}else return 0;
	if (onewire_init()){
		onewire_send(0xD1);
		time_slot(1);
	}else return 0;
	return 1;
}

char onewire_readrom(uint8_t* buf){
	if (onewire_init()){
		onewire_send(0x33);
		for(uint8_t i=0;i<8;i++){
			buf[i] = onewire_read();
		}
	}else return 0;
	return 1;
}
#endif /* 1WIRE_H_ */