
#pragma once		

#define IN_MAX_LEN_TEXT	1024*1024	// максимальный размер исходного кода =	1MB									
#define IN_CODE_ENDL '\n'			// символ переноса
#define STR_ENDL '\0'               // конец строки
#define TAB '\t'
#define SPACE ' '
#define LITERAL '\''
/************************************************************************************************************/
/*	таблица	проверки входной информации.	индекс = код	(Windows-1251)	символа							*/
/*	значения IN::F	- заперщенный	символ,	IN::Т - разрешенный символ	,IN::F -игнорировать (не вводить)	*/
/*	значения IN::E	- символ для операций,	IN::S - невидимые, IN::F - символ для литералов             	*/
/*	если	0 <= значение	< 255 -	то вводится данное	значение											*/
/************************************************************************************************************/

#define IN_CODE_TABLE {\
/* 0  |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |  A  |  B  |  C  |  D  |  E  |  F  |    */\
 IN::F,IN::F,IN::F,IN::F,IN::F,IN::F,IN::F,IN::F,IN::F,IN::S,  '|',IN::F,IN::F,IN::T,IN::F,IN::F, /*0*/\
 IN::F,IN::F,IN::F,IN::F,IN::F,IN::F,IN::F,IN::F,IN::F,IN::F,IN::F,IN::F,IN::F,IN::F,IN::F,IN::F, /*1*/\
 IN::S,IN::T,IN::T,IN::L,IN::L,IN::L,IN::L,IN::T,IN::E,IN::E,IN::E,IN::E,IN::E,IN::E,IN::T,IN::T, /*2*/\
 IN::T,IN::T,IN::T,IN::T,IN::T,IN::T,IN::T,IN::T,IN::T,IN::T,IN::L,IN::E,IN::T,IN::E,IN::T,IN::L, /*3*/\
 IN::L,IN::T,IN::T,IN::T,IN::T,IN::T,IN::T,IN::T,IN::T,IN::T,IN::T,IN::T,IN::T,IN::T,IN::T,IN::T, /*4*/\
 IN::T,IN::T,IN::T,IN::T,IN::T,IN::T,IN::T,IN::T,IN::T,IN::T,IN::T,IN::T,IN::L,IN::T,IN::T,IN::T, /*5*/\
 IN::L,IN::T,IN::T,IN::T,IN::T,IN::T,IN::T,IN::T,IN::T,IN::T,IN::T,IN::T,IN::T,IN::T,IN::T,IN::T, /*6*/\
 IN::T,IN::T,IN::T,IN::T,IN::T,IN::T,IN::T,IN::T,IN::T,IN::T,IN::T,IN::E,IN::T,IN::E,IN::T,IN::F, /*7*/\
     																								   \
 IN::F,IN::F,IN::F,IN::F,IN::F,IN::F,IN::F,IN::F,IN::F,IN::F,IN::F,IN::F,IN::F,IN::F,IN::F,IN::F, /*8*/\
 IN::F,IN::F,IN::F,IN::F,IN::F,IN::F,IN::F,IN::F,IN::F,IN::F,IN::F,IN::F,IN::F,IN::F,IN::F,IN::F, /*9*/\
 IN::F,IN::F,IN::F,IN::F,IN::F,IN::F,IN::F,IN::F,IN::L,IN::L,IN::F,IN::F,IN::F,IN::F,IN::F,IN::F, /*A*/\
 IN::F,IN::F,IN::F,IN::F,IN::F,IN::F,IN::F,IN::F,IN::F,IN::L,IN::F,IN::F,IN::F,IN::F,IN::F,IN::F, /*B*/\
 IN::L,IN::L,IN::L,IN::L,IN::L,IN::L,IN::L,IN::L,IN::L,IN::L,IN::L,IN::L,IN::L,IN::L,IN::L,IN::L, /*C*/\
 IN::L,IN::L,IN::L,IN::L,IN::L,IN::L,IN::L,IN::L,IN::L,IN::L,IN::L,IN::L,IN::L,IN::L,IN::L,IN::L, /*D*/\
 IN::L,IN::L,IN::L,IN::L,IN::L,IN::L,IN::L,IN::L,IN::L,IN::L,IN::L,IN::L,IN::L,IN::L,IN::L,IN::L, /*E*/\
 IN::L,IN::L,IN::L,IN::L,IN::L,IN::L,IN::L,IN::L,IN::L,IN::L,IN::L,IN::L,IN::L,IN::L,IN::L,IN::L, /*F*/\
}


namespace In
{
    struct	IN										// исходные данные
    {
        enum { F, T, I, E, S, L };		            // T - допустимый символ. F - недопустимый,	I -	игнорировать, иначе заменить, E - выражения арифм/лог, S - пробел, L - литералы
        int	size = 0;								//	размер исходного кода													
        int	lines = 0;								//	количество строк														
        int	ignor = 0;								//	количество проигнорированных символов										
        unsigned char* text = nullptr;
        short code[256] = IN_CODE_TABLE;			//	таблица проверки: T, F, I, E, S новое значение
    };
    IN getin(wchar_t infile[]);					    //	ввести и проверить входной поток кода (Windows - 1251)	

};


