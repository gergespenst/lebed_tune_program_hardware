/* 
* ZSelControl.cpp
*
* Created: 08.06.2015 15:05:37
* Author: USER
*/


#include "ZSelControl.h"

//////////////////////////////////////////////////////////////////////////
void SetZselCap1( int Cap1 )//�-� ������ �������� � ����������� ������������
{	
	char i = 0;
	
	ChangeReg(CAP1REG);
		SET_ZAH_CLKK_DIR(DIR_OUT);
		SET_ZAH_DATA_DIR(DIR_OUT);
		SET_ZAH_DATA(0);
		SET_ZAH_CLKK(0);
	for(i = 0; i < 8; i++)	{//������ 8 ��� ��������
		SET_ZAH_DATA(0);
		_delay_us(ZAH_SEL_DELAY);
		SET_ZAH_CLKK(1);
		_delay_us(ZAH_SEL_DELAY);
		SET_ZAH_CLKK(0);
		_delay_us(ZAH_SEL_DELAY);
	}
	for(i = 0; i < 10; i++)	{// 10 ��� ����������
		/*LCD_DisplayCharacter(g_hexOut[(Cap1 >> 9) & 0x0001]);*/
		SET_ZAH_DATA( (((unsigned int)Cap1 >> (9 - i)) & 0x0001) == 0x0001);
		_delay_us(ZAH_SEL_DELAY);
		SET_ZAH_CLKK(1);
		_delay_us(ZAH_SEL_DELAY);
		SET_ZAH_CLKK(0);
		_delay_us(ZAH_SEL_DELAY);
	}	
	//_delay_ms(1000);
	for(i = 0; i < 6; i++)	{//���������� �� 24 ��� ��� ������������ ������� 
			SET_ZAH_DATA(0);
			_delay_us(ZAH_SEL_DELAY);
			SET_ZAH_CLKK(1);
			_delay_us(ZAH_SEL_DELAY);
			SET_ZAH_CLKK(0);
			_delay_us(ZAH_SEL_DELAY);
		}
	SET_ZAH_CLKK(0);
	ChangeReg(CAP1REG);//�������������� ����� CS ��� ���������� ����������� ��������
}

//////////////////////////////////////////////////////////////////////////
void ChangeReg( regAddr regType)
{
	char i = 0;
	SET_ZAH_CLKS_DIR(DIR_OUT);
	SET_ZAH_DATA_DIR(DIR_OUT);
	SET_ZAH_DATA(0);
	SET_ZAH_CLKS(0);
_delay_us(15);//����� ��� ����������� ������� ������ �� ������� �� ���������� ������ CLKS
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
	SET_ZAH_SEL_nOTP(state != true)	;//���� ����������� ������ - ������ �� ������������� ����� �� ������ � ������ �������
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
FREQGRIDBORDER freqBordersGrid[] = {
	{2970,1,0,1},{2500,1,1,1},{2300,1,2,1},{2120,1,4,2},{1730,1,8,4},{1620,1,9,4},{1580,1,10,4},{1565,1,10,8},//,{1520,1,11,8},//{1100,1,12,8},
	{1550,2,0,1},{1300,2,1,1},{1270,2,2,1},{1150,2,4,2},{970,2,8,8},{800,2,13,8},{785,2,13,0xC},
	{780,4,0,1},{685,4,1,1},{660,4,2,1},{600,4,4,2},{500,4,8,2},{460,4,10,2},{440,4,12,4},{400,4,15,12},
	{395,8,0,1},{350,8,1,1},{330,8,2,1},{300,8,4,1},{250,8,8,2},{220,8,12,2},{200,8,15,3},{185,8,20,9},{165,8,25,9},{155,8,30,9},
		{150,8,31,9},
		{0,0,0,0}};

//////////////////////////////////////////////////////////////////////////
void FindFreqParam( int freq, ZSELPARAM * param )
{
	FREQGRIDBORDER freqBorder = freqBordersGrid[0];
	char i = 0;
	//����� ��������� L C link �� �������
	while (freqBorder.freq != 0)//���� �� �������� ����� ������
	{
		freqBorder = freqBordersGrid[i];//�������� ������� ������ ������
		if (freqBorder.freq >= freq)//���� ������� �������� ������ ��� ������������� ������� �� ������ ������
		{}else{//��� ������ ����� �� �������� � �������� ������ ��� �������������
		if(i > 0) i = i - 1; else i = 0;//���� ��� ��� ������� ������� �� ����� ���
		freqBorder = freqBordersGrid[i];//���� ��� ��� ������� �� �� ���� ������ �� ����� ���������� � ��������� �����
		break;
			}
	i++;
	}


	param->C = freqBorder.C;
	param->link = freqBorder.Link;
	param->L = freqBorder.L;
	param->A = 0x00;
	SetZselParam(param,ALLREG);
}
//////////////////////////////////////////////////////////////////////////

void TuneZselCap( ZSELPARAM * param )
{
	char ampCenter1,ampCenter2;
	int startC = 0x0000, stopC = 0x03FF, lengthOfDiap, indOfCenter, currentC;
	

	#define  EPSILON 2//�������� ���������� �������
	while (abs(stopC - startC) > EPSILON)//����� ��������� ���������
	{
		
		lengthOfDiap = (stopC - startC);
		indOfCenter = lengthOfDiap/2 + startC;
		currentC = indOfCenter;

		param->cap1 = indOfCenter - EPSILON;
		SetZselParam(param,CAP1REG);
		ampCenter1 =  read_steady_adc(ADC_UPAS_OUT_PIN);
		
		param->cap1 = indOfCenter + EPSILON;
		SetZselParam(param,CAP1REG);
		ampCenter2 =  read_steady_adc(ADC_UPAS_OUT_PIN);

		if (ampCenter1 < ampCenter2)
		{
			startC = indOfCenter;
		}
		else{
			stopC = indOfCenter;
		}
	}
	param->cap1 = indOfCenter;
	SetZselParam(param,ALLREG);
}

//////////////////////////////////////////////////////////////////////////
void SetZselFreq( int freq, ZSELPARAM *param )
{
	ZSELPARAM tempParam;
	FindFreqParam(freq,&tempParam);
	TuneZselCap(&tempParam);	
	*param = tempParam;//�������� ���������� ��������� ������
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


