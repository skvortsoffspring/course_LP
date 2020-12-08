#include "stdafx.h"
#include <stack>
#include <list>
#include <iostream>
#include <iostream>
#define ENDEXPRECION '\n'
using namespace std;

namespace PN {

	void seachingExpressions(LT::LexTable& lextable, IT::IdTable& idtable)
	{
		for (int i = 0; i < lextable.size; i++)
		{
			if (lextable.table[i].lexema[0] == EXPRESSIONS)
				if (polishNotation((i - 1), lextable, idtable))
				{
					cout << " выражение преобразовано в строке: " << lextable.table[i].sn << endl;
				}
				else
					cout << " выражение не преобразовано в строке: " << lextable.table[i].sn << endl;
		}
	}

	bool polishNotation(int lextable_pos, LT::LexTable& lextable, IT::IdTable& idtable)
	{
		stack <char> stack;
		list <char> operands;
		char exp(0);
		bool exit(true);
		bool expression = false;
		short sizeparametres(0);
		bool callFunction = false;
		bool parametresnotnull = false;

		for (int i = lextable_pos; exit; i++)
		{
			if (lextable.table[i].idxTI != LT_TI_NULLXDX)
				if (idtable.table[lextable.table[i].idxTI].idtype == IT::IDTYPE::P && callFunction)
				{
					if (!parametresnotnull)
						sizeparametres = 1;
					parametresnotnull = true;

				}

			if (lextable.table[i].lexema[0] == LEX_LITERAL)
			{
				if (!parametresnotnull)
					sizeparametres = 1;
				parametresnotnull = true;
			}

			if (lextable.table[i].lexema[0] == LEX_ID)
			{
				if (idtable.table[lextable.table[i].idxTI].idtype == IT::IDTYPE::E)
				{
					callFunction = true;
					continue;
				}
				else
					exp = lextable.table[i].lexema[0];//idtable.table[lextable.table[i].idxTI].id[0];
			}
			else if (lextable.table[i].expression[0])
				exp = lextable.table[i].expression[0];
			else
				exp = lextable.table[i].lexema[0];

			switch (exp)
			{
			case LEFTHESIS:
				stack.push(exp);
				break;
			case RIGHTESIS:
				while (stack.top() != LEFTHESIS)
				{
					operands.push_back(stack.top());
					stack.pop();
				}
				if (callFunction)
				{
					operands.push_back(CALL_FUNCTION);
					if (sizeparametres < MAX_PARAMETRES)
					{
						operands.push_back(sizeparametres + 48);
					}
				}
				stack.pop();
				parametresnotnull = false;
				break;
			case COMMA:
				while (stack.top() != LEFTHESIS)
				{
					operands.push_back(stack.top());
					stack.pop();
				}
				if (parametresnotnull)
					sizeparametres++;
				break;
			case ADD:
			case SUB:
				if (stack.empty())
				{
					stack.push(exp);
					expression = true;
				}
				else if (stack.top() == MUL ||
					stack.top() == DIV ||
					stack.top() == ADD ||
					stack.top() == SUB)
				{
					operands.push_back(stack.top());
					stack.pop();
					stack.push(exp);

				}
				else
					stack.push(exp);
				break;
			case MUL:
			case DIV:
				if (stack.empty())
				{
					stack.push(exp);
					expression = true;
				}
				else if
					(
						stack.top() == MUL ||
						stack.top() == DIV
						)
				{
					operands.push_back(stack.top());
					stack.pop();
					stack.push(exp);
				}
				else
					stack.push(exp);
				break;
			case SEMICOLON:
				while (!stack.empty())
				{
					operands.push_back(stack.top());
					stack.pop();
				}
				exit = false;
				break;
			default:
			{
				if (
					lextable.table[i].lexema[0] == LEX_ID ||
					lextable.table[i].lexema[0] == LEX_LITERAL ||
					lextable.table[i].lexema[0] == LEFTHESIS ||
					lextable.table[i].lexema[0] == RIGHTESIS
					)
					operands.push_back(exp);
				else
					return false;
				break;
			}

			}
		}
		if (!expression)
			return false;

		while (!operands.empty()) {

			cout << operands.front();
			operands.pop_front();

		}
		return true;
	}
}