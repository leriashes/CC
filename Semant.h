#pragma once
#include "defs.h"
#include "Scanner.h"

enum OBJ_TYPE {
	Empty = 0,
	ObjVar,		//переменная
	ObjFunct	//функция
};	

enum DATA_TYPE { NO_TYPE = 0, TYPE_INT, TYPE_SHORT, TYPE_FLOAT };

enum TYPE_DECL {DQ = 8, DD = 4, DW = 2, DB = 1 };

union DATA_VALUE {
	short DataAsShort;
	int DataAsInt;
	float DataAsFloat;
};

struct NData {
	DATA_TYPE type;		//тип значения
	DATA_VALUE value;	//значение
};

struct FStart {
	int uk;
	int line;
	int pos;
};

struct Node
{
	OBJ_TYPE objType;	//тип объекта
	LEX id;				//идентификатор объекта
	LEX id_asm;			//уникальный идентификатор
	NData data;			//тип значения и само значение
	FStart funcStart;	//начало функции

	TYPE_DECL type;		//декларируемый тип
	int len = 1;		//длина в единицах TYPE_DECL
	int level;			//уровень вложенности
	int stackAddr;		//смещение в стеке
};

class Tree			//элемент семантической таблицы
{
	LEX DT_Name[4] = { "неопр.", "int", "short", "float" };

private:
	Node* node;							//информация об объекте таблицы
	Tree* parent, * left, * right;		//родитель, левый и правый потомки

public:
	static TScanner* scan;
	static Tree* cur;		//текущий элемент дерева
	static Tree* lastcur;

	bool flagInterpret = true;
	bool flagReturn = false;

	Tree(Tree* p, Tree* l, Tree* r, Node* data);
	Tree(TScanner* scan);

	//Функции обработки бинарного дерева
	void SetLeft(Node* data);
	void SetRight(Node* data);

	Tree* FindRoot();
	Tree* FindUp(Tree* from, LEX id);
	Tree* FindUp(LEX id);
	Tree* FindUpOneLevel(Tree* from, LEX id);

	Tree* GetCurrentFunct();

	void Print();
	void CleanTree();
	void CleanChild();

	//Семантичексие подпрограммы
	void SetCur(Tree* a);			//установить текущий узел дерева
	Tree* GetCur();					//получить ссылку на текущий узел дерева
	Tree* SemInclude(LEX a, OBJ_TYPE ot, DATA_TYPE t);		//занесение идентификатора a в таблицу с типом t
	Tree* SemInclude(Tree* first);
	Tree* SemNewLevel();			//новый уровень (для составного оператора)
	Tree* SemReturnLevel();
	Tree* SemGetVar(LEX a);		//найти в таблице переменную с именем a и вернуть ссылку на соответсвующий элемент дерева
	Tree* SemGetFunct(LEX a);		//найти в таблице функцию с именем a и вернуть ссылку на соответсвующий элемент дерева
	int DupControl(Tree* addr, LEX a);			//проверка идентификатора a на повторное описание внутри блока

	void CheckTypeInt(DATA_TYPE type);

	DATA_TYPE GetType();
	DATA_TYPE GetTypebyLex(int lexType);

	DATA_VALUE* GetValue();

	void SetStart(int uk, int line, int pos);
	FStart GetStart();
	void Back();

	void SetLevel(int level);

	OBJ_TYPE GetObjType();
	int GetLevel();
	Tree* GetLeft();
	string GenPublicName();
};
