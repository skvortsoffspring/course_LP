#include "GRB.h"
#define GRB_ERROR_SERIES 600
#include <string.h>

namespace GRB
{	
	Rule::Chain::Chain(short psize, GRBALPHABET s, ...)
	{
		nt = new GRBALPHABET[size = psize];
		int* p = (int*)&s;
		for (short i = 0; i < psize; i++)
		{
			nt[i] = (GRBALPHABET)p[i];
		}
	};

	Rule::Rule(GRBALPHABET pnn, int piderror, short psize, GRB::Rule::Chain c, ...)
	{
		nn = pnn;
		iderror = piderror;
		chains = new Chain[size = psize];
		Chain* p = &c;
		for (int i = 0; i < size; i++) chains[i] = p[i];

	};

	Greibach::Greibach(GRBALPHABET psrartN, GRBALPHABET pstbottom, short psize, Rule r, ...)
	{
		startN = psrartN;
		stbottomT = pstbottom;
		rules = new Rule[size = psize];
		Rule* p = &r;
		for (int i = 0; i < size; i++) rules[i] = p[i];

	};
	/*functions and methods gramatic Greibach*/
	//Greibach getGreibach() { return GRB::greibach; };
	short Greibach::getRule(GRBALPHABET pnn, Rule& prule)
	{
		short rc = -1;
		short k = 0;
		while (k < size && rules[k].nn != pnn) k++;
		if (k < size) prule = rules[rc = k];
		return rc;
	};
	Rule Greibach::getRule(short n)
	{
		Rule rc;
		if (n < size) rc = rules[n];
		return rc;
	};
	char* Rule::getCRule(char* b, short nchain) // получит привило в виде N->цепочка
	{
		char bchain[200];
		b[0] = Chain::alphabet_to_char(nn);
		b[1] = '-';
		b[2] = '>';
		b[2] = 0x00;
		chains[nchain].getCChain(bchain);
		strcat_s(b, sizeof(bchain) + 5, bchain);
		return b;
	};
	short Rule::getNextChain(GRBALPHABET t, Rule::Chain& pchain, unsigned short j)
	{
		short rc = -1;
		while (j < size && chains[j].nt[0] != t) ++j;
		rc = (j < size ? j : -1);
		if (rc >= 0) pchain = chains[rc];
		return rc;
	};
	char* Rule::Chain::getCChain(char* b)	// получить первую строку правила
	{
		for (int i = 0; i < size; i++)b[i] = Chain::alphabet_to_char(nt[i]);
		b[size] = 0x00;
		return b;
	}
#define NS(n) Rule::Chain::N(n)
#define TS(n) Rule::Chain::T(n)

	Greibach greibach(NS('S'), TS('$'), 9,
		Rule(NS('S'), GRB_ERROR_SERIES + 0,
			2,	//Неверная структура программы
			Rule::Chain(13, TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS('{'), NS('N'), TS('r'), NS('X'), TS(';'), TS('}'), NS('S')),
			Rule::Chain(7, TS('m'), TS('{'), NS('N'), TS('r'), NS('X'), TS(';'), TS('}'))
		),
		Rule(NS('N'), GRB_ERROR_SERIES + 1,
			31,	//ошибочный оператор																
			Rule::Chain(3,	TS('o'), TS(';'), NS('N')),																						// overflow control o; ->N		
			Rule::Chain(4,  TS('i'), TS('n'), TS(';'), NS('N')),																			// type identificator
			Rule::Chain(4,  TS('t'), TS('i'), TS(';'), NS('N')),																			// type identificator
			Rule::Chain(5,  TS('w'), TS('('), TS(')'), TS(';'), NS('N')),
			Rule::Chain(5,  TS('i'), TS('='), NS('E'), TS(';'), NS('N')),																	// identificator = ->E ; ->N
			Rule::Chain(6,  TS('t'), TS('i'), TS('='), NS('E'), TS(';'), NS('N')),															// type identificator = ->E; ->N
			Rule::Chain(6,  TS('w'), TS('('), NS('E'), TS(')'), TS(';'), NS('N')),
			Rule::Chain(6,  TS('i'), TS('('), NS('E'), TS(')'), TS(';'), NS('N')),
			Rule::Chain(7,  TS('t'), TS('i'), TS('['), NS('E'), TS(']'), TS(';'), NS('N')),													// type identificator [->E] ; ->N
			Rule::Chain(8,  TS('i'), TS('['), NS('E'), TS(']'), TS('='), NS('E'), TS(';'), NS('N')),										// identificator [->E] = ->E ;->N
			Rule::Chain(8,  TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS(';'), NS('N')),										// type func identificator (->F) ; ->N
			Rule::Chain(10, TS('z'), TS('('), NS('X'), TS('c'), NS('X'), TS(')'), TS('{'), NS('N'), TS('}'), NS('B')),						// if(->E c ->E) {->N } ->else
			Rule::Chain(10, TS('z'), TS('('), NS('X'), TS('c'), NS('X'), TS(')'), TS('{'), NS('N'), TS('}'), NS('N')),						// if(->E c ->E) {->N } ->N
			Rule::Chain(8,	TS('z'), TS('('), NS('X'), TS(')'), TS('{'), NS('N'), TS('}'), NS('B')),										// if(->E c ->E) {->N } ->N
			Rule::Chain(8,  TS('z'), TS('('), NS('X'), TS(')'), TS('{'), NS('N'), TS('}'), NS('N')),										// if(->E c ->E) {->N } ->N
			Rule::Chain(11, TS('t'), TS('i'), TS('['), NS('E'), TS(']'), TS('='), TS('{'), NS('A'), TS('}'), TS(';'), NS('N')),				// type identificator [->E] = {->A} ; ->N
			Rule::Chain(12, TS('p'), TS('{'), NS('N'), TS('}'), TS('u'), TS('('), NS('X'), TS('c'), NS('X'), TS(')'), TS(';'), NS('N')),	// repeat {-N} until (->E c ->E);
			Rule::Chain(2,  TS('o'), TS(';')),																								// overflow control o; ->N		
			Rule::Chain(3,	TS('i'), TS('n'), TS(';')),																						// iterator
			Rule::Chain(3,	TS('t'), TS('i'), TS(';')),																						// type identificator
			Rule::Chain(4,	TS('w'), TS('('), TS(')'), TS(';')),
			Rule::Chain(4,	TS('i'), TS('='), NS('E'), TS(';')),																			// identificator = ->E;
			Rule::Chain(5,	TS('t'), TS('i'), TS('='), NS('E'), TS(';')),																    // type identificator = ->E;
			Rule::Chain(5,	TS('w'), TS('('), NS('E'), TS(')'), TS(';')),
			Rule::Chain(5,	TS('i'), TS('('), NS('E'), TS(')'), TS(';')),
			Rule::Chain(6,	TS('t'), TS('i'), TS('['), NS('E'), TS(']'), TS(';')),															// identificator [->E] ;
			Rule::Chain(7,	TS('z'), TS('('), NS('X'), TS(')'), TS('{'), NS('N'), TS('}')),													// if(->E c ->E) {->N }
			Rule::Chain(7,	TS('i'), TS('['), NS('E'), TS(']'), TS('='), NS('E'), TS(';')),													// identificator [->E] = ->E ;
			Rule::Chain(9,	TS('z'), TS('('), NS('X'), TS('c'), NS('X'), TS(')'), TS('{'), NS('N'), TS('}')),								// if(->E c ->E) {->N }
			Rule::Chain(10, TS('t'), TS('i'), TS('['), NS('E'), TS(']'), TS('='), TS('{'), NS('A'), TS('}'), TS(';')),						// type identificator [->E] = {->A} ; 
			Rule::Chain(11, TS('p'), TS('{'), NS('N'), TS('}'), TS('u'), TS('('), NS('X'), TS('c'), NS('X'), TS(')'), TS(';'))				// repeat {-N} until (->E c ->E);

		),
		Rule(NS('E'), GRB_ERROR_SERIES + 2,
			14,	//выражение
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l')),
			Rule::Chain(2, TS('i'), NS('M')),
			Rule::Chain(2, TS('l'), NS('M')),
			Rule::Chain(3, TS('i'), TS('n'), TS(';')),
			Rule::Chain(3, TS('l'), TS(','), NS('E')),																// XbX;	
			Rule::Chain(3, TS('i'), TS(','), NS('E')),
			Rule::Chain(4, TS('i'), TS('('), NS('W'), TS(')')),
			Rule::Chain(4, TS('i'), TS('['), NS('E'), TS(']')),
			Rule::Chain(4, TS('('), NS('E'), TS(')'), NS('M')),
			Rule::Chain(5, TS('i'), TS('('), NS('E'), TS(')'), NS('M')),
			Rule::Chain(5, TS('i'), TS('('), NS('W'), TS(')'), NS('M')),
			Rule::Chain(6, TS('i'), TS('['), NS('E'), TS(']'), TS(','), NS('E')),
			Rule::Chain(5, TS('i'), TS('['), NS('E'), TS(']'), NS('M'))
		),
		Rule(NS('M'), GRB_ERROR_SERIES + 3,
			2,	//Ошибка в выражении с арефмитическими операторами
			Rule::Chain(2, TS('v'), NS('E')),
			Rule::Chain(3, TS('v'), NS('E'), NS('M'))
		),
		Rule(NS('F'), GRB_ERROR_SERIES + 4,
			4,		// параметры функции при определении
			Rule::Chain(2, TS('t'), TS('i')),
			Rule::Chain(4, TS('t'), TS('i'), TS('['), TS(']')),
			Rule::Chain(4, TS('t'), TS('i'), TS(','), NS('F')),
			Rule::Chain(6, TS('t'), TS('i'), TS('['), TS(']'), TS(','), NS('F'))
		),
		Rule(NS('W'), GRB_ERROR_SERIES + 5, 
			6,	//параметры вызываемой ф-ции
			Rule::Chain(6, TS('i'), TS('['), NS('E'), TS(']'), TS(','), NS('W')),
			Rule::Chain(4, TS('i'), TS('['), NS('E'), TS(']')),
			Rule::Chain(3, TS('i'), TS(','), NS('W')),
			Rule::Chain(3, TS('l'), TS(','), NS('W')),
			Rule::Chain(1, TS('l')),
			Rule::Chain(1, TS('i'))
		),
		Rule(NS('A'), GRB_ERROR_SERIES + 6, 
			2,	// заполнение массива
			Rule::Chain(1, TS('l')),
			Rule::Chain(3, TS('l'), TS(','), NS('A'))		
		),
		Rule(NS('X'), GRB_ERROR_SERIES + 7,
			4,	
			Rule::Chain(1, TS('l')),
			Rule::Chain(1, TS('i')),
			Rule::Chain(4, TS('i'), TS('['), NS('E'), TS(']')),
			Rule::Chain(4, TS('l'), TS('['), NS('E'), TS(']'))
		),		
		Rule(NS('B'), GRB_ERROR_SERIES + 8, 
			4,	//  условный оператор else
			Rule::Chain(5, TS('e'), TS('{'), NS('E'), TS('}'), NS('N')),									// u{E},N
			Rule::Chain(4, TS('e'), TS('{'), NS('E'), TS('}')),												// u{E}
			Rule::Chain(5, TS('e'), TS('{'), NS('N'), TS('}'), NS('N')),									// u{E}
			Rule::Chain(4, TS('e'), TS('{'), NS('N'), TS('}'))												// u{E}
		)
	);

	Greibach getGreibach() { return greibach; }

}