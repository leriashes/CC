#pragma once
#include <deque>
#include "Scanner.h"
#include "Triada.h"

#define MAXCODE 1000

struct GlobalData
{
	LEX prevLex;
	DATA_TYPE dataType;
	int operat;

	int k;
	deque<DATA_TYPE> t;
	Triada code[MAXCODE];
	deque<Operand> res;
	deque<int> addr;
};
