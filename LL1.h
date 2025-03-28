#pragma once
#include "Scanner.h"
#include "Semant.h"

class LL1
{
private:
	void epsilon();

	int mag[5000];	//магазин LL(1)-анализатора
	int z;			//указатель магазина

public:
	static TScanner* scan;
	Tree* root;

	LL1(TScanner* scan);
	~LL1();
	int LL_1();
	void PrintTree();
};
