#pragma once
#include "defs.h"

struct Operand
{
	LEX lex; //��������. �������
	int number; //����� ������-������
	bool isLink; //�������� �� �������
	bool isConst; //���������
};

struct Triada
{
	LEX operation; //��������
	Operand operand1, operand2; //��������
};
