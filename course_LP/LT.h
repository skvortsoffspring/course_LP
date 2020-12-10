#pragma once
#include "cstring"
#define LEXEMA_FIXSIZE		1			// size length lexema
#define	LT_MAXSIZE			4096		// maximal size lexem	
#define	LT_TI_NULLXDX		0xffffffff	// not link table ID			
#define LEX_BOOL			't'			// type data bool
#define	LEX_LONG			't'			// type data long
#define	LEX_CHAR			't'			// type data byte
#define LEX_FLOAT			't'			// float data type
#define	LEX_STRING			't'			// type data string
#define LEX_TYPE			't'			// all types data
#define	LEX_ID				'i'			// лексема для идентификатора
#define	LEX_LITERAL			'l'			// лексема для литерала
#define	LEX_FUNCTION		'f'			// лексема для function
#define	LEX_RETURN			'r'			// лексема для return
#define LEX_MAIN			'm'			// лексема для main
#define	LEX_SEMICOLON		';'			// лексема для ;
#define	LEX_COMMA			','			// лексема для ,
#define	LEX_LEFTBRACE		'{'			// лексема для {
#define	LEX_RIGHTBRACE		'}'			// лексема для }
#define	LEX_LEFTHESIS		'('			// лексема для (
#define	LEX_RIGHTESIS		')'			// лексема для )
#define	LEX_ASSIGN			'='			// лексема для =
#define	LEX_PLUS			'v'			// лексема для +
#define	LEX_MINUS			'v'			// лексема для -
#define	LEX_STAR			'v'			// лексема для *
#define	LEX_DIRSLASH		'v'			// лексема для /
#define EXPRESSIONS			'v'			// лесема для всех выражений
#define LEX_LESS			'c'			// symbol compare <
#define LEX_MORE			'c'			// symbol compare >
#define LEX_EQUALS			'c'			// symbol compare ==
#define LEX_NOT_EQUALS		'c'			// symbol compare !=
#define LOGICALS			'c'			// all expressions
#define LEX_XOR				'b'			// symbol compare ^
#define LEX_INVERSION		'b'			// symbol compare ~
#define LEX_AND				'b'			// symbol compare |
#define LEX_OR				'b'			// symbol compare &
#define LEX_SHR				'b'			// symbol compare >>
#define LEX_SHL				'b'			// symbol compare <<
#define LEX_WRITE			'w'			// write to console
#define LEX_WRITEL			'w'			// write to console
#define LEX_SQUARE_RIGHT	']'			// right square less
#define LEX_SQUARE_LEFT		'['			// left square less
#define LEX_REPEAT			'p'			// repeat
#define LEX_UNTIL			'u'			// until
#define LEX_IF				'z'			// IF
#define LEX_ELSE			'e'			// ELSE
#define LEX_INCORDEC		'n'			// ++ or --

namespace LT									// таблица лексем
{
	struct Entry								// строка таблицы лексем
	{
		char lexema[LEXEMA_FIXSIZE + 1];		// лексема
		char expression[LEXEMA_FIXSIZE + 1];	// выражение +,-,/,*
		int sn;									// номер строки в исходном тексте
		int idxTI;								// индекс в таблице идентификаторов или LT_TI_NULLIDX
		Entry()
		{
			memset(lexema, 0x00, LEXEMA_FIXSIZE + 1);
			memset(expression, 0x00, LEXEMA_FIXSIZE + 1);
			sn = 0;
			idxTI = LT_TI_NULLXDX;
		}
		Entry(char a, int i) {
			lexema[0] = a;
			lexema[1] = 0x00;
			sn = i;
		}
	};

	struct LexTable								// экземпляр таблицы лексем
	{
		int maxsize;							// емкость таблицы лексем < LT_MAXSIZE
		int size = 0;							// текущий размер таблицы лексем < maxsize
		Entry* table;							// массив строк таблицы лексем
	};

	LexTable Create(int);						// создать таблицу лексем
	Entry Create(char, int);
	void Add(LexTable*, Entry);					// добавление лексем
	Entry GetEntry(LexTable&, int);				// получить строку таблицы лексем
	void PrintTableLex(LexTable*, wchar_t*);		// Напечатать таблицу лексем
	void Delete(LexTable&);						// удалить таблицу лексем (освободить память)

};