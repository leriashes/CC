#include "LL1.h"

TScanner* LL1::scan = (TScanner*)NULL;

void LL1::epsilon()	//обработка правила с пустой правой частью
{
	;
}

int LL1::LL_1() //функция синтаксического анализатора
{
	int t, fl = 1;
	LEX l;

	mag[z] = neterm_S;
	t = scan->Scanner(l);

	while (fl)
	{
		if (mag[z] <= MaxTypeTerminal)	//в верхушке магазина терминал
		{
			if (mag[z] == t)
			{
				if (t == TEnd)
				{
					fl = 0;
				}
				else
				{
					t = scan->Scanner(l);
					//z--;
				}
			}
			else
			{
				scan->PrintError("Неверный символ", l);
				return -1;
			}
		}
		else	//в верхушке магазина нетерминал
		{
			switch (mag[z])
			{
			case neterm_S:
				// S -> T O S | eps
				if (t == TEnd)
				{
					mag[z++] = TEnd;
				}
				else
				{
					mag[z++] = neterm_S;
					mag[z++] = neterm_O;
					mag[z++] = neterm_T;
				}

				break;

			case neterm_O:
				// O -> a D | main F
				if (t == TIdent)
				{
					mag[z++] = neterm_D;
					mag[z++] = TIdent;
				}
				else
				{
					mag[z++] = neterm_F;
					mag[z++] = TMain;
				}

				break;

			case neterm_D:
				// D -> X ; | F
				if (t == TLS)
				{
					mag[z++] = neterm_F;
				}
				else
				{
					mag[z++] = TSemicolon;
					mag[z++] = neterm_X;
				}

				break;

			case neterm_T:
				// T -> int | short I | long I | float
				if (t == TInt || t == TFloat)
				{
					mag[z++] = t;
				}
				else
				{
					mag[z++] = neterm_I;
					mag[z++] = t;
				}

				break;

			case neterm_I:
				// I -> int | eps
				if (t == TInt)
				{
					mag[z++] = TInt;
				}
				else
				{
					epsilon();
				}

				break;

			case neterm_X:
				// X -> E X1
				mag[z++] = neterm_X1;
				mag[z++] = neterm_E;
				break;

			case neterm_X1:
				// X1 -> , a E X1 | eps
				if (t == TComma)
				{
					mag[z++] = neterm_X1;
					mag[z++] = neterm_E;
					mag[z++] = TIdent;
					mag[z++] = TComma;
				}
				else
				{
					epsilon();
				}

				break;

			case neterm_E:
				// E -> = V | eps
				if (t == TSave)
				{
					mag[z++] = neterm_V;
					mag[z++] = TSave;
				}
				else
				{
					epsilon();
				}

				break;

			case neterm_F:
				// F -> ( ) Q
				mag[z++] = neterm_Q;
				mag[z++] = TRS;
				mag[z++] = TLS;
				break;

			case neterm_Q:
				// Q -> { U }
				mag[z++] = TFRS;
				mag[z++] = neterm_U;
				mag[z++] = TFLS;
				break;

			case neterm_U:
				// U -> T a X ; U | A U | eps
				if (t == TFRS)
				{
					epsilon();
				}
				else if (t == TInt || t == TShort || t == TLong || t == TFloat)
				{
					mag[z++] = neterm_U;
					mag[z++] = TSemicolon;
					mag[z++] = neterm_X;
					mag[z++] = TIdent;
					mag[z++] = neterm_T;
				}
				else
				{
					mag[z++] = neterm_U;
					mag[z++] = neterm_A;
				}

				break;

			case neterm_A:
				// A -> a P ; | Q | W | ; | R ; | B ; | main ( ) ;
				switch (t)
				{
				case TIdent:
					mag[z++] = TSemicolon;
					mag[z++] = neterm_P;
					mag[z++] = TIdent;
					break;

				case TFLS:
					mag[z++] = neterm_Q;
					break;

				case TWhile:
					mag[z++] = neterm_W;
					break;

				case TSemicolon:
					mag[z++] = TSemicolon;
					break;

				case TReturn:
					mag[z++] = TSemicolon;
					mag[z++] = neterm_R;
					break;

				case TBreak:
					mag[z++] = TSemicolon;
					mag[z++] = neterm_B;
					break;

				case TMain:
					mag[z++] = TSemicolon;
					mag[z++] = TRS;
					mag[z++] = TLS;
					mag[z++] = TMain;
					break;
				}
				break;

			case neterm_R:
				// R -> return V
				mag[z++] = neterm_V;
				mag[z++] = TReturn;
				break;

			case neterm_B:
				// B -> break
				mag[z++] = TBreak;
				break;

			case neterm_W:
				// W -> while ( V ) A
				mag[z++] = neterm_A;
				mag[z++] = TRS;
				mag[z++] = neterm_V;
				mag[z++] = TLS;
				mag[z++] = TWhile;
				break;

			case neterm_P:
				// P -> = V | ( )
				if (t == TSave)
				{
					mag[z++] = neterm_V;
					mag[z++] = TSave;
				}
				else
				{
					mag[z++] = TRS;
					mag[z++] = TLS;
				}

				break;

			case neterm_V:
				// V -> Z V1
				mag[z++] = neterm_V1;
				mag[z++] = neterm_Z;
				break;

			case neterm_V1:
				// V1 -> == Z V1 | != Z V1 | eps
				if (t == TEq || t == TNEq)
				{
					mag[z++] = neterm_V1;
					mag[z++] = neterm_Z;
					mag[z++] = t;
				}
				else
				{
					epsilon();
				}

				break;

			case neterm_Z:
				// Z -> Y Z1
				mag[z++] = neterm_Z1;
				mag[z++] = neterm_Y;
				break;

			case neterm_Z1:
				// Z1 -> < Y Z1 | <= Y Z1 | > Y Z1 | >= Y Z1 | eps
				if (t == TLT || t == TGT || t == TLE || t == TGE)
				{
					mag[z++] = neterm_V1;
					mag[z++] = neterm_Z;
					mag[z++] = t;
				}
				else
				{
					epsilon();
				}

				break;

			case neterm_Y:
				// Y -> L Y1
				mag[z++] = neterm_Y1;
				mag[z++] = neterm_L;
				break;

			case neterm_Y1:
				// Y1 -> << L Y1 | >> L Y1 | eps
				if (t == TLShift || t == TRShift)
				{
					mag[z++] = neterm_Y1;
					mag[z++] = neterm_L;
					mag[z++] = t;
				}
				else
				{
					epsilon();
				}

				break;

			case neterm_L:
				// L -> M L1
				mag[z++] = neterm_L1;
				mag[z++] = neterm_M;
				break;

			case neterm_L1:
				// L1 -> + M L1 | - M L1 | eps
				if (t == TPlus || t == TMinus)
				{
					mag[z++] = neterm_L1;
					mag[z++] = neterm_M;
					mag[z++] = t;
				}
				else
				{
					epsilon();
				}

				break;

			case neterm_M:
				// M -> N M1
				mag[z++] = neterm_M1;
				mag[z++] = neterm_N;
				break;

			case neterm_M1:
				// M1 -> * N M1 | / N M1 | % N M1 | eps
				if (t == TMult || t == TDiv || t == TMod)
				{
					mag[z++] = neterm_M1;
					mag[z++] = neterm_N;
					mag[z++] = t;
				}
				else
				{
					epsilon();
				}

				break;

			case neterm_N:
				// N -> + J | - J | J
				mag[z++] = neterm_J;

				if (t == TPlus || t == TMinus)
				{
					mag[z++] = t;
				}

				break;

			case neterm_J:
				// J -> a K | C | ( V ) | main ( )
				if (t == TIdent)
				{
					mag[z++] = neterm_K;
					mag[z++] = TIdent;
				}
				else if (t == TMain)
				{
					mag[z++] = TRS;
					mag[z++] = TLS;
					mag[z++] = TMain;
				}
				else if (t == TLS)
				{
					mag[z++] = TRS;
					mag[z++] = neterm_V;
					mag[z++] = TLS;
				}
				else
				{
					mag[z++] = neterm_C;
				}

				break;

			case neterm_K:
				// K -> ( ) | eps
				if (t == TLS)
				{
					mag[z++] = TRS;
					mag[z++] = TLS;
				}
				else
				{
					epsilon();
				}

				break;

			case neterm_C:
				// C -> c1 | c2
				if (t == TConstInt)
				{
					mag[z++] = TConstInt;
				}
				else
				{
					mag[z++] = TConstFloat;
				}

				break;
			}
		}

		z--;
	}

	return 1;
}

LL1::LL1(TScanner* scan)
{
	this->scan = scan;
	z = 0;
}
