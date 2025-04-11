#include "Translate.h"

Translate::Translate(Tree* root, GlobalData* global)
{
	this->root = root;
	this->global = global;
}

Translate::~Translate()
{
	delete root;
	delete global;
}

void Translate::SaveLex(LEX lex)
{
	memcpy(global->prevLex, lex, strlen(lex) + 1);
}

void Translate::deltaGetVar()
{
	global->dataType = root->SemGetVar(global->prevLex)->GetType();
}

void Translate::deltaStartDecl(int lexType)
{
	global->dataType = root->GetTypebyLex(lexType);
}

void Translate::deltaSetIdent()
{
	root->SemInclude(global->prevLex, ObjVar, global->dataType);
}

void Translate::deltaSetFunct()
{
	root->SemInclude(global->prevLex, ObjFunct, global->dataType);
}

void Translate::deltaReturnLevel()
{
	root->SemReturnLevel();
}

void Translate::deltaSetNewLevel()
{
	root->SemNewLevel();
}

void Translate::deltaGetFunct()
{
	root->SemGetFunct(global->prevLex);
}

void Translate::deltaConstType(int constTerm)
{
	if (constTerm == TConstInt)
		global->dataType = TYPE_INT;
	else
		global->dataType = TYPE_FLOAT;
}
