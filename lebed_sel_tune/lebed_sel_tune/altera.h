 /* ����� ������� ���������� ���������� � main.h*/
 #ifndef ALTERA_H_
	#define ALTERA_H_
 #define ALTERA_OUT_PORT			C
 #define ALTERA_SCLK                0
 #define ALTERA_CS1                 5 
 #define ALTERA_CS2                 6 
 #define ALTERA_CS3                 7 
 #define ALTERA_SDI                 1
 #define ALTERA_SDO                 2
 #define ALTERA_ENABLE              4
 #define ALTERA_MRES                3
 
 #define ALTERA_OUT_PORT_2			D
 

 
 #define ALTERA_RESET               0
 #define ALTERA_BUSY_FLAG           1    //���������� flash 

#include "main.h"
#ifndef _BV
	_BV(bit) (1<<bit)	
#endif
#ifndef SET_LINE
	#define SET_LINE(port,line,val) (val == 0)?(port &= ~_BV(line)):(port |= _BV(line))
#endif
 
  #define CONCAT(a,b)				(a ## b)
  #define CONCAT_PORT(a)			CONCAT(PORT,a)
  #define CONCAT_DDR(a)				CONCAT(DDR,a)
  #define CONCAT_PIN(a)				CONCAT(PIN,a)
  #define ALTERA_PORT_2				CONCAT_PORT(ALTERA_OUT_PORT_2)
  #define ALTERA_OUT_DDR_2			CONCAT_DDR(ALTERA_OUT_PORT_2)
    #define ALTERA_PORT				CONCAT_PORT(ALTERA_OUT_PORT)
    #define ALTERA_OUT_DDR			CONCAT_DDR(ALTERA_OUT_PORT)
 
 #define SET_ALTERA_SCLK(val) SET_LINE(CONCAT_PORT(ALTERA_OUT_PORT),ALTERA_SCLK,val)
 #define SET_ALTERA_CS1(val) SET_LINE(CONCAT_PORT(ALTERA_OUT_PORT),ALTERA_CS1,val)
 #define SET_ALTERA_CS2(val) SET_LINE(CONCAT_PORT(ALTERA_OUT_PORT),ALTERA_CS2,val)
 #define SET_ALTERA_CS3(val) SET_LINE(CONCAT_PORT(ALTERA_OUT_PORT),ALTERA_CS3,val)
 #define SET_ALTERA_SDI(val) SET_LINE(CONCAT_PORT(ALTERA_OUT_PORT),ALTERA_SDI,val)
 #define SET_ALTERA_ENABLE(val) SET_LINE(CONCAT_PORT(ALTERA_OUT_PORT),ALTERA_ENABLE,val)
 #define SET_ALTERA_MRES(val) SET_LINE(CONCAT_PORT(ALTERA_OUT_PORT),ALTERA_MRES,val)
 #define SET_ALTERA_RESET(val) SET_LINE(CONCAT_PORT(ALTERA_OUT_PORT_2),ALTERA_RESET,val)
 #define GET_ALTERA_BUSY  ((CONCAT_PIN(ALTERA_OUT_PORT_2) & _BV(ALTERA_BUSY_FLAG)) >> ALTERA_BUSY_FLAG)
 #define GET_ALTERA_SDO  ((CONCAT_PIN(ALTERA_OUT_PORT) & _BV(ALTERA_SDO)) >> ALTERA_SDO) 
 
 #define delay_us _delay_us
 #define delay_ms _delay_ms
//����������� ��������
// unsigned char C_0R_EEPROM[]={0x0A,0x0C,0x03,0x4B};
// unsigned char C_1R_EEPROM[]={0x14,0x00,0xB3,0xC0};
// unsigned char C_2R_EEPROM[]={0x07,0x00,0x00,0x30};
// unsigned char C_3R_EEPROM[]={0x36,0x10,0x19,0x04};
//       
/*
(C_nR[0],C_nR[1],C_nR[2],C_nR[3]) =  C_nR[31..0]
C_0R[31] - ������������ ����� ������� 768-192 ��� 
     0- ���� ���
     1- ���� ��� (debug)
 
C_0R[30..29] -  ������

C_0R[28]- ������ �� ��������������, ��������� ����������z 2-�� flash !!!!!
                1- ������ �����
                0- ����� ����� (*_ispr_5) ��� ������� �������.
                
!!!!!! � ������ (*_ispr_5) �������� ���� ����������� flash 


C_0R[27..24]-�������� �������� ������� X7E {
     C_1R[27] - ���� 1/2
     C_1R[26] - ���� 1/4
     C_1R[25] - ���� 1/8
     C_1R[24] - ���� 1/16}      

C_0R[23..20]-�������� �������� ������� X7D {
     C_1R[27] - ���� 1/2
     C_1R[26] - ���� 1/4
     C_1R[25] - ���� 1/8
     C_1R[24] - ���� 1/16}  

C_0R[19]- ���. ������� psd
C_0R[18]- ����. ������� ���������� ���
C_0R[17]- ���. ��������� ������������ �������������� ������ ��������� H(jw)
C_0R[16]-  0-��������� H(jw) ��� �������-���������� ���
           1-��������� H(jw) � ��������-����������� ���
C_0R[15]- ���������  H(jw) ��� ��- � ������ ������ ����������
           1- ����� �������
           0-���������� ������       
C_0R[14..13]- ������

C_0R[12] - �������� psd (���� ���)
C_0R[11]-���. ��������� ��� � ��� �� ������

C_0R[10]- ����������� �����  aes ��� Fs=48��� ����� �� 12��� (1 -����, 0 - ���)    
C_0R[9]- ����������� �����  aes (0 - 48kHz, 1 - 192kHz)
C_0R[8]- �������� ������� AES/EBU ������� �� ����� ���        
C_0R[7]- �������� ������� ������ ���� <���������>
C_0R[6..0] - ���������� ������ 128��� � (1-C_0R[6..0]/128)

-------------------------


C_1R[31]-
         0- ��� ���./����. ��������� ���./���� �������� �����   
         1- ����. �������� �����
         !!! ���  ���./���� ��������� ������ ������������ ������������� �������������� => ���� ��� ��������� �������


C_1R[30..29]-������
C_1R[28]-1/0  ���./����. ���������� �������� ������� ������ ��� ������������� (& enable �������� �������� ���������)

C_1R[27..26]-����� ������� �������� �� �������������� ������
        0--filter delay  Fs=100, Fc=4,516727  -   yn=(xn+xn-1)/8 +(1/2+1/4)yn
	1--filter delay  Fs=100, Fc=2,119     -   yn=(xn+xn-1)/16+(1/2+1/4+1/8)yn
	2--filter delay  Fs=100, Fc=1,0264     -   yn=(xn+xn-1)/32+(1/2+1/4+1/8+1/16)yn
	3--filter delay  Fs=100, Fc=0,513     -   yn=(xn+xn-1)/64+(1/2+1/4+1/8+1/16+1/32)yn
C_1R[25..24]-�������� ��������� �������� � �������������� ������ (������������ 768���)
        0-2^-5
        1-2^-4
        2-2^-3
        3-��� ���������
     
C_1R[23]- ���. ���������� sin �� ���� �� ��� � ������������ ����������

C_1R[22..21] - �������� ���� �������������� � ��������� �������
        0- +1
        1- +1/2
        2- +1/4
        3- +1/8

C_1R[20]-  0->1 ������ �������������� ������������� (����� ��������� �������������� ����� �� 0) 

C_1R[19]   -������� CIC 98304-768���, �� �����:
        0 -  ����� ���������
        1 -  ����� ���������� �������� �������� (��)

C_1R[18..16]  - ������
C_1R[15..12]  - ����������� ������ ������ ���� �� 128��� (��������� �������� ad9857 � loader.tdf)
C_1R[11..10]  - ������

C_1R[9..8]-���-�� ���������� ��������� �������
        - 0-2
        - 1-4
        - 2-8
        - 3-16
C_1R[7]- ���������� ���������� ������� �� ����������� ��������� 
        1 - ��� ����� �������� ����������� ���������
        0 - ���� ����������� ��������� ������������ 
C_1R[6]- ��������� ��������: 1-����������, 0- ������� ���������� 750���
C_1R[5]- ������� ���������� �������� - ����������� ���������� � ������ ������� ���������� 

C_1R[4]-  1-���. ������� ������ �� rs-232 "����������" (����� <> �� ��), 0- ����. 
C_1R[3]-  ��� ��������� ������� ��������������� ���������������
                        0-����������� ������+���
                        1-����������� ������+��� 
C_1R[2..0]- 5-��� ����
            6-��� ���� 
            4-NON
            3-�������� ���������� ������ (��������)
            2-��� ����#��� ����(C_1R[3]) + ��� � ������� C_3R[25..24]
            1-���
            0-���� AES/EBU    

            
C_2R[31..24]-�������� ������� ������ ��� ������������� (��������� ����� ��� ��������������� ������ 
��������� ��������������� ������� �� ����� 32 ������ 
C_2R[23..16]-�������� ��������� �� ����� ���
C_2R[15..8]-������� ������� X7E 
C_2R[7] - �������� ������� ������� ���-�� ���: 1- 384000,0- 192000 
C_2R[6..0]-���������� �� ������ ��� ��������� 1� (����� ���������� ���������� ��������� .mif ���������� ��������������� �� ����������...)

C_3R[31..28]- ���-�� ����� �������������� ����� ��� �������������
C_3R[27..26]- -�������� ��������� ������������ �������� ��������� ������� � �������������� ������
        0-lpm_ff_abs_sub_gain<7
        1-lpm_ff_abs_sub_gain<6
        2-lpm_ff_abs_sub_gain<5
        3-lpm_ff_abs_sub_gain<4

C_3R[25..24]- ������� ���� � �������� �������
        0- ���
        1-2^-8
        2-2^-9
        3-2^-10
C_3R[23]- 1/0 ����/���������
C_3R[22..16]- ���������� ���������
C_3R[15..12]-�������� ���������� ��������� � ������ 48���    (if ==0xa => ������������ �������� � ���������� ���������))
!!!!!!!!!!!! ���� �������� ������ 6=> ��� ��������� ���!!!!!!!!!!!!!!!!!!!!!!!!!!!

C_3R[11..8]- ����������� ����  ���  ���  ���������


  
C_3R[7.4]- ������������ ����� ��������� ��������������� �������������� (C_3R[7.4],h"ff")/375 (���)

C_3R[3..2]-�������� ��������� ��������������� �������������� �� ��������� ������� 
        0- abs_error_cav<7
        1- abs_error_cav<9
        2- abs_error_cav<11
        3- abs_error_cav<13 

C_3R[1..0]-������ ���������� �������� ������� ������������ (768���/2^10)

 ------------------



 
C_4R[31..16]-������� �������� ������� ������
C_4R[15..0]- ������� �������� ��������� ������

C_5R[31..28] - ����� ���������: 0-NON,1-A1B,2-F1B,3- G1B,4-VNI,5-X7D,6-A3E,7-X7E,8-J3E,9-R3E,A-H3E,B-B3E,C-J1B,D-B7B
C_5R[27..24] - ��������� �����(��): 0-0,1-20,2-100,3-125,4-170,5-200,6-400,7-500,8-1000,9-6000
C_5R[23..20] - �������� �����������(���):0-X,1-10,2-20,3-50,4-100,5-150,6-200,7-300,8-500,9-600,A-1200,B-2400
C_5R[19..16] - ������ ��� (��): 0-300..3400, 1-350..2700

      C_5R[15..13]    - ������
      C_5R[12] -  lock (1-���)
      
      C_5R[11] -  PIT  (1-���)
      C_5R[10] -  �� ���� <��K> �������.                  (1- ������)
      C_5R[9]  -  ���  <���> ����������.                  (1- ������)      
      C_5R[8]  -  �� ���� <���������> �������.            (1- ������)
      
      C_5R[7]  -  ���  <���������> ����������.            (1- ������)
      C_5R[6]  -  ��������� (1-���)
      C_5R[5]  -  �� ���� <���������> �������� ������ �����.    (1- ������)
      C_5R[4]  -  �� ���� <���������> �������� ������ �����.    (1- ������)

      C_5R[3]  -  �������������� ��������� �������� ��������� �������.   (1- ������)
      C_5R[2]  -  ��������������  ��������� ��������.     (1- ������)
      C_5R[1]  -  �� ���� <���>   �������� ������ �����.  (1- ������)  
      C_5R[0]  -  �� ���� <���>  �������� ������ �����.   (1- ������)

C_6R[31..16]-������� �������� ������� ������
C_6R[15..0]-������� �������� ��������� ������

C_7R[31..16]-����������� ����������� ��������������� ��������� ������
C_7R[15..0]- ���������� �������� ����� ��������� ������� � ��������� �������


*/  
void InitAltera();   
  void Write_Adress_SPI_ALTERA(unsigned int ADRESS);
  void Write_DATA_SPI_ALTERA(unsigned char DATA);
  unsigned char Read_DATA_SPI_ALTERA();
  void Write_Controll(unsigned int adress,unsigned char* ad);
  void Read_Controll(unsigned int adress,unsigned char* ad);

#endif