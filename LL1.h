#pragma once
#include "Scanner.h"

class LL1
{
private:
	void epsilon();

	int mag[5000];	//������� LL(1)-�����������
	int z;			//��������� ��������

public:
	static TScanner* scan;

	LL1(TScanner* scan);
	int LL_1();
};
