/*
* ���� � �������� ��� ������ � UART, ������� ��� UART0 ����������� � ������� 
*
*/
#ifndef UART_H_
#define UART_H_

#define TXB8 0  //Transmit Data Bit 8
#define RXB8 1  //Receive Data Bit 8
#define UPE  2  //Character Size
#define OVR  3  //Transmitter Enable
#define FE   4  //Receiver Enable
#define UDRE 5  //USART Data Register Empty Interrupt Enable
#define RXC  7  //RX Complete Interrupt Enable

#define FRAMING_ERROR       (1<<FE)
#define PARITY_ERROR        (1<<UPE)
#define DATA_OVERRUN        (1<<OVR)
#define DATA_REGISTER_EMPTY (1<<UDRE)
#define RX_COMPLETE         (1<<RXC)
//������ ���� � ����� main.h
#define BAUD_RATE0 115200
//#define BAUD_RATE1 115200
#ifdef BAUD_RATE0
	#define UBRR0 (F_CPU/(BAUD_RATE0*16L) - 1 )
	#define SET_BAUD_UART0 (UBRR0H = ((UBRR0 >> 8) & 0xFF));(UBRR0L = ((UBRR0) & 0xFF));
#endif
#ifdef BAUD_RATE1
	#define UBRR1 (F_CPU/(BAUD_RATE1*16L) - 1 )
	#define SET_BAUD_UART1 (UBRR1H = ((UBRR1 >> 8) & 0xFF));(UBRR1L = ((UBRR1) & 0xFF));
#endif

#ifndef uchar
	typedef unsigned char uchar;
#endif

#include <avr/interrupt.h>
#include <string.h>
	// ���������� �� �������� �� RS-485
	#define ENABLE_485_UART0        0x40
	#define ON_485_UART0			0x20
	#define HF_485_UART0			0x10
	#define PORT_DIRECT_485_UART0   DDRF
	#define PORT_ENABLE_485_UART0   PORTF

void InitUart(){
	// USART0 initialization
	// Communication Parameters: 8 Data, 1 Stop, Even
	// USART0 Receiver: On
	// USART0 Transmitter: On
	// USART0 Mode: Asynchronous
	// USART0 Baud Rate: BAUD_RATE0
	
// 	 DDRF=0xf0; PORTF=0xf7; //F3..F0   => input; F3=0
// 	 
	 PORT_ENABLE_485_UART0 |= ENABLE_485_UART0;//&= ~ENABLE_485_UART0; //��������� ������ ����������
	 PORT_DIRECT_485_UART0 |= ENABLE_485_UART0;
	 
	 PORT_DIRECT_485_UART0 |= ON_485_UART0;//��������� ����������
	 PORT_ENABLE_485_UART0 &= ~ON_485_UART0;
	 
	 PORT_DIRECT_485_UART0 |= HF_485_UART0;//��������� ���������� �� ������ �������
	 PORT_ENABLE_485_UART0 &= ~HF_485_UART0;
	 
	 PORT_DIRECT_485_UART0 |= 0x80;
	 PORT_ENABLE_485_UART0 |= 0x80;

	UCSR0A=0x00;
	UCSR0B=0x98;
	UCSR0C=0x26;
	SET_BAUD_UART0;
	}
//---------UART0--����-�������-�-�����������-------
//-------------------------------------------------
//���� ������
#define WRF 0x80		//������ ������� ��� ������ �� ������ >[ HEAD F F F F F F F ENDL] <[OK ENDL]
	#define WRF_L 7


	
#define WRFLCA 0x46		//������ ������� � ������� �� ������ >[ HEAD F F F F F F F ENDL] <[OK ENDL]
	#define WRFLCA_L 7
	
#define WRLCA 0x85		//������ LCA [ HEAD C1 C1 C1 L1 L1 C2 C2 C2 L2 L2 A A ENDL] <[OK ENDL]
	#define WRLCA_L 12
	
#define WRA  0x84		//��������� ����������� [ HEAD A A ENDL] <[OK ENDL]
	#define WRA_L 2
	
#define SETMODE 0x86 //��������� ������ ����� �������������� 0 - non, 1 - 2 tone, 2 - swype >[ HEAD T ENDL ] <[OK ENDL]
	#define SETMODE_L 1
	
#define SETOTP 0x87 //���������/��������� 1 - zap, 0 - otp >[ HEAD O ENDL] <[OK ENDL ]
	#define SETOTP_L 1
	
#define READALLFLASH 0x90 //������ ����� ����������� ������ >[HEAD ENDL] <[OK ENDL]
	#define  READALLFLASH_L 7
	
#define WRITEDATATOFLASH 0x91		//������ ������ � ������  >[ HEAD F F F F F F F C1 C1 C1 L1 L1 C2 C2 C2 L2 L2 A A ENDL] <[OK ENDL]
	#define WRITEDATATOFLASH_L 19	//�������� ������� � ���������. 
	
#define ERASEFLASH 0x92//�������� ���������� >[HEAD ENDL] <[OK ENDL]
	#define ERASEFLASH_L 0

#define SETCOEF 0x95//��������� ������������ ��� 1 � �� ������ >[HEAD C C ENDL] <[OK ENDL]
	#define SETCOEF_L 2
	
#define SAVECOEF 0x96//������ ������������ � EEPROM
	#define SAVECOEF_L 0
	
#define SAVEDETCOEF 0x97//���������� �������������� ������������ ��������� � ������
	#define SAVEDETCOEF_L 0

#define READALLDETCOEF 0x98//������ ���� ������������� ���������
	#define READALLDETCOEF_L  0

#define WRFZAH 0x99		//������ ������� � ���������� ��������� �������� >[ HEAD F F F F F F F ENDL] <[OK ENDL]
	#define WRFZAH_L 7

#define ID "LSP0" //������������� ����������
#define FINDDEVICE 0xFF
	#define FINDDEVICE_L 0

#define COMOK 0x06
#define ERR 0x16
#define ENDL 0x03
#define READA 0x81//������ ���������
#define READA_L 0


extern void WriteFFunc(unsigned char *);
extern void WriteFZahFunc(unsigned char *);
extern  void WriteFLCAFunc(unsigned char*);
extern  void WriteLCAFunc(unsigned char*);
extern  void WriteAFunc(unsigned char*);
extern  void WriteMode(unsigned char*);
extern  void WriteOtp(unsigned char*);
extern  void ReadAllFlash(unsigned char*);

extern  void WriteData2Flash(unsigned char*);
extern  void WriteEraseFlash(unsigned char*);
extern  void ReadAmpl(unsigned char*);
extern  void SetCoef(unsigned char*);
extern  void SaveCoef(unsigned char*);
extern  void SaveDetectorCoef(unsigned char*);
extern	void ReadDetectorCoef(unsigned char*);
extern  void FindDevice(unsigned char*);
//������ ����� ������
const unsigned char g_comandList[] PROGMEM = {WRF,WRFLCA,WRLCA,WRA,SETMODE,SETOTP,READALLFLASH,
											WRITEDATATOFLASH,ERASEFLASH,READA,SETCOEF,SAVECOEF,SAVEDETCOEF,
											FINDDEVICE,READALLDETCOEF,WRFZAH};
//������ ���������� ���� � �������
const unsigned char g_comandLengthList[] PROGMEM = {WRF_L,WRFLCA_L,WRLCA_L,WRA_L,SETMODE_L,SETOTP_L,READALLFLASH_L,
											WRITEDATATOFLASH_L,ERASEFLASH_L,READA_L,SETCOEF_L,SAVECOEF_L,SAVEDETCOEF_L,
											FINDDEVICE_L,READALLDETCOEF_L,WRFZAH_L};
//������ ���������� �� ������� ��������������� ������ �������
void  (* const g_commandFunc[])(unsigned char*)  PROGMEM = {WriteFFunc,WriteFLCAFunc,WriteLCAFunc,WriteAFunc,WriteMode,WriteOtp,
	  ReadAllFlash,WriteData2Flash,WriteEraseFlash,ReadAmpl,SetCoef,SaveCoef,SaveDetectorCoef,
	  FindDevice,ReadDetectorCoef,WriteFZahFunc};


#define COM_BUFF_SIZE 32

unsigned char g_comBuff[COM_BUFF_SIZE];
unsigned char g_comBufCnt = 0;
int g_comNumOfByte = 0;
//char AMPREADED=0,FLCAWRITED=0,FREADED=0,LCAREADED=0,MODECHANGED=0,STARTREADEDFLASH=0,STARTWRFLASH=0,STARTERASE = 0,OTPREADED = 0;

void SendCOMBytes(unsigned char* data,char n){
	cli();
	for(unsigned char t = 0; t < n; t++){
		
	
	while( (UCSR0A & DATA_REGISTER_EMPTY) == 0 );//������� ��������� �������� �������
	UDR0 = data[t];
	}
	sei();
}


char CheckComandFormat(signed int indOfCom, char data_size){//������� �������� ����� ������� �� ������������
	if ((indOfCom >= 0) && (indOfCom < (signed int)sizeof(g_comandList)))
	{//�������� �� ����� �������
		unsigned char len = pgm_read_byte(&g_comandLengthList[indOfCom]); 
		if ( len == (data_size - 2))
		{
			return 1 == 1;
		}
	}
	return 1 == 0;
}

void SendERR(){// ������� ��� �������� ��������� ������
	unsigned char a[] = {ERR,ENDL};
	SendCOMBytes(a,2);
}

void SendOk(){//������� ��� ��������� ��
	unsigned char a[] = {COMOK,ENDL};
	SendCOMBytes(a,2);
}

 

ISR (USART0_RX_vect)
{
	cli();
	unsigned char data = UDR0;
	//SendCOMBytes(&data,1);//�������*/
	if ( memchr_P(g_comandList,data,sizeof(g_comandList)) != NULL){//���� �������� ������ �� ������ ������ �� �������� ����� �������
		g_comBufCnt = 0;//���������� ������� �������� ����
		
	//	SendCOMBytes(&data,1);//�������
	}
	
	//���������  �����
	if( g_comBufCnt == COM_BUFF_SIZE ) g_comBufCnt=0;

	g_comBuff[g_comBufCnt] = data;
	g_comBufCnt++;
	//SendCOMBytes(&data,1);
	if((data == ENDL) && (memchr_P(g_comandList,g_comBuff[0],sizeof(g_comandList)) != NULL) )//���� ������� ����� ��������� � ������� ���� � ������ �� ������������
	{	
		//��������� ������ ������� � ������� ��� �������� ������� �������� � �������� ��������
		signed int indexOfComand =  (int)memchr_P(g_comandList,g_comBuff[0],sizeof(g_comandList)) - (int)g_comandList;
		if ( CheckComandFormat(indexOfComand,g_comBufCnt)){//���� ������ �������� ������� �� ������������ ��
			 (( void (*)(unsigned char*))pgm_read_word(&g_commandFunc[indexOfComand]))(g_comBuff);//���������� ��������������� �������
			}
			else SendERR();//���� �� ������ �� �������� �������
	g_comBufCnt = 0;				
	};
	sei();
}
//////////////////////////////////////////////////////////////////////////
////////// ������� ��������������� �������� //////////////////////////////
/////////////////////////////////////////////////////////////////////////
	/* ������� � ���������� ������������ � ����� ��������������*/
	extern void WriteFreqToPrk(unsigned char* F);
	extern unsigned int CharFtoInd(uchar* f);
	extern void SetnOTP(char val);
	extern void SetOutLevel(uchar lvl);
	/*������� � ���������� �� ����� ����*/
	extern STATEOFPLATE g_plateState;
	extern unsigned int  g_C1,g_C2;
	extern unsigned char g_L1,g_L2;
	extern unsigned char g_A;
	extern void LCA2out(unsigned int C1,unsigned char L1,unsigned int C2,unsigned char L2,unsigned char A,unsigned int* OUT);
	/* ������� � ���������� ������������ � ����� ���������*/
	extern void SendLCA2Sel(unsigned char* LCAout);
	extern void SendA(unsigned int* LCAout);
	/* ������� ���*/
	extern char read_adc(unsigned char pin);
	
void WriteFFunc(unsigned char * data)// ������� ������ ������� ��� ������ �� ����
{
	for (unsigned char i = 0; i < WRF_L; i++)
	{
	 g_plateState.freq[i] = data[i + 1] & 0x0F;
	}
	WriteFreqToPrk(g_plateState.freq);
	SendOk();
}

void WriteFLCAFunc(unsigned char * data)//������ ������� � ������� �� ������ >[ HEAD F F F F F F F ENDL] <[OK ENDL]
{
	for (unsigned char i = 0; i < WRF_L; i++)
	{
		g_plateState.freq[i] = data[i + 1] & 0x0F;
	}
	WriteFreqToPrk(g_plateState.freq);//���������� ������� � �������������
	ReadLCAoutFromFlash(CharFtoInd(g_plateState.freq),(uchar*)g_plateState.outLCA);//������ �� ������ ����������
	SendLCA2Sel((uchar*)g_plateState.outLCA);// ���������� � ��������
	SendOk();
	_delay_ms(5);
	//������ LCA [ HEAD C1 C1 C1 L1 L1 C2 C2 C2 L2 L2 A A ENDL] <[OK ENDL]
	outLCA2LCA(&g_C1,&g_L1,&g_C2,&g_L2,&g_A,g_plateState.outLCA);
	unsigned char reply[] = {WRLCA,
							((g_C1 & 0xF00) >> 8 ) + 0x30,((g_C1 & 0x0F0) >> 4 ) + 0x30,((g_C1 & 0x00F)) + 0x30,
							((g_L1 & 0xF0) >> 4 ) + 0x30,((g_L1 & 0x0F)) + 0x30,
							((g_C2 & 0xF00) >> 8 ) + 0x30,((g_C2 & 0x0F0) >> 4 ) + 0x30,((g_C2 & 0x00F)) + 0x30,
							((g_L2 & 0xF0) >> 4 ) + 0x30,((g_L2 & 0x0F)) + 0x30,
							((g_A & 0xF0) >> 4 ) + 0x30,((g_A & 0x0F)) + 0x30,
							ENDL};
							_delay_ms(10);
							SendCOMBytes(reply,14);
}

	
void data2LCA(uchar* data){//������� �������������� ������ � LCA � �� ������ � EEPROM
		data--;
		g_C1 = ( ((unsigned int)(data[1] & 0x0F)) << 8 ) + ( ((unsigned int)(data[2] & 0x0F)) << 4 ) + ( ((unsigned int)(data[3] & 0x0F)) << 0 );
		g_L1 = ( ((unsigned char)(data[4] & 0x0F)) << 4 ) + ( ((unsigned char)(data[5] & 0x0F)) << 0 );
		
		g_C2 = ( ((unsigned int)(data[6] & 0x0F)) << 8 ) + ( ((unsigned int)(data[7] & 0x0F)) << 4 ) + ( ((unsigned int)(data[8] & 0x0F)) << 0 );
		g_L2 = ( ((unsigned char)(data[9] & 0x0F)) << 4 ) + ( ((unsigned char)(data[10] & 0x0F)) << 0 );
		
		g_A = ( ((unsigned char)(data[11] & 0x0F)) << 4 ) + ( ((unsigned char)(data[12] & 0x0F)) << 0 );
		LCA2out(g_C1,g_L1,g_C2,g_L2,g_A,g_plateState.outLCA);
}



void WriteLCAFunc(unsigned char * data)//������ LCA [ HEAD C1 C1 C1 L1 L1 C2 C2 C2 L2 L2 A A ENDL] <[OK ENDL]
{

	data2LCA(&data[1]);
	//g_C1 = ( ((unsigned int)(data[1] & 0x0F)) << 8 ) + ( ((unsigned int)(data[2] & 0x0F)) << 4 ) + ( ((unsigned int)(data[3] & 0x0F)) << 0 );
	//g_L1 = ( ((unsigned char)(data[4] & 0x0F)) << 4 ) + ( ((unsigned char)(data[5] & 0x0F)) << 0 );
	//
	//g_C2 = ( ((unsigned int)(data[6] & 0x0F)) << 8 ) + ( ((unsigned int)(data[7] & 0x0F)) << 4 ) + ( ((unsigned int)(data[8] & 0x0F)) << 0 );
	//g_L2 = ( ((unsigned char)(data[9] & 0x0F)) << 4 ) + ( ((unsigned char)(data[10] & 0x0F)) << 0 );
	//
	//g_A = ( ((unsigned char)(data[11] & 0x0F)) << 4 ) + ( ((unsigned char)(data[12] & 0x0F)) << 0 );
	//LCA2out(g_C1,g_L1,g_C2,g_L2,g_A,g_plateState.outLCA);
	SendLCA2Sel((uchar*)g_plateState.outLCA);
	SendOk();
}


void WriteAFunc(unsigned char * data)//������ �����������
{
	g_A = ( ((unsigned char)(data[1] & 0x0F)) << 4 ) + ( ((unsigned char)(data[2] & 0x0F)) << 0 );//���������� � �
	LCA2out(g_C1,g_L1,g_C2,g_L2,g_A,g_plateState.outLCA);//���������� � EEPROM
	SendA((unsigned int*)g_plateState.outLCA);//����������
	SendOk();
}

void WriteMode(unsigned char * data)//���� ��������
{
	SendCOMBytes(data,1);
}



void WriteOtp(unsigned char * data)// ���������/���������
{

	SetnOTP((char)data[1] & 0x0F);
	SendOk();
}


void ReadAllFlash(unsigned char * data)//������ ���������� ��� �������� �������
{
		for (unsigned char i = 0; i < WRF_L; i++)
		{
			g_plateState.freq[i] = data[i + 1] & 0x0F;
		}
	unsigned int ind = CharFtoInd(g_plateState.freq);//���������� ������������� ������� � ������
	//uchar indC[2];
	//indC[0] = (uchar)((ind & 0xFF00)>>8);
	//indC[1] = (uchar)(ind&0x00FF);
	//SendCOMBytes(indC,2);
	unsigned char a[6]={0,0,0,0,0,0};
	ReadDataInFlashIIC(&ind,a,6);//������ ������ �� ������
	data2LCA(a);
	SendCOMBytes(a,6);
}



void WriteData2Flash(unsigned char * data)//���� ��������
{//>[ HEAD F F F F F F F C1 C1 C1 L1 L1 C2 C2 C2 L2 L2 A A ENDL] <[OK ENDL]
	unsigned int ind = CharFtoInd(&data[1]);//����������� ����������� ������� � ������
	data2LCA(&data[8]);//���������� ����� ������� ������ � LCA � ���������� �� � LCAOut
	
	WriteLCAout2Flash(ind,(uchar*)g_plateState.outLCA)	;//����� �� ������ 
//	SendCOMBytes((uchar*)g_plateState.outLCA,6);
//	_delay_ms(20);
	unsigned char a[6]={0,0,0,0,0,0};
	ReadDataInFlashIIC(&ind,a,6);
//SendCOMBytes(a,6);
	if (memcmp((void*)a,(void*)g_plateState.outLCA,6) == 0)
		SendOk();
		else
			SendERR();
}

void WriteEraseFlash(unsigned char * data)//���� ��������
{
	asm("jmp 0xF000");
	SendCOMBytes(data,1);
}

void ReadAmpl( unsigned char* )//������ ��������� �� ����� ���
{	
	uchar out[6];
	uchar ampl ;/*= read_adc(ADC_UPAS_IN_PIN);
	
	out[0] = READA;
	out[1] = (uchar)((ampl & 0xf0) >> 4) + 0x30;
	out[2] = ((ampl & 0x0F)) + 0x30;*/
	ampl = read_steady_adc(ADC_UPAS_OUT_PIN);
	out[3] = (uchar)((ampl & 0xf0) >> 4) + 0x30;
	out[4] = ((ampl & 0x0F)) + 0x30;
	out[5] = ENDL;
	SendCOMBytes(out,6);
}
extern unsigned int SolveFreqIndCorrCoef();
 void SetCoef(unsigned char* data){
	 uchar coef;
	 coef = (data[1] & 0x0F)*16 + (data[2] & 0x0F);
	 SetOutLevel(coef);

	 
	 g_plateState.corr_coef[SolveFreqIndCorrCoef()] = coef;
	 SendOk();
 }

void SaveCoef(unsigned char* data){
	SaveStateToEEPROM(&g_plateState,&g_eepromPlateState);
	 SendOk();
}
//////////////////////////////////////////////////////////////////////////
extern void SaveDetCoef();

void SaveDetectorCoef(unsigned char* data){
	SaveDetCoef();
	SendOk();
}
//----------------------------------------
extern void ReadDetCalInFlash( unsigned int ,uchar*  );
void ReadDetectorCoef(unsigned char*){
	//TODO: ������ ���� ������������� ���������
	
	uchar msg = READALLDETCOEF,tempMsg;
	SendCOMBytes(&msg,1);
	for(char i = 0; i < 28; i++){
	ReadDetCalInFlash((unsigned int)i,(uchar*)&msg);
	
	SendCOMBytes(&msg,1)	;
	}
	msg = ENDL;
	SendCOMBytes(&msg,1);
}

void FindDevice(unsigned char*){
	unsigned char end = FINDDEVICE;
	SendCOMBytes(&end,1);
	SendCOMBytes((unsigned char*)ID,4);
	end = ENDL;
	SendCOMBytes(&end,1);
	
}
//----------------------------------------

 void WriteFZahFunc( unsigned char * data)
{
		for (unsigned char i = 0; i < WRFZAH_L; i++)
		{
			g_plateState.freq[i] = data[i + 1] & 0x0F;
		}
		WriteFreqToPrk(g_plateState.freq);
		
		_delay_ms(10);
			int freq = g_plateState.freq[0]* 1000 + g_plateState.freq[1]* 100 +g_plateState.freq [2]*10 + g_plateState.freq[3];
			bool tempMode = g_plateState.mode;
			g_plateState.mode = false;
			ChangeModePrk(&g_plateState);
						
			SetZselFreq(freq, &g_plateState.zSelParam);
			g_plateState.mode = tempMode;
			ChangeModePrk(&g_plateState);
		_delay_ms(10);
		
			TuneZselAtten(&g_plateState.zSelParam);
		
		
		SendOk();
}

#endif