#ifndef LCD_KNK_H_
#define LCD_KNK_H_

#include "main.h"
//дефайны которые должны быть в файле main.h
//#define port_lcd            PORTA
//#define ddr_lcd             DDRA
//#define pin_lcd             PINA

#define	SET_LCD_E           port_lcd |= 0x04
#define CLEAR_LCD_E         port_lcd &= ~0x04
#define SET_LCD_DATA        port_lcd |= 0x01
#define SET_LCD_CMD         port_lcd &= ~0x01 
#define SET_LCD_READ        port_lcd |= 0x02
#define SET_LCD_WRITE       port_lcd &= ~0x02 

#define LCD_ON			0x0C   
#define LCD_CURS_ON		0x0F
#define LCD_OFF			0x08
#define LCD_HOME		0x02  
#define LCD_CLEAR		0x01
#define LCD_NEW_LINE		0xC0
#define LCD_FUNCTION_SET	0x38
#define LCD_MODE_SET		0x06
#define NUM_OF_CHARS_PER_STR 20
#define NUM_OF_ROWS 2

	#ifndef uchar
		typedef unsigned char uchar;
	#endif
#include <avr/pgmspace.h>
#include <util/delay.h>

void LCD_WriteControl (unsigned char CMD);
void LCD_WriteData (unsigned char Data); 
void LCD_Write4Data (unsigned char Data) ; 
void LCD_Write4Control (unsigned char CMD);
void Init_LCD(void);
void LCD_Clear(void);  
void LCD_Home(void);
void LCD_DisplayCharacter (char Char);
void LCD_DisplayString (char row, char column , char  *string);
void LCD_Cursor (char row, char column);
void LCD_Cursor_On (void);
void LCD_Cursor_Off (void);
void LCD_Display_On (void);  
void LCD_DisplayCharacterRus (char Char);
void LCD_DisplayStringRus (char row, char column , char *string);
unsigned char LCD_Busy ( void );
void LCD_DisplayDrawLongString(char *string);
void LCD_DisplayFlashLongString(char *string);
#endif