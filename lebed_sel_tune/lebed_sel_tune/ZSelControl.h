/* 
* ZSelControl.h
*
* Created: 08.06.2015 15:05:37
* Author: USER
*/


#ifndef __ZSELCONTROL_H__
#define __ZSELCONTROL_H__

#include "main.h"

#define ZAH_SEL_PORT		PORTE
#define ZAH_SEL_PORT_DDR	DDRE
#define ZAH_CLKK_PIN		6
#define ZAH_CLKS_PIN		5
#define ZAH_DATA_PIN		7
#define ZAH_nOTP_PIN		4

#define ZAH_CAP1_PORT		PORTF
#define ZAH_CAP1_PORT_DDR	DDRF
#define ZAH_CAP1_PIN		7



#define ZAH_SEL_DELAY 5

#ifndef SET_LINE
#define SET_LINE(port,line,val) (val == 0)?(port &= ~_BV(line)):(port |= _BV(line))
#endif


#ifndef SET_LINE_DIR
enum {DIR_IN,DIR_OUT};
#define SET_LINE_DIR(port,line,val) (val == 0)?(port &= ~_BV(line)):(port |= _BV(line))
#endif


#define SET_ZAH_CLKK(a)				SET_LINE(ZAH_SEL_PORT,ZAH_CLKK_PIN,a)
#define SET_ZAH_CLKS(a)				SET_LINE(ZAH_SEL_PORT,ZAH_CLKS_PIN,a)
#define SET_ZAH_DATA(a)				SET_LINE(ZAH_SEL_PORT,ZAH_DATA_PIN,a)
#define SET_ZAH_SEL_nOTP(a)			SET_LINE(ZAH_SEL_PORT,ZAH_nOTP_PIN,a)
#define SET_ZAH_CAP1_CLK(a)			SET_LINE(ZAH_CAP1_PORT,ZAH_CAP1_PIN,a)

#define SET_ZAH_CLKK_DIR(a)				SET_LINE_DIR(ZAH_SEL_PORT_DDR,ZAH_CLKK_PIN,a)
#define SET_ZAH_CLKS_DIR(a)				SET_LINE_DIR(ZAH_SEL_PORT_DDR,ZAH_CLKS_PIN,a)
#define SET_ZAH_DATA_DIR(a)				SET_LINE_DIR(ZAH_SEL_PORT_DDR,ZAH_DATA_PIN,a)
#define SET_ZAH_SEL_nOTP_DIR(a)			SET_LINE_DIR(ZAH_SEL_PORT_DDR,ZAH_nOTP_PIN,a)
#define SET_ZAH_CAP1_DIR(a)				SET_LINE_DIR(ZAH_CAP1_PORT_DDR,ZAH_CAP1_PIN,a)
		   
enum regAddr {LREG = 1, LINKREG = 3, CAPREG = 2, CAP1REG = 5, CAP2REG = 6, AREG = 4};

void InitZsel();	
void SetZselL (char l);
void SetZselLink(char Link);
void SetZselCap (char Cap);
void SetZselCap1(int Cap1);
void SetZselCap2(int Cap2);
void SetZselA (char A);
void ChangeReg	(char);
void WriteByte(char);

#endif //__ZSELCONTROL_H__
