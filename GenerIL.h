#pragma once
#include "Triada.h"
#include "Semant.h"
#include "Translate.h"
#include <deque>
#include <vector>

typedef pair<string, bool> RegInfo;

class GenerIL
{
private:
	Tree* root;
	GlobalData* global;
	ofstream file;
	int pc;

	vector<RegInfo> intReg;


	void generatePublic(Tree* node);
	void generateDeclVars(Tree* node);
	void generateFunctions();
	void generateLocals(Tree* node, int offs);
	void generateCommands();
	int countLocals(Tree* node, int offs);

	string getOperand(Operand operand);

	void initRegisters();
	string getIntReg();
	void freeIntReg(string reg_name);


public:
	GenerIL(Tree* root, GlobalData* global);

	void deltaGener(int operation);
	void deltaGenerUnar(int operation);
	void deltaPushType();
	void deltaPushConstType();
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

	void generateCode();
};
