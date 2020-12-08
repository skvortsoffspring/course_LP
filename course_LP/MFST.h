#pragma once
#include "GRB.h"
#include "LT.h"
#include <stack>
#include "Error.h"
#define MFST_DIAGN_MAXSIZE 2*ERROR_MAXSIZE_MESSAGE
#define MFST_DIAGN_NUMBER 3

template <typename T>
class Stack 
{
private:
	std::stack<T> steck;
public:
	void push(T st) {
		steck.push(st);
	}
	void pop() {
		steck.pop();
	}
	T top() {
		return steck.top();
	}
	int size() {
		return steck.size();
	}
	T operator[](int s) {
		std::stack<T> st;
		st = steck;
		for (int i = (s + 1); i < steck.size(); i++)
		{
			st.pop();
		}
		return st.top();
	}
};

typedef Stack<short> MFSTSTSTACK;

#define NS(n)	GBR::Rule::Chain::N(n)
#define TS(n)	GBR::Rule::Chain::T(n)
#define ISNS(n) GBR::Rule::Chain::isN(n)
                  
static int FST_TRACE_n = -1;
static char rbuf[205]{}, sbuf[205]{}, lbuf[1024]{};

#define MFST_TRACE_START	std::cout.setf(std::ios::left);									\
							std::cout.width (4); std::cout <<" Шаг"<< ':';					\
							std::cout.width(20); std::cout <<" Правило";					\
							std::cout.width(30); std::cout <<" Входная лента";				\
							std::cout.width(20); std::cout <<" Стек";						\
							std::cout <<std::endl;

#define MFST_TRACE1			std::cout.setf(std::ios::left);									\
							std::cout.width (4); std::cout<<++FST_TRACE_n << ": ";			\
							std::cout.width(20); std::cout<<rule.getCRule(rbuf, nrulechain);\
							std::cout.width(30); std::cout<<getCLenta(lbuf, lenta_position);\
							std::cout.width(20); std::cout<<getCSt(sbuf);					\
							std::cout <<std::endl;

#define MFST_TRACE2			std::cout.setf(std::ios::left);									\
							std::cout.width (4); std::cout<<FST_TRACE_n<< ": ";				\
							std::cout.width(20); std::cout<< " ";							\
							std::cout.width(30); std::cout<<getCLenta(lbuf, lenta_position);\
							std::cout.width(20); std::cout<<getCSt(sbuf);					\
							std::cout <<std::endl;

#define MFST_TRACE3			std::cout.setf(std::ios::left);									\
							std::cout.width (4); std::cout<<++FST_TRACE_n << ": ";			\
							std::cout.width(20); std::cout<< " ";							\
							std::cout.width(30); std::cout<<getCLenta(lbuf, lenta_position);\
							std::cout.width(20); std::cout<<getCSt(sbuf);					\
							std::cout <<std::endl;

#define MFST_TRACE4(c)		std::cout.width(4);  std::cout << ++FST_TRACE_n << ": "; std::cout.width(20); std::cout << c << std::endl;
#define MFST_TRACE5(c)		std::cout.width(4);  std::cout << FST_TRACE_n   << ": "; std::cout.width(20); std::cout << c << std::endl;
#define MFST_TRACE6(c,k)	std::cout.width(4);  std::cout << FST_TRACE_n   << ": "; std::cout.width(20); std::cout << c << k << std::endl;
#define	MFST_TRACE7			std::cout.width(4);  std::cout << state.lenta_position<<": ";						\
							std::cout.width(20); std::cout << rule.getCRule(rbuf, state.nrulechain) << std::endl;


namespace MFST
{
	struct MFSTState			// состояние автомата (для сохранения)
	{
		short lenta_position;	// позиция на ленте
		short nrule;			// номер текущего правила
		short nrulechain;		// номер текущей цепочки, текущего правила
		MFSTSTSTACK st;			// стек автомата

		MFSTState();
		MFSTState(
			short pposition,	// позиция на ленте
			MFSTSTSTACK pst,	// стек автомата
			short pnrulechain	// номер текущей цепочки, текущего правила
		);
		MFSTState(
			short pposition,	// позиция на ленте
			MFSTSTSTACK pst,	// стек автомата
			short pnrule,		// номер текущего правила
			short pnrulechain	// номер текущей цепочки, текущего правила
		);

	};

	struct MFST					// магазинный автомат
	{
		enum RC_STEP			//код возврата функции step
		{
			NS_OK,				// найдено правило и цепочка, цепочка записана в стек 
			NS_NORULE,			// не найдено правило грамматики (ошибка в грамматике)
			NS_NORULECHAIN,		// не найдена походящая цепочка правила (ошибка в исходном коде)
			NS_ERROR,			// неизвесный нетерминальный символ грамматики
			TS_OK,				// тек. символ ленты == вершине стека, продвинулась лента, pop стека
			TS_NOK,				// тек. символ ленты != вершине стека, восстановленно состояние 
			LENTA_END,			// теущая позиция ленты >= lenta_size 
			SURPRISE			// неожиданный код возврата (ошибка в step)
		};

		struct MFST_Diagnosis				// диагностика
		{
			short	lenta_position;		// позиция на ленте 
			RC_STEP	rc_step;				// код завершения шага 
			short	nrule;					// номер правила 
			short	nrule_chain;			// номер цепочки правила
			MFST_Diagnosis();
			MFST_Diagnosis(					// диагностика
				short plenta_position,		// позиция  на ленте
				RC_STEP prc_step,			// код завершения шага
				short pnrule,				// номер правила
				short pnrule_chain			// номер цепочки правила
			);
		} diagnosis[MFST_DIAGN_NUMBER];		// последние самые глубокие сообщения

		GRBALPHABET* lenta;					// перекодированная (TS/NS) лента (из LEX)
		short lenta_position;				// текущая позиция на ленте
		short nrule;						// номер текущего правила
		short nrulechain;					// номер текущей цепочки, текущего правила
		short lenta_size;					// размер ленты
		GRB::Greibach grebach;				// грамматика Грейбах
		LT::LexTable lex;					// результат работы лексического анализатоа
		MFSTSTSTACK st;						// стек автомата
		Stack<MFSTState> storestate;		// стек для сохранения состояний

		MFST();
		MFST(
			const LT::LexTable plex,		// результат работы лексического анализатоа
			GRB::Greibach pgrebach			// грамматика Грейбах
		);

		char* getCSt(char* buf);								//	получить содержиое стека
		char* getCLenta(char* buf, short pos, short n = 25);	//	лента: n символов, начиная с pos
		char* getDiagnosis(short n, char* buf);					//	получить n-ую строку диагностики или '\0'
		
		bool savestate();										//	сохранить состояние автомата
		bool resetstate();										//	восстановить состояние автомата
		bool push_chain(GRB::Rule::Chain chain);				//	поместить цепочку правила в стек

		RC_STEP step();											//	выполнить шаг автомата
		bool start();											//	запустить автомат
		bool savediagnosis(RC_STEP pprc_step);

		void printRules();										//	вывести последовательность правил

		struct Deducation
		{
			short size;					// количество шагов в выводе
			short* nrules;				// номер правила грамматики
			short* nrulechains;			// номер цепочек правила грамтики(nrules)
			Deducation() {
				size = 0;
				nrules = 0;
				nrulechains = 0;
			};
		}deducation;

		bool savededucation();			// сохранить дерево вывода
	};
}
			