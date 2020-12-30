#include "stdafx.h"

namespace SA {
	void SemanticAnalises(LT::LexTable* lextable, IT::IdTable* idtable)
	{
		IT::IDDATATYPE datatype = IT::NUL;

		for (int i = 0; i < lextable->size; i++)
		{

			if (lextable->table[i].lexema[0] == EXPRESSIONS && lextable->table[i].lexema[0] != LEX_WRITE) {
				for (; lextable->table[i].lexema[0] != LEX_SEMICOLON && lextable->table[i].lexema[0] != LEX_WRITE; i++) {
					if (lextable->table[i].lexema[0] == LEX_LEFTHESIS)
						break;
					if (lextable->table[i].idxTI != TI_NULLIDX)
					{
						if (idtable->table[lextable->table[i].idxTI].iddatatype == IT::STR && lextable->table[i].lexema[0] != LEX_WRITE)
							throw ERROR_THROW_LINE(149, lextable->table[i].sn);
					}
				}

			}
			
			else if (lextable->table[i].lexema[0] == LEX_ID && lextable->table[i - 1].lexema[0] != LEX_FUNCTION)
			{
				if (lextable->table[i].idxTI != TI_NULLIDX)
				{
					if (idtable->table[lextable->table[i].idxTI].idtype == IT::F)
					{
						int posparam = lextable->table[i].idxTI + 1;
						i++;
						for (int j = 0; lextable->table[i].lexema[0] != LEX_RIGHTESIS; i++)
						{
							if (lextable->table[i].idxTI != TI_NULLIDX)
							{
								switch (idtable->table[lextable->table[i].idxTI].iddatatype)
								{
								case IT::IDDATATYPE::LONG:
								{
									if (idtable->table[posparam++].iddatatype != IT::IDDATATYPE::LONG)
										throw ERROR_THROW_LINE(142, lextable->table[i].sn);
									break;
								}
								case IT::IDDATATYPE::STR:
								{
									if (idtable->table[posparam++].iddatatype != IT::IDDATATYPE::STR)
										throw ERROR_THROW_LINE(142, lextable->table[i].sn);
									break;
								}
								case IT::IDDATATYPE::CHAR:
								{
									if (idtable->table[posparam++].iddatatype != IT::IDDATATYPE::CHAR)
										throw ERROR_THROW_LINE(142, lextable->table[i].sn);
									break;
								}
								case IT::IDDATATYPE::BOOL:
								{
									if (idtable->table[posparam++].iddatatype != IT::IDDATATYPE::BOOL)
										throw ERROR_THROW_LINE(142, lextable->table[i].sn);
									break;
								}
								default: break;
								}
							}
						}
					}
					if (lextable->table[i].idxTI != TI_NULLIDX)
					if (idtable->table[lextable->table[i].idxTI].idtype == IT::E)
					{
						int posexpfunc = lextable->table[i].idxTI;
						i++;
						for (int j = 0; lextable->table[i].lexema[0] != LEX_RIGHTESIS; i++)
						{
							if (lextable->table[i].idxTI != TI_NULLIDX)
							{
								if (!strcmp(idtable->table[posexpfunc].extfunct, "strlen"))
									for (int j = 0; lextable->table[i].lexema[0] != LEX_RIGHTESIS; i++)
									{
										if (lextable->table[i].idxTI != TI_NULLIDX)
											if (idtable->table[lextable->table[i].idxTI].iddatatype != IT::IDDATATYPE::STR)
												throw ERROR_THROW_LINE(142, lextable->table[i].sn);
									}
								else if (!strcmp(idtable->table[posexpfunc].extfunct, "strcat"))
									for (int j = 0; lextable->table[i].lexema[0] != LEX_RIGHTESIS; i++)
									{
										if (lextable->table[i].idxTI != TI_NULLIDX)
											if (idtable->table[lextable->table[i].idxTI].iddatatype != IT::IDDATATYPE::STR)
												throw ERROR_THROW_LINE(142, lextable->table[i].sn);
									}
								else if (!strcmp(idtable->table[posexpfunc].extfunct, "strcopy"))
									for (int j = 0; lextable->table[i].lexema[0] != LEX_RIGHTESIS; i++)
									{
										if (lextable->table[i].idxTI != TI_NULLIDX)
											if (idtable->table[lextable->table[i].idxTI].iddatatype != IT::IDDATATYPE::STR)
												throw ERROR_THROW_LINE(142, lextable->table[i].sn);
									}
								else if (!strcmp(idtable->table[posexpfunc].extfunct, "random"))
									for (int j = 0; lextable->table[i].lexema[0] != LEX_RIGHTESIS; i++)
									{
										if (lextable->table[i].idxTI != TI_NULLIDX)
											if (idtable->table[lextable->table[i].idxTI].iddatatype != IT::IDDATATYPE::LONG)
												throw ERROR_THROW_LINE(142, lextable->table[i].sn);
									}
								i--;
							}
						}
					}
				}
			}
			else if (lextable->table[i].lexema[0] == LEX_ID && lextable->table[i - 1].lexema[0] == LEX_FUNCTION || lextable->table[i].lexema[0] == LEX_MAIN)
			{
				datatype = idtable->table[lextable->table[i].idxTI].iddatatype;
			}
			else if (lextable->table[i].lexema[0] == LEX_RETURN)
			{
				switch (idtable->table[lextable->table[i+1].idxTI].iddatatype)
				{
				case IT::IDDATATYPE::LONG:
				{
					if (datatype != IT::IDDATATYPE::LONG)
						throw ERROR_THROW_LINE(144, lextable->table[i].sn);
					break;
				}
				case IT::IDDATATYPE::STR:
				{
					if (datatype != IT::IDDATATYPE::STR)
						throw ERROR_THROW_LINE(144, lextable->table[i].sn);
					break;
				}
				case IT::IDDATATYPE::CHAR:
				{
					if (datatype != IT::IDDATATYPE::CHAR)
						throw ERROR_THROW_LINE(144, lextable->table[i].sn);
					break;
				}
				case IT::IDDATATYPE::BOOL:
				{
					if (datatype != IT::IDDATATYPE::BOOL)
						throw ERROR_THROW_LINE(144, lextable->table[i].sn);
					break;
				}
				default: break;
				}
			}
		}
	}
}