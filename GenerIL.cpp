#include "GenerIL.h"

void GenerIL::generatePublic(Tree* node)
{
	if (node != NULL && node->GetLevel() == 0)
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

void GenerIL::generateFunctions()
{
	for (pc = 0; pc < global->k; pc++)
	{
		if (global->code[pc].operation == procOper)
		{
			Tree* node = global->code[pc].operand1.node;
			int offs = countLocals(node->GetRight()->GetLeft(), 0);

			file << endl << endl << "_TEXT SEGMENT" << endl;
			generateLocals(node->GetRight()->GetLeft(), -offs);
			file << node->GetAsmId() << " PROC" << endl;

			file << "    push ebp" << endl;
			file << "    mov ebp, esp" << endl;
			file << "    and esp, -8" << endl;
			file << "    sub esp, " << offs << endl;
			file << "    push eax" << endl;
			file << "    push ebx" << endl;			
			file << "    push ecx" << endl;
			file << "    push edx" << endl;
			file << "    push esi" << endl;
			file << "    push edi" << endl;

			generateCommands(node->GetAsmId());

			file << "    pop eax" << endl;
			file << "    pop ebx" << endl;
			file << "    pop ecx" << endl;
			file << "    pop edx" << endl;
			file << "    pop esi" << endl;
			file << "    pop edi" << endl;
			file << "    mov esp, epb" << endl;
			file << "    pop ebp" << endl;
			file << "    ret 0" << endl;

			file << node->GetAsmId() << " ENDP" << endl;
			file << "_TEXT ENDS" << endl;
		}
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
			file << node->GetAsmId() << " = " << offs << "        ; size = " << node->GetSize() << endl;
			node->SetOffset(offs);
			offs += node->GetSize();
			generateLocals(node->GetLeft(), offs);
		}
	}
}

void GenerIL::generateCommands(string funcname)
{
	initRegisters();
	set<int> labels;

	for (; pc < global->k; pc++)
	{ 
		Triada* triada = &global->code[pc];

		if (triada->operation == endpOper)
		{
			break;
		}

		string reg;

		switch (triada->operation)
		{
		case epilogOper:
			file << "$LN" << pc + 1 << "@" << funcname << ":" << endl;
			break;

		case TSave:
			file << "$LN" << pc + 1 << "@" << funcname << ":" << endl;
			file << "    mov " << getOperand(triada->operand1) << ", ";
			printSecondOperand(triada);
			file << endl;
			break;

		case TEq:
			file << "$LN" << pc + 1 << "@" << funcname << ":" << endl;
			reg = getFirstOperand(triada);
			file << "    cmp " << reg << ", ";
			printSecondOperand(triada);
			file << "    jne $LN" << global->code[pc + 1].operand2.number + 1 << "@" << funcname << endl;
			labels.insert(global->code[pc + 1].operand2.number);
			file << endl;
			break;

		case TNEq:
			file << "$LN" << pc + 1 << "@" << funcname << ":" << endl;
			reg = getFirstOperand(triada);
			file << "    cmp " << reg << ", ";
			printSecondOperand(triada);
			file << "    je $LN" << global->code[pc + 1].operand2.number + 1 << "@" << funcname << endl;
			labels.insert(global->code[pc + 1].operand2.number);
			file << endl;
			break;

		case TLT:
			file << "$LN" << pc + 1 << "@" << funcname << ":" << endl;
			reg = getFirstOperand(triada);
			file << "    cmp " << reg << ", ";
			printSecondOperand(triada);
			file << "    jge $LN" << global->code[pc + 1].operand2.number + 1 << "@" << funcname << endl;
			labels.insert(global->code[pc + 1].operand2.number);
			file << endl;
			break;

		case TGT:
			file << "$LN" << pc + 1 << "@" << funcname << ":" << endl;
			reg = getFirstOperand(triada);
			file << "    cmp " << reg << ", ";
			printSecondOperand(triada);
			file << "    jle $LN" << global->code[pc + 1].operand2.number + 1 << "@" << funcname << endl;
			labels.insert(global->code[pc + 1].operand2.number);
			file << endl;
			break;

		case TLE:
			file << "$LN" << pc + 1 << "@" << funcname << ":" << endl;
			reg = getFirstOperand(triada);
			file << "    cmp " << reg << ", ";
			printSecondOperand(triada);
			file << "    jg $LN" << global->code[pc + 1].operand2.number + 1 << "@" << funcname << endl;
			labels.insert(global->code[pc + 1].operand2.number);
			file << endl;
			break;

		case TGE:
			file << "$LN" << pc + 1 << "@" << funcname << ":" << endl;
			reg = getFirstOperand(triada);
			file << "    cmp " << reg << ", ";
			printSecondOperand(triada);
			file << "    jl $LN" << global->code[pc + 1].operand2.number + 1 << "@" << funcname << endl;
			labels.insert(global->code[pc + 1].operand2.number);
			file << endl;
			break;

		case TPlus:
			file << "$LN" << pc + 1 << "@" << funcname << ":" << endl;
			reg = getFirstOperand(triada);
			file << "    add "<< reg << ", ";
			printSecondOperand(triada);
			file << endl;
			break;

		case TMinus:
			file << "$LN" << pc + 1 << "@" << funcname << ":" << endl;
			reg = getFirstOperand(triada);
			file << "    sub " << reg << ", ";
			printSecondOperand(triada);
			file << endl;
			break;

		case TMult:
			file << "$LN" << pc + 1 << "@" << funcname << ":" << endl;
			reg = getFirstOperand(triada);
			file << "    imul "<< reg << ", ";
			printSecondOperand(triada);
			file << endl;
			break;

		case TDiv:
			file << "$LN" << pc + 1 << "@" << funcname << ":" << endl;
			reservIntReg("eax");
			reservIntReg("edx");
			file << "    cdq" << endl;

			getFirstOperand(triada);
			freeIntReg("edx");
			file << "    idiv ";
			printSecondOperand(triada);
			file << endl;
			break;

		case TMod:
			file << "$LN" << pc + 1 << "@" << funcname << ":" << endl;
			reservIntReg("eax");
			reservIntReg("edx");
			file << "    cdq" << endl;

			getFirstOperand(triada);
			file << "    idiv ";
			freeIntReg("eax");
			printSecondOperand(triada);
			file << endl;
			break;

		case gotoOper:
			file << "$LN" << pc + 1 << "@" << funcname << ":" << endl;
			file << "    jmp $LN" << triada->operand1.number + 1 << "@" << funcname << endl;
			file << endl;
			break;
		}
		
	}
}

int GenerIL::countLocals(Tree* node, int offs)
{
	int t = 0;

	if (node != NULL)
	{
		if (node->GetObjType() == Empty)
		{
			t += countLocals(node->GetRight()->GetLeft(), offs);
		}
		else
		{
			t = node->GetSize();
			if (offs % node->GetSize() != 0)
			{
				int delta = node->GetSize() - (offs % node->GetSize());
				t += delta;
			}
			offs += t;
			t += countLocals(node->GetLeft(), offs);
		}
	}

	return t;
}

string GenerIL::getFirstOperand(Triada* triada)
{
	string reg;

	if (!triada->operand1.isLink)
	{
		if (triada->operation == TDiv || triada->operation == TMod)
		{
			reg = "eax";
		}
		else
		{
			reg = getIntReg();
		}

		triada->result.nameResult = reg;
		triada->result.flagRegister = true;
		regToTriad[reg] = pc;

		file << "    mov " << reg << ", " << getOperand(triada->operand1) << endl;
	}
	else
	{
		reg = global->code[triada->operand1.number].result.nameResult;
		triada->result.nameResult = reg;
		triada->result.flagRegister = true;
		regToTriad[reg] = pc;
	}

	return reg;
}

void GenerIL::printSecondOperand(Triada* triada)
{
	string reg;

	if (triada->operand2.isLink)
	{
		if (global->code[triada->operand2.number].result.flagRegister)
		{
			reg = global->code[triada->operand2.number].result.nameResult;
			file << reg << endl;
			freeIntReg(reg);
		}
	}
	else
		file << getOperand(triada->operand2) << endl;
}

string GenerIL::getOperand(Operand operand)
{
	if (operand.isConst)
	{
		return operand.lex;
	}
	else if (operand.node->GetLevel() > 0)
	{
		string type;

		switch (operand.node->GetTypeDecl()) {
		case DD:
			type = "DWORD";
			break;
		case DQ:
			type = "QWORD";
			break;
		case DW:
			type = "WORD";
			break;
		case DB:
			type = "BYTE";
			break;
		}

		return type + " PTR " + operand.node->GetAsmId() + "[ebp]";
	}
	else
	{
		return "[" + operand.node->GetAsmId() + "]";
	}
}

void GenerIL::initRegisters()
{
	intReg.clear();
	intReg.push_back({ "eax", false });
	intReg.push_back({ "ebx", false });
	intReg.push_back({ "ecx", false });
	intReg.push_back({ "edx", false });
	intReg.push_back({ "esi", false });
	intReg.push_back({ "edi", false });
}

string GenerIL::getIntReg()
{
	for (auto& reg : intReg) {
		if (!reg.second) {
			reg.second = true;
			return reg.first;
		}
	}

	std::cerr << "Ошибка: не хватает регистров\n";
	exit(1);
}

void GenerIL::freeIntReg(string reg_name)
{
	for (auto& reg : intReg) {
		if (reg.first == reg_name) {
			reg.second = false;
			regToTriad.erase(reg.first);
			return;
		}
	}
}

void GenerIL::reservIntReg(string reg_name)
{
	for (auto& reg : intReg) {
		if (reg.first == reg_name) {
			if (reg.second == true)
			{
				int triadIndex = regToTriad[reg_name];
				string new_reg = getIntReg();
				if (new_reg == "eax" || new_reg == "edx")
				{
					string cl_reg = new_reg;
					new_reg = getIntReg();
					freeIntReg(cl_reg);
				}
				global->code[triadIndex].result.nameResult = new_reg;

				file << "    mov " << new_reg << ", " << reg_name << endl;
			}
			else
			{
				reg.second = true;
			}
			return;
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

void GenerIL::deltaPushConstType()
{
	global->t.push_back(global->constType);
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

	result.node = var;

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

		generateFunctions();

		file.close();
	}
	else {
		std::cerr << "Не удалось открыть файл!" << std::endl;
	}
}
