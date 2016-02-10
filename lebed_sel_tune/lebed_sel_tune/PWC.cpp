/* 
* PWC.cpp
*
* Created: 10.02.2016 10:20:02
* Author: USER
*/


#include "PWC.h"
TPWCPARAM g_pwcParam;
unsigned char g_lev_att;
char rd_byte(void) {char i,a; a=0; DIOS_in;
	for (i=0; i<8; i++) {a=a*2; if ((PINB & 0x01)!=0) a+=1; set_CLKS;
	/*delay_us(1);*/ clr_CLKS;}; /*delay_us(1);*/    DIOS_out; return a;   }
void wr_byte(char a)  {char i;
		for (i=0; i<8; i++)  {if ((a & 0x80)!=0) set_DIOS; set_CLKS;
		/*delay_us(1);*/ clr_CLKS; set_DIOS;clr_DIOS; a=a*2;}; /*delay_us(1);*/}

void wr_par_set(void) //Запись общих параметров (com>+13dat>)
{char j/*,p6,p7,p8,p12*/;
	beg: clr_nSIN; _delay_us(4); set_nSIN; clr_nCSS; wr_byte(1);
	//p6=par_set[5]; /*TLF1*/          p7=par_set[6]; /*TLF2*/
	//p8=par_set[7]; /*ZVK*/           p12=par_set[8];/*VNI*/
	for (j=0; j</*6*/9; j++) wr_byte(g_pwcParam.par_set[j]);
	//                    wr_byte(p6);wr_byte(p7);wr_byte(p8);
	for (j=9; j<13;j++) wr_byte(g_pwcParam.par_set[j]); /*wr_byte(p12);*/ set_nCSS;
	nSIN_in; if ((PIND & 0x10)==0) goto endd; nSIN_out;
if ((g_pwcParam.par_stm[27] & 0x04)!=0) goto beg;            endd:;}

void wr_par_stm(void){char j; //Запись параметров конф.модулятора (com>+28dat>)
	beg: clr_nSIN; _delay_us(4); set_nSIN;
	clr_nCSS; wr_byte(14);
	for (j=0; j<28; j++) wr_byte(g_pwcParam.par_stm[j]);             set_nCSS;
	nSIN_in; if ((PIND & 0x10)==0) goto endd; nSIN_out;
if ((g_pwcParam.par_stm[27] & 0x02)!=0) goto beg;            endd:;}

void wr_par_stk(void) //Запись канальных параметров (com>+13dat>)с зациклив.
{char j;
	beg: clr_nSIN; _delay_us(4); set_nSIN;
	clr_nCSS; wr_byte(15);
	for (j=0; j<13; j++)  wr_byte(g_pwcParam.par_stk[j]);
	set_nCSS; nSIN_in; if ((PIND & 0x10)==0) goto endd; nSIN_out;
if ((g_pwcParam.par_stm[27] & 0x40)!=0) goto beg;                    endd:;}

void wr_par_stp(void) //Запись параметров приемника (com>+32dat>)
{char j,c,b;  b=g_pwcParam.par_stp[1] & 0x80;  // def_lev_att();
	//if(b!=0)c=lev_att & 0x7f; else 
	c= g_pwcParam.par_stp[1] & 0x7f;
	clr_nCSS; wr_byte(11);
	for (j=0;j<32;j++){if(j==1)wr_byte(g_lev_att & 0x7f); else wr_byte(g_pwcParam.par_stp[j]);};
set_nCSS;}



void InitPwc()
{
  DDRA=0x00; PORTA=0xff;//откл.порта А для загрузки памяти ПЛИСС
  DDRB=0xff; PORTB=0xf6;//RES=1,RUN=0,DIOS=0
  DDRC=0xff; PORTC=0x00; //C7..C0=0 => output
  DDRD=0x1f; PORTD=0xff; //D7,D6,D5 => input
  DDRE=0xef; PORTE=0xaf; //CLKS=0
  DDRF=0xf0; PORTF=0xf7; //F3..F0   => input; F3=0
  DDRG=0x1b; PORTG=0x1b;
 ReadPwcParamFromEEPROM(&g_pwcParam);
 clr_RES; clr_RUN; set_nOTP;
 clr_RUN;
 clr_nCON; _delay_ms(1);  set_nCON;
 while ( (PINA & 0x04) == 0){_delay_ms(1);};
 wr_par_stk(); wr_par_set();wr_par_stm();
 wr_par_stp();  set_RES; clr_RES; set_RUN; //wr_par_etH(); init_wizH();
 wr_par_stk();  wr_par_stp();	
   clr_nOTP; //clr_nOTP_;
   clr_nCSS; wr_byte(2); set_nCSS;
   set_nOTP;
   set_nPRI;
}
//----------------------------------------
void ReadPwcParamFromEEPROM( TPWCPARAM * pwc_param )
{
			for (char i=0; i<30; i++) pwc_param->par_set[i]=eeprom_read_byte((const uint8_t *)i+1024);
		    for (char i=0; i<28; i++) pwc_param->par_stm[i]=eeprom_read_byte((const uint8_t *)i+30+1024);
		    for (char i=0; i<32; i++) pwc_param->par_stp[i]=eeprom_read_byte((const uint8_t *)i+58+1024);
		    for (char i=0; i<13; i++) pwc_param->par_stk[i]=eeprom_read_byte((const uint8_t *)i+90+1024);
			for (char i=0; i<64; i++) pwc_param->att[i] =	eeprom_read_byte((const uint8_t *)i+1280);
	
}
//----------------------------------------
void OpenPwc( char open )
{
	if (open)
	{	 clr_nOTP;
		clr_nCSS; wr_byte(3); set_nCSS;//clr_nOTP;
		clr_nPRI;
	}else{
		clr_nCSS; wr_byte(2); set_nCSS; 
		set_nPRI;
	}
}
//------------------------------------------
//TODO Глобальная переменная не есть хорошо, для всех функций ниже не есть хорошо
unsigned int SolveFreqIndCorrCoef(unsigned char *freq){
	return (freq[0]*100 + freq[1] *10 + freq[2] - 15) /5;
}
void SetPwcFreq( unsigned char * freq )
{
	g_pwcParam.par_stk[4]= freq[0]*16 + freq[1];
	g_pwcParam.par_stk[5]= freq[2]*16 + freq[3];
	g_pwcParam.par_stk[6]= freq[4]*16 + freq[5];
	g_pwcParam.par_stk[7]= freq[6]*16 + 0;
	clr_nPRI; clr_nOTP;
	wr_par_stk(); wr_par_stp();
	SetAttFromInd(SolveFreqIndCorrCoef(freq));
	
}
//----------------------------------------
//сохранение массива выходных затуханий
void SaveAtts()
{
	for (char i=0; i<64; i++) eeprom_update_byte((uint8_t *)i+1280,g_pwcParam.att[i]);
}
//----------------------------------------
void SetAtt( unsigned char att,unsigned char attInd )
{
	g_pwcParam.att[attInd] = att;
	g_lev_att = g_pwcParam.att[attInd];
	wr_par_stp();
}
//----------------------------------------
void SetAttFromInd( unsigned char attInd )
{
		g_lev_att = g_pwcParam.att[attInd];
		wr_par_stp();
}
