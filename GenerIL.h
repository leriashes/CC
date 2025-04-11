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
	void deltaPushType();
	void deltaPushRes(Operand result);

	Operand R(int operand);
	Operand R();
};
