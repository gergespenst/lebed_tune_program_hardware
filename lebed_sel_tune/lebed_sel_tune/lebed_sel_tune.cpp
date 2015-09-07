/*
 * lebed_sel_tune.cpp
 *
 * Created: 29.09.2014 12:10:40
 *  Author: USER
 */ 
#include "main.h"
#include "menu_impl.h"
#include "uart.h"
void SetOutLevelFromFreq();
//////////////////////////////////////////////////////////////////////////
void KeySelect(unsigned char a){//эту функцию следует перенести в класс меню
	switch (a){
		case 0x01:g_currentItem->escFunction(g_currentItem);break;
		case 0x02:g_currentItem->okFunction(g_currentItem);break;
		case 0x03:g_currentItem->upFunction(g_currentItem);break;
		case 0x04:g_currentItem->downFunction(g_currentItem);break;
		case 0x05:g_currentItem->leftFunction(g_currentItem);break;
		case 0x06:g_currentItem->rightFunction(g_currentItem);break;
	}
}
//////////////////////////////////////////////////////////////////////////
void WriteFreqToPrk(uchar* freq){
	uchar datatmp[13]={0x00,0x00,0x00,0x00,
						freq[0]*16 + freq[1],
						freq[2]*16 +  freq[3],
						freq[4]*16 +  freq[5],
						freq[6]*16,
						0x00,0x00,0x00,0x00,0x00};
	
	Write_Controll(0x07F7, &datatmp[0]);
	Write_Controll(0x07F6, &datatmp[4]);
	Write_Controll(0x07F5, &datatmp[8]);

	uchar Instr[4] = {0x04,0,0,0};

	Write_Controll(0x07F4, &Instr[0]);
	delay_us(100);
	while(!GET_ALTERA_BUSY);
	SetOutLevelFromFreq();
}
//////////////////////////////////////////////////////////////////////////
void ChangeModePrk(STATEOFPLATE* state){
		Read_Controll(0x0FFE, state->C_R[1]);
		if (state->mode == 0)
				state->C_R[1][3] = ((state->C_R[1][3])& 0xF8) + 4;
			else
				state->C_R[1][3] = ((state->C_R[1][3])& 0xF8) + 6;
		Write_Controll(0x07FE, state->C_R[1]);	
}
//////////////////////////////////////////////////////////////////////////
// Изменение выходного напряжения платы
/////////////////////////////////
//от 1,5 до 28,5 через 0,5 МГц
void SetOutLevel(uchar lvl){
	lvl = lvl & 0x7F;
	Read_Controll(0x0FFD, g_plateState.C_R[2]);
		g_plateState.C_R[2][3] = (g_plateState.C_R[2][3] & 0x80) | lvl;
	Write_Controll(0x07FD, g_plateState.C_R[2]);
}
//вычисление адреса частоты для установки 1 вольта на выходе
unsigned int SolveFreqIndCorrCoef(){
	return (g_plateState.freq[0]*100 + g_plateState.freq[1] *10 + g_plateState.freq[2] - 15) /5;
}
void SetOutLevelFromFreq(){
	g_plateState.C_R[2][3] = (g_plateState.C_R[2][3] & 0x80) | g_plateState.corr_coef[SolveFreqIndCorrCoef()];
	Write_Controll(0x07FD, g_plateState.C_R[2]);
}
//////////////////////////////////////////////////////////////////////////
/************************************************************************/
/* Функции для чтения записи калибровочной кривой детектора селектора   */
/************************************************************************/
//Вычисление индекса частоты в массиве коэффициентов калибровки
//Разбиение частотоного диапазона	//[1,5 - 3) [3 - 4) [4 - 5) ... [29 - 30] 
//Точки измерения					//    2.5     3.5     4.5         29.5	  
//Индексы в массиве					//	   0		1		2

unsigned char DetFreqInd(unsigned char* freq){
	if (freq[0]*10 + freq[1] == 30)
		return 29;
	if (freq[0]*10 + freq[1] == 1)
		return 0;
	return (freq[0]*10 + freq[1]) - 2;	
}
//////////////////////////////////////////////////////////////////////////
void SaveSelectorSign(){
		//TODO: Добавляется запись константы свидетельствующей о том что селктор настроен и работает
		unsigned int shift_addr = 0xFFF8;
		uchar data[2] = {0x5A,0x3E};
		WriteData2FlashIIC(shift_addr,data,2);
}
//////////////////////////////////////////////////////////////////////////
void SaveDetCoef(){
	upas_out = read_steady_adc(ADC_UPAS_OUT_PIN);
	WriteDetCal2Flash(DetFreqInd(g_plateState.freq),&upas_out);
	SaveSelectorSign();
	
}

//////////////////////////////////////////////////////////////////////////
unsigned char ReadDetCoef(){
	uchar temp_coef;
	ReadDetCalInFlash(DetFreqInd(g_plateState.freq),&temp_coef);
	return temp_coef;
}
/************************************************************************/
//////////////////////////////////////////////////////////////////////////
//Функция подстройки уровня
void TuneLvlLebedSel(){
	SetLebedSelFreqInFlash();
	uchar normalLvl = ReadDetCoef();
	upas_out = read_steady_adc(ADC_UPAS_OUT_PIN);
	uchar delta = 2;
	uchar maxA = 63;
	while ( abs((char)upas_out - (char)normalLvl) > delta)
	{
		if (((char)upas_out - (char)normalLvl) > 0)
			g_A += 1;
		else 
			g_A -= 1;
		if ( (g_A == 0) || (g_A == maxA)) break;
		LCA2out(g_C1,g_L1,g_C2,g_L2,g_A,g_plateState.outLCA);//записываем в EEPROM
		SendA((unsigned int*)g_plateState.outLCA);
		if (!(g_A & 0x20))
			_delay_us(500);
			else
			_delay_us(100);
		upas_out = read_steady_adc(ADC_UPAS_OUT_PIN);
	}
		
}
//////////////////////////////////////////////////////////////////////////
void InitPrk(){
	ReadStateFromEEPROM(&g_plateState,(void*)&g_eepromPlateState);
	InitAltera();
	
	Write_Controll(0x07FF, g_plateState.C_R[0]);
	Write_Controll(0x07FE, g_plateState.C_R[1]);
	Write_Controll(0x07FD, g_plateState.C_R[2]);
	Write_Controll(0x07FC, g_plateState.C_R[3]);
	
	delay_ms(10);
	SET_ALTERA_RESET(0);
	delay_ms(5);
	SET_ALTERA_ENABLE(1);
	_delay_ms(10);
	WriteFreqToPrk(g_plateState.freq);
	ChangeModePrk(&g_plateState);
}
//////////////////////////////////////////////////////////////////////////
void SetnOTP(char val){
		if (val == 0)
		{
			PORTB &= ~_BV(7);
		}else
			PORTB |= _BV(7);
			
		if (g_plateState.selType == LEBED_TYPE)
		{
			SET_SEL_nOTP(val);
		}
		if (g_plateState.selType == ZAH_TYPE)
		{
			SetZselOpen(val == 0);//TODO: отпирание запирание селектора захарова
		}		
		
}
//////////////////////////////////////////////////////////////////////////
void SetOTPLines(){
// 	if (g_plateState.output)
// 	{
// 		PORTB |= _BV(7);
// 	}else
// 		PORTB &= ~_BV(7);
	
}
//////////////////////////////////////////////////////////////////////////


//#include "OneWire.h"


int main(void)
{
	Init_LCD();
	LCD_Clear();
	LCD_DisplayStringRus(0,0,"      ЗАГРУЗКА      ");
	InitKeyboard();
	InitPrk();
	InitSelPort();
	IICInit();
	InitUart();
	
	
// 	----------------------------------------
//Экспериментальный кусок, эксперименты с ключами от домофона
// 	while (1)
// 	{
// 	
// 	therm_reset();
// 	_delay_ms(50);
// 	therm_write_byte(0x33);
// 	char temp[8];
// 	for (char i = 0; i< 8; i++)
// 	{
// 		temp[i] = therm_read_byte();
// 	}
// 	LCD_Clear();
// 		for (char i = 0; i< 8; i++)
// 		{
// 			LCD_DisplayCharacter(g_hexOut[(temp[i] & 0xF0)>>4]);
// 			LCD_DisplayCharacter(g_hexOut[(temp[i] & 0x0F)>>0]);
// 			
// 		}
// 		_delay_ms(1000);
// 		char data[4];
// 		char key_to_write[] = {0x01,0xff,0xff,0xff,0xff,0x00,0x00,0x9b};
// 		if (temp[7] != 0xFF)
// 		{
// 			LCD_Clear();
// 			for (char i = 0 ; i < 8; i++)
// 			{
// 			
// 			therm_reset();
// 			    data[0] = 0x3C; // "WRITE" command
// 			    data[1] = i; // programming i-th byte
// 			    data[2] = 0;
// 			    data[3] = key_to_write[i];
// 				for (char j = 0; j < 4; j++)
// 				{
// 					therm_write_byte(data[j]);
// 				}
// 				char crc = therm_read_byte();
// 				LCD_DisplayCharacter(g_hexOut[(crc & 0xF0)>>4]);
// 				LCD_DisplayCharacter(g_hexOut[(crc & 0x0F)>>0]);
// 				//_delay_ms(100);
// 				send_programming_impulse();
// 			}
// 			LCD_DisplayCharacter('!');
// 			_delay_ms(1000);
// 		}
// 						
// 	}
// 	//----------------------------------------
	
	
	
	
	
	 g_currentItem = &g_mainMenuItem;
	//g_currentItem = & g_prkSettingMenuItem;
	#ifdef DDC_BOARD
		g_currentItem = &g_zselSettingMenuItem;
	#endif
	g_currentItem->draw();

	SendCOMBytes((uchar*)"REBOOT",7);
	DDRB |= _BV(7);//Вывод отпирания для платы предкорректора
	
    while(1)
    {
// 		char a = read_adc(ADC_UPAS_OUT_PIN);
// 		LCD_DisplayCharacter(g_hexOut[(a & 0xF0) >> 4]);
// 		LCD_DisplayCharacter(g_hexOut[(a & 0x0F) ]);
// 		_delay_ms(100);
// 		LCD_Clear();
      ScanKeyboard(KeySelect);
	
    }
}