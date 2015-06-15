/* 
* ZSelControl.h
*
* Created: 08.06.2015 15:05:37
* Author: USER
*/


#ifndef __ZSELCONTROL_H__
#define __ZSELCONTROL_H__

#include "main.h"

//данные функции должны быть релизованы в проекте для проверки памяти селектора, или в виде заглушек
extern void WriteData2FlashIIC(unsigned int startAddr,uchar *data,uchar numOfDataBytes);
extern void ReadDataInFlashIIC(unsigned int* startAddr,uchar *data,uchar numOfDataBytes);

#define ZAH_SEL_PORT		SEL_PORT
#define ZAH_SEL_PORT_DDR	SEL_PORT_DDR
#define ZAH_CLKK_PIN		CLKK_PIN
#define ZAH_CLKS_PIN		CLKS_PIN
#define ZAH_DATA_PIN		SEL_DATA_PIN
#define ZAH_nOTP_PIN		SEL_nOTP_PIN

// #define ZAH_CAP1_PORT		PORTF
// #define ZAH_CAP1_PORT_DDR	DDRF
// #define ZAH_CAP1_PIN		7



#define ZAH_SEL_DELAY 2

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


#define SET_ZAH_CLKK_DIR(a)				SET_LINE_DIR(ZAH_SEL_PORT_DDR,ZAH_CLKK_PIN,a)
#define SET_ZAH_CLKS_DIR(a)				SET_LINE_DIR(ZAH_SEL_PORT_DDR,ZAH_CLKS_PIN,a)
#define SET_ZAH_DATA_DIR(a)				SET_LINE_DIR(ZAH_SEL_PORT_DDR,ZAH_DATA_PIN,a)
#define SET_ZAH_SEL_nOTP_DIR(a)			SET_LINE_DIR(ZAH_SEL_PORT_DDR,ZAH_nOTP_PIN,a)

		   
enum regAddr {ALLREG = 0, LREG = 1, LINKREG = 3, CAPREG = 2, CAP1REG = 5, CAP2REG = 6, AREG = 4};

// структура для хранения параметров селектора захарова
typedef struct{
	char L;
	char C;
	char link;
	char A;
	int cap1;
	int cap2;
} ZSELPARAM;

typedef struct{
	int  freq;
	char L;
	char C;
	char Link;
	} FREQGRIDBORDER;

void InitZsel();	
void SetZselParam(ZSELPARAM *param,regAddr typeOfParam);
void SetZselCap1(int Cap1);
void ChangeReg (regAddr regType);
void WriteByte(char);
void SetZselOpen(bool state);
void SetZselFreq(int freq, ZSELPARAM *param);
void TuneZselAtten(ZSELPARAM * param);

bool TestZsel(char typeOfTest);

#endif //__ZSELCONTROL_H__
