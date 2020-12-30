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
			if (lextable.table[i].lexema[0] == EXPRESSIONS) 
			{
				position = polishNotation((i - 1), lextable, idtable);
				i = position;
			}
			else if (lextable.table[i].lexema[0] == LEX_ASSIGN)
			{
				//if(lextable.table[i].idxTI != TI_NULLIDX)
				//	if (idtable.table[lextable.table[i].idxTI].idtype != IT::PA)
				//	{
						position = polishNotation((i + 1), lextable, idtable);
						i = position;
				  //}
			}
			
		}

	}

	int polishNotation(int lextable_pos, LT::LexTable& lextable, IT::IdTable& idtable)
	{
		stack <LT::Entry> stack;
		list <LT::Entry> operands;
		char exp;
		int opndsindex = 0;
		bool exit= true;
		bool foundfunc = false;
		bool foundarr  = false;
		int position = lextable_pos;
		int position2 = lextable_pos;
		int count = lextable_pos;
		LT::Entry temp = LT::Entry();

		for (int i = lextable_pos; exit; i++)
		{
			if (lextable.table[i].lexema[0] == EXPRESSIONS)
			{
				exp = lextable.table[i].expression[0];
			}
			else
			{
				if (lextable.table[i].idxTI != TI_NULLIDX)
					switch (idtable.table[lextable.table[i].idxTI].idtype) {
					case IT::F:
					case IT::E:
						foundfunc = true;
						break;
					case IT::PA:
							foundarr = true;
						break;
					}
				exp = lextable.table[i].lexema[0];
			}
			if (foundarr) {
				if (lextable.table[i].lexema[0] == RIGHTSQUARE)
				{
					while ((bool)opndsindex)
					{
						operands.push_back(stack.top());
						stack.pop();
						opndsindex--;
					}
					operands.push_back(lextable.table[i]);
					foundarr = false;
					continue;
				}
				else if (lextable.table[i].lexema[0] == LEFTSQUARE)
				{
					opndsindex = 0;
					operands.push_back(lextable.table[i]);
					continue;
				}
			}
			if (foundfunc)
			{
				operands.push_back(lextable.table[i]);
				if (lextable.table[i].lexema[0] == RIGHTESIS)
					foundfunc = false;
			}
			else
			{
				switch (exp)
				{
				case LEFTHESIS:
					stack.push(lextable.table[i]);
					break;
				case RIGHTESIS:
					while (stack.top().lexema[0] != LEFTHESIS)
					{
						operands.push_back(stack.top());
						stack.pop();
					}
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
					opndsindex++;
					if (stack.empty())
					{
						stack.push(lextable.table[i]);
					}
					else if (
						stack.top().expression[0] == MUL ||
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
					opndsindex++;
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
					{
						operands.push_back(lextable.table[i]);
					}
					else
						return position + 1;
					break;
				}

				}
				position = i;
			}
		}
		
		for (int i = position2; !operands.empty(); i++) 
		{			
			lextable.table[i] = operands.front();
			operands.pop_front();		
			count++;
		}
		while (count != position)
		{
			lextable.table[count++].lexema[0] = '©';
		}
		return position + 1;
	}
}