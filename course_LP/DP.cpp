#include "stdafx.h"
#define TOKEN	token_rekognizer(data.string, &ENTRY);
#define ENTRY	idtable->table[idtable->current_size]

//#include "../generator_graphs/graphs.h"


namespace GM
{
	void dataProcesing(unsigned char* text, std::fstream* stream, LT::LexTable* lextable, IT::IdTable* idtable)
	{

		unsigned char* start = text;			// указатель на начало слова
		unsigned char* end = start;				// указатель на конец  слова
		Data data;	

		while (true)
		{
			if (!*end) {
				if (data.braces)
					throw ERROR_THROW_LINE(135, data.count_lines);
				break;
			}
																			// special symbols digital literal or identificator
			if (*end == '.')									// , - float
				if (isdigit(*(end + 1)))
					end++;
			else if((*end == 'x' || *end == 'b') && isdigit(*(end+1)))		// x- heximal, b- binary
				end++;
			if(*end == '_')													// _ - identificator
				end++;

			switch (*start)
			{
			case '=':
				if (*end == '=')
					end++;
				break;
			case '!':
				if (*end == '=')
					end++;
				break;
			case '>':
				if (*end == '=')
					end++;
				else if (*end == '>')
					end++;
				break;
			case '<':
				if (*end == '=')
					end++;
				else if (*end == '<')
					end++;
				break;
			case '-':
				if (*(start - 1) == '=') {
					start++;
					data.negativeValue = true;
				}
				else if(*(start + 1) == '-') {
					end++;
				}
				break;
			case '+':
				if (*(start + 1) == '+') {
					end++;
				}
				break;
			case '{':
				data.braces++;
				break;
			case '}':
				data.braces--;
				if(!data.braces)
					memset(data.prefix, 0, PREFIX_SIZE);
				break;
			case '[':
				if(idtable->table[idtable->current_size - 1].iddatatype && idtable->table[idtable->current_size - 1].idtype == IT::V)
					idtable->table[idtable->current_size-1].idtype = IT::A;
				break;
			
			}


			if ((!alphaCirillicDigit(*end) || !alphaCirillicDigit(*start)) && !data.switch_string)		// не цифры, не символы и не литерал
			{
				if (data.negativeValue) {
					start--;
					data.negativeValue = false;
				}

				data.string = new  char[end - start + 2];
				strncpy(data.string, (char*)start, end - start);
				data.string[end - start] = STR_ENDL;

				if (*data.string == NEGATIVE && lextable->table[lextable->size - 1].lexema[0] == LEX_EQUALS)
					data.negativeValue = true;

 				data.token = TOKEN;												// распознователь ключевых слов
				data.count = lextable->size;
				if (!data.token) 
				{
					std::cout << "Error token not recognized" << std::endl;
					throw ERROR_THROW_LINE(128, data.count_lines);				// ни один автомат не сработал
				}

				LT::Entry entryLT = LT::Create(data.token, data.count_lines);

				switch (data.token) {
				case LEX_FUNCTION:
					data.datatype = idtable->table[idtable->current_size].iddatatype;
					data.visibility_in_parametres = true;
					data.positionfuncID = idtable->current_size;
					if (data.braces)
						throw ERROR_THROW_LINE(135, data.count_lines);
					break;
				case LEX_RIGHTESIS:
					data.visibility_in_parametres = false;
					data.positionfuncID = -1;
					break;
				case LEX_LITERAL:
					if (data.visibility_in_parametres)
						idtable->table[idtable->current_size].idtype = IT::P;
					LiteralCreate(*idtable, data.string, data.count_lines, data.negativeValue, data.visibility_in_parametres);
					break;
				case EXPRESSIONS:
				case LOGICALS:
					entryLT.expression[0] = data.string[0];
					entryLT.expression[1] = STR_ENDL;
					break;
				case LEX_RETURN:
					data.check_type_return = true;
					break;
				case LEX_RIGHTBRACE:
					if (data.braces <= -1)
						throw ERROR_THROW_LINE(136, data.count_lines);
						break;
				case LEX_ID:
				case LEX_MAIN:
					if (ENTRY.idtype == IT::IDTYPE::F)
						if (!(bool)ENTRY.iddatatype)
							throw ERROR_THROW_LINE(127, data.count_lines);			// не указан тип для функции

					IdentificatorCreate(idtable, data, stream);
					if ((bool)ENTRY.iddatatype || data.token == LEX_MAIN)
					{
						entryLT.idxTI =lextable->size;
					}
					else
						entryLT.idxTI = ENTRY.idxfirstLE;

					IT::Add(*idtable, ENTRY);
					break;
				}

				LT::Add(lextable, entryLT);

				start = end;
				delete[] data.string;
			}

			if (*end == LITERAL) {
				if (data.switch_string)
					data.switch_string = false;
				else
					data.switch_string = true;
			}

			if ((*end == SPACE || *end == SEPARATER) && !data.switch_string) {
				if (*end == SEPARATER)
					data.count_lines++;
				start = end;
				start++;
				end = start;
			}
			end++;
		}
	}

	void IdentificatorCreate(IT::IdTable* idtable, Data& data, std::fstream* stream)
	{

		switch (ENTRY.idtype) {
		case IT::IDTYPE::F:
		{
			IT::IsId(*idtable, data.string, data.prefix, data.count_lines,&data.visibility_in_parametres, ENTRY.iddatatype, ENTRY.idtype);
			if (strlen(data.string) > PREFIX_SIZE)
				*stream << "Идентификатор функции (" << data.string << ") усечен!\n";
			
			strncpy_s(data.prefix, data.string, PREFIX_SIZE);
			strncpy_s(ENTRY.id, data.string, ID_MAXSIZE);
			if(data.token != LEX_MAIN)
				data.visibility_in_parametres = true;
			ENTRY.idxfirstLE = data.count;
			break;
		}
		case IT::IDTYPE::E:
		{
			strncpy_s(ENTRY.extfunct, data.string, EXT_FUNCTION);
			if (strlen(data.string) > EXT_FUNCTION)
				throw ERROR_THROW_LINE(134, data.count_lines)
			strncpy_s(ENTRY.prefix, data.prefix, PREFIX_SIZE);
			ENTRY.idxfirstLE = data.count;
			break;
		}

		default:
		{
			if (ENTRY.iddatatype)
			{
				
				if (data.visibility_in_parametres) {
					ENTRY.idtype = IT::IDTYPE::P;
					idtable->table[data.positionfuncID].parametresfunc++;
					if (strlen(data.string) > PREFIX_SIZE)
						*stream << "Идентификатор параметра (" << data.string << ") усечен!\n";
				}
				else
				{
					ENTRY.idtype = IT::IDTYPE::V;
					if (strlen(data.string) > PREFIX_SIZE)
						*stream << "Идентификатор переменнной (" << data.string << ") усечен!\n";
					if(!*data.prefix)
						throw ERROR_THROW_LINE(137, data.count_lines)
				}
				IT::IsId(*idtable, data.string, data.prefix, data.count_lines,data.visibility_in_parametres, ENTRY.iddatatype, ENTRY.idtype);
				strncpy_s(ENTRY.prefix, data.prefix, PREFIX_SIZE);
				strncpy_s(ENTRY.id, data.string, ID_MAXSIZE);
				ENTRY.idxfirstLE = data.count;
			}
			else
			{
				strncpy_s(ENTRY.id, data.string, ID_MAXSIZE);
				strncpy_s(ENTRY.prefix, data.prefix, PREFIX_SIZE);
				ENTRY.idxfirstLE = IT::IsId(*idtable, data.string, data.prefix, data.count_lines,data.visibility_in_parametres, ENTRY.iddatatype, ENTRY.idtype);
				if(ENTRY.idxfirstLE == TI_NULLIDX)
					throw ERROR_THROW_LINE(133, data.count_lines)
			}
			break;
		}
		}
	}

	char token_rekognizer(char* string, IT::Entry* entry) {
		bool result = false;

		switch (string[0]) {

		case '|':
		{
			FST::FST graph(string, 1,
				FST::NODE(1,
					FST::RELATION('|', 0)
				)
			);
			if (result = execute(graph)) {
				return LEX_NOT_EQUALS;
				break;
			}
		}
		case '&':
		{
			FST::FST graph(string, 1,
				FST::NODE(1,
					FST::RELATION('&', 0)
				)
			);
			if (result = execute(graph)) {
				return LEX_AND;
				break;
			}

		}
		case '(':
		{
			FST::FST graph(string, 1,
				FST::NODE(1,
					FST::RELATION('(', 0)
				)
			);
			if (result = execute(graph)) {
				return LEX_LEFTHESIS;
				break;
			}

		}
		case ')':
		{
			FST::FST graph(string, 1,
				FST::NODE(1,
					FST::RELATION(')', 0)
				)
			);
			if (result = execute(graph)) {
				return LEX_RIGHTESIS;
			}

		}
		case '[':
		{
			FST::FST graph(string, 1,
				FST::NODE(1,
					FST::RELATION('[', 0)
				)
			);
			if (result = execute(graph)) {
				return LEX_SQUARE_LEFT;
			}

		}
		case ']':
		{
			FST::FST graph(string, 1,
				FST::NODE(1,
					FST::RELATION(']', 0)
				)
			);
			if (result = execute(graph)) {
				return LEX_SQUARE_RIGHT;
			}

		}
		case '*':
		{
			FST::FST graph(string, 1,
				FST::NODE(1,
					FST::RELATION('*', 0)
				)
			);
			if (result = execute(graph)) {
				return LEX_STAR;
				break;
			}

		}
		case '+':
		{
			FST::FST graph_inc(string, 3,
				FST::NODE(1,
					FST::RELATION('+', 1)
				),
				FST::NODE(1,
					FST::RELATION('+', 2)
				),
				FST::NODE()
			);
			if (result = execute(graph_inc)) {
				return LEX_INCORDEC;
			}
			FST::FST graph(string, 1,
				FST::NODE(1,
					FST::RELATION('+', 0)
				)
			);
			if (result = execute(graph)) {
				return LEX_PLUS;
			}

		}
		case ',':
		{
			FST::FST graph(string, 1,
				FST::NODE(1,
					FST::RELATION(',', 0)
				)
			);
			if (result = execute(graph)) {
				return LEX_COMMA;
				break;
			}

		}
		case '-':
		{
			FST::FST graph_dec(string, 3,
				FST::NODE(1,
					FST::RELATION('-', 1)
				),
				FST::NODE(1,
					FST::RELATION('-', 2)
				),
				FST::NODE()
			);
			if (result = execute(graph_dec)) {
				return LEX_INCORDEC;
			}
			FST::FST graph(string, 1,
				FST::NODE(1,
					FST::RELATION('-', 0)
				)
			);
			if (result = execute(graph)) {
				return LEX_MINUS;
			}

		}
		case '/':
		{
			FST::FST graph(string, 1,
				FST::NODE(1,
					FST::RELATION('/', 0)
				));
			if (result = execute(graph)) {
				return LEX_DIRSLASH;
			}

		}
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case '0':
		{
			FST::FST graph_digit_dec(string, 1,
				FST::NODE(11,
					FST::RELATION('1', 0),
					FST::RELATION('2', 0),
					FST::RELATION('3', 0),
					FST::RELATION('4', 0),
					FST::RELATION('5', 0),
					FST::RELATION('6', 0),
					FST::RELATION('7', 0),
					FST::RELATION('8', 0),
					FST::RELATION('9', 0),
					FST::RELATION('0', 0),
					FST::RELATION('-', 0)
				)
			);
			if (result = execute(graph_digit_dec)) {
				entry->iddatatype = IT::IDDATATYPE::LONG;
				entry->idcalculus = IT::CALCULUS::DEC;
				return LEX_LITERAL;
			}
			FST::FST graph_digit_oct(string, 3,
				FST::NODE(1, FST::RELATION('0', 1)),
				FST::NODE(10,
					FST::RELATION('1', 1),
					FST::RELATION('2', 1),
					FST::RELATION('3', 1),
					FST::RELATION('4', 1),
					FST::RELATION('5', 1),
					FST::RELATION('6', 1),
					FST::RELATION('7', 1),
					FST::RELATION('0', 1)
				),
				FST::NODE()
			);
			if (result = execute(graph_digit_oct)) {
				entry->iddatatype = IT::IDDATATYPE::LONG;
				entry->idcalculus = IT::CALCULUS::OCT;
				return LEX_LITERAL;
			}
			FST::FST graph_digit_hex(string, 3,
				FST::NODE(1, FST::RELATION('0', 1)),
				FST::NODE(1, FST::RELATION('x', 2)),
				FST::NODE(16,
					FST::RELATION('0', 2),
					FST::RELATION('1', 2),
					FST::RELATION('2', 2),
					FST::RELATION('3', 2),
					FST::RELATION('4', 2),
					FST::RELATION('5', 2),
					FST::RELATION('6', 2),
					FST::RELATION('7', 2),
					FST::RELATION('8', 2),
					FST::RELATION('9', 2),
					FST::RELATION('a', 2),
					FST::RELATION('b', 2),
					FST::RELATION('c', 2),
					FST::RELATION('d', 2),
					FST::RELATION('e', 2),
					FST::RELATION('f', 2)

				)
			);
			if (result = execute(graph_digit_hex)) {
				entry->iddatatype = IT::IDDATATYPE::LONG;
				entry->idcalculus = IT::CALCULUS::HEX;
				return LEX_LITERAL;
			}
			FST::FST graph_digit_bin(string, 3,
				FST::NODE(1, FST::RELATION('0', 1)),
				FST::NODE(1, FST::RELATION('b', 2)),
				FST::NODE(2,
					FST::RELATION('1', 2),
					FST::RELATION('0', 2)
				)
			);
			if (result = execute(graph_digit_bin)) {
				entry->iddatatype = IT::IDDATATYPE::LONG;
				entry->idcalculus = IT::CALCULUS::BIN;
				return LEX_LITERAL;
			}
			FST::FST graph_digit_float(string, 2,
				FST::NODE(12,
					FST::RELATION('1', 0),
					FST::RELATION('2', 0),
					FST::RELATION('3', 0),
					FST::RELATION('4', 0),
					FST::RELATION('5', 0),
					FST::RELATION('6', 0),
					FST::RELATION('7', 0),
					FST::RELATION('8', 0),
					FST::RELATION('9', 0),
					FST::RELATION('0', 0),
					FST::RELATION('-', 0),
					FST::RELATION('.', 1)
				),
				FST::NODE(10,
					FST::RELATION('1', 1),
					FST::RELATION('2', 1),
					FST::RELATION('3', 1),
					FST::RELATION('4', 1),
					FST::RELATION('5', 1),
					FST::RELATION('6', 1),
					FST::RELATION('7', 1),
					FST::RELATION('8', 1),
					FST::RELATION('9', 1),
					FST::RELATION('0', 1)
				)
			);
			if (result = execute(graph_digit_float)) {
				entry->iddatatype = IT::IDDATATYPE::FLOAT;
				return LEX_LITERAL;
			}

		}
		case ';':
		{
			FST::FST graph(string, 1,
				FST::NODE(1,
					FST::RELATION(';', 0)
				));
			if (result = execute(graph)) {
				return LEX_SEMICOLON;
			}

		}
		case '<':
		{
			FST::FST graph_l(string, 3,
				FST::NODE(1, FST::RELATION('<', 1)),
				FST::NODE(1, FST::RELATION('<', 2)),
				FST::NODE());
			if (result = execute(graph_l)) {
				return LEX_SHR;
				break;
			}
			FST::FST graph(string, 1,
				FST::NODE(1,
					FST::RELATION('<', 0)
				));
			if (result = execute(graph)) {
				return LEX_LESS;
				break;
			}

		}
		case '>':
		{
			FST::FST graph_r(string, 3,
				FST::NODE(1,FST::RELATION('>', 1)),
				FST::NODE(1,FST::RELATION('>', 2)),
				FST::NODE());
			if (result = execute(graph_r)) {
				return LEX_SHR;
				break;
			}

			FST::FST graph(string, 1,
				FST::NODE(1,
					FST::RELATION('>', 0)
				));
			if (result = execute(graph)) {
				return LEX_MORE;
				break;
			}

		}
		case '=':
		{
			FST::FST graph(string, 3,
				FST::NODE(1, FST::RELATION('=', 1)),
				FST::NODE(1, FST::RELATION('=', 2)),
				FST::NODE());
			if (result = execute(graph)) {
				return LEX_EQUALS;
			}

			FST::FST graph_assign(string, 1,
				FST::NODE(1,
					FST::RELATION('=', 0)
				));
			if (result = execute(graph_assign)) {
				return LEX_ASSIGN;
			}

		}case '!':
		{
			FST::FST graph(string, 3,
				FST::NODE(1, FST::RELATION('!', 1)),
				FST::NODE(1, FST::RELATION('=', 2)),
				FST::NODE());
			if (result = execute(graph)) {
				return LEX_NOT_EQUALS;
			}

		}
		case '^':
		{
			FST::FST graph(string, 1,
				FST::NODE(1,
					FST::RELATION('^', 0)
				));
			if (result = execute(graph)) {
				return LEX_XOR;
				break;
			}
		}
		case 'b':
		{
			FST::FST graph_byte(string, 5,
				FST::NODE(1, FST::RELATION('b', 1)),
				FST::NODE(1, FST::RELATION('y', 2)),
				FST::NODE(1, FST::RELATION('t', 3)),
				FST::NODE(1, FST::RELATION('e', 4)),
				FST::NODE());
			if (result = execute(graph_byte)) {
				entry->iddatatype = IT::IDDATATYPE::BYTE;
				return LEX_BYTE;
				break;
			}
			FST::FST graph_bool(string, 5,
				FST::NODE(1, FST::RELATION('b', 1)),
				FST::NODE(1, FST::RELATION('o', 2)),
				FST::NODE(1, FST::RELATION('o', 3)),
				FST::NODE(1, FST::RELATION('l', 4)),
				FST::NODE());
			if (result = execute(graph_bool)) {
				entry->iddatatype = IT::IDDATATYPE::BOOL;
				return LEX_BOOL;
				break;
			}
		}
		case 'd':
		{
			FST::FST graph_date(string, 5,
				FST::NODE(1, FST::RELATION('d', 1)),
				FST::NODE(1, FST::RELATION('a', 2)),
				FST::NODE(1, FST::RELATION('t', 3)),
				FST::NODE(1, FST::RELATION('e', 4)),
				FST::NODE());
			if (result = execute(graph_date)) {
				entry->idtype = IT::IDTYPE::E;
				entry->iddatatype = IT::IDDATATYPE::STR;
				return LEX_ID;
				break;
			}

		}
		case 'e':
		{
			FST::FST graph_else(string, 5,
				FST::NODE(1, FST::RELATION('e', 1)),
				FST::NODE(1, FST::RELATION('l', 2)),
				FST::NODE(1, FST::RELATION('s', 3)),
				FST::NODE(1, FST::RELATION('e', 4)),
				FST::NODE());
			if (result = execute(graph_else)) {
				return LEX_ELSE;
				break;
			}
		}
		case 'f':
		{
			FST::FST graph_function(string, 9,
				FST::NODE(1, FST::RELATION('f', 1)),
				FST::NODE(1, FST::RELATION('u', 2)),
				FST::NODE(1, FST::RELATION('n', 3)),
				FST::NODE(1, FST::RELATION('c', 4)),
				FST::NODE(1, FST::RELATION('t', 5)),
				FST::NODE(1, FST::RELATION('i', 6)),
				FST::NODE(1, FST::RELATION('o', 7)),
				FST::NODE(1, FST::RELATION('n', 8)),
				FST::NODE());
			if (result = execute(graph_function)) {
				entry->idtype = IT::IDTYPE::F;
				return LEX_FUNCTION;
			}
			FST::FST graph_float(string, 6,
				FST::NODE(1, FST::RELATION('f', 1)),
				FST::NODE(1, FST::RELATION('l', 2)),
				FST::NODE(1, FST::RELATION('o', 3)),
				FST::NODE(1, FST::RELATION('a', 4)),
				FST::NODE(1, FST::RELATION('t', 5)),
				FST::NODE());
			if (result = execute(graph_float)) {
				entry->iddatatype = IT::IDDATATYPE::FLOAT;
				return LEX_FLOAT;
			}
			FST::FST graph_false(string, 6,
				FST::NODE(1, FST::RELATION('f', 1)),
				FST::NODE(1, FST::RELATION('a', 2)),
				FST::NODE(1, FST::RELATION('l', 3)),
				FST::NODE(1, FST::RELATION('s', 4)),
				FST::NODE(1, FST::RELATION('e', 5)),
				FST::NODE());
			if (result = execute(graph_false)) {
				entry->iddatatype = IT::IDDATATYPE::LONG;
				entry->idcalculus = IT::CALCULUS::DEC;
				strcpy(string, "0");
				entry->value.vbool = false;
				return LEX_LITERAL;
			}

		}
		case 'i':
		{
			FST::FST graph_include(string, 8,
				FST::NODE(1, FST::RELATION('i', 1)),
				FST::NODE(1, FST::RELATION('n', 2)),
				FST::NODE(1, FST::RELATION('c', 3)),
				FST::NODE(1, FST::RELATION('l', 4)),
				FST::NODE(1, FST::RELATION('u', 5)),
				FST::NODE(1, FST::RELATION('d', 6)),
				FST::NODE(1, FST::RELATION('e', 7)),
				FST::NODE());
			if (result = execute(graph_include)) {
				return LEX_ID;
				break;
			}
			FST::FST graph_canditional(string, 3,
				FST::NODE(1, FST::RELATION('i', 1)),
				FST::NODE(1, FST::RELATION('f', 2)),
				FST::NODE());
			if (result = execute(graph_canditional)) {
				return LEX_IF;
				break;
			}

		}
		case 'l':
		{
			FST::FST graph_int(string, 5,
				FST::NODE(1, FST::RELATION('l', 1)),
				FST::NODE(1, FST::RELATION('o', 2)),
				FST::NODE(1, FST::RELATION('n', 3)),
				FST::NODE(1, FST::RELATION('g', 4)),
				FST::NODE());
			if (result = execute(graph_int)) {
				entry->iddatatype = IT::IDDATATYPE::LONG;
				return LEX_LONG;
				break;
			}
		}
		case 'm':
		{
			FST::FST graph_main(string, 5,
				FST::NODE(1, FST::RELATION('m', 1)),
				FST::NODE(1, FST::RELATION('a', 2)),
				FST::NODE(1, FST::RELATION('i', 3)),
				FST::NODE(1, FST::RELATION('n', 4)),
				FST::NODE());
			if (result = execute(graph_main)) {
				entry->iddatatype = IT::IDDATATYPE::LONG;
				entry->idtype = IT::IDTYPE::F;
				return LEX_MAIN;
			}
		}
		case 'r':
		{
			FST::FST graph_randome(string, 8,
				FST::NODE(1, FST::RELATION('r', 1)),
				FST::NODE(1, FST::RELATION('a', 2)),
				FST::NODE(1, FST::RELATION('n', 3)),
				FST::NODE(1, FST::RELATION('d', 4)),
				FST::NODE(1, FST::RELATION('o', 5)),
				FST::NODE(1, FST::RELATION('m', 6)),
				FST::NODE(1, FST::RELATION('e', 7)),
				FST::NODE());
			if (result = execute(graph_randome)) {
				entry->idtype = IT::IDTYPE::E;
				entry->iddatatype = IT::IDDATATYPE::LONG;
				return LEX_ID;
				break;
			}
			FST::FST graph_return(string, 7,
				FST::NODE(1, FST::RELATION('r', 1)),
				FST::NODE(1, FST::RELATION('e', 2)),
				FST::NODE(1, FST::RELATION('t', 3)),
				FST::NODE(1, FST::RELATION('u', 4)),
				FST::NODE(1, FST::RELATION('r', 5)),
				FST::NODE(1, FST::RELATION('n', 6)),
				FST::NODE());
			if (result = execute(graph_return)) {
				return LEX_RETURN;
				break;
			}
			FST::FST graph_repeat(string, 7,
				FST::NODE(1, FST::RELATION('r', 1)),
				FST::NODE(1, FST::RELATION('e', 2)),
				FST::NODE(1, FST::RELATION('p', 3)),
				FST::NODE(1, FST::RELATION('e', 4)),
				FST::NODE(1, FST::RELATION('a', 5)),
				FST::NODE(1, FST::RELATION('t', 6)),
				FST::NODE());
			if (result = execute(graph_repeat)) {
				return LEX_REPEAT;
				break;
			}

		}
		case 's':
		{
			FST::FST graph_string(string, 7,
				FST::NODE(1, FST::RELATION('s', 1)),
				FST::NODE(1, FST::RELATION('t', 2)),
				FST::NODE(1, FST::RELATION('r', 3)),
				FST::NODE(1, FST::RELATION('i', 4)),
				FST::NODE(1, FST::RELATION('n', 5)),
				FST::NODE(1, FST::RELATION('g', 6)),
				FST::NODE());
			if (result = execute(graph_string)) {
				entry->iddatatype = IT::IDDATATYPE::STR;
				return LEX_STRING;
				break;
			}
		}
		case 't':
		{
			FST::FST graph_true(string, 5,
				FST::NODE(1, FST::RELATION('t', 1)),
				FST::NODE(1, FST::RELATION('r', 2)),
				FST::NODE(1, FST::RELATION('u', 3)),
				FST::NODE(1, FST::RELATION('e', 4)),
				FST::NODE());
			if (result = execute(graph_true)) {
				entry->iddatatype = IT::IDDATATYPE::LONG;
				entry->idcalculus = IT::CALCULUS::DEC;
				strcpy(string, "1");
				entry->value.vbool = true;
				return LEX_LITERAL;
			}
		}
		case 'u':
		{
			FST::FST graph_until(string, 6,
				FST::NODE(1, FST::RELATION('u', 1)),
				FST::NODE(1, FST::RELATION('n', 2)),
				FST::NODE(1, FST::RELATION('t', 3)),
				FST::NODE(1, FST::RELATION('i', 4)),
				FST::NODE(1, FST::RELATION('l', 5)),
				FST::NODE());
			if (result = execute(graph_until)) {
				return LEX_UNTIL;
				break;
			}

		}
		case 'w':
		{
			FST::FST graph_write(string, 6,
				FST::NODE(1, FST::RELATION('w', 1)),
				FST::NODE(1, FST::RELATION('r', 2)),
				FST::NODE(1, FST::RELATION('i', 3)),
				FST::NODE(1, FST::RELATION('t', 4)),
				FST::NODE(1, FST::RELATION('e', 5)),
				FST::NODE());
			if (result = execute(graph_write)) {
				entry->idtype = IT::IDTYPE::E;
				return LEX_WRITE;
				break;
			}
			FST::FST graph_writeline(string, 10,
				FST::NODE(1, FST::RELATION('w', 1)),
				FST::NODE(1, FST::RELATION('r', 2)),
				FST::NODE(1, FST::RELATION('i', 3)),
				FST::NODE(1, FST::RELATION('t', 4)),
				FST::NODE(1, FST::RELATION('e', 5)),
				FST::NODE(1, FST::RELATION('l', 6)),
				FST::NODE(1, FST::RELATION('i', 7)),
				FST::NODE(1, FST::RELATION('n', 8)),
				FST::NODE(1, FST::RELATION('e', 9)),
				FST::NODE());
			if (result = execute(graph_writeline)) {
				entry->idtype = IT::IDTYPE::E;
				return LEX_WRITEL;
				break;
			}
		}
		case '{':
		{
			FST::FST graph(string, 1,
				FST::NODE(1,
					FST::RELATION('{', 0)
				));
			if (result = execute(graph)) {
				return LEX_LEFTBRACE;
			}

		}
		case '}':
		{
			FST::FST graph(string, 1,
				FST::NODE(1,
					FST::RELATION('}', 0)
				));
			if (result = execute(graph)) {
				return LEX_RIGHTBRACE;
			}

		}
		case '~':
		{
			FST::FST graph(string, 1,
				FST::NODE(1,
					FST::RELATION('~', 0)
				));
			if (result = execute(graph)) {
				return LEX_INVERSION;
				break;
			}
		}
		case '\'':
		{
			FST::FST graph_literal(string, 3,
				FST::NODE(1,FST::RELATION('\'', 1)),
				FST::NODE(146,
					FST::RELATION('a', 1),
					FST::RELATION('b', 1),
					FST::RELATION('c', 1),
					FST::RELATION('d', 1),
					FST::RELATION('e', 1),
					FST::RELATION('f', 1),
					FST::RELATION('g', 1),
					FST::RELATION('h', 1),
					FST::RELATION('k', 1),
					FST::RELATION('l', 1),
					FST::RELATION('m', 1),
					FST::RELATION('n', 1),
					FST::RELATION('o', 1),
					FST::RELATION('p', 1),
					FST::RELATION('q', 1),
					FST::RELATION('r', 1),
					FST::RELATION('s', 1),
					FST::RELATION('t', 1),
					FST::RELATION('u', 1),
					FST::RELATION('v', 1),
					FST::RELATION('w', 1),
					FST::RELATION('x', 1),
					FST::RELATION('y', 1),
					FST::RELATION('z', 1),
					FST::RELATION('A', 1),
					FST::RELATION('B', 1),
					FST::RELATION('C', 1),
					FST::RELATION('D', 1),
					FST::RELATION('E', 1),
					FST::RELATION('F', 1),
					FST::RELATION('G', 1),
					FST::RELATION('H', 1),
					FST::RELATION('I', 1),
					FST::RELATION('L', 1),
					FST::RELATION('K', 1),
					FST::RELATION('L', 1),
					FST::RELATION('M', 1),
					FST::RELATION('N', 1),
					FST::RELATION('O', 1),
					FST::RELATION('P', 1),
					FST::RELATION('Q', 1),
					FST::RELATION('R', 1),
					FST::RELATION('S', 1),
					FST::RELATION('T', 1),
					FST::RELATION('U', 1),
					FST::RELATION('V', 1),
					FST::RELATION('W', 1),
					FST::RELATION('X', 1),
					FST::RELATION('Y', 1),
					FST::RELATION('Z', 1),
					FST::RELATION('1', 1),
					FST::RELATION('2', 1),
					FST::RELATION('3', 1),
					FST::RELATION('4', 1),
					FST::RELATION('5', 1),
					FST::RELATION('6', 1),
					FST::RELATION('7', 1),
					FST::RELATION('8', 1),
					FST::RELATION('9', 1),
					FST::RELATION('0', 1),
					FST::RELATION(' ', 1),
					FST::RELATION('А', 1),
					FST::RELATION('Б', 1),
					FST::RELATION('В', 1),
					FST::RELATION('Г', 1),
					FST::RELATION('Д', 1),
					FST::RELATION('Е', 1),
					FST::RELATION('Ё', 1),
					FST::RELATION('Ж', 1),
					FST::RELATION('З', 1),
					FST::RELATION('И', 1),
					FST::RELATION('К', 1),
					FST::RELATION('Л', 1),
					FST::RELATION('М', 1),
					FST::RELATION('Н', 1),
					FST::RELATION('О', 1),
					FST::RELATION('П', 1),
					FST::RELATION('Р', 1),
					FST::RELATION('С', 1),
					FST::RELATION('Т', 1),
					FST::RELATION('У', 1),
					FST::RELATION('Ф', 1),
					FST::RELATION('Х', 1),
					FST::RELATION('Ц', 1),
					FST::RELATION('Ч', 1),
					FST::RELATION('Ш', 1),
					FST::RELATION('Щ', 1),
					FST::RELATION('Ь', 1),
					FST::RELATION('Ъ', 1),
					FST::RELATION('Э', 1),
					FST::RELATION('Ю', 1),
					FST::RELATION('Я', 1),
					FST::RELATION('а', 1),
					FST::RELATION('б', 1),
					FST::RELATION('в', 1),
					FST::RELATION('г', 1),
					FST::RELATION('д', 1),
					FST::RELATION('е', 1),
					FST::RELATION('ё', 1),
					FST::RELATION('ж', 1),
					FST::RELATION('з', 1),
					FST::RELATION('и', 1),
					FST::RELATION('к', 1),
					FST::RELATION('л', 1),
					FST::RELATION('м', 1),
					FST::RELATION('н', 1),
					FST::RELATION('о', 1),
					FST::RELATION('п', 1),
					FST::RELATION('р', 1),
					FST::RELATION('с', 1),
					FST::RELATION('т', 1),
					FST::RELATION('у', 1),
					FST::RELATION('ф', 1),
					FST::RELATION('х', 1),
					FST::RELATION('ц', 1),
					FST::RELATION('ч', 1),
					FST::RELATION('ш', 1),
					FST::RELATION('щ', 1),
					FST::RELATION('ь', 1),
					FST::RELATION('ъ', 1),
					FST::RELATION('э', 1),
					FST::RELATION('ю', 1),
					FST::RELATION('я', 1),
					FST::RELATION('-', 1),
					FST::RELATION('=', 1),
					FST::RELATION('+', 1),
					FST::RELATION('*', 1),
					FST::RELATION(';', 1),
					FST::RELATION(':', 1),
					FST::RELATION('(', 1),
					FST::RELATION(')', 1),
					FST::RELATION('{', 1),
					FST::RELATION('}', 1),
					FST::RELATION('!', 1),
					FST::RELATION('?', 1),
					FST::RELATION(',', 1),
					FST::RELATION('.', 1),
					FST::RELATION('<', 1),
					FST::RELATION('>', 1),
					FST::RELATION('№', 1),
					FST::RELATION('#', 1),
					FST::RELATION('&', 1),
					FST::RELATION('@', 1),
					FST::RELATION('\\',1),
					FST::RELATION('~',1),
					FST::RELATION('\'',2)),
				FST::NODE());
			if (result = execute(graph_literal)) {
				entry->iddatatype = IT::IDDATATYPE::STR;
				return LEX_LITERAL;
				break;
			}
		}
		default:
		{
			FST::FST graph_identificator(string, 1,
				FST::NODE(28,
					FST::RELATION('a', 0),
					FST::RELATION('b', 0),
					FST::RELATION('c', 0),
					FST::RELATION('d', 0),
					FST::RELATION('e', 0),
					FST::RELATION('f', 0),
					FST::RELATION('g', 0),
					FST::RELATION('h', 0),
					FST::RELATION('i', 0),
					FST::RELATION('j', 0),
					FST::RELATION('k', 0),
					FST::RELATION('l', 0),
					FST::RELATION('m', 0),
					FST::RELATION('n', 0),
					FST::RELATION('o', 0),
					FST::RELATION('p', 0),
					FST::RELATION('q', 0),
					FST::RELATION('r', 0),
					FST::RELATION('s', 0),
					FST::RELATION('t', 0),
					FST::RELATION('u', 0),
					FST::RELATION('v', 0),
					FST::RELATION('w', 0),
					FST::RELATION('x', 0),
					FST::RELATION('y', 0),
					FST::RELATION('z', 0),
					FST::RELATION('_', 0),
					FST::RELATION('.', 0)),
				FST::NODE());
			if (result = execute(graph_identificator)) {
				return LEX_ID;
				break;
			}
		}
		}
		return 0;
	}

	bool alphaCirillicDigit(char symbol) {
		if (isalpha(symbol))
			return true;
		else if (isdigit(symbol))
			return true;
		else if (symbol >= 'а' && symbol <= 'я')
			return true;
		else
			return false;
	}

}
