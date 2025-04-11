#pragma once
#include "Semant.h"

struct GlobalData
{
	LEX prevLex;
	DATA_TYPE dataType;
};

class Translate
{
	Tree* root;
	GlobalData* global;

public:
	Translate(Tree* root, GlobalData* global);
	~Translate();
	void SaveLex(LEX lex);

	void deltaGetVar();
	void deltaStartDecl(int lexType);
	void deltaSetIdent();
	void deltaSetFunct();
	void deltaReturnLevel();
	void deltaSetNewLevel();
	void deltaGetFunct();
};

