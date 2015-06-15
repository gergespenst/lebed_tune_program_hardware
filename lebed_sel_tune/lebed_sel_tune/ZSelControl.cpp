/* 
* ZSelControl.cpp
*
* Created: 08.06.2015 15:05:37
* Author: USER
*/


#include "ZSelControl.h"

//////////////////////////////////////////////////////////////////////////
void SetZselCap1( int Cap1 )//ф-я записи значения в электронный потенциометр
{	
	char i = 0;
	
	ChangeReg(CAP1REG);
		SET_ZAH_CLKK_DIR(DIR_OUT);
		SET_ZAH_DATA_DIR(DIR_OUT);
		SET_ZAH_DATA(0);
		SET_ZAH_CLKK(0);
	for(i = 0; i < 8; i++)	{//первые 8 бит пустышки
		SET_ZAH_DATA(0);
		_delay_us(ZAH_SEL_DELAY);
		SET_ZAH_CLKK(1);
		_delay_us(ZAH_SEL_DELAY);
		SET_ZAH_CLKK(0);
		_delay_us(ZAH_SEL_DELAY);
	}
	for(i = 0; i < 10; i++)	{// 10 бит информации
		/*LCD_DisplayCharacter(g_hexOut[(Cap1 >> 9) & 0x0001]);*/
		SET_ZAH_DATA( (((unsigned int)Cap1 >> (9 - i)) & 0x0001) == 0x0001);
		_delay_us(ZAH_SEL_DELAY);
		SET_ZAH_CLKK(1);
		_delay_us(ZAH_SEL_DELAY);
		SET_ZAH_CLKK(0);
		_delay_us(ZAH_SEL_DELAY);
	}	
	//_delay_ms(1000);
	for(i = 0; i < 6; i++)	{//дополнение до 24 бит для соответствия формату 
			SET_ZAH_DATA(0);
			_delay_us(ZAH_SEL_DELAY);
			SET_ZAH_CLKK(1);
			_delay_us(ZAH_SEL_DELAY);
			SET_ZAH_CLKK(0);
			_delay_us(ZAH_SEL_DELAY);
		}
	SET_ZAH_CLKK(0);
	ChangeReg(CAP1REG);//передергивание линии CS для применения записанного значения
}

//////////////////////////////////////////////////////////////////////////
void ChangeReg( regAddr regType)
{
	char i = 0;
	SET_ZAH_CLKS_DIR(DIR_OUT);
	SET_ZAH_DATA_DIR(DIR_OUT);
	SET_ZAH_DATA(0);
	SET_ZAH_CLKS(0);
_delay_us(15);//пауза для прохождения сигнала сброса на счетчик по спадающему фронту CLKS
	for (i = 0; i < regType; i++)
	{
		SET_ZAH_DATA(1);
		_delay_us(ZAH_SEL_DELAY);
		SET_ZAH_DATA(0);
		_delay_us(ZAH_SEL_DELAY);
	}
	
	SET_ZAH_CLKS(1);
	SET_ZAH_DATA(0);
}
//////////////////////////////////////////////////////////////////////////
void WriteByte( char data)
{
	
	char i = 0;
	SET_ZAH_CLKK_DIR(DIR_OUT);
	SET_ZAH_DATA_DIR(DIR_OUT);
	SET_ZAH_DATA(0);
	SET_ZAH_CLKK(0);
	for(i = 0; i < 8; i++)	{
		
		SET_ZAH_DATA((data & 0x80) == 0x80);
		_delay_us(ZAH_SEL_DELAY);
		SET_ZAH_CLKK(1);
		_delay_us(ZAH_SEL_DELAY);
		SET_ZAH_CLKK(0);
		_delay_us(ZAH_SEL_DELAY);
		data = data << 1;
	}
	SET_ZAH_DATA(0);
	SET_ZAH_CLKK(0);
}
//////////////////////////////////////////////////////////////////////////
void InitZsel()
{
	SET_ZAH_CLKK_DIR(DIR_OUT);
	SET_ZAH_CLKS_DIR(DIR_OUT);
	SET_ZAH_DATA_DIR(DIR_OUT);				
	SET_ZAH_SEL_nOTP_DIR(DIR_OUT);		
		
}
//////////////////////////////////////////////////////////////////////////
void SetZselParam( ZSELPARAM *param,regAddr typeOfParam )
{
	switch (typeOfParam){
		case ALLREG:	{};
		case LREG:		{
			ChangeReg(LREG);
			WriteByte(param->L);
			if(ALLREG != typeOfParam) break;
			};
		case LINKREG:		{
			ChangeReg(LINKREG);
			WriteByte(param->link);
			if(ALLREG != typeOfParam) break;
		};
		case CAPREG:		{
			ChangeReg(CAPREG);
			WriteByte(param->C);
			if(ALLREG != typeOfParam) break;
		};
		case CAP1REG:		{
			SetZselCap1(param->cap1);
			if(ALLREG != typeOfParam) break;
		};
		case AREG:		{
			ChangeReg(AREG);
			WriteByte(param->A);
			if(ALLREG != typeOfParam) break;
		};
		case CAP2REG:		{
			
			if(ALLREG != typeOfParam) break;
		};
		default:break;
	}
}
//////////////////////////////////////////////////////////////////////////
void SetZselOpen( bool state )
{
	SET_ZAH_SEL_nOTP_DIR(DIR_OUT);	
	SET_ZAH_SEL_nOTP(state != true)	;//если установлено отперт - истина то устанавливаем линию на выходе в низкий уровень
}
//////////////////////////////////////////////////////////////////////////
bool TestZsel( char typeOfTest )
{
	unsigned char data[2] = {0xBE,0xEF};
	unsigned int addr = 0xFFF8;
	switch (typeOfTest)
	{
	case 0:{
		WriteData2FlashIIC(addr,data,2);
		_delay_ms(100);
		ReadDataInFlashIIC(&addr,data,2);
		return ((data[0] == 0xBE) & (data[1] == 0xEF));
		}break;
	case (1):
	case (2):{
		ZSELPARAM tempSelparam = {0x80,0x80,0x80,0x80,0x0380,0x0380};
		char flag = 0;
		while (1)
		{
			SetZselParam(&tempSelparam,ALLREG);
			if (2 == typeOfTest)
			{
				_delay_ms(1);
			}else _delay_ms(200);
			
			if (0 == flag)
			{
				tempSelparam.A		= tempSelparam.A	 /2;
				tempSelparam.C		= tempSelparam.C	 /2;
				tempSelparam.L		= tempSelparam.L	 /2;
				tempSelparam.link	= tempSelparam.link	 /2;
				tempSelparam.cap1	= tempSelparam.cap1	 /2;
				tempSelparam.cap2	= tempSelparam.cap2	 /2;
				if(0x01 == tempSelparam.A ) flag = 1;
			}else{
				tempSelparam.A		= tempSelparam.A	 *2;
				tempSelparam.C		= tempSelparam.C	 *2;
				tempSelparam.L		= tempSelparam.L	 *2;
				tempSelparam.link	= tempSelparam.link	 *2;
				tempSelparam.cap1	= tempSelparam.cap1	 *2;
				tempSelparam.cap2	= tempSelparam.cap2	 *2;	
				if(0x80 == tempSelparam.A ) flag = 0;			
			}
			
		}
		
	}break;
	
	}
}
//////////////////////////////////////////////////////////////////////////
char hex[] ="0123456789ABCDEF";	
FREQGRIDBORDER freqBordersGrid[] = {{795,4,0,1},{685,4,1,1},{660,4,2,1},{600,4,4,2},{500,4,8,2},{460,4,10,2},{440,4,12,4},{400,4,15,12},{0,0,0,0}};
void SetZselFreq( int freq, ZSELPARAM *param )
{
	ZSELPARAM tempParam;
	FREQGRIDBORDER freqBorder = freqBordersGrid[0];
	char i = 0;
	while (freqBorder.freq != 0)
	{
		freqBorder = freqBordersGrid[i];
		if (freqBorder.freq >= freq)
		{
			
		}else{
			freqBorder = freqBordersGrid[i - 1];
			break;
		}
		
		i++;
	}
	
	
	tempParam.C = freqBorder.C;
	tempParam.link = freqBorder.Link;
	tempParam.L = freqBorder.L;
	tempParam.A = 0x1F;
	
	
	char ampStart, ampStop, ampCenter1,ampCenter2;
	int startC = 0x0000, stopC = 0x03FF, lengthOfDiap, indOfCenter, currentC;
	char str[] = "C:XXX Amp:XX\n";
	
	//tempParam.cap1 = startC;
 	SetZselParam(&tempParam,ALLREG);
// 	//_delay_ms(100);
// 	ampStart = read_steady_adc(ADC_UPAS_OUT_PIN);
// 	
// // 			str[2]	= hex[((startC & 0xF00) >> 8)];
// // 			str[3]	= hex[((startC & 0x0F0) >> 4)];
// // 			str[4]	= hex[((startC & 0x00F) >> 0)];
// // 			str[10] = hex[((ampStart & 0xF0) >> 4)];
// // 			str[11] = hex[((ampStart & 0x0F) >> 0)];
// // 			extern void SendCOMBytes(unsigned char*,char);
// // 			SendCOMBytes((unsigned char*)str,14);
// 			
// 	tempParam.cap1 = stopC;
// 	SetZselParam(&tempParam,CAP1REG);
// 	//_delay_ms(100);
// 	ampStop =  read_steady_adc(ADC_UPAS_OUT_PIN);
// 				str[2]	= hex[((stopC & 0xF00) >> 8)];
// 				str[3]	= hex[((stopC & 0x0F0) >> 4)];
// 				str[4]	= hex[((stopC & 0x00F) >> 0)];
// 				str[10] = hex[((ampStop & 0xF0) >> 4)];
// 				str[11] = hex[((ampStop & 0x0F) >> 0)];
// 				extern void SendCOMBytes(unsigned char*,char);
// 				SendCOMBytes((unsigned char*)str,14);
#define  EPSILON 2
	while (abs(stopC - startC) > EPSILON)
	{
		
		lengthOfDiap = (stopC - startC);
		indOfCenter = lengthOfDiap/2 + startC;
		currentC = indOfCenter;

		tempParam.cap1 = indOfCenter - EPSILON;
		
		SetZselParam(&tempParam,CAP1REG);
		
		ampCenter1 =  read_steady_adc(ADC_UPAS_OUT_PIN);
		
				tempParam.cap1 = indOfCenter + EPSILON;
				
				SetZselParam(&tempParam,CAP1REG);
				
				ampCenter2 =  read_steady_adc(ADC_UPAS_OUT_PIN);
// 		str[2]	= hex[((currentC & 0xF00) >> 8)];
// 		str[3]	= hex[((currentC & 0x0F0) >> 4)];
// 		str[4]	= hex[((currentC & 0x00F) >> 0)];
// 		str[10] = hex[((ampCenter1 & 0xF0) >> 4)];
// 		str[11] = hex[((ampCenter1 & 0x0F) >> 0)];
// 		extern void SendCOMBytes(unsigned char*,char);
// 		SendCOMBytes((unsigned char*)str,14);
// 		_delay_ms(15);
// 		 
		if (ampCenter1 < ampCenter2)
		{
			startC = indOfCenter;
			ampStart = ampCenter1;
		}
		else{
			stopC = indOfCenter;
			ampStop = ampCenter1;
		}
	}
	
	
	*param = tempParam;
}
//////////////////////////////////////////////////////////////////////////
void TuneZselAtten( ZSELPARAM * param )
{
	#define NORMAL_LVL 0xF0
	ZSELPARAM tempParam = *param;
	
	char startA = 0, stopA = 0x1F,length,center;
	char amplStart, amplStop,amplCenter;
	
	tempParam.A = startA;
	SetZselParam(&tempParam,AREG);
	amplStart = read_steady_adc(ADC_UPAS_OUT_PIN);
	
	tempParam.A = stopA;
	SetZselParam(&tempParam,AREG);
	amplStop = read_steady_adc(ADC_UPAS_OUT_PIN);
	
	while((stopA - startA) > 1) {
		center = startA  + ( stopA - startA)/2;
		
		tempParam.A = center;
		SetZselParam(&tempParam,AREG);
		amplCenter = read_steady_adc(ADC_UPAS_OUT_PIN);

		if (((signed int)(NORMAL_LVL - amplCenter) ) *((signed int)(NORMAL_LVL - amplStart) ) > 0)
		{
			startA = center;
			amplStart = amplCenter;
		}else
		{
			stopA = center;
			amplStop = center;
			
		}
	}
	*param = tempParam;
	
}

