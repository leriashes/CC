#pragma once
#include "defs.h"
#include "Semant.h"

struct Operand
{
	LEX lex; //��������. �������
	int number; //����� ������-������
	bool isLink; //�������� �� �������
	bool isConst; //���������
	Tree* node;
};

struct Triada
{
	int operation; //��������
	Operand operand1, operand2; //��������
};
