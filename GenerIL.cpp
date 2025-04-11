#include "GenerIL.h"

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

void GenerIL::deltaPushType()
{
	global->t.push_back(global->dataType);
}

void GenerIL::deltaPushRes(Operand result)
{
	global->res.push_back(result);
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

	return result;
}

void GenerIL::saveOperator(int operation)
{
	global->operation.push_back(operation);
}

void GenerIL::printTriadaCode()
{
	cout << "\n\n������\n";

	for (int i = 0; i < global->k; i++)
	{
		Triada triada = global->code[i];

		cout << i + 1 << ") " << operationToSymbols(triada.operation) << " ";

		if (triada.operand1.isLink)
			cout << "(" << triada.operand1.number + 1 << ") ";
		else
			cout << triada.operand1.lex << " ";

		if (triada.operation < 2100)
		{
			if (triada.operand2.isLink)
				cout << "(" << triada.operand2.number + 1 << ") ";
			else
				cout << triada.operand2.lex << " ";
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

	default:        
		return "UNKNOWN_OP";
	}
}
