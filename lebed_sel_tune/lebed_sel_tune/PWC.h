/* 
* PWC.h
*
* Created: 10.02.2016 10:20:02
* Author: USER
* ���� � ��������� ��� ���������� ��� � ����
*/


#ifndef __PWC_H__
#define __PWC_H__
#include <avr/eeprom.h>

typedef struct TPWCPARAM{
	char par_set[30];//��������� �����			start 1024
	char par_stm[28];//��������� ����������		start 1024 + 30
	char par_spt[32];//��������� ��������������	start 1024 + 58
	char par_stk[13];//��������� ���������		start 1024 + 90
	char att[64];//�������� ����������			start 1024 + 256		1280
};

// void ReadPwcParamFromEEPROM(TPWCPARAM * pwc_param);
// void WritePwcParamToEEPROM(TPWCPARAM * pwc_param);
// 
// void InitPwc();
// void SetPwcFreq(uchar* freq);
// void SetPwcMode(char mode);
// void OpenPwc(char open);




#endif //__PWC_H__
