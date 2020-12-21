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
		int position = 0;
		for (int i = 0; i < lextable.size; i++)
		{
			if (lextable.table[i].lexema[0] == EXPRESSIONS) {
				position = polishNotation((i - 1), lextable, idtable);
				i = position;
			}
		}
		for (int i = 0; i < lextable.size; i++)
		{
			
				if (lextable.table[i].lexema[0] == EXPRESSIONS&& lextable.table[i].lexema[0] != LEX_WRITE) {
					for (; lextable.table[i].lexema[0] != LEX_SEMICOLON && lextable.table[i].lexema[0] != LEX_WRITE; i++) {
						if (lextable.table[i].lexema[0] == LEX_LEFTHESIS)
							break;
						if (lextable.table[i].idxTI != TI_NULLIDX)
						{
							if (idtable.table[lextable.table[i].idxTI].iddatatype == IT::STR && lextable.table[i].lexema[0] != LEX_WRITE)
								throw ERROR_THROW_LINE(150, lextable.table[i].sn);
						}
					}

				}
			
		}

	}

	int polishNotation(int lextable_pos, LT::LexTable& lextable, IT::IdTable& idtable)
	{
		stack <LT::Entry> stack;
		list <LT::Entry> operands;
		char exp(0);
		bool exit(true);
		int position = 0;
		int position2 = lextable_pos;
		LT::Entry temp = LT::Entry();

		for (int i = lextable_pos; exit; i++)
		{
			if(lextable.table[i].lexema[0] == EXPRESSIONS)
			{
				exp = lextable.table[i].expression[0]; 
			}
			else
			{
				exp = lextable.table[i].lexema[0];
			}
			switch (exp)
			{
			case LEFTHESIS:
				operands.push_back(lextable.table[i]);
				break;
			case RIGHTESIS:
				while (stack.top().lexema[0] != LEFTHESIS)
				{
					operands.push_back(stack.top());
					stack.pop();
				}
				if(!stack.empty())
				stack.pop();
				break;
			case COMMA:
				while (stack.top().expression[0] != LEFTHESIS)
				{
					operands.push_back(stack.top());
					stack.pop();
				}
				break;
			case ADD:
			case SUB:
				if (stack.empty())
				{
					stack.push(lextable.table[i]);
				}
				else if (stack.top().expression[0] == MUL ||
					stack.top().expression[0] == DIV ||
					stack.top().expression[0] == ADD ||
					stack.top().expression[0] == SUB)
				{
					operands.push_back(stack.top());
					stack.pop();
					stack.push(lextable.table[i]);

				}
				else
					stack.push(lextable.table[i]);
				break;
			case MUL:
			case DIV:
				if (stack.empty())
				{
					stack.push(lextable.table[i]);
				}
				else if
					(
						stack.top().expression[0] == MUL ||
						stack.top().expression[0] == DIV
					)
				{
					operands.push_back(stack.top());
					stack.pop();
					stack.push(lextable.table[i]);
				}
				else
					stack.push(lextable.table[i]);
				break;
			case SEMICOLON:
			case LEX_SQUARE_RIGHT:
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
					operands.push_back(lextable.table[i]);
				else
					return position + 1;
				break;
			}

			}
			position = i;
		}
		
		for (int i = position2; !operands.empty(); i++) {
			
			lextable.table[i] = operands.front();
			operands.pop_front();
			
		}
		//
while (lextable.table[position2].lexema[0] != LEX_SEMICOLON) {
		//
	if (lextable.table[position2].lexema[0] == LEFTHESIS || lextable.table[position2].lexema[0] == RIGHTESIS)
		//
		lextable.table[position2].lexema[0] = '@';
		//
	position2++;
		//
}
		return position+1;
	}
}