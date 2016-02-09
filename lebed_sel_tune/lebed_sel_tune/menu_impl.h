/*
 * menu_impl.h
 *
 * Created: 29.09.2014 13:39:51
 *  Author: USER
 */ 


#ifndef MENU_IMPL_H_
#define MENU_IMPL_H_

#include <main.h>
#include "CMenuItem.h"

#include <avr/eeprom.h>
CMenuItem *g_currentItem;

//объявление пунктов меню
extern CMenuItem g_mainMenuItem;
//extern CMenuItem g_settingsMenuItem;
extern CMenuItem g_prkSettingMenuItem;
extern CMenuItem g_zselSettingMenuItem;
extern CMenuItem g_lcaMenuItem;

#define NUM_OF_CHARS_PER_STR 20
//строка для вывода
uchar g_drawingString[NUM_OF_CHARS_PER_STR*2+1] = {"testovaja figna fghj kjhgdfsertyuhnmklfd"};
//
	char TempStr1[NUM_OF_CHARS_PER_STR + 1 ] = {"                   "},		//временные переменные в которые выводятся
	TempStr2[NUM_OF_CHARS_PER_STR + 1] = {"                    "};		//строки для отрисовки
//
uchar g_hexOut[] = {"0123456789ABCDEFM"};
//переменные для вывода LCA на экран
unsigned int g_C1 = 0xDD9;
unsigned int g_C2 = 0x66D;
unsigned char g_L1 = 0x65;
unsigned char g_L2 = 0x4B;
unsigned char g_A = 0x29;

enum SelType {LEBED_TYPE,ZAH_TYPE};	// именя типов селектора
//структура хранящая состояние платы
typedef struct StateOfPlate{
	bool output;
	bool mode;
	
	uchar freq[7];
	uchar C_R[4][4];
	unsigned int outLCA[3];
	unsigned char detCoef;
	unsigned char corr_coef[57];//коэффициенты коррекции уровня платы от 1,5 МГц до 29,5 МГц через 0,5 МГц
	uchar selType;
	ZSELPARAM zSelParam;		
} STATEOFPLATE;

 STATEOFPLATE g_plateState;
 //требуется прописать в свойствах проекта для секции с++  -std=c++11
 STATEOFPLATE EEMEM g_eepromPlateState = {bool(0),bool(0),{1,0,0,0,0,0,0}, 
												{{0x0A,0x0C,0x03,0x4B},
												{0x14,0x00,0xB3,0xC4},
												{0x07,0x00,0x00,0x30},
												{0x36,0x10,0x19,0x04}},
												{0,0,0}, 0, {0,0},
												bool(0)	,{0x12,0x34,0x56,0x78,0x9ABC,0xBEEF}};
 void SaveStateToEEPROM(STATEOFPLATE* state, void* eepromPointer){
	 eeprom_update_block(state,eepromPointer,sizeof(STATEOFPLATE)); 
 }
 void ReadStateFromEEPROM(STATEOFPLATE* state, void* eepromPointer){
	 eeprom_read_block(state,eepromPointer,sizeof(STATEOFPLATE));
 }
//////////////////////////////////////////////////////////////////////////
uchar upas_in, upas_out;

/************************************************************************/
/*  Функции общие для всех пунктов***************************************/
/************************************************************************/
// void rightFunc(CMenuItem *item){
// 	item->incrCurPos();
// 	item->draw();
// }
// 
// void leftFunc(CMenuItem *item){
// 	item->decrCurPos();
// 	item->draw();
// }
//функции влево вправо переопределены чтоб по кольцу проходить регистры
void rightFunc(CMenuItem *item){
	if (item->getCurPos() == ((item->getCurRow() == 1)?(item->firstNumOfPermitedPosition -1):(item->secondNumOfPermitedPosition -1) ))
	{
		item->changeRow();
	} else
	item->incrCurPos();
	item->draw();
}

void leftFunc(CMenuItem *item){
	if (item->getCurPos() == 0)
	{
		item->changeRow();
	}
	item->decrCurPos();
	item->draw();
}
void escFunc(CMenuItem* item){
	item->resetCur();
	g_currentItem = item->getParent();
	//
	uchar datatmp[13];
	
	#ifndef DDC_BOARD
		Read_Controll(0x0FF7,&datatmp[0]);
		Read_Controll(0x0FF6,&datatmp[4]);
		Read_Controll(0x0FF5,&datatmp[8]);
		g_plateState.freq[0] = (datatmp[4] & 0xF0) >> 4;
		g_plateState.freq[1] = (datatmp[4] & 0x0F) ;
		g_plateState.freq[2] = (datatmp[5] & 0xF0) >> 4;
		g_plateState.freq[3] = (datatmp[5] & 0x0F) ;
		g_plateState.freq[4] = (datatmp[6] & 0xF0) >> 4;
		g_plateState.freq[5] = (datatmp[6] & 0x0F) ;
		g_plateState.freq[6] = (datatmp[7] & 0xF0) >> 4;
	#endif
	//
	 upas_in = 0;// read_adc(ADC_UPAS_IN_PIN);
	 upas_out = read_steady_adc(ADC_UPAS_OUT_PIN);
	 
	 if (g_plateState.selType == LEBED_TYPE)
	 {
		 	 			 g_lcaMenuItem.draw();//отображаются прочитанные из флеш значения LCA
		 	 			 _delay_ms(1500);
	 }
	 if (g_plateState.selType == ZAH_TYPE)
	 {
		 g_zselSettingMenuItem.draw();//отображаются прочитанные из флеш значения LCA
		 _delay_ms(1500);
	 }
	 
	g_currentItem->draw();
}

void Str2DrawStr(uchar* s1, uchar* s2, uchar* sDraw){
	memset(sDraw,0x00,NUM_OF_CHARS_PER_STR*2+1);
	memcpy(sDraw,(const char*)s1,strlen((const char*)s1));
	memcpy(&sDraw[NUM_OF_CHARS_PER_STR],(const char*)s2,strlen((const char*)s2));
}

void DrawOnLCD(uchar* str){
	lcd_clrscr();
	lcd_put_long_s((char*)str);
	lcd_gotoxy((g_currentItem->getCursorPosition()).column - 1,(g_currentItem->getCursorPosition()).row - 1);
}

/************************************************************************/
/************************ Главное меню **********************************/
/************************************************************************/
/* В этом меню производится установка частоты, отпирание/запирание ВЧ выхода,*/
/* настройка селектора на выбранной частоте, установка типа сигнала 2 тона или NON*/
/************************************************************************/


void okFuncMainMenu(CMenuItem* item);
void drawFuncMainMenu(CMenuItem* item);
void upFuncMainMenu(CMenuItem* item);
void downFuncMainMenu(CMenuItem* item);

CMenuItem g_mainMenuItem((char*)"F=XXXXX,XX кГц Настр",(char*)"XXX MOD:XX Уст S:X ",
&g_mainMenuItem,
{{&g_zselSettingMenuItem},1},
okFuncMainMenu,escFunc,drawFuncMainMenu,leftFunc,rightFunc,upFuncMainMenu,downFuncMainMenu,
{{3,4,5,6,7,9,10,16},8},
{{1,9,12,18},4});
//////////////////////////////////////////////////////////////////////////
//переменные для данного пункта меню
#define SETTING_POS 2//индекс меню настроек на экране
#define NASTR_POS 7//индекс настройки на экране
#define FREQ_POS 0//индекс частоты ан экране
#define SELTYPE_POS 3//индекс типа селектора на экране

//строки для вывода отпирания запирания
uchar OnOff[2][4] ={"ОТП","ЗАП"};

//строки для вывода режима
uchar Mode[2][3] ={"1T","2T"};
//строки для типа селектора
uchar Seltype[2] = {'L','Z'};
//////////////////////////////////////////////////////////////////////////
extern void WriteFreqToPrk(uchar* freq);//функция установки частоты в предкорректор, должна быть определена в другом месте
extern void ChangeModePrk(STATEOFPLATE*);//функция смены режима с одного тона на два
extern void outLCA2LCA(unsigned int *C1,unsigned char *L1,unsigned int *C2,unsigned char *L2,unsigned char *A,unsigned int* IN);
extern unsigned char ReadDetCoef();
// extern unsigned int CharFtoInd(unsigned char* f);
// extern void ReadLCAoutFromFlash(unsigned int addr,uchar *data);
// extern void SendLCA2Sel(unsigned char* LCAout);
//////////////////////////////////////////////////////////////////////////
void SetLebedSelFreqInFlash(){
	CharFtoInd(g_plateState.freq);
	ReadLCAoutFromFlash(CharFtoInd(g_plateState.freq),(uchar*)g_plateState.outLCA);//читаем из флешки комбинацию
	
	outLCA2LCA(&g_C1,&g_L1,&g_C2,&g_L2,&g_A,g_plateState.outLCA);//записываем ее в L C A
	
	SetnOTP(1);//запираем выход предкорректора
	SendLCA2Sel((uchar*)g_plateState.outLCA);// отправляем в селектор
	SetnOTP(g_plateState.output);//отпираем
	

}


//////////////////////////////////////////////////////////////////////////
void okFuncMainMenu(CMenuItem* item){ // Обработка нажатия ОК

	 if (1 == item->getCurRow())
	 {
		 if (item->getCurPos() < NASTR_POS)
		 {

			 //TODO :: Здесь должна быть функция установки частоты
			 SaveStateToEEPROM(&g_plateState,&g_eepromPlateState);//сохраняется частота в eeprom
			 WriteFreqToPrk(g_plateState.freq);//устанавлвиается частота генератора
			
			if (g_plateState.selType == LEBED_TYPE)
			{
				 SetLebedSelFreqInFlash();//устанавливается частота селектора
				g_plateState.detCoef = ReadDetCoef();
				g_currentItem = &g_lcaMenuItem;				 
			}
			if (g_plateState.selType == ZAH_TYPE)
			{
				//TODO: вписать сюда установку частоты селектора захарова
				int freq = g_plateState.freq[0]* 1000 + g_plateState.freq[1]* 100 +g_plateState.freq [2]*10 + g_plateState.freq[3]; 
				bool tempMode = g_plateState.mode; 
				g_plateState.mode = false;
				ChangeModePrk(&g_plateState);
				
				SetZselFreq(freq, &g_plateState.zSelParam);
				g_plateState.mode = tempMode;
				ChangeModePrk(&g_plateState);
				
			}		
			

		 }
	 
		 if (item->getCurPos() == NASTR_POS)
		 {
			 bool tempMode = g_plateState.mode;
			 g_plateState.mode = false;
			 ChangeModePrk(&g_plateState);
			 
			extern void TuneLvlLebedSel() ;
			if (g_plateState.selType == LEBED_TYPE)
			{
				TuneLvlLebedSel();			 
			}
			if (g_plateState.selType == ZAH_TYPE)
			{
				
				
				TuneZselAtten(&g_plateState.zSelParam);
				//TODO: вписать сюда настройку уровня селектора захарова
			}			 
					 
			 g_plateState.mode = tempMode;
			 ChangeModePrk(&g_plateState);
			 
		 }
	 }
	if (2 == item->getCurRow())
	 {

		 if (item->getCurPos()  == SETTING_POS)
		 {
			 
			 
			 if (g_plateState.selType == LEBED_TYPE)
			 {
				 
			 }
			 if (g_plateState.selType == ZAH_TYPE)
			 {
				 g_currentItem = &g_zselSettingMenuItem;
				 g_currentItem->draw();
			 }
			 
		 }
		 
		 
	 }
	g_currentItem->draw();
}
//////////////////////////////////////////////////////////////////////////
void upFuncMainMenu(CMenuItem* item){

		if ( (item->getCurRow() == 2) && (item->getCurPos() == 0 ) ) {
						g_plateState.output = !g_plateState.output;
						SetnOTP(g_plateState.output);}
		if ( (item->getCurRow() == 2) && (item->getCurPos() == 1) ){
								g_plateState.mode = !(g_plateState.mode);
								ChangeModePrk(&g_plateState);
		}
		if ( (item->getCurRow() == 2) && (item->getCurPos() == SELTYPE_POS) ){
			g_plateState.selType = !(g_plateState.selType);
			 SaveStateToEEPROM(&g_plateState,&g_eepromPlateState);	
		}		
		if ( (item->getCurRow() == 1) && (item->getFirstStr()[item->getCursorPosition().column - 1] == 'X') )
				(g_plateState.freq[item->getCurPos() - FREQ_POS] < 9)?
					(g_plateState.freq[item->getCurPos() - FREQ_POS]++)
						:(g_plateState.freq[item->getCurPos() - FREQ_POS] = 0);
		
	
	item->draw();	
}
//////////////////////////////////////////////////////////////////////////
void downFuncMainMenu(CMenuItem* item){
			
		if ( (item->getCurRow() == 2) && (item->getCurPos() == 0 ) ){
						g_plateState.output = !g_plateState.output;
						SetnOTP(g_plateState.output);}
						
		if ( (item->getCurRow() == 2) && (item->getCurPos() == 1) ){
						g_plateState.mode = !(g_plateState.mode);
						ChangeModePrk(&g_plateState);
						}
		if ( (item->getCurRow() == 2) && (item->getCurPos() == SELTYPE_POS) ){
			g_plateState.selType = !(g_plateState.selType);
			 SaveStateToEEPROM(&g_plateState,&g_eepromPlateState);
		}						
		if ( (item->getCurRow() == 1) && (item->getFirstStr()[item->getCursorPosition().column - 1] == 'X') )
					(g_plateState.freq[item->getCurPos() - FREQ_POS] > 0)?
						(g_plateState.freq[item->getCurPos() - FREQ_POS]--)
							:(g_plateState.freq[item->getCurPos() - FREQ_POS] = 9);				
			item->draw();
}
//////////////////////////////////////////////////////////////////////////
void drawFuncMainMenu(CMenuItem* item){

	memcpy(TempStr1,item->getFirstStr(),//копируем строку из пункта меню во временную строку
			(strlen(item->getFirstStr()) > NUM_OF_CHARS_PER_STR)?NUM_OF_CHARS_PER_STR:strlen(item->getFirstStr()));//потенциально может скопировать мусор за пределами строки
	memcpy(TempStr2,item->getSecondStr(),
			(strlen(item->getSecondStr()) > NUM_OF_CHARS_PER_STR)?NUM_OF_CHARS_PER_STR:strlen(item->getSecondStr()));
	if (g_plateState.output == 0)
			memcpy(TempStr2,OnOff[0],3)	;
	else memcpy(TempStr2,OnOff[1],3)	;
	
	memcpy(strchr(TempStr2,'X'),Mode[g_plateState.mode],2);
	unsigned char OutStrPos = 0;
	for (unsigned char i = 0; i < strlen(TempStr1); i++){// выводим во временную строку на позиции символов Х частоту и режим
		if (TempStr1[i] == 'X') {
			TempStr1[i] = g_hexOut[g_plateState.freq[OutStrPos]];
			OutStrPos++;
		}
	}
// 		TempStr2[strchr(TempStr2,'X') - TempStr2] = g_hexOut[(upas_in & 0xF0)>>4];
// 		TempStr2[strchr(TempStr2,'X') - TempStr2] = g_hexOut[(upas_in & 0x0F)];
	if (g_plateState.selType == 0)
		memcpy(strchr(TempStr2,'X') ,(void*)&Seltype[0],1)	;
	else memcpy(strchr(TempStr2,'X') ,(void*)&Seltype[1],1)	;
// 	TempStr2[strchr(TempStr2,'X') - TempStr2] = g_hexOut[(upas_in & 0xF0)>>4];
// 	TempStr2[strchr(TempStr2,'X') - TempStr2] = g_hexOut[(upas_in & 0x0F)];
// 
// 	TempStr2[strchr(TempStr2,'X') - TempStr2] = g_hexOut[(upas_out & 0xF0)>>4];
// 	TempStr2[strchr(TempStr2,'X') - TempStr2] = g_hexOut[(upas_out & 0x0F)];
	
	Str2DrawStr((uchar*)TempStr1,(uchar*)TempStr2,g_drawingString);
	
	DrawOnLCD(g_drawingString);
}


/************************************************************************/
/***************  Меню параметров предкорректора  ***********************/
/************************************************************************/
/************************************************************************/
/* В данном пункте меню предоставлен доступ к регистрам предкорректора  */
/* первые два экрана позволяют редактировать регистры, остальные для чтения*/
/************************************************************************/
void okFuncPrkMenu(CMenuItem* item);
void drawFuncPrkMenu(CMenuItem* item);
void upFuncPrkMenu(CMenuItem* item);
void downFuncPrkMenu(CMenuItem* item);
void escFuncPrkMenu(CMenuItem* item);
// void rightFuncPrkItem(CMenuItem *item);
// void leftFuncPrkItem(CMenuItem *item);	


CMenuItem g_prkSettingMenuItem((char*)"#X@C_XR@XX@XX@XX@XX@",(char*)"@@@C_XR@XX@XX@XX@XX@",
&g_mainMenuItem,
{{&g_prkSettingMenuItem},1},
okFuncPrkMenu,escFuncPrkMenu,drawFuncPrkMenu,leftFunc,rightFunc,upFuncPrkMenu,downFuncPrkMenu,
{{1,9,10,12,13,15,16,18,19},9},
{{9,10,12,13,15,16,18,19},8});
//////////////////////////////////////////////////////////////////////////
//Переменные для данного пункта меню
#define NUM_OF_PRK_DISP 2 //количество экранов с регистрами	
char prk_numOfDisplay = 0; //номер отображаемого экрана регистров
//////////////////////////////////////////////////////////////////////////
void okFuncPrkMenu(CMenuItem* item){
	//TODO Здесь должна быть функция записывающая выбранные регистры в плис
	SaveStateToEEPROM(&g_plateState,(void*)&g_eepromPlateState);
		Write_Controll(0x07FF, g_plateState.C_R[0]);
		Write_Controll(0x07FE, g_plateState.C_R[1]);
		Write_Controll(0x07FD, g_plateState.C_R[2]);
		Write_Controll(0x07FC, g_plateState.C_R[3]);
}
//////////////////////////////////////////////////////////////////////////


void escFuncPrkMenu(CMenuItem* item){
	if ( (item->getCurRow() == 1) && (item->getCurPos() == 0) )
		g_currentItem = item->getParent();
	else
		item->resetCur();
	Read_Controll(0x0FFF, g_plateState.C_R[0]);
	Read_Controll(0x0FFE, g_plateState.C_R[1]);
	Read_Controll(0x0FFD, g_plateState.C_R[2]);
	Read_Controll(0x0FFC, g_plateState.C_R[3]);
	g_currentItem->draw();
}

//////////////////////////////////////////////////////////////////////////
void drawFuncPrkMenu(CMenuItem* item){
		memcpy(TempStr1,item->getFirstStr(),//копируем строку из пункта меню во временную строку
			(strlen(item->getFirstStr()) > NUM_OF_CHARS_PER_STR)?NUM_OF_CHARS_PER_STR:strlen(item->getFirstStr()));//потенциально может скопировать мусор за пределами строки
		memcpy(TempStr2,item->getSecondStr(),
			(strlen(item->getSecondStr()) > NUM_OF_CHARS_PER_STR)?NUM_OF_CHARS_PER_STR:strlen(item->getSecondStr()));
		
		TempStr1[strchr(TempStr1,'X') - TempStr1] = g_hexOut[prk_numOfDisplay];//находим первый Х в строке и меняем его на номер экрана
																				//индекс символа вычисляем как развность указателей
																				//потенциально опасно
		TempStr1[strchr(TempStr1,'X') - TempStr1] = g_hexOut[prk_numOfDisplay*2];
		TempStr2[strchr(TempStr2,'X') - TempStr2] = g_hexOut[prk_numOfDisplay*2+1];
		
		unsigned char OutStrPos = 0;
			for (unsigned char i = 0; i < strlen(TempStr1); i++){// выводим во временную строку на позиции символов Х значения регистров
				if (TempStr1[i] == 'X') {
					if ( (OutStrPos % 2) == 0)
					{//если четная позиция то выводим старший полубайт
						TempStr1[i] = g_hexOut[(g_plateState.C_R[prk_numOfDisplay*2][OutStrPos/2] & 0xF0 ) >> 4];
						TempStr2[i] = g_hexOut[(g_plateState.C_R[prk_numOfDisplay*2 + 1][OutStrPos/2] & 0xF0 ) >> 4];
					}else//иначе выводим младший полубайт
						{
						TempStr1[i] = g_hexOut[(g_plateState.C_R[prk_numOfDisplay*2][OutStrPos/2] & 0x0F)];
						TempStr2[i] = g_hexOut[(g_plateState.C_R[prk_numOfDisplay*2 + 1][OutStrPos/2] & 0x0F)];
						}
					OutStrPos++;
				}
				if ('@' == TempStr1[i] ) TempStr1[i] = ' ';
				if ('@' == TempStr2[i] ) TempStr2[i] = ' ';
			}


	Str2DrawStr((uchar*)TempStr1,(uchar*)TempStr2,g_drawingString);
	
	DrawOnLCD(g_drawingString);
}
//////////////////////////////////////////////////////////////////////////
void upFuncPrkMenu(CMenuItem* item){
	if ( (item->getCurPos() == 0) && (item->getCurRow() == 1) )
		(prk_numOfDisplay < NUM_OF_PRK_DISP - 1)?(prk_numOfDisplay++):(prk_numOfDisplay = 0);
	else//( (item->getCurPos() - item->getCurRow()%2) > 0 )
		{	//если находимся не в верхнем левом углу то
			//номер регистра зависит от номера экрана и строки на которой находистя курсор
			//на экране по два регистра, следовательно номер экрана умножаем на два
			//и прибавляем 0 для первой строки и 1 для второй
			//если позиция четная то изменяем старшие разряды, а если нечеетная то младшие
			//по тому что нумерация позиция идет с нуля
			//по этой же причине номер позиции вычисляется в зависимости от номера строки
			//если стркоа первая - то надо вычесть единицу по тому что там на одну позицию больше
			//индекс в массиве в два раза меньше чем номер позиции в строке
			
			char numPos = (item->getCurPos() - item->getCurRow()%2);
			#define  TEMP_CHAR (g_plateState.C_R[prk_numOfDisplay*2 + item->getCurRow()/2][numPos/2])

			if (0 == numPos%2)
			{
				TEMP_CHAR = (TEMP_CHAR & 0x0F ) | ((TEMP_CHAR & 0xF0) + 0x10);
			}else
			TEMP_CHAR = (TEMP_CHAR & 0xF0) | ((TEMP_CHAR + 0x01) & 0x0F);
			
		}
	item->draw();
}
//////////////////////////////////////////////////////////////////////////
void downFuncPrkMenu(CMenuItem* item){
	if ( (item->getCurPos() == 0) && (item->getCurRow() == 1) )
		(prk_numOfDisplay > 0)?(prk_numOfDisplay--):(prk_numOfDisplay = NUM_OF_PRK_DISP - 1);
	else//( (item->getCurPos() - item->getCurRow()%2) > 0 )
	{	//если находимся не в верхнем левом углу то
		//номер регистра зависит от номера экрана и строки на которой находистя курсор
		//на экране по два регистра, следовательно номер экрана умножаем на два 
		//и прибавляем 0 для первой строки и 1 для второй
		//если позиция четная то изменяем старшие разряды, а если нечеетная то младшие
		//по тому что нумерация позиция идет с нуля
		//по этой же причине номер позиции вычисляется в зависимости от номера строки
		//если стркоа первая - то надо вычесть единицу по тому что там на одну позицию больше
		//индекс в массиве в два раза меньше чем номер позиции в строке
		
		char numPos = (item->getCurPos() - item->getCurRow()%2);
		#define  TEMP_CHAR (g_plateState.C_R[prk_numOfDisplay*2 + item->getCurRow()/2][numPos/2])
		
		if (0 == numPos%2)
		{
			TEMP_CHAR = (TEMP_CHAR & 0x0F ) | ((TEMP_CHAR & 0xF0) - 0x10);
		}else
			TEMP_CHAR = (TEMP_CHAR & 0xF0) | ((TEMP_CHAR - 0x01) & 0x0F);
		
	}
		
	
		item->draw();
}		
//////////////////////////////////////////////////////////////////////////
/************************************************************************/
/* Меню отображения параметров селектора                                */
/************************************************************************/
void drawFuncLCAMenu(CMenuItem* item);
void escFuncLCA(CMenuItem* item);
CMenuItem g_lcaMenuItem((char*)"@A=XX@L1=XX@C1=XXX@@",(char*)"DC=XX@L2=XX@C2=XXXXX",
&g_mainMenuItem,
{{&g_mainMenuItem},1},
0,escFuncLCA,drawFuncLCAMenu,0,0,0,0,
{{1},0},
{{1},0});
	
	
	void escFuncLCA( CMenuItem* item )
	{
		g_currentItem = item->getParent();
		g_currentItem->draw();
	}


//////////////////////////////////////////////////////////////////////////
// Функции для работы с селектором

//////////////////////////////////////////////////////////////////////////
void reverseBits(unsigned int orig,unsigned int *result){
	unsigned int temp = 0;

	for(unsigned char bit = 0; bit < 16; bit++) {
		temp += ((orig >> (unsigned int)(15 - bit)) & 0x0001 ) << (unsigned int)bit;
	}
	*result = temp;
}
//////////////////////////////////////////////////////////////////////////
void LCA2out(unsigned int C1,unsigned char L1,unsigned int C2,unsigned char L2,unsigned char A,unsigned int* OUT){
	reverseBits(C1, &C1);
	C1 = C1 >> 4;//сдвиг обусловлен расположением значения С в OUT
	
	reverseBits(C2, &C2);
	C2 = C2 >> 4;
	
	OUT[0] = (C1 & 0x0FFF) + (((unsigned int)(L1 & 0x07)) << 13);
	OUT[1] = ((unsigned int)(L1 & 0x78) >> 3)+ ((C2 & 0x0FFF) << 4);
	A ^= _BV(5); //инверсия 6 бита для включения RZ					//эта часть записывает копию бита RZ для серийной платы селектора
	OUT[2] = (((unsigned int)L2) << 1) + (((unsigned int)~A) << 8) + ((((unsigned int)~A) & _BV(5)) >> 5);
	
	//переписываем для хранения OUT в EEPROM
	//eeprom_update_word(&OUT[0],(C1 & 0x0FFF) + (((unsigned int)(L1 & 0x07)) << 13));
	//eeprom_update_word(&OUT[1],((unsigned int)(L1 & 0x78) >> 3)+ ((C2 & 0x0FFF) << 4));
	//eeprom_update_word(&OUT[2],(((unsigned int)L2) << 1) + (((unsigned int)~A) << 8));
}
//////////////////////////////////////////////////////////////////////////
void outLCA2LCA(unsigned int *C1,unsigned char *L1,unsigned int *C2,unsigned char *L2,unsigned char *A,unsigned int* IN){
	*C1 = IN[0] & 0x0FFF;
	*L1 = ((unsigned char)((IN[0] & 0xE000) >> 13)) + (((unsigned char)(IN[1] & 0x000F)) << 3);
	
	*C2 = (IN[1] & 0xFFF0) >> 4;
	*L2 = ( (unsigned char)(IN[2] & 0x00FE) >> 1 ) & 0x7F;
	
	*A = (~( (unsigned char)( (IN[2] & 0x3F00) >> 8 ) )) & 0x3F;
	*A ^= _BV(5); //инверсия 6 бита для включения RZ
	
	reverseBits(*C1, C1);
	*C1 = *C1 >> 4;
	reverseBits(*C2, C2);
	*C2 = *C2 >> 4;
}	
//////////////////////////////////////////////////////////////////////////
void drawFuncLCAMenu(CMenuItem* item){

		LCA2out(g_C1,g_L1,g_C2,g_L2,g_A,g_plateState.outLCA);
	outLCA2LCA(&g_C1,&g_L1,&g_C2,&g_L2,&g_A,g_plateState.outLCA);
		memcpy(TempStr1,item->getFirstStr(),//копируем строку из пункта меню во временную строку
			(strlen(item->getFirstStr()) > NUM_OF_CHARS_PER_STR)?NUM_OF_CHARS_PER_STR:strlen(item->getFirstStr()));//потенциально может скопировать мусор за пределами строки
		memcpy(TempStr2,item->getSecondStr(),
			(strlen(item->getSecondStr()) > NUM_OF_CHARS_PER_STR)?NUM_OF_CHARS_PER_STR:strlen(item->getSecondStr()));
		TempStr1[strchr(TempStr1,'X') - TempStr1] = g_hexOut[(g_A & 0xF0) >> 4] ;
		TempStr1[strchr(TempStr1,'X') - TempStr1] = g_hexOut[(g_A & 0x0F) ] ;
		
		TempStr1[strchr(TempStr1,'X') - TempStr1] = g_hexOut[(g_L1 & 0xF0)>>4 ] ;
		TempStr1[strchr(TempStr1,'X') - TempStr1] = g_hexOut[(g_L1 & 0x0F) ] ;
		
		TempStr1[strchr(TempStr1,'X') - TempStr1] = g_hexOut[(g_C1 & 0x0F00) >> 8] ;
		TempStr1[strchr(TempStr1,'X') - TempStr1] = g_hexOut[(g_C1 & 0x00F0) >>4] ;
		TempStr1[strchr(TempStr1,'X') - TempStr1] = g_hexOut[(g_C1 & 0x000F) ] ;
		//
		TempStr2[strchr(TempStr2,'X') - TempStr2] = g_hexOut[(g_plateState.detCoef & 0xF0) >>4] ;
		TempStr2[strchr(TempStr2,'X') - TempStr2] = g_hexOut[(g_plateState.detCoef & 0x0F) ] ;
		//
		TempStr2[strchr(TempStr2,'X') - TempStr2] = g_hexOut[(g_L2 & 0xF0) >>4] ;
		TempStr2[strchr(TempStr2,'X') - TempStr2] = g_hexOut[(g_L2 & 0x0F) ] ;
				
		TempStr2[strchr(TempStr2,'X') - TempStr2] = g_hexOut[(g_C2 & 0x0F00)>> 8] ;
		TempStr2[strchr(TempStr2,'X') - TempStr2] = g_hexOut[(g_C2 & 0x00F0)>> 4] ;
		TempStr2[strchr(TempStr2,'X') - TempStr2] = g_hexOut[(g_C2 & 0x000F) ] ;
		
		char a  = read_adc(ADC_UPAS_OUT_PIN);
				TempStr2[strchr(TempStr2,'X') - TempStr2] = g_hexOut[(a & 0x00F0)>> 4] ;
				TempStr2[strchr(TempStr2,'X') - TempStr2] = g_hexOut[(a & 0x000F) ] ;
for (unsigned char i = 0; i < strlen(TempStr1); i++){// выводим во временную строку на позиции символов Х значения регистров
	if ('@' == TempStr1[i] ) TempStr1[i] = ' ';
	if ('@' == TempStr2[i] ) TempStr2[i] = ' ';
}

		Str2DrawStr((uchar*)TempStr1,(uchar*)TempStr2,g_drawingString);
		DrawOnLCD(g_drawingString);
}

/************************************************************************/
/***************  Меню параметров селектора захарова ********************/
/************************************************************************/
/************************************************************************/
void okFuncZselMenu(CMenuItem* item);
void drawFuncZselMenu(CMenuItem* item);
void upFuncZselMenu(CMenuItem* item);
void downFuncZselMenu(CMenuItem* item);
void escFuncZselMenu(CMenuItem* item);
void rightFuncZselItem(CMenuItem *item);
void leftFuncZselItem(CMenuItem *item);

#define ATEN_POS 19
#define CAP_POS	4
#define FREQ_ZSEL_POS 17
CMenuItem g_zselSettingMenuItem((char*)"L=XX@C=XX@Li=XX@A=XX",(char*)"C1=XXXX+XX@@@@F:XXXX",
&g_mainMenuItem,
{{&g_zselSettingMenuItem},1},
okFuncZselMenu,escFuncZselMenu,drawFuncZselMenu,leftFuncZselItem,rightFuncZselItem,upFuncZselMenu,downFuncZselMenu,
{{3,4,8,9,14,15,ATEN_POS,ATEN_POS+1},8},
{{CAP_POS,CAP_POS + 1,CAP_POS + 2 ,CAP_POS +3 ,8,FREQ_ZSEL_POS ,FREQ_ZSEL_POS + 1,FREQ_ZSEL_POS + 2,FREQ_ZSEL_POS + 3},9});
//перенесено в начало
// typedef struct{
// 	char L;
// 	char C;
// 	char link;
// 	char A;
// 	int cap1;
// 	int cap2;
// 	} ZSELPARAM;
// ZSELPARAM zSelParam;
void * str1Params[4] = {&g_plateState.zSelParam.L,&g_plateState.zSelParam.C,&g_plateState.zSelParam.link,&g_plateState.zSelParam.A};
void * str2Params[2] = {&g_plateState.zSelParam.cap1,&g_plateState.zSelParam.cap2};

	
regAddr str1Reg[4] = {LREG,CAPREG,LINKREG,AREG};
regAddr str2Reg[2] = {CAP1REG,CAP2REG};
//////////////////////////////////////////////////////////////////////////
void okFuncZselMenu( CMenuItem* item )
{
	if ((item->getCurRow() == 1) && (item->getCursorPosition().column/ATEN_POS == 1))//если курсор на аттенюаторе
	{
		TuneZselAtten(&g_plateState.zSelParam);
		
	}else
	if ((item->getCurRow() == 2) && (item->getCursorPosition().column/CAP_POS == 1))//если курсор на емкости
	{
		//TODO: вставить сюда плавную настройку емкости
		TuneZselCap(&g_plateState.zSelParam);
	}else
	if ((item->getCurRow() == 2) && (item->getCursorPosition().column/FREQ_ZSEL_POS == 1))//если курсор на частоте
	{
		WriteFreqToPrk(g_plateState.freq);//устанавлвиается частота генератора
		SetZselFreq(g_plateState.freq[0]* 1000 + g_plateState.freq[1]* 100 + g_plateState.freq[2]* 10 + g_plateState.freq[3] ,&g_plateState.zSelParam);
		
	}	else	
			SetZselParam(&g_plateState.zSelParam,ALLREG);
		
		SaveStateToEEPROM(&g_plateState,&g_eepromPlateState);//сохраняется частота в eeprom
		item->draw();
	//SetZselFreq(0, &g_plateState.zSelParam);
	
}
//////////////////////////////////////////////////////////////////////////
void drawFuncZselMenu( CMenuItem* item )
{
			memcpy(TempStr1,item->getFirstStr(),//копируем строку из пункта меню во временную строку
			(strlen(item->getFirstStr()) > NUM_OF_CHARS_PER_STR)?NUM_OF_CHARS_PER_STR:strlen(item->getFirstStr()));//потенциально может скопировать мусор за пределами строки
			memcpy(TempStr2,item->getSecondStr(),
			(strlen(item->getSecondStr()) > NUM_OF_CHARS_PER_STR)?NUM_OF_CHARS_PER_STR:strlen(item->getSecondStr()));
	TempStr1[strchr(TempStr1,'X') - TempStr1] = g_hexOut[(g_plateState.zSelParam.L & 0xF0) >> 4] ;
	TempStr1[strchr(TempStr1,'X') - TempStr1] = g_hexOut[(g_plateState.zSelParam.L & 0x0F) ] ;
														 
	TempStr1[strchr(TempStr1,'X') - TempStr1] = g_hexOut[(g_plateState.zSelParam.C & 0xF0) >> 4 ] ;
	TempStr1[strchr(TempStr1,'X') - TempStr1] = g_hexOut[(g_plateState.zSelParam.C & 0x0F) ] ;
	
	TempStr1[strchr(TempStr1,'X') - TempStr1] = g_hexOut[(g_plateState.zSelParam.link & 0xF0) >> 4 ] ;
	TempStr1[strchr(TempStr1,'X') - TempStr1] = g_hexOut[(g_plateState.zSelParam.link & 0x0F) ] ;
	
	TempStr1[strchr(TempStr1,'X') - TempStr1] = g_hexOut[(g_plateState.zSelParam.A & 0xF0) >> 4 ] ;
	TempStr1[strchr(TempStr1,'X') - TempStr1] = g_hexOut[(g_plateState.zSelParam.A & 0x0F) ] ;													  

														  
	//													 
	TempStr2[strchr(TempStr2,'X') - TempStr2] = g_hexOut[(g_plateState.zSelParam.cap1 & 0xF000)>> 12] ;
	TempStr2[strchr(TempStr2,'X') - TempStr2] = g_hexOut[(g_plateState.zSelParam.cap1 & 0x0F00)>> 8] ;
	TempStr2[strchr(TempStr2,'X') - TempStr2] = g_hexOut[(g_plateState.zSelParam.cap1 & 0x00F0)>> 4] ;
	TempStr2[strchr(TempStr2,'X') - TempStr2] = g_hexOut[(g_plateState.zSelParam.cap1 & 0x000F) ] ;	
	
	
	uchar a = read_adc(ADC_UPAS_OUT_PIN);//здесь используется простое чтение значения ацп по тому что при двухтоновом сигнале 
	//чтение после стабилизации зависает (из-за того что стабилизация уровня не наступает)
	
	TempStr2[strchr(TempStr2,'X') - TempStr2] = g_hexOut[(a & 0xF0) >> 4];
	TempStr2[9] = g_hexOut[(a & 0x0F) ];
														
	TempStr2[strchr(TempStr2,'X') - TempStr2] = g_hexOut[(g_plateState.freq[0])] ;
	TempStr2[strchr(TempStr2,'X') - TempStr2] = g_hexOut[(g_plateState.freq[1])];
	TempStr2[strchr(TempStr2,'X') - TempStr2] = g_hexOut[(g_plateState.freq[2])];
	TempStr2[strchr(TempStr2,'X') - TempStr2] = g_hexOut[(g_plateState.freq[3])];
	
	
	
	
	for (unsigned char i = 0; i < strlen(TempStr1); i++){// выводим во временную строку на позиции символов Х значения регистров
		if ('@' == TempStr1[i] ) TempStr1[i] = ' ';
		if ('@' == TempStr2[i] ) TempStr2[i] = ' ';
	}	
	
		Str2DrawStr((uchar*)TempStr1,(uchar*)TempStr2,g_drawingString);
		DrawOnLCD(g_drawingString);
}
//////////////////////////////////////////////////////////////////////////
void upFuncZselMenu( CMenuItem* item )
{
	if (g_currentItem->getCurRow() == 1)
	{
		char temp;
		char ind = item->getCurPos();
		char* param = (char*) str1Params[ind/2];
		temp = ((*param) & ((ind%2)?(0x0F):(0xF0))) >> (4*(1 - ind%2));
		if(temp < 15) temp++;
			else temp = 0;
		*param = ((*param) & ((1 - ind%2)?(0x0F):(0xF0))) + (temp << (4*(1 - ind%2)));
		
		//str1Func[ind/2](*param);
		SetZselParam(&g_plateState.zSelParam,str1Reg[ind/2]);//TODO: может меняться от длины параметра
		SaveStateToEEPROM(&g_plateState,&g_eepromPlateState);//сохраняется частота в eeprom
	}
	if (g_currentItem->getCurRow() == 2)
	{	
		char temp;
		if (item->getCursorPosition().column/FREQ_ZSEL_POS == 1)
		{
			char* tempp = (char*)&g_plateState.freq[item->getCursorPosition().column - FREQ_ZSEL_POS];
			if(*tempp < 9) (*tempp)++;
			else *tempp = 0;
		}else{
		
		char ind = item->getCurPos();
		int* param = (int*) str2Params[ind/5];
		temp = ((*param) & (unsigned int)((0xF) << (4*(3 - ind%4)))) >> (4*(3-ind%4));
		if (ind == 4)
		{
			*param = ((*param) + 1);

		}else{
  		if(temp < 15) temp++;
  		else temp = 0;
		  
 		*param = ((*param) & ~(unsigned int)((0xF) << (4*(3 - ind%4)))) + (temp << (4*(3-ind%4)));
		}
		//str2Func[ind/4](*param);//TODO: может меняться от длины параметра
		SetZselParam(&g_plateState.zSelParam,str2Reg[ind/5]);//TODO: может меняться от длины параметра
		SaveStateToEEPROM(&g_plateState,&g_eepromPlateState);//сохраняется частота в eeprom
		}
	}	
	 
	g_currentItem->draw();
}
//////////////////////////////////////////////////////////////////////////
void downFuncZselMenu( CMenuItem* item )
{
		if (g_currentItem->getCurRow() == 1)
		{
			char temp;
			char ind = g_currentItem->getCurPos();
			char* param = (char*) str1Params[ind/2];
			//	LCD_DisplayCharacter(g_hexOut[ind]);
			//_delay_ms(1000);
			temp = ((*param) & ((ind%2)?(0x0F):(0xF0))) >> (4*(1 - ind%2));
			//LCD_DisplayCharacter(g_hexOut[*param & 0x0F]);
			//			LCD_DisplayCharacter(g_hexOut[temp]);
			
			if(temp > 0) temp--;
			else temp = 15;
			//	LCD_DisplayCharacter(g_hexOut[temp]);
			*param = ((*param) & ((1 - ind%2)?(0x0F):(0xF0))) + (temp << (4*(1 - ind%2)));
			//str1Func[ind/2](*param);//TODO: может меняться от длины параметра
			// 		LCD_DisplayCharacter(g_hexOut[*param >> 4]);
			// 		_delay_ms(1000);
			SetZselParam(&g_plateState.zSelParam,str1Reg[ind/2]);//TODO: может меняться от длины параметра
			SaveStateToEEPROM(&g_plateState,&g_eepromPlateState);//сохраняется частота в eeprom
		}
	if (g_currentItem->getCurRow() == 2)
	{
		char temp;
		if (item->getCursorPosition().column/FREQ_ZSEL_POS == 1)
		{
			char* tempp = (char*)&g_plateState.freq[item->getCursorPosition().column - FREQ_ZSEL_POS];
			if(*tempp > 0) (*tempp)--;
			else *tempp = 9;
			}else{
		char ind = g_currentItem->getCurPos();
		int* param = (int*) str2Params[ind/5];
		temp = ((*param) & (unsigned int)((0xF) << (4*(3 - ind%4)))) >> (4*(3-ind%4));
		if (ind == 4)
		{
			*param = (*param)-1;
		}else{
			if(temp > 0) temp--;
			else temp = 15;
		
		*param = ((*param) & ~(unsigned int)((0xF) << (4*(3 - ind%4)))) + (temp << (4*(3-ind%4)));
		}
		//str2Func[ind/4](*param);//TODO: может меняться от длины параметра
		SetZselParam(&g_plateState.zSelParam,str2Reg[ind/5]);//TODO: может меняться от длины параметра
		SaveStateToEEPROM(&g_plateState,&g_eepromPlateState);//сохраняется частота в eeprom
		}
	}
	
		g_currentItem->draw();
}
//////////////////////////////////////////////////////////////////////////
void escFuncZselMenu( CMenuItem* item )
{
	if ( (item->getCurRow() == 1) && (item->getCurPos() == 0) )
		g_currentItem = item->getParent();
	
	else
		g_currentItem->resetCur();
		//TuneZselAtten(&g_plateState.zSelParam);;

	g_currentItem->draw();	
}
//////////////////////////////////////////////////////////////////////////
void rightFuncZselItem( CMenuItem *item )
{

	rightFunc(item);
		if (item->getCurRow() == 1)
		{

			ChangeReg(str1Reg[item->getCurPos()/2]);//TODO: может меняться от длины параметра
		}
		if (item->getCurRow() == 2)
		{
			ChangeReg(str2Reg[item->getCurPos()/4]);//TODO: может меняться от длины параметра
		}
}
//////////////////////////////////////////////////////////////////////////
void leftFuncZselItem( CMenuItem *item )
{
		
	leftFunc(item);
	if (item->getCurRow() == 1)
	{
		ChangeReg(str1Reg[item->getCurPos()/2]);//TODO: может меняться от длины параметра
	}
	if (item->getCurRow() == 2)
	{
		ChangeReg(str2Reg[item->getCurPos()/4]);//TODO: может меняться от длины параметра
	}
}




#endif /* MENU_IMPL_H_ */