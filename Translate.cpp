#include "Translate.h"

Translate::Translate(TScanner* scan)
{
	this->root = new Tree(scan);
	this->global = new GlobalData();
}

Translate::~Translate()
{
	delete root;
	delete global;
}

void Translate::PrintTree()
{
	root->Print();
}

void Translate::CleanTree()
{
	root->CleanTree();
}

void Translate::SaveLex(LEX lex)
{
	memcpy(global->prevLex, lex, strlen(lex) + 1);
}

void Translate::deltaGetVar()
{
	root->SemGetVar(global->prevLex);
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
