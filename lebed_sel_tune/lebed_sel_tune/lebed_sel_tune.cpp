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
char g_hex[] ="0123456789ABCDEF";	
void KeySelect(unsigned char a){//эту функцию следует перенести в класс меню

	switch (a){
		case 0x20:g_currentItem->escFunction(g_currentItem);break;
		case 0x10:g_currentItem->okFunction(g_currentItem);break;
		case 0x21:g_currentItem->upFunction(g_currentItem);break;
		case 0x11:g_currentItem->downFunction(g_currentItem);break;
		case 0x22:g_currentItem->leftFunction(g_currentItem);break;
		case 0x12:g_currentItem->rightFunction(g_currentItem);break;
		
	}
}
///////////////////////////////////////////////////////////////////////////////////////
//******************************************************************************//
//**  Функции управления формирователем сигналов				*//
//*****************************************************************************//

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
///////////////////////////////////////////////////////////////////////////
//************* Изменение выходного напряжения платы**********//
//////////////////////////////////////////////////////////////////////////
//от 1,5 до 28,5 через 0,5 МГц
//установка уровня для текущей частоты
void SetOutLevel(uchar lvl){
// 	lvl = lvl & 0x7F;
// 	g_plateState.corr_coef[SolveFreqIndCorrCoef()] = lvl & 0x7F;
	SetAtt(lvl & 0x7F,SolveFreqIndCorrCoef());
	
}
//вычисление адреса частоты для установки 1 вольта на выходе
unsigned int SolveFreqIndCorrCoef(){
	return (g_plateState.freq[0]*100 + g_plateState.freq[1] *10 + g_plateState.freq[2] - 15) /5;
}
void SetOutLevelFromFreq(){
	SetAttFromInd(SolveFreqIndCorrCoef())	;
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

//----------------------------------------
#include "PWC.h"
//////////////////////////////////////////////////////////////////////////
void SetnOTP(char val){
// 		if (val == 0)
// 		{
// 			PORTB &= ~_BV(7);
// 		}else
// 			PORTB |= _BV(7);
// 		
		OpenPwc(val == 0)	;
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



int main(void)
{
	lcd_init(LCD_DISP_ON_CURSOR_BLINK);

	lcd_puts("      ЗАГРУЗКА      ");

	    
				    
	InitKeyboard();
	ReadStateFromEEPROM(&g_plateState,(void*)&g_eepromPlateState);
	//InitPrk();
	InitPwc();
	InitSelPort();
 	IICInit();
 	InitUart();

	 sei();
 	 g_currentItem = &g_mainMenuItem;
// 	//g_currentItem = & g_prkSettingMenuItem;
// 	#ifdef DDC_BOARD
// 		g_currentItem = &g_zselSettingMenuItem;
// 	#endif
//g_currentItem->okFunction(g_currentItem);
 	g_currentItem->draw();
// 
 	SendCOMBytes((uchar*)"REBOOT",7);
// 	DDRB |= _BV(7);//Вывод отпирания для платы предкорректора
// 	
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