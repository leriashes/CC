#pragma once
#include <iostream>
#include <fstream>
using namespace std;

#define MAX_TEXT 100000     //макисмальная длина текста ИМ
#define MAX_LEX 100         //максимальная длина лексемы
#define MAX_KEYW 8			//число ключевых слов
#define MAX_CONST 10		//длина константы

typedef char IM[MAX_TEXT];   //текст ИМ
typedef char LEX[MAX_LEX];   //лексема

//ключевые слова
#define TInt       1		//int
#define TShort     2		//short
#define TLong      3		//long
#define TFloat     4		//float
#define TMain      5		//main
#define TReturn    6		//return
#define TWhile     7		//while
#define TBreak     8		//break

//идентификаторы
#define TIdent      20		//идентификатор

//константы
#define TConstInt      30	//целочисленная константа
#define TConstFloat    31	//константа с фиксированной точкой

//специальные знаки
#define TComma      40		// ,
#define TSemicolon  41		// ;
#define TLS         42		// (
#define TRS         43		// )
#define TFLS        44		// {
#define TFRS        45		// }

//знаки операций
#define TSave       50		// =
#define TEq         51		// ==
#define TNEq        52		// !=
#define TLT         53		// <
#define TGT         54		// >
#define TLE         55		// <=
#define TGE         56		// >=
#define TLShift     57		// <<
#define TRShift     58		// >>
#define TPlus       59		// +
#define TMinus      60		// -
#define TMult       61		// *
#define TDiv        62		// /
#define TMod        63		// %

//конец исходного модуля
#define TEnd        100

//ошибочный символ
#define TError      200

#define MaxTypeTerminal		TError

//нетерминалы
#define neterm_S	1000
#define neterm_O	1001
#define neterm_D	1002
#define neterm_T	1003
#define neterm_I	1004
#define neterm_X	1005
#define neterm_X1	1006
#define neterm_E	1007
#define neterm_F	1008
#define neterm_Q	1009
#define neterm_U	1010
#define neterm_A	1011
#define neterm_R	1012
#define neterm_B	1013
#define neterm_W	1014
#define neterm_P	1015
#define neterm_V	1016
#define neterm_V1	1017
#define neterm_Z	1018
#define neterm_Z1	1019
#define neterm_Y	1020
#define neterm_Y1	1021
#define neterm_L	1022
#define neterm_L1	1023
#define neterm_M	1024
#define neterm_M1	1025
#define neterm_N	1026
#define neterm_J	1027
#define neterm_K	1028
#define neterm_C	1029

#define MaxTypeNeterminal		neterm_C

//операционные символы (семантика)
#define sem_getVar		2000
#define sem_startDecl	2001
#define sem_setIdent	2002
#define sem_setFunct	2003
#define sem_returnLevel	2004
#define sem_setNewLevel	2005
#define sem_getFunct	2006
#define sem_constType	2007

#define sem_match		2010
#define sem_matchLeft	2011
#define sem_matchInt	2012
#define sem_push		2013
#define sem_gener		2014	
#define sem_generIf		2015
#define sem_formIf		2016
#define sem_generGoto	2017
#define sem_generNop	2018
#define sem_setAddr		2019
#define sem_generCall	2020
#define sem_startFunc	2022
#define sem_endFunc		2021
#define sem_generReturn	2023
#define sem_generBreak	2024
#define sem_generUnar	2025

#define MaxTypeSem		sem_generUnar

#define ifOper			2090	//if

#define intToFloat		2100
#define shortToInt		2101
#define shortToFloat	2102
#define intToShort		2103
#define floatToInt		2104
#define floatToShort	2105

#define callOper		2110
#define returnOper		2111
#define gotoOper		2112	//goto
#define procOper		2113

#define breakOper		2120
#define prologOper		2121
#define epilogOper		2122
#define retOper			2123
#define endpOper		2124
#define nopOper			2125
