#pragma once
#include "Scanner.h"
#include "Translate.h"
#include "GenerIL.h"

class LL1
{
private:
	void epsilon();

	int mag[5000];	//������� LL(1)-�����������
	int z;			//��������� ��������

public:
	static TScanner* scan;
	Translate* translate;
	Tree* root;
	GenerIL* genIL;
	GlobalData* global;

	LL1(TScanner* scan);
	~LL1();
	int LL_1();
	void PrintTree();
	void PrintTriada();

	void GenCode();
};
