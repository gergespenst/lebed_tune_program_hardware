/*
 * CMenuItem.h
 *
 * Created: 29.11.2013 10:34:58
 *  Author: 123
 * Файл содержит класс меню для дисплея. Цель - создание унифицированного класса меню.
 * для каждого пункта меню создается экземпляр класса в котором устанавливаются собственные
 * обработчики событий от клавиш и процедура вывода на экран. Возможно следует определить метод
 * класса получающий код нажатой клавиши и производящий ее обработку.
 */ 
/*Размышления.
* возможно стоит разрешенные позиции оформить как структуру в которой будут позиции и их количество
* возможно то же самое следует сделать со списком потомков
*/

#ifndef CMENUITEM_H_
#define CMENUITEM_H_
#include "string.h"
class CMenuItem;
//структура содержащая координаты курсора в меню.
//испульзуется для упрощения функции getCursorPosition
typedef struct SCursorPosition{
	char row;
	char column;
	} SCURSORPOSITION;
	
//структура содержащая разрешенные позиции и количество позиций
typedef struct SPermitedPositions{
	char permitedPositions[21];
	char numOfPermitedPositions;
} SPERMITEDPOSITIONS;

//структура для списка потомков
typedef struct SChildList{
	CMenuItem *pChilds[10];
	char numOfChild;
} SCHILDLIST;

//extern void SendCOMBytes(unsigned char* data,char n);

void noEventMenu(CMenuItem*){
//	item->draw();
//	SendCOMBytes((unsigned char*)"NOP",3);
	};	//функция заглушка, не делает ничего



class CMenuItem{
public:
	CMenuItem *pParent;			//ссылка на предыдущий пункт меню
	CMenuItem *pChild[10];		//ссылка на следующий пункт меню
	
	char numOfChild;			//количество потомков
	
	char  *firstStr;				//первая строка пункта меню
	char *secondStr;			//вторая строка пункта меню
	
	char firstPermitedPosition[21];		//массив разрешенных для установки курсора позиций в первой строке
	char firstNumOfPermitedPosition;	//количество разрешенных в первой строке позиций

	char secondPermitedPosition[21];	//массив разрешенных для установки курсора позиций во второй строке
	char secondNumOfPermitedPosition;	//количество разрешенных во второй строке позиций
	char secondOutputPositions;
	
	unsigned char cursorPosition;	//номер текущей разрешенной позиции курсора, начинается с нуля
	unsigned char cursorRow;			//номер строки
	char numOfPos[2];		//массив с количествами разрешенных позиций дял каждой строки
		
	//TODO: для универсальности меню переписать класс для задаваемого дефайном количества строк
	
	void initClass(char* firstLine, char* secondLine,//общая часть конструкторов
					CMenuItem* parent, CMenuItem* child[] ,char numOfChild,
					void (*okFunc)(CMenuItem*) ,void (*escFunc)(CMenuItem*) ,void (*drawFunc)(CMenuItem*) ,
					void (*leftFunc)(CMenuItem*) ,void (*rightFunc)(CMenuItem*) ,
					void (*upFunc)(CMenuItem*) ,void (*downFunc)(CMenuItem*) ,
					char numOfFirstPermPos , char * firstPermPos ,
					char numOfSecondPermPos , char * secondPermPos );
					
public:
	CMenuItem();
	
	CMenuItem( char* firstLine, char* secondLine,//конструктор с явным указанием всех параметров
				CMenuItem* parent, CMenuItem* child[] ,char numOfChild,
				void (*okFunc)(CMenuItem*),	void (*escFunc)(CMenuItem*),void (*drawFunc)(CMenuItem*),
				void (*leftFunc)(CMenuItem*),void (*rightFunc)(CMenuItem*),
				void (*upFunc)(CMenuItem*),void (*downFunc)(CMenuItem*),
				char numOfFirstPermPos, char * firstPermPos, 
				char numOfSecondPermPos, char * secondPermPos	);
				
	CMenuItem( char* firstLine, char* secondLine,//конструктор из структур
				CMenuItem* parent, CMenuItem* child[] ,char numOfChild,
				void (*okFunc)(CMenuItem*),	void (*escFunc)(CMenuItem*),void (*drawFunc)(CMenuItem*),
				void (*leftFunc)(CMenuItem*),void (*rightFunc)(CMenuItem*),
				void (*upFunc)(CMenuItem*),void (*downFunc)(CMenuItem*),
				SPERMITEDPOSITIONS firstPermit,
				SPERMITEDPOSITIONS secondPermit);
					
	CMenuItem( char* firstLine, char* secondLine,//конструктор из структур
				CMenuItem* parent,
				SCHILDLIST child,
				void (*okFunc)(CMenuItem*),	void (*escFunc)(CMenuItem*),void (*drawFunc)(CMenuItem*),
				void (*leftFunc)(CMenuItem*),void (*rightFunc)(CMenuItem*),
				void (*upFunc)(CMenuItem*),void (*downFunc)(CMenuItem*),
				SPERMITEDPOSITIONS firstPermit,
				SPERMITEDPOSITIONS secondPermit );
					
	~CMenuItem();
	

	
	void setOkFunc(void (*okFunc)(CMenuItem*));		//привязывает функцию реакции на нажатие кнопки OK
	void setEscFunc(void (*escFunc)(CMenuItem*));	//привязывает функцию реакции на нажатие кнопки esc
	
	void setLeftFunc(void (*func)(CMenuItem*));		//привязывает реакцию на нажатие кнопки влево
	void setRightFunc(void (*func)(CMenuItem*));	//привязывает реакцию на нажатие кнопки вправо
	
	void setUpFunc(void (*func)(CMenuItem*));	//привязывает реакцию на нажатие кнопки вверх
	void setDownFunc(void (*func)(CMenuItem*));	//привязывает реакцию на нажатие кнопки вниз
	
	void setDrawFunc(void (*drawFunc)(CMenuItem*));	//привязывает функцию рисования объекта
	
//	void setDisplayData(char* data,char replacedSymbol);
	/*-----указатели на функции----------------------*/
	void (*okFunction)(CMenuItem*);					//нажатие ОК
	void (*escFunction)(CMenuItem*);				//нажатие esc
	void (*leftFunction)(CMenuItem*);				//нажатие влево
	void (*rightFunction)(CMenuItem*);				//нажатие вправо
	void (*upFunction)(CMenuItem*);				//нажатие вверх
	void (*downFunction)(CMenuItem*);				//нажатие вниз
	
	void (*drawItem)(CMenuItem*);					//рисование объекта
	
	/*-----геттеры---------------------------*/
	char* getFirstStr();					//первая строка
	char* getSecondStr();					//вторая строка
	CMenuItem* getParent();					//родитель
	CMenuItem* getChild();					//потомок
	SCURSORPOSITION getCursorPosition();	//позиция курсора на экране
	
	char getCurPos(){return cursorPosition;};			//получение индекса позиции на экране
	void setCurPos(char pos){cursorPosition = pos;};	//утсановка индекса позиции на экране : не безопасно, нет проверок на корректность
	
	char getCurRow(){return cursorRow;};		//получение номера текущей строки
	void setCurRow(char row){cursorRow = row;};	//установка номера текущей строки
	
	void resetCur();
	/*-----функции манипуляции с полями класса-----*/
	void changeRow();	//переход к следующей строке
	
	void incrCurPos();	//функция перехода к следующему положению курсора на экране
	void decrCurPos();	//переход к предыдущему положению курсора на экране
	
	void draw();		//обертка для функции рисования
};

//конструктор по умолчанию. Создает объект со строками нул нул, нопами функций и являющийся потомком и родителем самого себя
CMenuItem::CMenuItem(){
	memcpy(firstStr,"Null",21);
	memcpy(secondStr,"Null",21);
	pParent = this;
	pChild[0] = this;
	firstNumOfPermitedPosition = 0;
	secondNumOfPermitedPosition = 0;
	
	okFunction = noEventMenu;
	escFunction = noEventMenu;
	leftFunction = noEventMenu;
	rightFunction = noEventMenu;
	downFunction = noEventMenu;
	upFunction = noEventMenu;
	
	drawItem = noEventMenu;
}

void 
	CMenuItem::initClass(char* firstLine, char* secondLine,
						CMenuItem* parent, CMenuItem* child[] ,char numOfChilds = 1,
						void (*okFunc)(CMenuItem*) = NULL,	void (*escFunc)(CMenuItem*) = noEventMenu,void (*drawFunc)(CMenuItem*) = noEventMenu,
						void (*leftFunc)(CMenuItem*) = NULL,void (*rightFunc)(CMenuItem*) = noEventMenu,
						void (*upFunc)(CMenuItem*) = NULL,void (*downFunc)(CMenuItem*) = noEventMenu,
						char numOfFirstPermPos = 0, char * firstPermPos = NULL,
						char numOfSecondPermPos = 0, char * secondPermPos = NULL)
			{
				firstStr = firstLine;
				secondStr = secondLine;
				pParent = parent;//pChild(child),
				firstNumOfPermitedPosition = numOfFirstPermPos;
				secondNumOfPermitedPosition = numOfSecondPermPos;
				cursorRow = 1;
				cursorPosition = 0;
				numOfChild = numOfChilds;
				for (unsigned char i = 0; i < numOfChild; i++)
				{
					pChild[i] = child[i];//+sizeof(CMenuItem*)*i;
				}
				
				if (firstNumOfPermitedPosition == 0) firstPermitedPosition[0] = 1;
					else for(unsigned char i = 0; i < firstNumOfPermitedPosition; i++){
						firstPermitedPosition[i] = firstPermPos[i];
					}
				
				if (numOfFirstPermPos == 0) secondPermitedPosition[0] = 1;
					else for(unsigned char i = 0; i < secondNumOfPermitedPosition; i++){
						secondPermitedPosition[i] = secondPermPos[i];
					}
				if (okFunc == NULL) setOkFunc(noEventMenu);
					else setOkFunc(okFunc);
					
				if (drawFunc == NULL) setDrawFunc(noEventMenu);
					else setDrawFunc(drawFunc);
					
				if (escFunc == NULL) setEscFunc(noEventMenu);
					else setEscFunc(escFunc);
				
				if (leftFunc == NULL) setLeftFunc(noEventMenu);
					else setLeftFunc(leftFunc);
								
				if (rightFunc == NULL) setRightFunc(noEventMenu);
					else setRightFunc(rightFunc);
				
				if (upFunc == NULL) setUpFunc(noEventMenu);
					else setUpFunc(upFunc);
					
				if (downFunc == NULL) setDownFunc(noEventMenu);
					else setDownFunc(downFunc);
				//setDownFunc(downFunc);
				//setDrawFunc(drawFunc);
				//setEscFunc(escFunc);
				//setLeftFunc(leftFunc);
				//setRightFunc(rightFunc);
				numOfPos[0] = firstNumOfPermitedPosition;
				numOfPos[1] = secondNumOfPermitedPosition;	
	}

//конструктор задающий все параметры класса
CMenuItem::CMenuItem( char* firstLine, char* secondLine,
					CMenuItem* parent, CMenuItem* child[] ,char numOfChild = 1,
					void (*okFunc)(CMenuItem*) = noEventMenu,	void (*escFunc)(CMenuItem*) = noEventMenu,void (*drawFunc)(CMenuItem*) = noEventMenu,
					void (*leftFunc)(CMenuItem*) = noEventMenu,void (*rightFunc)(CMenuItem*) = noEventMenu,
					void (*upFunc)(CMenuItem*) = NULL,void (*downFunc)(CMenuItem*) = noEventMenu,
					char numOfFirstPermPos = 0, char * firstPermPos = NULL,
					char numOfSecondPermPos = 0, char * secondPermPos = NULL ) 
					{
			initClass(firstLine,secondLine,parent,child,numOfChild,okFunc,escFunc,drawFunc,leftFunc,rightFunc,upFunc,downFunc,
						numOfFirstPermPos,firstPermPos,numOfSecondPermPos,secondPermPos);
};
//конструктор со структурами разрешенных позиций
CMenuItem::CMenuItem( char* firstLine, char* secondLine,
					CMenuItem* parent, CMenuItem* child[] ,char numOfChild = 1,
					void (*okFunc)(CMenuItem*) = noEventMenu,	void (*escFunc)(CMenuItem*) = noEventMenu,void (*drawFunc)(CMenuItem*) = noEventMenu,
					void (*leftFunc)(CMenuItem*) = noEventMenu,void (*rightFunc)(CMenuItem*) = noEventMenu,
					void (*upFunc)(CMenuItem*) = NULL,void (*downFunc)(CMenuItem*) = noEventMenu,
					SPERMITEDPOSITIONS firstPermit = SPERMITEDPOSITIONS(),
					SPERMITEDPOSITIONS secondPermit	= SPERMITEDPOSITIONS())
					{
					initClass(firstLine,secondLine,parent,child,numOfChild,okFunc,escFunc,drawFunc,leftFunc,rightFunc,upFunc,downFunc,
								firstPermit.numOfPermitedPositions,firstPermit.permitedPositions,
								secondPermit.numOfPermitedPositions,secondPermit.permitedPositions);
}
		
CMenuItem::CMenuItem( char* firstLine, char* secondLine,//конструктор из структур
			CMenuItem* parent,// CMenuItem* child[] ,char numOfChild = 1,
			SCHILDLIST child,
			void (*okFunc)(CMenuItem*) = noEventMenu,	void (*escFunc)(CMenuItem*) = noEventMenu,void (*drawFunc)(CMenuItem*) = noEventMenu,
			void (*leftFunc)(CMenuItem*) = noEventMenu,void (*rightFunc)(CMenuItem*) = noEventMenu,
			void (*upFunc)(CMenuItem*) = noEventMenu,void (*downFunc)(CMenuItem*) = noEventMenu,
			SPERMITEDPOSITIONS firstPermit = SPERMITEDPOSITIONS() ,
			SPERMITEDPOSITIONS secondPermit = SPERMITEDPOSITIONS() )	
			{
			initClass(firstLine,secondLine,parent,
						child.pChilds,child.numOfChild,okFunc,escFunc,drawFunc,leftFunc,rightFunc,upFunc,downFunc,
						firstPermit.numOfPermitedPositions,firstPermit.permitedPositions,
						secondPermit.numOfPermitedPositions,secondPermit.permitedPositions);
}
//деструктор. пустой. вписан для порядка. не вызывается никогда в коде		
CMenuItem::~CMenuItem(){

}
		
/*----------геттеры--------------------*/		
char* //возвращает первую строку меню
	CMenuItem::getFirstStr(){
	return firstStr;
}

char* //возвращает вторую строку меню
	CMenuItem::getSecondStr(){
	return secondStr;
}

CMenuItem* //возвращает ссылку на родителя
	CMenuItem::getParent(){
	return pParent;
};

CMenuItem* //аозвращает ссылку на потомка
	CMenuItem::getChild(){
	if (cursorPosition < numOfChild)
	{
		return pChild[cursorPosition];
	}
	else	
		return pChild[0];
};
	
SCURSORPOSITION //возвращает координаты курсора для вывода его на экран. TODO: переписать для случая с произвольным числом строк
	CMenuItem::getCursorPosition(){////////////////////НАМЕК НА ДУБЛИРОВАНИЕ КОДА! НИХАРАШО!
		SCURSORPOSITION pos;
		pos.row = cursorRow;
		if (cursorRow == 1)
			pos.column = firstPermitedPosition[cursorPosition] ;
		if (cursorRow == 2)
			pos.column = secondPermitedPosition[cursorPosition];	
	return pos;
}


/*------------модификаторы полей-------------*/
void //переход к следующему положению курсора
	CMenuItem::incrCurPos(){	
		if (cursorPosition == (numOfPos[cursorRow-1] - 1)) cursorPosition = 0;
			else cursorPosition++;	
}


void //переход к предыдущему положению курсора
	CMenuItem::decrCurPos(){
		if (cursorPosition == 0) cursorPosition = numOfPos[cursorRow - 1] - 1;
			else cursorPosition--;
}


void //переход к следующей строке
	CMenuItem::changeRow(){/////////ДУБЛИРОВАНИЕ КОДА! АХТУНГ! АЙАЙАЙ! НИХАРАШО!!
		
	if(cursorRow == 1)
	{
		if (numOfPos[1] != 0)
		{
			cursorRow = 2;
			cursorPosition = 0;
		}
	}
	else
		if (cursorRow == 2)
		{
			if (numOfPos[0] != 0)
			{
				cursorRow = 1;
				cursorPosition = 0;
			}
		}
		
}

void//сброс курсора в начало координат 
	CMenuItem::resetCur(){
	cursorPosition = 0;
	cursorRow = 1;
}

void //обертка дял рисования объекта
	CMenuItem::draw(){
	drawItem(this);
}
//// ЭТА ФУНКЦИЯ ДОЛЖНА БЫТЬ СНАРУЖИ
//void //установка данных для отображения. Выводит переданный массив на места в строках отмеченные символом replSym
	//CMenuItem::setDisplayData(char data[],char replSym){
		//char dataLen = strlen(data);
		//char s1len = strlen(firstStr);
		//char s2len = strlen(secondStr);
		//SendCOMBytes(data,dataLen);
		//
		//for(char i = 0,dataPos = 0; i < (s1len + s2len); i++)
		//{
//
			//if (dataPos < dataLen)
			//{
				//if (i < s1len)
				//{
					//SendCOMBytes(&firstStr[i] ,1);
					//if (firstStr[i] == replSym)
					//{
						//firstStr[i] = data[dataPos]	;
						//dataPos++ ;
					//}	
				//} else
				//{
					//SendCOMBytes(&dataPos ,1);
					//SendCOMBytes(&data[dataPos] ,1);
					//if (secondStr[i - s1len] == replSym)
					//{
						//secondStr[i - s1len] = data[dataPos];
						//dataPos++ ;
					//}
					//
				//}
				//
			//}
			//
		//}
		//
//}

//void //установка данных для отображения 
	//CMenuItem::setDisplayData(char data[]){
		//char dataLen = strlen(data);
		//char s1len = strlen(firstStr);
		//char s2len = strlen(secondStr);
		//SendCOMBytes(&s1len,1);
		//if ((dataLen >= firstNumOfPermitedPosition - 1)
		//&&(s1len >= firstNumOfPermitedPosition - 1)
		//)
		//{//если длина данных больше или равна количеству разрешенных символов то выводим данные на их места
			//
			//for (char i = 1; i < firstNumOfPermitedPosition; i++)
				//{
					//firstStr[firstPermitedPosition[i] - 1] = data[i-1];
					//SendCOMBytes(&data[i-1],1);
				//}
				//
			//if ((dataLen - firstNumOfPermitedPosition + 1 >= secondNumOfPermitedPosition - 1)
			//&&(s2len >= secondNumOfPermitedPosition - 1)
			//)
				//{
					//for (char i = 1; i < secondNumOfPermitedPosition; i++)
					//{
					//secondStr[secondPermitedPosition[i] - 1] =  data[firstNumOfPermitedPosition + i - 2];
					//
					//SendCOMBytes(&data[firstNumOfPermitedPosition + i - 2],1);
					//}
				//} else 
						//if (s2len < secondNumOfPermitedPosition - 1)
							 //{memcpy(secondStr,"Error length",12);}
//
		//} else
			//memcpy(firstStr,"Error length",12);
//}



/*------установка функций----------------*/		
void //устанавливает функцию на клавишу ОК
	CMenuItem::setOkFunc(void (*okFunc)(CMenuItem*)){
		okFunction = okFunc;
}

void //устанавливает функцию рисования
	CMenuItem::setDrawFunc(void (*drawFunc)(CMenuItem*)){
		drawItem = drawFunc;	
}


void //устанавливает реакцию на нажатие клавиши ескейп
	CMenuItem::setEscFunc(void (*eskFunc)(CMenuItem*)){
		escFunction =  eskFunc;
	}


void //устанавливает реакцию на нажатие клавиши влево
	CMenuItem::setLeftFunc(void (*func)(CMenuItem*)){
	leftFunction =  func;
}

void //устанавливает реакцию на нажатие клавигши вправо
	CMenuItem::setRightFunc(void (*func)(CMenuItem*)){
	rightFunction =  func;
}


void //устанавливает реакцию на нажатие клавигши вверх
	CMenuItem::setUpFunc(void (*func)(CMenuItem*)){
	upFunction =  func;
}
void //устанавливает реакцию на нажатие клавигши вниз
	CMenuItem::setDownFunc(void (*func)(CMenuItem*)){
	downFunction =  func;
}
	
#endif /* CMENUITEM_H_ */