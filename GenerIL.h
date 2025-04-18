#pragma once
#include "Triada.h"
#include "Semant.h"
#include "Translate.h"
#include <deque>

class GenerIL
{
private:
	Tree* root;
	GlobalData* global;

public:
	GenerIL(Tree* root, GlobalData* global);

	void deltaGener(int operation);
	void deltaGenerUnar(int operation);
	void deltaPushType();
	void deltaPushRes(Operand result);
	void deltaSetAddr();
	void deltaGenerIf();
	void deltaFormIf();
	void deltaGenerGoto();
	void deltaGenerNop();
	void deltaGenerCall();
	void deltaGenerReturn();
	void deltaGenerBreak();
	void deltaStartFunc();
	void deltaEndFunc();
	void deltaMatch();
	void deltaMatchLeft();
	void deltaMatchInt();

	int genMatch(DATA_TYPE first, DATA_TYPE result);
	Operand currentLink();

	Operand R(int operand);
	Operand R();

	void saveOperator(int operation);

	void needIntType(Operand operand);
	void printTriadaCode();
	string operationToSymbols(int operation);
};
