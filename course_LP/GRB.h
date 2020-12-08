#pragma once
#include "Error.h"

typedef short GRBALPHABET;	// символы алфавита грамматики терминалы > 0,нетерминалы < 0

namespace GRB
{
	struct Rule	//правило в грамматике 
	{
		GRBALPHABET nn;			//нетерминал (левый символ правила) < 0
		int iderror;			//идентификатор диагностического сообщения
		short size;				//цепочка(правая часть правила)

		struct Chain
		{
			short size;			//длина цепочки
			GRBALPHABET* nt;	//цепочка терминалов(>0) и нетерминальных (<0)

			Chain() {size = 0; nt = 0; };

			Chain(short psize, GRBALPHABET s, ...);	// psize количество символов в цепочке, s символы (терминал или не терминал)

			char* getCChain(char* b);									//получить правую сторону правила
			static GRBALPHABET T(char t) { return GRBALPHABET(t); }		// терминал
			static GRBALPHABET N(char n) { return -GRBALPHABET(n); }	// не терминал
			static bool isT(GRBALPHABET s) { return s > 0; }			// терминал?	
			static bool isN(GRBALPHABET s) { return s < 0; }			// нетерминал?
			static char alphabet_to_char(GRBALPHABET s) { return isT(s) ? char(s) : char(-s); };

		}*chains;

		Rule() {nn = 0x00; size = 0; }

		Rule(
			GRBALPHABET pnn,		//нетерминал (< 0)
			int iderror,			//идентификатор диагностического сообщения (Error)
			short psize,			//количество цепочек - правых частей правила
			Chain c, 				//множество цепочек - правых частей правила
			...
		);

		char* getCRule(char* b, short nchain);							//получить правило в виде N->цепочка
		short getNextChain(GRBALPHABET t, Rule::Chain& pchain,unsigned short j);//получить следующую за j подходящую цепочку вернуть ее номер или -1
	};

	struct Greibach				// грамматика Грейбах
	{
		GRBALPHABET size;		//количество правил
		GRBALPHABET startN;		//стартовый символ
		GRBALPHABET stbottomT;  //дно стека
		Rule* rules;			//множество правил

		Greibach() 
		{
			size = 0; 
			startN = 0; 
			stbottomT = 0; 
			rules = 0;
		};
		Greibach(
			GRBALPHABET startN,
			GRBALPHABET pstbottomT,
			short psize,
			Rule r, 
			...
			);

		short getRule(GRBALPHABET pnn, Rule& prule);	//получить правило, возвращается номер правила или -1
		Rule getRule(short n);							//получить правило по номеру
	};

	Greibach getGreibach();								//получить грамматику
};
	