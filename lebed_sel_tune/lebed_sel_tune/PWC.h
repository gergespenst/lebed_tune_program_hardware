/* 
* PWC.h
*
* Created: 10.02.2016 10:20:02
* Author: USER
* Файл с йункциями для управления ПВЦ в ПЛИС
*/


#ifndef __PWC_H__
#define __PWC_H__
#include <avr/eeprom.h>

typedef struct TPWCPARAM{
	char par_set[30];//параметры общие			start 1024
	char par_stm[28];//параметры модулятора		start 1024 + 30
	char par_spt[32];//параметры предкорректора	start 1024 + 58
	char par_stk[13];//канальные параметры		start 1024 + 90
	char att[64];//выходной аттенюатор			start 1024 + 256		1280
};

// void ReadPwcParamFromEEPROM(TPWCPARAM * pwc_param);
// void WritePwcParamToEEPROM(TPWCPARAM * pwc_param);
// 
// void InitPwc();
// void SetPwcFreq(uchar* freq);
// void SetPwcMode(char mode);
// void OpenPwc(char open);




#endif //__PWC_H__
