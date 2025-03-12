#pragma once
#include "Scanner.h"

class LL1
{
private:
	void epsilon();

	int mag[5000];	//магазин LL(1)-анализатора
	int z;			//указатель магазина

public:
	static TScanner* scan;

	LL1(TScanner* scan);
	int LL_1();
};
