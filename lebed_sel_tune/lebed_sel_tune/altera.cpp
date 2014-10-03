/*
 * altera.cpp
 *
 * Created: 29.09.2014 12:12:14
 *  Author: USER
 */ 
 #include "altera.h"        
 //////////////////////////////////////////////////////////////////////////
  void Write_Adress_SPI_ALTERA(unsigned int ADRESS)
{
 unsigned char m;
 unsigned int tmp[12]={2048,1024,512,256,128, 64, 32, 16, 8, 4 ,2 ,1};  
 
 SET_ALTERA_SCLK(0);
 _delay_us(1); 
 for (m=0;m<12;m++)
 {   

 SET_ALTERA_SDI(((ADRESS&tmp[m])==tmp[m]));
 SET_ALTERA_SCLK(1);
 delay_us(4); //скважность около 2
  SET_ALTERA_SCLK(0);
 } 
 
 SET_ALTERA_SDI(0);
}    
//////////////////////////////////////////////////////////////////////////
  void Write_DATA_SPI_ALTERA(unsigned char DATA)
{
 unsigned char m;
 unsigned char tmp[8]={128, 64, 32, 16, 8, 4 ,2 ,1};  
  SET_ALTERA_SCLK(0);
 delay_us(1);  
 for (m=0;m<8;m++)
 {   

 SET_ALTERA_SDI((DATA&tmp[m])==tmp[m]); 
   SET_ALTERA_SCLK(1);
 delay_us(4); //скважность около 2
   SET_ALTERA_SCLK(0); 
 } 
  SET_ALTERA_SDI(0);
}  
//////////////////////////////////////////////////////////////////////////
  unsigned char Read_DATA_SPI_ALTERA()
{
 unsigned char m,b;
 unsigned char tmp[8]={128, 64, 32, 16, 8, 4 ,2 ,1};  
 
 b=0;
 for (m=0;m<8;m++)
 {  
  SET_ALTERA_SCLK(0);
  _delay_us(3);
 b=b + GET_ALTERA_SDO*tmp[m]; 
  SET_ALTERA_SCLK(1); 
 delay_us(2);  // скважность около 2
 }  
  SET_ALTERA_SCLK(0);  
 return b;
}
//////////////////////////////////////////////////////////////////////////

  void Write_Controll(unsigned int adress,unsigned char* ad)
{
unsigned char k;  
 SET_ALTERA_CS2(0); SET_ALTERA_CS3(0);
 SET_ALTERA_CS1(0);
Write_Adress_SPI_ALTERA(adress);
delay_us(1);
for (k=0;k<4;k++) Write_DATA_SPI_ALTERA(ad[k]); //  Write_DATA_SPI_ALTERA(*(ad+k));
 SET_ALTERA_CS1(1);  
}

//////////////////////////////////////////////////////////////////////////
  void Read_Controll(unsigned int adress,unsigned char* ad)
{
unsigned char k; 
 SET_ALTERA_CS2(0); SET_ALTERA_CS3(0); 
 SET_ALTERA_CS1(0);
Write_Adress_SPI_ALTERA(adress);
delay_us(1);
for (k=0;k<4;k++)  ad[k]=Read_DATA_SPI_ALTERA(); //*(ad+k)=Read_DATA_SPI_ALTERA();
 SET_ALTERA_CS1(1);                                                                          
}

void InitAltera()
{
	//PORTC=0x00;DDRC=0xff;DDRC.2=0;
	ALTERA_PORT = 0x00;
	ALTERA_OUT_DDR = 0xFF;
	SET_LINE(ALTERA_OUT_DDR,2,0);
	 SET_ALTERA_SCLK(0); SET_ALTERA_CS1(1);  SET_ALTERA_SDI(0);
	 DDRG=0xff;PORTG=0x00;
	ALTERA_OUT_DDR_2 = 0x01;
	ALTERA_PORT_2 = 0x00;
	SET_ALTERA_ENABLE(0);
	SET_ALTERA_RESET(1);
	delay_ms(500);
	DDRG=0xff;PORTG=0x00;
	delay_ms(5);PORTG=0x01;DDRG=0x00;delay_ms(500);
	
	while((PING&0x02)==0) {delay_ms(50);}
		_delay_ms(50);

// 	Write_Controll(0x07FF, &C_0R[0]);
// 	Write_Controll(0x07FE, &C_1R[0]);
// 	Write_Controll(0x07FD, &C_2R[0]);
// 	Write_Controll(0x07FC, &C_3R[0]);
// 
// 	delay_ms(5);
}











