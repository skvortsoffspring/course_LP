
#include "stdafx.h"
//#define EXPRESSIONS		'v'	// лесема для всех выражений 
#define MAX_PARAMETRES	10		// максимальное число параметров
#define ADD				'+'		// cложение
#define SUB				'-'		// вычитание
#define MUL				'*'		// умножение
#define DIV				'/'		// деление
#define	LEFTHESIS		'('		// (
#define	RIGHTESIS		')'		// )
#define EQUALS			'='		// для поиска цепочки
#define	SEMICOLON		';'		// лексема для конца строки;
#define CALL_FUNCTION	'@'		// вызов функции
#define	COMMA			','		// ,

namespace PN
{
	void seachingExpressions(LT::LexTable& lextable, IT::IdTable& idtable);
	bool polishNotation(int lextable_pos, LT::LexTable& lextable, IT::IdTable& idtable);
}