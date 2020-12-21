#pragma once
#define ID_MAXSIZE		10										// максимальное количество сиволов в идентификаторе
#define	PREFIX_SIZE		10
#define EXT_FUNCTION	32
#define TI_MAXSIZE		4096									// максимальное количество эл-ов в таблице идентификаторов 
#define TI_LONG_DEFAULT	0x00000000								// значение по умолчанию дл€ типа integer 
#define TI_DEFAULT		0x00									// значение по умолчанию дл€ типа string 
#define TI_NULLIDX		0xffffffff								// нет элемента таблицы идентификаторов
#define TI_STR_MAXSIZE	255
#define PARM_ID_DEFAULT_EXT L"file.id.txt"						// вывод идентификаторов и литералов
#define FLOAT_MAX		3.4028235E+38
#define FLOAT_MIN		-3.4028235E+38

namespace IT													// таблица идентификатов
{
	enum IDDATATYPE { NUL, LONG, STR, CHAR, FLOAT, BOOL };		// типы данных идентификаторов: integer, string, byte
	enum IDTYPE		{ N, V, F, P, L, S, E , A, C, W, K, PA};	// типы идентификаторов: переменна€, функци€, параметр, 
																// литерал, внешн€€ функци€, массив, константа, указатель
	enum CALCULUS	{ NC, DEC, OCT, HEX, BIN };					// системы исчислени€
	
	struct Entry												// строка таблицы идентификаторов
	{
		int idxfirstLE;											//  
		char id[ID_MAXSIZE + 1];								// идентификатор (автоматически усекаетс€ до ID_MAXSIZE
		char prefix[PREFIX_SIZE + 1];							// область видимости
		char extfunct[EXT_FUNCTION + 1];						// внешн€€ функци€
		int parametresfunc;
		IDDATATYPE iddatatype;									// тип данных
		IDTYPE idtype ;											// тип идентификатора
		CALCULUS idcalculus;
		union
		{
			unsigned char vchar;
			unsigned int vint;									// значение integer
			float vfloat;
			bool vbool;

			struct
			{
				unsigned char len;						// количество символов в string
				char str[TI_STR_MAXSIZE - 1];			// символы string
			} 
			vstr[TI_STR_MAXSIZE];						// значение string
		}value;											// значение идентификатора

		Entry()
		{
			idxfirstLE = TI_NULLIDX;
			memset(prefix, 0x00, PREFIX_SIZE);
			memset(id, 0x00, ID_MAXSIZE);
			memset(extfunct, 0x00, EXT_FUNCTION);
			parametresfunc = 0;
			iddatatype = NUL;
			idtype = N;
			idcalculus = NC;
			value.vint = 0;
			value.vbool = 0;
			value.vchar = 0;
		}
	};

	struct IdTable											// экземпл€р таблицы идентификаторов
	{
		int maxsize;										// емкость таблицы идентификаторов < TI_MAXSIZE
		int current_size;									// текущий размер таблицы идентификаторов < maxsize
		Entry* table;										// массив строк таблицы идентификаторов
	};

	IdTable Create(int);
	void Add(IdTable&, Entry);								// добавить в строку идентификаторов
	Entry GetEntry(IdTable&, int n);						// получить строку таблицы идентификаторов
	Entry AddLiteral(Entry&);								// 
	int IsId(IdTable&,LT::LexTable&, char[], char[], int, bool, IDDATATYPE,IDTYPE);					// возврат : возврат строки (если есть), TI_NULLIDX(если нет)
	int IsLiteral(IdTable&, char*);							// возрат IT_TI_NULLIDX если есть уже идентичный идентификатор
	int IsLiteral(IdTable&, unsigned int);					// возрат IT_TI_NULLIDX если есть уже идентичный идентификатор
	int IsLiteral(IdTable&, unsigned char);					// возрат IT_TI_NULLIDX если есть уже идентичный идентификатор
	int IsLiteral(IdTable&, float);							// возрат IT_TI_NULLIDX если есть уже идентичный идентификатор
	int LiteralCreate(IdTable,LT::LexTable, char*, int, bool&,bool);	// добавить литерал в таблицу
	void PrintIdTable(const wchar_t* in, IdTable&);			// печать таблицы идентификаторов
	void Delete(IdTable&);									// удалить таблицу лексем (освободить место)
	Entry Reset();
}