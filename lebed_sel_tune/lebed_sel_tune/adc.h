/*
 * adc.h
 *
 * Created: 02.10.2014 11:12:32
 *  Author: USER
 */ 


#ifndef ADC_H_
#define ADC_H_
// ��� ������� ������ ���� � main.h
// #define ADC_UPAS_OUT_PIN 6
// #define ADC_UPAS_IN_PIN 7

#include "main.h"
       
 char read_adc(unsigned char pin) ;//����������� ���� ���������� c ������� ADC;           
//////////////////////////////////////////////////////////////////////////
// void ADC_k400us(void)//����������� Vdet c ������� ADC c ���������� �������.���������
//     {char i,c; Vdetd=0; kk=0; c=par_stm[25] & 0x04;//�������� ��������� = 2/4 ms
//      for(i=0;i<16;i++){delay_ms(2); if(c!=0) delay_ms(2);
//             ADC_200us(); if(Vdet==Vdetd) goto endd;  Vdetd=Vdet; kk+=1;};   endd:;}  
//              
// void ADL_k400us(void)//����������� Vdet c ������� ADL c ���������� �������.���������
//     {char i,c,d; kk=0; c=par_stm[25] & 0x04;//�������� ��������� = 5/10 ms * 3 
//      for(i=0;i<16;i++){delay_ms(5); if(c!=0)delay_ms(5); ADC_200us(); d=Vdet;
//                        delay_ms(5); if(c!=0)delay_ms(5); ADC_200us(); Vdetd=Vdet;
//                        delay_ms(5); if(c!=0)delay_ms(5); ADC_200us(); 
//                        if((Vdet==Vdetd)&&(Vdet==d)) goto endd; kk+=1;};   endd:;}                 
//             
// void ADH_k400us(void)//����������� Vdet c ���������� �������.��������� 90 mc
//     {char i,c; Vdetd=0; kk=0; 
//      for(i=0;i<16;i++){delay_ms(30); ADC_200us(); c=Vdet;
//                        delay_ms(30); ADC_200us(); Vdetd=Vdet;
//                        delay_ms(30); ADC_200us(); 
//                        if((Vdet==Vdetd)&&(Vdet==c)) goto endd; kk+=1;};   endd:;}                            
//        
// 


#endif /* ADC_H_ */
