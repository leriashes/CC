#include "GenerIL.h"

void GenerIL::generatePublic(Tree* node)
{
	if (node->GetLevel() == 0)
	{
		file << "PUBLIC " + node->GenPublicName() << endl;
	}

	if (node->GetLeft() != NULL)
	{
		generatePublic(node->GetLeft());
	}
}

void GenerIL::generateDeclVars(Tree* node)
{
	if (node->GetObjType() == ObjVar && node->GetLevel() == 0)
	{
		file << node->GenPublicDecl() << endl;
	}

	if (node->GetLeft() != NULL)
	{
		generateDeclVars(node->GetLeft());
	}
}

void GenerIL::generateFunctions(Tree* node)
{
	if (node->GetObjType() == ObjFunct && node->GetLevel() == 0)
	{
		file << endl << "_TEXT SEGMENT" << endl;
		generateLocals(node->GetRight()->GetLeft(), 0);
		file << node->GetAsmId() << " PROC" << endl;
		file << node->GetAsmId() << " ENDP" << endl;
		file << "_TEXT ENDS" << endl;
	}

	if (node->GetLeft() != NULL)
	{
		generateFunctions(node->GetLeft());
	}
}

void GenerIL::generateLocals(Tree* node, int offs)
{
	if (node != NULL)
	{
		if (node->GetObjType() == Empty) 
		{
			generateLocals(node->GetRight()->GetLeft(), offs);
		}
		else
		{

			file << node->GetId() << "$";
			if (node->GetLevel() > 2)
				file << node->GetLevel() - 2;
			//offs += node->GetSize();
			if (offs % node->GetSize() != 0)
				offs += node->GetSize() - (offs % node->GetSize());
			offs += node->GetSize();
			file << " = " << offs << endl;
			generateLocals(node->GetLeft(), offs);
		}
	}
}

GenerIL::GenerIL(Tree* root, GlobalData* global)
{
	this->root = root;
	this->global = global;
}

void GenerIL::deltaGener(int operation)
{
	Triada triada;

	triada.operand2 = global->res.back();
	global->res.pop_back();
	global->t.pop_back();

	triada.operand1 = global->res.back();
	global->res.pop_back();

	triada.operation = operation;

	if (global->operation.size() != 1)
	{
		global->res.push_back(currentLink());
	}
	else
	{
		global->t.pop_back();
	}

	global->code[global->k++] = triada;
}

void GenerIL::deltaGenerUnar(int operation)
{
	if (operation == TMinus)
	{
		Triada triada;

		triada.operand2 = global->res.back();
		global->res.pop_back();

		triada.operand1.isConst = true;
		triada.operand1.isLink = false;
		memcpy(triada.operand1.lex, "0", strlen("0") + 1);

		triada.operation = operation;

		if (global->operation.size() != 1)
		{
			global->res.push_back(currentLink());
		}
		else
		{
			global->t.pop_back();
		}

		global->code[global->k++] = triada;
	}
}

void GenerIL::deltaPushType()
{
	global->t.push_back(global->dataType);
}

void GenerIL::deltaPushRes(Operand result)
{
	global->res.push_back(result);
}

void GenerIL::deltaSetAddr()
{
	global->addr.push_back(global->k);
}

void GenerIL::deltaGenerIf()
{
	Triada triada;

	triada.operand1.number = global->k + 1;
	triada.operand1.isLink = true;

	triada.operation = ifOper;

	global->code[global->k++] = triada;
}

void GenerIL::deltaFormIf()
{
	int addr = global->addr.back() - 1;

	while (global->code[addr].operation != ifOper)
	{
		global->addr.pop_back();

		global->code[addr].operand1.number = global->k + 1;
		global->code[addr].operand1.isLink = true;

		addr = global->addr.back() - 1;
	}

	global->addr.pop_back();

	global->code[addr].operand2.number = global->k + 1;
	global->code[addr].operand2.isLink = true;
}

void GenerIL::deltaGenerGoto()
{
	Triada triada;

	int addr = global->addr.back();

	triada.operand1.number = addr;
	triada.operand1.isLink = true;

	triada.operation = gotoOper;

	global->code[global->k++] = triada;
}

void GenerIL::deltaGenerNop()
{
	Triada triada;

	triada.operation = nopOper;

	global->code[global->k++] = triada;
}

void GenerIL::deltaGenerCall()
{
	Triada triada;

	triada.operand1 = global->res.back();
	global->res.pop_back();

	triada.operation = callOper;

	if (global->operation.size() != 1)
	{
		global->res.push_back(currentLink());
	}
	else
	{
		global->t.pop_back();
	}

	global->code[global->k++] = triada;
}

void GenerIL::deltaGenerReturn()
{
	Triada triada;

	triada.operand1 = global->res.back();
	global->res.pop_back();
	global->res.pop_back();
	global->t.pop_back();
	global->t.pop_back();

	triada.operation = returnOper;

	global->code[global->k++] = triada;
}

void GenerIL::deltaGenerBreak()
{
	Triada triada;

	triada.operation = gotoOper;

	global->code[global->k++] = triada;
}

void GenerIL::deltaStartFunc()
{
	Triada triada;

	triada.operand1 = global->res.back();
	triada.operation = procOper;

	global->code[global->k++] = triada;

	triada.operation = prologOper;
	global->code[global->k++] = triada;
}

void GenerIL::deltaEndFunc()
{
	Triada triada;

	triada.operation = epilogOper;
	global->code[global->k++] = triada;

	triada.operation = retOper;
	global->code[global->k++] = triada;

	triada.operation = endpOper;
	global->code[global->k++] = triada;
}

void GenerIL::deltaMatch()
{
	DATA_TYPE second = global->t.back();
	global->t.pop_back();

	DATA_TYPE first = global->t.back();
	global->t.pop_back();

	DATA_TYPE resType = first;

	if (first != second)
	{
		if (first == NO_TYPE || second == NO_TYPE)
			resType = NO_TYPE;
		else if (first == TYPE_FLOAT || second == TYPE_FLOAT)
			resType = TYPE_FLOAT;
		else if (first == TYPE_INT || second == TYPE_INT)
			resType = TYPE_INT;
		else
			resType = TYPE_SHORT;

		if (first != resType)
		{
			int operation = genMatch(first, resType);

			Triada triada;

			Operand operand2 = global->res.back();
			global->res.pop_back();

			triada.operand1 = global->res.back();
			global->res.pop_back();

			global->res.push_back(currentLink());
			global->res.push_back(operand2);

			triada.operation = operation;

			global->code[global->k++] = triada;
		}
		else if (second != resType)
		{
			int operation = genMatch(second, resType);

			Triada triada;

			triada.operand1 = global->res.back();
			global->res.pop_back();

			Operand operand1 = global->res.back();
			global->res.pop_back();

			global->res.push_back(operand1);
			global->res.push_back(currentLink());

			triada.operation = operation;

			global->code[global->k++] = triada;
		}
	}

	global->t.push_back(resType);
	global->t.push_back(resType);
}

void GenerIL::deltaMatchLeft()
{
	DATA_TYPE second = global->t.back();
	global->t.pop_back();

	DATA_TYPE first = global->t.back();
	global->t.pop_back();

	DATA_TYPE resType = first;

	if (first != second)
	{
		int operation = genMatch(second, resType);

		Triada triada;

		triada.operand1 = global->res.back();
		global->res.pop_back();

		Operand operand1 = global->res.back();
		global->res.pop_back();

		global->res.push_back(operand1);
		global->res.push_back(currentLink());

		triada.operation = operation;

		global->code[global->k++] = triada;
	}

	global->t.push_back(resType);
	global->t.push_back(resType);
}

void GenerIL::deltaMatchInt()
{
	DATA_TYPE second = global->t.back();
	global->t.pop_back();

	DATA_TYPE first = global->t.back();
	global->t.pop_back();

	Operand operand2 = global->res.back();

	if (second != TYPE_INT && second != TYPE_SHORT)
	{
		needIntType(operand2);
	}

	global->res.pop_back();

	Operand operand1 = global->res.back();

	if (first != TYPE_INT && first != TYPE_SHORT)
	{
		needIntType(operand1);
	}

	global->res.push_back(operand2);

	DATA_TYPE resType = TYPE_SHORT;

	if (first == TYPE_INT || second == TYPE_INT)
	{
		resType = TYPE_INT;
	}

	if (first != resType)
	{
		int operation = genMatch(first, resType);

		Triada triada;

		global->res.pop_back();

		triada.operand1 = operand1;
		global->res.pop_back();

		global->res.push_back(currentLink());
		global->res.push_back(operand2);

		triada.operation = operation;

		global->code[global->k++] = triada;
	}
	else if (second != resType)
	{
		int operation = genMatch(second, resType);

		Triada triada;

		triada.operand1 = operand2;
		global->res.pop_back();
		global->res.pop_back();

		global->res.push_back(operand1);
		global->res.push_back(currentLink());

		triada.operation = operation;

		global->code[global->k++] = triada;
	}

	global->t.push_back(resType);
	global->t.push_back(resType);
}

int GenerIL::genMatch(DATA_TYPE first, DATA_TYPE result)
{
	int res = 0;

	if (first == TYPE_INT)
	{
		if (result == TYPE_FLOAT)
		{
			res = intToFloat;
		}
		else if (result == TYPE_SHORT)
		{
			res = intToShort;
		}
	}
	else if (first == TYPE_SHORT)
	{
		if (result == TYPE_INT)
		{
			res = shortToInt;
		}
		else if (result == TYPE_FLOAT)
		{
			res = shortToFloat;
		}
	}
	else if (first == TYPE_FLOAT)
	{
		if (result == TYPE_INT)
		{
			res = floatToInt;
		}
		else if (result == TYPE_SHORT)
		{
			res = floatToShort;
		}
	}

	return res;
}

Operand GenerIL::currentLink()
{
	Operand link;

	link.isLink = true;
	link.isConst = false;
	link.number = global->k;

	return link;
}

Operand GenerIL::R(int operand)
{
	Operand result;

	if (operand == 30 || operand == 31)
	{
		result.isConst = true;
		result.isLink = false;
		memcpy(result.lex, global->prevLex, strlen(global->prevLex) + 1);
	}
	else
	{
		result.isConst = false;
		result.isLink = true;
		result.number = global->k - 1;
	}

	return result;
}

Operand GenerIL::R()
{
	Operand result;

	result.isConst = false;
	result.isLink = false;
	memcpy(result.lex, global->prevLex, strlen(global->prevLex) + 1);

	Tree* var = root->FindUp(root->GetCur(), global->prevLex);

	if (var != nullptr)
	{
		var->GetAsmId(&result.lex);
	}

	return result;
}

void GenerIL::saveOperator(int operation)
{
	global->operation.push_back(operation);
}

void GenerIL::needIntType(Operand operand)
{
	if (!operand.isLink)
	{
		root->scan->PrintError("Выражение должно относиться к целочисленному типу", operand.lex);
	}
	else
	{
		root->scan->PrintError("Выражение должно относиться к целочисленному типу");
	}
}

void GenerIL::printTriadaCode()
{
	cout << "\n\nТРИАДЫ\n";

	for (int i = 0; i < global->k; i++)
	{
		Triada triada = global->code[i];

		cout << i + 1 << ") " << operationToSymbols(triada.operation) << " ";

		if (triada.operation < breakOper)
		{
			if (triada.operand1.isLink)
				cout << "(" << triada.operand1.number + 1 << ") ";
			else
				cout << triada.operand1.lex << " ";

			if (triada.operation < intToFloat || triada.operation == ifOper)
			{
				if (triada.operand2.isLink)
					cout << "(" << triada.operand2.number + 1 << ") ";
				else
					cout << triada.operand2.lex << " ";
			}
		}

		cout << endl;
	}
}

string GenerIL::operationToSymbols(int operation)
{
	switch (operation)
	{
	case TSave:
		return "=";

	case TEq:
		return "==";

	case TNEq:      
		return "!=";

	case TLT:       
		return "<";

	case TGT:       
		return ">";

	case TLE:       
		return "<=";

	case TGE:       
		return ">=";

	case TLShift:   
		return "<<";

	case TRShift:   
		return ">>";

	case TPlus:    
		return "+";

	case TMinus:    
		return "-";

	case TMult:    
		return "*";

	case TDiv:     
		return "/";

	case TMod:     
		return "%";

	case intToFloat:
		return "i->f";

	case shortToInt:
		return "s->i";

	case shortToFloat:
		return "s->f";

	case intToShort:
		return "i->s";

	case floatToInt:
		return "f->i";

	case floatToShort:
		return "f->s";

	case ifOper:
		return "if";

	case gotoOper:
		return "goto";

	case nopOper:
		return "nop";

	case callOper:
		return "call";

	case procOper:
		return "proc";

	case prologOper:
		return "prolog";

	case epilogOper:
		return "epilog";

	case retOper:
		return "ret";

	case endpOper:
		return "endp";

	case returnOper:
		return "return";

	case breakOper:
		return "break";

	default:        
		return "UNKNOWN_OP";
	}
}

void GenerIL::generateCode()
{
	int level = 0;
	root->SetLevel(level);

	file = ofstream("prog.asm");

	if (file.is_open()) {
		file << "_BSS SEGMENT" << endl;

		generatePublic(root);

		file << endl;

		generateDeclVars(root);

		file << endl;

		generateFunctions(root);

		file.close();
	}
	else {
		std::cerr << "Не удалось открыть файл!" << std::endl;
	}
}
