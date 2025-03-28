#pragma once
#include "Scanner.h"
#include "Semant.h"

class LL1
{
private:
	void epsilon();

	int mag[5000];	//������� LL(1)-�����������
	int z;			//��������� ��������

public:
	static TScanner* scan;
	Tree* root;

	LL1(TScanner* scan);
	~LL1();
	int LL_1();
	void PrintTree();
};
