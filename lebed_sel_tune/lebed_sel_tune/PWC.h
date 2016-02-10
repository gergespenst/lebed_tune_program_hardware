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
#include <util/delay.h>
#define set_nCSS   PORTE|= 0x20; // nCSS=1 -----
#define clr_nCSS   PORTE&=~0x20; // nCSS=0 - связь с ПЛИС
#define set_CLKS   PORTE|= 0x40; // CLKS=1 -----
#define clr_CLKS   PORTE&=~0x40; // CLKS=0
#define set_DIOS   PORTB|= 0x01; // DIOS=1 -----
#define clr_DIOS   PORTB&=~0x01; // DIOS=0
#define DIOS_out   DDRB|=  0x01; // DIOS => output -----
#define DIOS_in    DDRB&=~0x01;  // DIOS => input
#define set_nSIN   PORTD|= 0x10; // nSIN=1 -----
#define clr_nSIN   PORTD&=~0x10; // nSIN=0
#define nSIN_out   DDRD|= 0x10;  // nSIN => output ----
#define nSIN_in    DDRD&=~0x10;  // nSIN => input
#define set_nCSS   PORTE|= 0x20; // nCSS=1 -----
#define clr_nCSS   PORTE&=~0x20; // nCSS=0 - связь с ПЛИС
#define set_RES    PORTB|= 0x04; // RES=1 -----
#define clr_RES    PORTB&=~0x04; // RES=0
#define set_RUN    PORTB|= 0x08; // RUN=1 -----
#define clr_RUN    PORTB&=~0x08; // RUN=0
#define set_nOTP   PORTE|= 0x04; // nOTP=1 ----
#define clr_nOTP   PORTE&=~0x04; // nOTP=0 - отпирание
#define set_nCON   PORTA|= 0x01; // nCON=1 ----
#define clr_nCON   PORTA&=~0x01; // nCON=0 - перезагрузка ПЛИС
#define set_nPRI   PORTE|= 0x08; // nPRI=1 ----
#define clr_nPRI   PORTE&=~0x08; // nPRI=0 - приоритет

typedef struct TPWCPARAM{
	char par_set[30];//параметры общие			start 1024
	char par_stm[28];//параметры модулятора		start 1024 + 30
	char par_stp[32];//параметры предкорректора	start 1024 + 58
	char par_stk[13];//канальные параметры		start 1024 + 90
	char att[64];//выходной аттенюатор			start 1024 + 256		1280
};

void ReadPwcParamFromEEPROM(TPWCPARAM * pwc_param);
// void WritePwcParamToEEPROM(TPWCPARAM * pwc_param);
// 
void InitPwc();
void SetPwcFreq(unsigned char* freq);
// void SetPwcMode(char mode);
void OpenPwc(char open);
void SetAtt(unsigned char att,unsigned char attInd);
void SetAttFromInd(unsigned char attInd);
void SaveAtts();




#endif //__PWC_H__
