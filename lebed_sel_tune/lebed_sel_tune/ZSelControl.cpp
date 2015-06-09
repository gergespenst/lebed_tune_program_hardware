/* 
* ZSelControl.cpp
*
* Created: 08.06.2015 15:05:37
* Author: USER
*/


#include "ZSelControl.h"



void SetZselL( char L )
{
	ChangeReg(LREG);
	WriteByte(L);
}
//////////////////////////////////////////////////////////////////////////
void SetZselLink( char Link )
{
	ChangeReg(LINKREG);
	WriteByte(Link);
	
}
//////////////////////////////////////////////////////////////////////////
void SetZselCap( char Cap )
{
	ChangeReg(CAPREG);
	WriteByte(Cap);
}
//////////////////////////////////////////////////////////////////////////
void SetZselCap1( int Cap1 )
{	
	char i = 0;
	
	ChangeReg(CAP1REG);
	SET_ZAH_CAP1_DIR(DIR_OUT);
	SET_ZAH_CAP1_CLK(0);
	for(i = 0; i < 8; i++)	{
		SET_ZAH_DATA(0);
		_delay_us(ZAH_SEL_DELAY);
		SET_ZAH_CAP1_CLK(1);
		_delay_us(ZAH_SEL_DELAY);
		SET_ZAH_CAP1_CLK(0);
		_delay_us(ZAH_SEL_DELAY);
	}
	char g_hexOut[] = "0123456789abcdef";
	for(i = 0; i < 10; i++)	{
		/*LCD_DisplayCharacter(g_hexOut[(Cap1 >> 9) & 0x0001]);*/
		SET_ZAH_DATA( (((unsigned int)Cap1 >> (9 - i)) & 0x0001) == 0x0001);
		_delay_us(ZAH_SEL_DELAY);
		SET_ZAH_CAP1_CLK(1);
		_delay_us(ZAH_SEL_DELAY);
		SET_ZAH_CAP1_CLK(0);
		_delay_us(ZAH_SEL_DELAY);
	}	
	//_delay_ms(1000);
	for(i = 0; i < 6; i++)	{
			SET_ZAH_DATA(0);
			_delay_us(ZAH_SEL_DELAY);
			SET_ZAH_CAP1_CLK(1);
			_delay_us(ZAH_SEL_DELAY);
			SET_ZAH_CAP1_CLK(0);
			_delay_us(ZAH_SEL_DELAY);
		}
}
//////////////////////////////////////////////////////////////////////////
void SetZselCap2( int Cap2 )
{
	ChangeReg(CAP2REG);	
	
}
//////////////////////////////////////////////////////////////////////////
void SetZselA( char A )
{
	ChangeReg(AREG);
	WriteByte(A);
	
}
//////////////////////////////////////////////////////////////////////////
void ChangeReg( char regType)
{
	char i = 0;
	SET_ZAH_CLKS(0);

	for (i = 0; i < regType; i++)
	{
		SET_ZAH_DATA(1);
		_delay_us(ZAH_SEL_DELAY);
		SET_ZAH_DATA(0);
		_delay_us(ZAH_SEL_DELAY);
	}
	
	SET_ZAH_CLKS(1);
}
//////////////////////////////////////////////////////////////////////////
void WriteByte( char data)
{
	
	char i = 0;
	SET_ZAH_DATA(0);
	SET_ZAH_CLKK(0);
	for(i = 0; i < 8; i++)	{
		
		SET_ZAH_DATA((data & 0x1) == 1);
		_delay_us(ZAH_SEL_DELAY);
		SET_ZAH_CLKK(1);
		_delay_us(ZAH_SEL_DELAY);
		SET_ZAH_CLKK(0);
		_delay_us(ZAH_SEL_DELAY);
		data = data >> 1;
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

