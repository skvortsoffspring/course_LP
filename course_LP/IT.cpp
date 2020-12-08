#include "stdafx.h"

#define ENTRY	idtable.table[idtable.current_size]
namespace IT
{
	IdTable Create(int size)
	{
		IdTable idtable;
		idtable.current_size = 0;
		idtable.maxsize = size;
		idtable.table = new Entry[size];
		return idtable;
	}

	void Add(IdTable& idtable, Entry entry)
	{

		if (idtable.current_size < idtable.maxsize)
		{
			if(entry.idtype != L)
				if (entry.iddatatype == IT::IDDATATYPE::LONG)
					idtable.table[idtable.current_size].value.vint = TI_LONG_DEFAULT;
				else if (entry.iddatatype == IT::IDDATATYPE::BYTE)
				{
					idtable.table[idtable.current_size].value.vchar = 0x00;
				}
				else if (entry.iddatatype == IT::IDDATATYPE::FLOAT)
				{
					idtable.table[idtable.current_size].value.vfloat = TI_LONG_DEFAULT;
				}
				else if (entry.iddatatype == IT::IDDATATYPE::STR)
				{
					idtable.table[idtable.current_size].value.vstr->len = TI_DEFAULT;
				}
				else if (entry.iddatatype == IT::IDDATATYPE::BOOL)
				{
					idtable.table[idtable.current_size].value.vbool = TI_DEFAULT;
				}
			
			idtable.current_size++;
		}
		else
			throw ERROR_THROW(122);

	}

	Entry GetEntry(IdTable& idtable, int n)
	{
			return idtable.table[n];
	}

	int IsId(IdTable& idtable, char* id,char* prefix, int line,bool visibility,IDDATATYPE dtype, IDTYPE ttype) {
		for (int i = 0; i < idtable.current_size; i++)
		{
			if (!strcmp(idtable.table[i].id, id) && idtable.table[i].idtype == F) {
				idtable.table[idtable.current_size].idtype = F;
				idtable.table[idtable.current_size].iddatatype = idtable.table[i].iddatatype;
				visibility = true;
				return idtable.table[i].idxfirstLE;
			}
			
			if (!strcmp(idtable.table[i].id, id) && !strcmp(idtable.table[i].prefix, prefix))
			{
				if (idtable.table[i].idtype == F && idtable.table[i].idtype == ttype)
					throw ERROR_THROW_LINE(125, line);
				if (idtable.table[i].idtype == V &&  idtable.table[i].idtype == ttype)
					throw ERROR_THROW_LINE(126, line);
				if (idtable.table[i].idtype == P && idtable.table[idtable.current_size].idtype == V)
					throw ERROR_THROW_LINE(126, line);
				idtable.table[idtable.current_size].idtype = idtable.table[i].idtype;
				return idtable.table[i].idxfirstLE;

			}
			if (!strncmp(idtable.table[i].id, id, 10) && dtype)
			{
				if (idtable.table[i].idtype == IT::F)
					return idtable.table[i].idxfirstLE;
			}
		}
		return LT_TI_NULLXDX;
	}

	int IsLiteral(IdTable& idtable, char* str) {
		for (int i = 0; i < idtable.current_size; i++)
		{
			if (!strcmp(idtable.table[i].value.vstr->str, str))
				return i;
		}
		return LT_TI_NULLXDX;
	}
	int IsLiteral(IdTable& idtable,unsigned int vint) {
		for (int i = 0; i < idtable.current_size; i++)
		{
			if (idtable.table[i].value.vint == vint)
				return i;
		}
		return LT_TI_NULLXDX;
	}
	int IsLiteral(IdTable& idtable, unsigned char vchar)
	{
		for (int i = 0; i < idtable.current_size; i++)
		{
			if (idtable.table[i].value.vint == vchar)
				return i;
		}
		return LT_TI_NULLXDX;
	}
	int IsLiteral(IdTable& idtable, float vfloat) {
		for (int i = 0; i < idtable.current_size; i++)
		{
			if (idtable.table[i].value.vint == vfloat)
				return i;
		}
		return LT_TI_NULLXDX;
	}

	void LiteralCreate(IdTable& idtable, char* string, int countLine,bool& negativeValue, bool parametres)
	{	
		idtable.table[idtable.current_size].idtype = L;
		int ID;
		long long value;
		switch (idtable.table[idtable.current_size].idcalculus) {

			case HEX:
				value = std::stoll(string, nullptr, 16);
				break;
			case OCT:
				value = std::stoll(string, nullptr, 8);
				break;
			case BIN:
				if (*string == 'b')string++;
				else string = string + 2;
				value = std::stoll(string, nullptr, 2);
				break;
			case DEC:
				value = atoll(string);
				break;

		}

		switch (idtable.table[idtable.current_size - 1].iddatatype)				// check data type for type variable
		{

			case BYTE:
			{
				if (idtable.table[idtable.current_size].iddatatype != LONG && parametres)
					throw ERROR_THROW_LINE(132, countLine);
			}
			case BOOL:
			{
				if ((idtable.table[idtable.current_size].iddatatype == LONG ||
					idtable.table[idtable.current_size].iddatatype == BOOL) &&
					parametres)
					throw ERROR_THROW_LINE(132, countLine);
			}
			case LONG:
			{
				if (idtable.table[idtable.current_size].iddatatype != LONG && parametres) {
					throw ERROR_THROW_LINE(132, countLine);
				}					
			}
			case FLOAT:
			{
				if (idtable.table[idtable.current_size].iddatatype != FLOAT && parametres)
					throw ERROR_THROW_LINE(132, countLine);
			}
			case STR:
			{
				if (idtable.table[idtable.current_size].iddatatype != STR && parametres)
					throw ERROR_THROW_LINE(132, countLine);
			}
		}

		switch (idtable.table[idtable.current_size].iddatatype)					// add literal for table id
		{

		case BYTE:
		{
			if (int(value) > UCHAR_MAX || int(value) < NULL)	// limit char
				throw ERROR_THROW_LINE(129, countLine);

			ID = IsLiteral(idtable, (unsigned int)value);

			if (ID == LT_TI_NULLXDX) {
				idtable.table[idtable.current_size].iddatatype = BYTE;
				idtable.table[idtable.current_size].value.vchar = (unsigned char)value;
			}
			else
			{
				idtable.table[idtable.current_size].idxfirstLE = ID;
			}
			IT::Add(idtable, idtable.table[idtable.current_size]);

			break;
		}
		case BOOL:
		{
			if (int(value) > true || int(value) < false)	// limit char
				throw ERROR_THROW_LINE(129, countLine);

			ID = IsLiteral(idtable, (unsigned int)value);

			if (ID == LT_TI_NULLXDX) {
				idtable.table[idtable.current_size].iddatatype = BOOL;
				idtable.table[idtable.current_size].value.vbool = (bool)value;
			}
			else
			{
				idtable.table[idtable.current_size].idxfirstLE = ID;
			}
			IT::Add(idtable, idtable.table[idtable.current_size]);

			break;
		}
		case LONG:
		{
			

			if (value > ULONG_MAX || value < NULL)				// limit int
				throw ERROR_THROW_LINE(130, countLine);

			ID = IsLiteral(idtable, (unsigned int)value);

			if (ID == LT_TI_NULLXDX) {
				idtable.table[idtable.current_size].iddatatype = LONG;
				idtable.table[idtable.current_size].value.vint = (unsigned int)value;
			}
			else
			{
				idtable.table[idtable.current_size].idxfirstLE = ID;
			}
			IT::Add(idtable, idtable.table[idtable.current_size]);

			break;
		}
		case FLOAT:
		{
			char a = '.';
			char *index = strchr(string,a);
			*index = ',';
			double valuef = (atof(string));

			if (valuef > FLOAT_MAX || valuef < FLOAT_MIN)
				throw ERROR_THROW_LINE(131, countLine);

			ID = IsLiteral(idtable, (float)valuef);

			if (ID == LT_TI_NULLXDX) {
				idtable.table[idtable.current_size].iddatatype = FLOAT;
				idtable.table[idtable.current_size].value.vfloat = (float)valuef;
			}
			else
			{
				idtable.table[idtable.current_size].idxfirstLE = ID;
			}
			IT::Add(idtable, idtable.table[idtable.current_size]);

			break;
		}
		case STR:
		{
			int size = (strlen(string) - 2);						// -2 - минус ковычки

			if (size > TI_STR_MAXSIZE)
				throw ERROR_THROW_LINE(123, countLine);

			char temp[TI_STR_MAXSIZE];
			strncpy_s(temp, (string + 1), size);
			ID = IsLiteral(idtable, temp);
			if (ID == LT_TI_NULLXDX) {
				idtable.table[idtable.current_size].iddatatype = IT::IDDATATYPE::STR;
				strcpy_s(idtable.table[idtable.current_size].value.vstr->str, temp);
				idtable.table[idtable.current_size].value.vstr->len = (char)(size);
				IT::Add(idtable, idtable.table[idtable.current_size]);
			}
			else
			{
				idtable.table[idtable.current_size].idxfirstLE = ID;
			}
			break;
		}
		}
		
		
		
		
	}

	void PrintIdTable(const wchar_t* out, IdTable& idtable)
	{
		short lengthRow = 20;
		std::fstream fout(out, std::ios::out);

		char buffer[48];
		time_t rawtime;
		struct tm* timeinfo;
		time(&rawtime);
		timeinfo = localtime(&rawtime);
		strftime(buffer, 48, "%d-%m-%Y --%A-- %H:%M:%S ", timeinfo);

		fout << "ID table, modified time: " << buffer << IN_CODE_ENDL << IN_CODE_ENDL;

		fout.setf(std::ios::left);
		fout.width(lengthRow); fout << "visible";
		fout.width(lengthRow); fout << "variable"; 
		fout.width(lengthRow); fout << "type";
		fout.width(lengthRow); fout << "value int";
		fout.width(lengthRow); fout << "value str";
		fout.width(lengthRow); fout << "idxfirstLE" << IN_CODE_ENDL;

		for (int i = 0; i < idtable.current_size; i++)
		{
			if (idtable.table[i].idtype == IT::IDTYPE::V) {
				fout.width(lengthRow);
				fout << idtable.table[i].prefix;
				fout.width(lengthRow);
				fout << idtable.table[i].id;

				if (idtable.table[i].iddatatype == BYTE) {
					fout.width(lengthRow); fout << "byte";
					if (!idtable.table[i].value.vchar) {
						fout.width(lengthRow); fout << "0x00";
					}
					else
					{
						fout.width(lengthRow); fout << idtable.table[i].value.vchar;
					}
					fout.width(lengthRow); fout << "-";
				}
				else if (idtable.table[i].iddatatype == BOOL) {
					fout.width(lengthRow); fout << "bool";
					fout.width(lengthRow); fout << idtable.table[i].value.vbool;
					fout.width(lengthRow); fout << "-";
				}
				else if (idtable.table[i].iddatatype == LONG) {
					fout.width(lengthRow); fout << "long";
					fout.width(lengthRow); fout << idtable.table[i].value.vint;
					fout.width(lengthRow); fout << "-";
				}
				else if (idtable.table[i].iddatatype == FLOAT) {
					fout.width(lengthRow); fout << "float";
					fout.width(lengthRow); fout << idtable.table[i].value.vfloat;
					fout.width(lengthRow); fout << "-";
				}
				else if (idtable.table[i].iddatatype == STR) {
					fout.width(lengthRow); fout << "string";
					fout.width(lengthRow); fout << "-";
					fout.width(lengthRow); fout << (int)idtable.table[i].value.vstr->len;
				}
				else 
					fout.width(lengthRow * 3 + 1); fout << " ";
								
				if (idtable.table[i].iddatatype)
				{
					fout << idtable.table[i].idxfirstLE;
				}
				else
				{
					fout << "TI[" << idtable.table[i].idxfirstLE << "]";
				}
				fout << IN_CODE_ENDL;
			}
			
		}
		fout << IN_CODE_ENDL;
		fout.setf(std::ios::left);
		fout.width(lengthRow); fout << "visible";
		fout.width(lengthRow); fout << "function";
		fout.width(lengthRow);	fout << "type";
		fout.width(lengthRow);	fout << "idfirstLE" << IN_CODE_ENDL;

		for (int i = 0; i < idtable.current_size; i++)
		{
			if (idtable.table[i].idtype == IT::IDTYPE::F) {

				fout.width(lengthRow); fout << idtable.table[i].prefix;
				fout.width(lengthRow); fout << idtable.table[i].id;

				if (idtable.table[i].iddatatype == IT::IDDATATYPE::LONG) {
					fout.width(lengthRow); fout << "long ";
				}
				else if (idtable.table[i].iddatatype == IT::IDDATATYPE::STR) {
					fout.width(lengthRow); fout << "string ";
				}				
				else if (idtable.table[i].iddatatype == IT::IDDATATYPE::BYTE) {
					fout.width(lengthRow); fout << "byte";
				}
				else if (idtable.table[i].iddatatype == IT::IDDATATYPE::FLOAT) {
					fout.width(lengthRow); fout << "float";
				}				
				else if (idtable.table[i].iddatatype == IT::IDDATATYPE::BOOL) {
					fout.width(lengthRow); fout << "bool";
				}
				fout.width(lengthRow); fout << idtable.table[i].idxfirstLE;

				fout << IN_CODE_ENDL;
			}

		}fout << IN_CODE_ENDL;
		fout.setf(std::ios::left);
		fout.width(lengthRow); fout << "visible";
		fout.width(lengthRow); fout << "ext function";
		fout.width(lengthRow);	fout << "type"; 
		fout.width(lengthRow);	fout << "idxFirstLE" << IN_CODE_ENDL;

		for (int i = 0; i < idtable.current_size; i++)
		{
			if (idtable.table[i].idtype == IT::IDTYPE::E) {

				fout.width(lengthRow); fout << idtable.table[i].prefix;
				fout.width(lengthRow); fout << idtable.table[i].extfunct;

				if (idtable.table[i].iddatatype == IT::IDDATATYPE::LONG) {
					fout.width(lengthRow); fout << "long ";
				}
				else if (idtable.table[i].iddatatype == IT::IDDATATYPE::STR) {
					fout.width(lengthRow); fout << "string ";
				}				
				else if (idtable.table[i].iddatatype == IT::IDDATATYPE::BYTE) {
					fout.width(lengthRow); fout << "byte";
				}
				else if (idtable.table[i].iddatatype == IT::IDDATATYPE::FLOAT) {
					fout.width(lengthRow); fout << "float";
				}				
				else if (idtable.table[i].iddatatype == IT::IDDATATYPE::BOOL) {
					fout.width(lengthRow); fout << "bool";
				}
				fout.width(lengthRow); fout << idtable.table[i].idxfirstLE;

				fout << IN_CODE_ENDL;
			}

		}
		/// <summary>
		/// </summary>
		/// <param name="out"></param>
		/// <param name="idtable"></param>
		fout << IN_CODE_ENDL;
		fout.setf(std::ios::left);
		fout.width(lengthRow); fout << "visible";
		fout.width(lengthRow); fout << "array";
	fout.width(lengthRow);	fout << "type"; 
		fout.width(lengthRow);	fout << "idxFirstLE" << IN_CODE_ENDL;

		for (int i = 0; i < idtable.current_size; i++)
		{
			if (idtable.table[i].idtype == IT::IDTYPE::A) {

				fout.width(lengthRow); fout << idtable.table[i].prefix;
				fout.width(lengthRow); fout << idtable.table[i].id;

				if (idtable.table[i].iddatatype == IT::IDDATATYPE::LONG) {
					fout.width(lengthRow); fout << "long ";
				}
				else if (idtable.table[i].iddatatype == IT::IDDATATYPE::STR) {
					fout.width(lengthRow); fout << "string ";
				}
				else if (idtable.table[i].iddatatype == IT::IDDATATYPE::BYTE) {
					fout.width(lengthRow); fout << "byte";
				}
				else if (idtable.table[i].iddatatype == IT::IDDATATYPE::FLOAT) {
					fout.width(lengthRow); fout << "float";
				}
				else if (idtable.table[i].iddatatype == IT::IDDATATYPE::BOOL) {
					fout.width(lengthRow); fout << "bool";
				}
				fout.width(lengthRow); fout << idtable.table[i].idxfirstLE;
				fout << IN_CODE_ENDL;
			}
			
		}
		/// <summary>
		/// 
		/// </summary>
		/// <param name="out"></param>
		/// <param name="idtable"></param>
		fout << IN_CODE_ENDL;
		fout.setf(std::ios::left);
		fout.width(lengthRow); fout << "visible";
		fout.width(lengthRow); fout << "parameter";
		fout.width(lengthRow); fout << "type";
		fout.width(lengthRow);	fout << "idxfirstle" << IN_CODE_ENDL;

		for (int i = 0; i < idtable.current_size; i++)
		{
			if (idtable.table[i].idtype == IT::IDTYPE::P) {

				if (idtable.table[i].id[0]) {
					fout.width(lengthRow); fout << idtable.table[i].prefix;
					fout.width(lengthRow); fout << idtable.table[i].id;
				}
				if (idtable.table[i].iddatatype == IT::IDDATATYPE::LONG) 
				{
					fout.width(lengthRow); 
					fout << "long ";
				}
				else if (idtable.table[i].iddatatype == IT::IDDATATYPE::BYTE) 
				{
					fout.width(lengthRow); 
					fout << "byte";
				}
				else if (idtable.table[i].iddatatype == IT::IDDATATYPE::BOOL) 
				{
					fout.width(lengthRow); 
					fout << "bool";
				}
				else if (idtable.table[i].iddatatype == IT::IDDATATYPE::FLOAT) 
				{
					fout.width(lengthRow); 
					fout << "float";
				}				
				else if (idtable.table[i].iddatatype == IT::IDDATATYPE::STR) 
				{
					fout.width(lengthRow); 
					fout << "string ";
				}
				else
				{
					fout.width(lengthRow);
					fout << "------";
				}

				if (idtable.table[i].idxfirstLE == TI_NULLIDX)
				{
					fout.width(lengthRow); fout << i;
				}
				else
				{
					fout << "TI[" << idtable.table[i].idxfirstLE << "]";
				}
				fout.width(lengthRow); fout << idtable.table[i].idxfirstLE;
				fout << IN_CODE_ENDL;
			}

		}

		fout << IN_CODE_ENDL;
		fout.setf(std::ios::left);
		fout.width(lengthRow); fout << "type";
		fout.width(lengthRow);	fout << "size(string)";
		fout.width(lengthRow);	fout << "idxfirstle";
		fout << "value" << IN_CODE_ENDL;

		for (int i = 0; i < idtable.current_size; i++)
		{
			if (idtable.table[i].idtype == IT::IDTYPE::L) {
				switch (idtable.table[i].iddatatype) {
					case LONG:
					{
						fout.width(lengthRow); fout << "long";
						fout.width(lengthRow); fout << "------";
						fout.width(lengthRow); fout << idtable.table[i].idxfirstLE;
						fout.width(lengthRow); fout << idtable.table[i].value.vfloat;
						break;
					}
					case FLOAT:
					{
						fout.width(lengthRow); fout << "float";
						fout.width(lengthRow); fout << "------";
						fout.width(lengthRow); fout << idtable.table[i].idxfirstLE;
						fout.width(lengthRow); fout << idtable.table[i].value.vfloat;
						break;
					}
					case STR:
					{
						fout.width(lengthRow); fout << "string ";
						if (idtable.table[i].idxfirstLE == TI_NULLIDX)
						{
							fout.width(lengthRow); fout << (int)idtable.table[i].value.vstr->len;
							fout.width(lengthRow); fout << i;
							fout.width(lengthRow); fout << idtable.table[i].value.vstr->str;
						}
						else
						{
							fout << "TI[" << idtable.table[i].idxfirstLE << "]";
						}
						break;
					}
				}
				fout << IN_CODE_ENDL;
			}

		}
		fout.close();
	}

	void Delete(IdTable& idtable) {
		delete[] idtable.table;
		idtable.table = nullptr;
	}
}