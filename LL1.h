#pragma once
#include "Scanner.h"
#include "Translate.h"

class LL1
{
private:
	void epsilon();

	int mag[5000];	//магазин LL(1)-анализатора
	int z;			//указатель магазина

public:
	static TScanner* scan;
	Translate* translate;

	LL1(TScanner* scan);
	~LL1();
	int LL_1();
	void PrintTree();
};
