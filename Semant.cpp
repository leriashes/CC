#include "Semant.h"
#include <format>
Tree* Tree::cur = (Tree*)NULL;
Tree* Tree::lastcur = (Tree*)NULL;
TScanner* Tree::scan = (TScanner*)NULL;

Tree::Tree(TScanner* scan)
{
	this->scan = scan;

	node = new Node();
	node->objType = Empty;
	node->data.type = NO_TYPE;

	parent = NULL;
	left = NULL;
	right = NULL;

	cur = this;
	lastcur = this;
}

Tree::Tree(Tree* p, Tree* l, Tree* r, Node* data)
{
	node = new Node();

	parent = p;
	left = l;
	right = r;

	memcpy(node, data, sizeof(Node));

	string id_asm = string(node->id) + "@" + std::format("{}", static_cast<void*>(node));
	memcpy(node->id_asm, id_asm.c_str(), id_asm.size() + 1);
}

void Tree::SetLeft(Node* data)
{
	left = new Tree(this, NULL, NULL, data);
}

void Tree::SetRight(Node* data)
{
	right = new Tree(this, NULL, NULL, data);
}

Tree* Tree::FindUp(Tree* from, LEX id)
{
	Tree* i = from;

	while ((i != NULL) && memcmp(id, i->node->id, max(strlen(i->node->id), strlen(id))) != 0)
		i = i->parent;

	return i;
}

Tree* Tree::FindRoot()
{
	Tree* i = cur;

	while ((i != NULL) && (i->parent != NULL))
		i = i->parent;

	return i;
}

Tree* Tree::FindUp(LEX id)
{
	return FindUp(this, id);
}

Tree* Tree::FindUpOneLevel(Tree* from, LEX id)
{
	Tree* i = from;		//текущая вершина поиска

	while ((i != NULL) && (i->parent == NULL || i->parent->right != i))
	{
		if (memcmp(id, i->node->id, max(strlen(i->node->id), strlen(id))) == 0)
			return i;	//найден совпадающий идентификатор

		i = i->parent;
	}

	return NULL;
}

void Tree::Print()
{
	if (node->objType != Empty)
		printf("Вершина с данными %s (%s) ------>", node->id, DT_Name[node->data.type]);
	else
		printf("Вершина ПУСТАЯ ------>");

	if (left != NULL)
	{
		if (left->node->objType != Empty)
			printf("      слева данные %s", left->node->id);
		else
			printf("      слева ПУСТАЯ вершина");
	}


	if (right != NULL)
	{
		if (right->node->objType != Empty)
			printf("      справа данные %s", right->node->id);
		else
			printf("      справа ПУСТАЯ вершина");
	}

	printf("\n");

	if (left != NULL)
		left->Print();

	if (right != NULL)
	{
		printf("\n\nРОДИТЕЛЬ: ");
		if (node->objType != Empty)
			printf("Вершина %s\n", node->id);
		else
			printf("Вершина ПУСТАЯ\n");
		right->Print();
	}

}

void Tree::SetCur(Tree* a)
{
	cur = a;
}

Tree* Tree::GetCurrentFunct()
{
	if (parent == NULL)
		return NULL;

	if (node->objType == Empty && parent->node->objType == ObjFunct && parent->right == this)
		return parent;

	return parent->GetCurrentFunct();
}

Tree* Tree::GetCur()
{
	return cur;
}

Tree* Tree::SemInclude(LEX a, OBJ_TYPE ot, DATA_TYPE t)
{
	if (!flagInterpret) return NULL;

	if (DupControl(cur, a))
	{
		printf("\n\n\nСЕМАНТИЧЕСКОЕ ДЕРЕВО\n\n");
		Tree* root = FindRoot();
		root->Print();
		printf("\n\n");
		scan->PrintError("Повторное описание идентификатора", a);
	}

	Tree* v;
	Node n;

	memcpy(n.id, a, strlen(a) + 1);
	n.objType = ot;
	n.data.type = t;

	switch (t)
	{
	case TYPE_SHORT:
		n.type = DW;
		break;
	case TYPE_INT:
		n.type = DQ;
		break;
	case TYPE_FLOAT:
		n.type = DD;
		break;
	}

	string id_asm = string(node->id) + "@" + std::format("{}", static_cast<void*>(node));
	memcpy(node->id_asm, id_asm.c_str(), id_asm.size() + 1);

	if (this->node->objType == Empty && this->parent == NULL && this->left == NULL && this->right == NULL)
		memcpy(node, &n, sizeof(Node));
	else
	{
		cur->SetLeft(&n);
		cur = cur->left;
	}

	if (ot == ObjFunct)
	{
		v = cur;
		memcpy(&n.id, &"", 2);
		n.objType = Empty;

		cur->SetRight(&n);
		cur = cur->right;
		return v;
	}

	return cur;
}

Tree* Tree::SemInclude(Tree* first)
{
	if (!flagInterpret) return NULL;

	if (first->node->objType == ObjFunct)
	{
		Tree* v;
		Node n;

		memcpy(n.id, first->node->id, strlen(first->node->id) + 1);
		n.objType = first->node->objType;
		n.data.type = first->node->data.type;
		n.funcStart = first->node->funcStart;

		if (this->node->objType == Empty && this->parent == NULL && this->left == NULL && this->right == NULL)
			memcpy(node, &n, sizeof(Node));
		else
		{
			Tree* lastleft = first->left;
			first->SetLeft(&n);
			lastcur = cur;
			cur = first->left;
			cur->left = lastleft;
		}

		v = cur;
		memcpy(&n.id, &"", 2);
		n.objType = Empty;

		cur->SetRight(&n);
		cur = cur->right;
		return v;
	}
	else
		return NULL;
}

Tree* Tree::SemGetVar(LEX a)
{
	if (!flagInterpret) return NULL;

	Tree* v = FindUp(cur, a);

	if (v == NULL)
	{
		printf("\n\n\nСЕМАНТИЧЕСКОЕ ДЕРЕВО\n\n");
		Tree* root = FindRoot();
		root->Print();
		printf("\n\n");
		scan->PrintError("Отсутствует описание идентификатора", a);
	}

	if (v->node->objType == ObjFunct)
	{
		printf("\n\n\nСЕМАНТИЧЕСКОЕ ДЕРЕВО\n\n");
		Tree* root = FindRoot();
		root->Print();
		printf("\n\n");
		scan->PrintError("Неверное использование имени функции", a);
	}

	return v;
}

Tree* Tree::SemNewLevel()
{
	if (!flagInterpret) return NULL;

	Node n;

	memcpy(&n.id, &"", 2);
	n.objType = Empty;

	cur->SetLeft(&n);
	cur = cur->left;
	Tree* v = cur;

	cur->SetRight(&n);
	cur = cur->right;

	return v;
}

Tree* Tree::SemReturnLevel()
{
	if (!flagInterpret) return NULL;

	if (cur->parent == NULL or cur->parent->right == cur)
	{
		cur = cur->parent;
		return cur;
	}
	
	cur = cur->parent;
	return SemReturnLevel();
}

Tree* Tree::SemGetFunct(LEX a)
{
	if (!flagInterpret) return NULL;

	Tree* v = FindUp(cur, a);

	if (v == NULL)
	{
		printf("\n\n\nСЕМАНТИЧЕСКОЕ ДЕРЕВО\n\n");
		Tree* root = FindRoot();
		root->Print();
		printf("\n\n");
		scan->PrintError("Отсутствует описание функции", a);
	}

	if (v->node->objType != ObjFunct)
	{
		printf("\n\n\nСЕМАНТИЧЕСКОЕ ДЕРЕВО\n\n");
		Tree* root = FindRoot();
		root->Print();
		printf("\n\n");
		scan->PrintError("Идентификатор не является именем функции", a);
	}

	return v;
}

int Tree::DupControl(Tree* addr, LEX a)
{
	if (FindUpOneLevel(addr, a) == NULL) return 0;
	return 1;
}

DATA_TYPE Tree::GetType()
{
	return node->data.type;
}

void Tree::CleanChild()
{
	if (right != NULL)
	{
		right->CleanTree();
		delete right;
		right = NULL;
	}
}

void Tree::CleanTree()
{
	if (left != NULL)
	{
		left->CleanTree();
		delete left;
		left = NULL;
	}

	CleanChild();
}

DATA_TYPE Tree::GetTypebyLex(int lexType)
{
	if (lexType == TShort)
		return TYPE_SHORT;

	if (lexType == TFloat)
		return TYPE_FLOAT;

	return TYPE_INT;
}

void Tree::CheckTypeInt(DATA_TYPE type)
{
	if (type != TYPE_INT && type != TYPE_SHORT)
	{
		scan->PrintError("Выражение должно относится к целочисленному типу");
	}
}

DATA_VALUE* Tree::GetValue()
{
	return &(node->data.value);
}

void Tree::SetStart(int uk, int line, int pos)
{
	node->funcStart.uk = uk;
	node->funcStart.line = line;
	node->funcStart.pos = pos;
}

FStart Tree::GetStart()
{
	return node->funcStart;
}

void Tree::Back()
{
	cur->CleanChild();
	Tree* lastleft = cur->left;
	cur = cur->parent;
	delete cur->left;
	cur->left = lastleft;
	cur = lastcur;
}

void Tree::SetLevel(int level)
{
	node->level = level;

	if (right != NULL)
	{
		right->SetLevel(level + 1);
	}

	if (left != NULL)
	{
		left->SetLevel(level);
	}
}

OBJ_TYPE Tree::GetObjType()
{
	return node->objType;
}

int Tree::GetLevel()
{
	return node->level;
}

Tree* Tree::GetLeft()
{
	return left;
}

Tree* Tree::GetRight()
{
	return right;
}

void Tree::GetAsmId(LEX* id)
{
	memcpy(*id, node->id_asm, strlen(node->id_asm) + 1);
}

string Tree::GetAsmId()
{
	return string(node->id_asm);
}

string Tree::GetId()
{
	return string(node->id);
}

int Tree::GetSize()
{
	return node->type * node->len;
}

string Tree::GenPublicName()
{
	return string(this->node->id_asm) + " ; " + string(this->node->id);
}

string Tree::GenPublicDecl()
{
	string type;
	
	switch (node->type) {
	case DD:
		type = "DD";
		break;
	case DQ:
		type = "DQ";
		break;
	case DW:
		type = "DW";
		break;
	case DB:
		type = "DB";
		break;
	}

	return string(this->node->id_asm) + " " + type + " " + std::format("0{:X}H", node->len) + " DUP(?) ; " + string(this->node->id);
}
