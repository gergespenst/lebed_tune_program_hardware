/*
 * CMenuItem.h
 *
 * Created: 29.11.2013 10:34:58
 *  Author: 123
 * ���� �������� ����� ���� ��� �������. ���� - �������� ���������������� ������ ����.
 * ��� ������� ������ ���� ��������� ��������� ������ � ������� ��������������� �����������
 * ����������� ������� �� ������ � ��������� ������ �� �����. �������� ������� ���������� �����
 * ������ ���������� ��� ������� ������� � ������������ �� ���������.
 */ 
/*�����������.
* �������� ����� ����������� ������� �������� ��� ��������� � ������� ����� ������� � �� ����������
* �������� �� �� ����� ������� ������� �� ������� ��������
*/

#ifndef CMENUITEM_H_
#define CMENUITEM_H_
#include "string.h"
class CMenuItem;
//��������� ���������� ���������� ������� � ����.
//������������ ��� ��������� ������� getCursorPosition
typedef struct SCursorPosition{
	char row;
	char column;
	} SCURSORPOSITION;
	
//��������� ���������� ����������� ������� � ���������� �������
typedef struct SPermitedPositions{
	char permitedPositions[21];
	char numOfPermitedPositions;
} SPERMITEDPOSITIONS;

//��������� ��� ������ ��������
typedef struct SChildList{
	CMenuItem *pChilds[10];
	char numOfChild;
} SCHILDLIST;

//extern void SendCOMBytes(unsigned char* data,char n);

void noEventMenu(CMenuItem*){
//	item->draw();
//	SendCOMBytes((unsigned char*)"NOP",3);
	};	//������� ��������, �� ������ ������



class CMenuItem{
public:
	CMenuItem *pParent;			//������ �� ���������� ����� ����
	CMenuItem *pChild[10];		//������ �� ��������� ����� ����
	
	char numOfChild;			//���������� ��������
	
	char  *firstStr;				//������ ������ ������ ����
	char *secondStr;			//������ ������ ������ ����
	
	char firstPermitedPosition[21];		//������ ����������� ��� ��������� ������� ������� � ������ ������
	char firstNumOfPermitedPosition;	//���������� ����������� � ������ ������ �������

	char secondPermitedPosition[21];	//������ ����������� ��� ��������� ������� ������� �� ������ ������
	char secondNumOfPermitedPosition;	//���������� ����������� �� ������ ������ �������
	char secondOutputPositions;
	
	unsigned char cursorPosition;	//����� ������� ����������� ������� �������, ���������� � ����
	unsigned char cursorRow;			//����� ������
	char numOfPos[2];		//������ � ������������ ����������� ������� ��� ������ ������
		
	//TODO: ��� ��������������� ���� ���������� ����� ��� ����������� �������� ���������� �����
	
	void initClass(char* firstLine, char* secondLine,//����� ����� �������������
					CMenuItem* parent, CMenuItem* child[] ,char numOfChild,
					void (*okFunc)(CMenuItem*) ,void (*escFunc)(CMenuItem*) ,void (*drawFunc)(CMenuItem*) ,
					void (*leftFunc)(CMenuItem*) ,void (*rightFunc)(CMenuItem*) ,
					void (*upFunc)(CMenuItem*) ,void (*downFunc)(CMenuItem*) ,
					char numOfFirstPermPos , char * firstPermPos ,
					char numOfSecondPermPos , char * secondPermPos );
					
public:
	CMenuItem();
	
	CMenuItem( char* firstLine, char* secondLine,//����������� � ����� ��������� ���� ����������
				CMenuItem* parent, CMenuItem* child[] ,char numOfChild,
				void (*okFunc)(CMenuItem*),	void (*escFunc)(CMenuItem*),void (*drawFunc)(CMenuItem*),
				void (*leftFunc)(CMenuItem*),void (*rightFunc)(CMenuItem*),
				void (*upFunc)(CMenuItem*),void (*downFunc)(CMenuItem*),
				char numOfFirstPermPos, char * firstPermPos, 
				char numOfSecondPermPos, char * secondPermPos	);
				
	CMenuItem( char* firstLine, char* secondLine,//����������� �� ��������
				CMenuItem* parent, CMenuItem* child[] ,char numOfChild,
				void (*okFunc)(CMenuItem*),	void (*escFunc)(CMenuItem*),void (*drawFunc)(CMenuItem*),
				void (*leftFunc)(CMenuItem*),void (*rightFunc)(CMenuItem*),
				void (*upFunc)(CMenuItem*),void (*downFunc)(CMenuItem*),
				SPERMITEDPOSITIONS firstPermit,
				SPERMITEDPOSITIONS secondPermit);
					
	CMenuItem( char* firstLine, char* secondLine,//����������� �� ��������
				CMenuItem* parent,
				SCHILDLIST child,
				void (*okFunc)(CMenuItem*),	void (*escFunc)(CMenuItem*),void (*drawFunc)(CMenuItem*),
				void (*leftFunc)(CMenuItem*),void (*rightFunc)(CMenuItem*),
				void (*upFunc)(CMenuItem*),void (*downFunc)(CMenuItem*),
				SPERMITEDPOSITIONS firstPermit,
				SPERMITEDPOSITIONS secondPermit );
					
	~CMenuItem();
	

	
	void setOkFunc(void (*okFunc)(CMenuItem*));		//����������� ������� ������� �� ������� ������ OK
	void setEscFunc(void (*escFunc)(CMenuItem*));	//����������� ������� ������� �� ������� ������ esc
	
	void setLeftFunc(void (*func)(CMenuItem*));		//����������� ������� �� ������� ������ �����
	void setRightFunc(void (*func)(CMenuItem*));	//����������� ������� �� ������� ������ ������
	
	void setUpFunc(void (*func)(CMenuItem*));	//����������� ������� �� ������� ������ �����
	void setDownFunc(void (*func)(CMenuItem*));	//����������� ������� �� ������� ������ ����
	
	void setDrawFunc(void (*drawFunc)(CMenuItem*));	//����������� ������� ��������� �������
	
//	void setDisplayData(char* data,char replacedSymbol);
	/*-----��������� �� �������----------------------*/
	void (*okFunction)(CMenuItem*);					//������� ��
	void (*escFunction)(CMenuItem*);				//������� esc
	void (*leftFunction)(CMenuItem*);				//������� �����
	void (*rightFunction)(CMenuItem*);				//������� ������
	void (*upFunction)(CMenuItem*);				//������� �����
	void (*downFunction)(CMenuItem*);				//������� ����
	
	void (*drawItem)(CMenuItem*);					//��������� �������
	
	/*-----�������---------------------------*/
	char* getFirstStr();					//������ ������
	char* getSecondStr();					//������ ������
	CMenuItem* getParent();					//��������
	CMenuItem* getChild();					//�������
	SCURSORPOSITION getCursorPosition();	//������� ������� �� ������
	
	char getCurPos(){return cursorPosition;};			//��������� ������� ������� �� ������
	void setCurPos(char pos){cursorPosition = pos;};	//��������� ������� ������� �� ������ : �� ���������, ��� �������� �� ������������
	
	char getCurRow(){return cursorRow;};		//��������� ������ ������� ������
	void setCurRow(char row){cursorRow = row;};	//��������� ������ ������� ������
	
	void resetCur();
	/*-----������� ����������� � ������ ������-----*/
	void changeRow();	//������� � ��������� ������
	
	void incrCurPos();	//������� �������� � ���������� ��������� ������� �� ������
	void decrCurPos();	//������� � ����������� ��������� ������� �� ������
	
	void draw();		//������� ��� ������� ���������
};

//����������� �� ���������. ������� ������ �� �������� ��� ���, ������ ������� � ���������� �������� � ��������� ������ ����
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

//����������� �������� ��� ��������� ������
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
//����������� �� ����������� ����������� �������
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
		
CMenuItem::CMenuItem( char* firstLine, char* secondLine,//����������� �� ��������
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
//����������. ������. ������ ��� �������. �� ���������� ������� � ����		
CMenuItem::~CMenuItem(){

}
		
/*----------�������--------------------*/		
char* //���������� ������ ������ ����
	CMenuItem::getFirstStr(){
	return firstStr;
}

char* //���������� ������ ������ ����
	CMenuItem::getSecondStr(){
	return secondStr;
}

CMenuItem* //���������� ������ �� ��������
	CMenuItem::getParent(){
	return pParent;
};

CMenuItem* //���������� ������ �� �������
	CMenuItem::getChild(){
	if (cursorPosition < numOfChild)
	{
		return pChild[cursorPosition];
	}
	else	
		return pChild[0];
};
	
SCURSORPOSITION //���������� ���������� ������� ��� ������ ��� �� �����. TODO: ���������� ��� ������ � ������������ ������ �����
	CMenuItem::getCursorPosition(){////////////////////����� �� ������������ ����! ��������!
		SCURSORPOSITION pos;
		pos.row = cursorRow;
		if (cursorRow == 1)
			pos.column = firstPermitedPosition[cursorPosition] ;
		if (cursorRow == 2)
			pos.column = secondPermitedPosition[cursorPosition];	
	return pos;
}


/*------------������������ �����-------------*/
void //������� � ���������� ��������� �������
	CMenuItem::incrCurPos(){	
		if (cursorPosition == (numOfPos[cursorRow-1] - 1)) cursorPosition = 0;
			else cursorPosition++;	
}


void //������� � ����������� ��������� �������
	CMenuItem::decrCurPos(){
		if (cursorPosition == 0) cursorPosition = numOfPos[cursorRow - 1] - 1;
			else cursorPosition--;
}


void //������� � ��������� ������
	CMenuItem::changeRow(){/////////������������ ����! ������! ������! ��������!!
		
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

void//����� ������� � ������ ��������� 
	CMenuItem::resetCur(){
	cursorPosition = 0;
	cursorRow = 1;
}

void //������� ��� ��������� �������
	CMenuItem::draw(){
	drawItem(this);
}
//// ��� ������� ������ ���� �������
//void //��������� ������ ��� �����������. ������� ���������� ������ �� ����� � ������� ���������� �������� replSym
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

//void //��������� ������ ��� ����������� 
	//CMenuItem::setDisplayData(char data[]){
		//char dataLen = strlen(data);
		//char s1len = strlen(firstStr);
		//char s2len = strlen(secondStr);
		//SendCOMBytes(&s1len,1);
		//if ((dataLen >= firstNumOfPermitedPosition - 1)
		//&&(s1len >= firstNumOfPermitedPosition - 1)
		//)
		//{//���� ����� ������ ������ ��� ����� ���������� ����������� �������� �� ������� ������ �� �� �����
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



/*------��������� �������----------------*/		
void //������������� ������� �� ������� ��
	CMenuItem::setOkFunc(void (*okFunc)(CMenuItem*)){
		okFunction = okFunc;
}

void //������������� ������� ���������
	CMenuItem::setDrawFunc(void (*drawFunc)(CMenuItem*)){
		drawItem = drawFunc;	
}


void //������������� ������� �� ������� ������� ������
	CMenuItem::setEscFunc(void (*eskFunc)(CMenuItem*)){
		escFunction =  eskFunc;
	}


void //������������� ������� �� ������� ������� �����
	CMenuItem::setLeftFunc(void (*func)(CMenuItem*)){
	leftFunction =  func;
}

void //������������� ������� �� ������� �������� ������
	CMenuItem::setRightFunc(void (*func)(CMenuItem*)){
	rightFunction =  func;
}


void //������������� ������� �� ������� �������� �����
	CMenuItem::setUpFunc(void (*func)(CMenuItem*)){
	upFunction =  func;
}
void //������������� ������� �� ������� �������� ����
	CMenuItem::setDownFunc(void (*func)(CMenuItem*)){
	downFunction =  func;
}
	
#endif /* CMENUITEM_H_ */