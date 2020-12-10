#pragma once
#include "LT.h"
#include "IT.h"
#define SEPARATER '|'
#define NEGATIVE '-'

namespace  GM
{
	struct Data
	{
		bool switch_string;						// acces all symbol for literal
		int count_lines;						// counts lines in array
		int count;	   							// number lexema
		int countparametres;					// count parametres function 
		char braces;							// count braces if value > 1 or < 0 error
		char token;								// saved returned token
		char prefix[PREFIX_SIZE+ 1];			// prefix for parametres and variables
		char positionfuncID;
		bool visibility_in_body;				// visibility for body function		bool visibility_in_parametres;		// visible for parametres
		char* string = nullptr;					// ptr for string
		bool negativeValue = false;				// negative values for string or may be other types
		bool visibility_in_parametres = false;
		bool check_type_return = false;
		IT::IDDATATYPE datatype = IT::NUL;

		Data() {
			switch_string = false;
			count_lines = 1;
			count = 0;
			token = 0;
			memset(prefix, 0, PREFIX_SIZE);
			visibility_in_body = false;
			visibility_in_parametres = false;
			countparametres = 0;
			positionfuncID = 0;
			braces = 0;
		}
	};
	
	char token_rekognizer(char* string, IT::Entry*);								// returned token
	void dataProcesing(unsigned char*, std::fstream*, LT::LexTable*, IT::IdTable*);	// data processing all expressions
	void IdentificatorCreate(IT::IdTable*,LT::LexTable*, Data&, std::fstream*);									// add identifier
	bool alphaCirillicDigit(char);													// checked symbol (latin, cirilic or digit)
}