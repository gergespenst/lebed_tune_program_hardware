/*
 * lcd_knk.cpp
 * функции дял работы с дисплеем 
 * подключенным по схеме копьева
 * Created: 29.09.2014 12:18:52
 *  Author: USER
 */ 
#include "lcd_knk.h"
//*****************************************************//
// This routine will return the busy flag from the LCD //
//*****************************************************//
  const unsigned char  ascii_lcd[64]  PROGMEM={0x41,  0xa0,  0x42,  0xa1,  0xe0,  0x45,  0xa3,  0xa4,  0xa5,  0xa6,  0x4b,  0xa7,  0x4d,  0x48,  0x4f,  0xa8,  0x50,  0x43,  0x54,  0xa9,  0xaa,  0x58,  0xe1,  0xab,  0xac,  0xe2,  0xad,  0xae,  0x62,  0xaf,  0xb0,  0xb1,  0x61,  0xb2,  0xb3,  0xb4,  0xe3,  0x65,  0xb6,  0xb7,  0xb8,  0xb9,  0xba,  0xbb,  0xbc,  0xbd,  0x6f,  0xbe,  0x70,  0x63,  0xbf,  0x79,  0xe4,  0x78,  0xe5,  0xc0,  0xc1,  0xe6,  0xc2,  0xc3,  0xc4,  0xc5,  0xc6,  0xc7};
unsigned char LCD_Busy ( void )
{
	unsigned char temp;
	CLEAR_LCD_E;	   				 // Disable LCD
	//port_lcd&=~0xF0;
	ddr_lcd = 0x0F;		                	// Make I/O Port input
	SET_LCD_READ;					// Set LCD to READ
	SET_LCD_CMD;
	_delay_us(5);					// Set LCD to command
	SET_LCD_E;
	_delay_us(3);
	temp = pin_lcd;
	_delay_us(3);			        	// Load data from port
	CLEAR_LCD_E;
	_delay_us(5);

	SET_LCD_E;
	_delay_us(3);
	CLEAR_LCD_E;

	ddr_lcd = 0xFF;	   				 // Disable LCD
	return temp;					 // return busy flag
}






// ********************************************** //
// *** Write a 1/2 control instruction to the LCD *** //
// ********************************************** //

void LCD_Write4Control (unsigned char CMD)
{
	while ((LCD_Busy()&0X80)==0X80);	// Test if LCD busy
	CLEAR_LCD_E;	   		// Disable LCD
	port_lcd&= ~0xF0;			// Load data to port
	port_lcd|=CMD;
	SET_LCD_WRITE ;				// Set LCD to write
	SET_LCD_CMD;                 // Set LCD to command
	_delay_us(5);
	SET_LCD_E;	   				// Write data to LCD
	_delay_us(3);
	CLEAR_LCD_E;
	_delay_us(5);	   			// Disable LCD
	port_lcd&= ~0xF0;
	//delay_ms(2);
}


// ********************************************** //
// *** Write a control instruction to the LCD *** //
// ********************************************** //

void LCD_WriteControl (unsigned char CMD)
{
	// while (LCD_Busy()& 0X80);	// Test if LCD busy
	LCD_Write4Control(CMD&0xF0);
	LCD_Write4Control((CMD&0x0F)*16);
	//port_lcd|= 0xF0;
}





// ***************************************** //
// *** Write 1/2 byte of data to the LCD *** //
// ***************************************** //
void LCD_Write4Data (unsigned char Data)

{
	while ((LCD_Busy()&0X80)==0X80); 		// Test if LCD Busy
	CLEAR_LCD_E;	   		// Disable LCD
	port_lcd&= ~0xF0;			// Load data to port
	port_lcd|=Data;
	SET_LCD_WRITE ;			// Set LCD to write
	SET_LCD_DATA;                 // Set LCD to command
	_delay_us(5);
	SET_LCD_E;	   		// Write data to LCD
	_delay_us(3);
	CLEAR_LCD_E;
	_delay_us(5);	   		// Disable LCD
	port_lcd&= ~0xF0;
	//delay_ms(1);
	
}

// ***************************************** //
// *** Write one byte of data to the LCD *** //
// ***************************************** //
void LCD_WriteData (unsigned char Data)

{
	//while (LCD_Busy() & 0X80); 		// Test if LCD Busy
	LCD_Write4Data(Data&0xF0);//Запись старшего полубайта
	LCD_Write4Data((Data&0x0F)*16); //Запись младшего полубайта
	port_lcd|= 0xF0;
}



// ********************************* //
// *** Initialize the LCD driver *** //
// ********************************* //

void Init_LCD(void)
{
	char k;
	for (k=0;k<3;k++)
	{
		_delay_ms(15);
		CLEAR_LCD_E;port_lcd&= ~0xF0;			// Load data to port
		port_lcd|=0x30; SET_LCD_WRITE ;				// Set LCD to write
		SET_LCD_CMD;                 // Set LCD to command
		_delay_us(5);
		SET_LCD_E;	   				// Write data to LCD
		_delay_us(3);
		CLEAR_LCD_E;
		_delay_us(5);	   			// Disable LCD
		port_lcd&= ~0xF0;

	}
	//delay_ms(5);
	LCD_Write4Control (0x20);
	_delay_ms(1);
	LCD_WriteControl (0x2F);  //Разрядность шины 4, кол-во строк 2, шрифт 5x10.
	LCD_Clear();//Очистить дисплей, установить курсор в начальную позицию.
	LCD_WriteControl (0x06);//Сдвиг курсора вправо.
	LCD_WriteControl (0x0E);//Включить дисплей, зажечь курсор.
}



// ************************************************ //
// *** Clear the LCD screen (also homes cursor) *** //
// ************************************************ //
void LCD_Clear(void)
{
	LCD_WriteControl(0x01);
}


// *********************************************** //
// *** Position the LCD cursor at row 1, col 1 *** //
// *********************************************** //

void LCD_Home(void)
{
	LCD_WriteControl(0x02);
}


// ****************************************************************** //
// *** Display a single character, at the current cursor location *** //
// ****************************************************************** //

void LCD_DisplayCharacter (char Char)
{
	LCD_WriteData (Char);
}

void LCD_DisplayCharacterRus (char Char)
{
	if (Char>=0xc0) LCD_WriteData (pgm_read_byte(&ascii_lcd[Char-0xc0]));
	if (Char<0xc0) LCD_WriteData (Char);
}



// ******************************************************************* //
// *** Display a string at the specified row and column, using RAM *** //
// ******************************************************************* //
void LCD_DisplayString (char row, char column , char *string)
{
	LCD_Cursor (row, column);
	while (*string)
	LCD_DisplayCharacter (*string++);
	
}

void LCD_DisplayStringRus (char row, char column , char *string)
{
	LCD_Cursor (row, column);
	while (*string)
	LCD_DisplayCharacterRus (*string++);
	
}



// *************************************************** //
// *** Position the LCD cursor at "row", "column". *** //
// *************************************************** //
void LCD_Cursor (char row, char column)
{
	switch (row) {
		case 1: LCD_WriteControl (0x80 + column - 1); break;
		case 2: LCD_WriteControl (0xc0 + column - 1); break;
		case 3: LCD_WriteControl (0x94 + column - 1); break;
		case 4: LCD_WriteControl (0xd4 + column - 1); break;
		default: break;
	}
}




// ************************** //
// *** Turn the cursor on *** //
// ************************** //
void LCD_Cursor_On (void)
{
	LCD_WriteControl (LCD_CURS_ON);
}


// *************************** //
// *** Turn the cursor off *** //
// *************************** //
void   LCD_Cursor_Off (void)
{
	LCD_WriteControl (LCD_ON);
}


// ******************** //
// *** Turn Off LCD *** //
// ******************** //

void LCD_Display_Off (void)
{
	LCD_WriteControl(LCD_OFF);
}


// ******************* //
// *** Turn On LCD *** //
// ******************* //

void LCD_Display_On (void)
{
	LCD_WriteControl(LCD_ON);
}
// ************************************ //
// ***Вывод строки длиннее 20 символов* //
// ***строка выводится как две строки * //
// ************************************ //

void LCD_DisplayDrawLongString(char *string){
	char i = 0;
	LCD_Cursor(1,1);
	while (*string)
	{
		if(i == NUM_OF_CHARS_PER_STR) LCD_Cursor(2,1);
		LCD_DisplayCharacterRus (*string++);
		i++;
	}
}
// ************************************ //
// ***Вывод строки длиннее 20 символов* //
// ***из памяти программ*************** //
// ***строка выводится как две строки * //
// ************************************ //
void LCD_DisplayFlashLongString(char *str){
	
	char i = 0;
	LCD_Cursor(1,1);
	char string = pgm_read_byte(str);
	while (string)
	{
		if(i == NUM_OF_CHARS_PER_STR) LCD_Cursor(2,1);
		
		LCD_DisplayCharacterRus (string);
		str++;
		string = pgm_read_byte(str);
		i++;
	}
	
}
