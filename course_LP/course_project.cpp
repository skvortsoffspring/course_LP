#include "stdafx.h"
#include <chrono>

int _tmain(int argc, _TCHAR* argv[])
{
	if (2 > 2)
	{
		int a;
	}
	setlocale(LC_ALL, "rus");
	Log::LOG log = Log::INITLOG;
	try {
		double AllTimeTranslations = 0.0;
		Parm::PARM parm = Parm::getparm(argc, argv);
		log = Log::getlog(parm.log);
		Log::WriteLog(log);
		Log::WriteParm(log, parm);
		In::IN in = In::getin(parm.in);
		Log::WriteIn(log, in);
		Log::WriteLine(log, L"Файл: ", L"считан ", L"без ", L"ошибок ", L"");

		LT::LexTable tableLEX = LT::Create(LT_MAXSIZE);
		IT::IdTable idTableID = IT::Create(TI_MAXSIZE);

		std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
		GM::dataProcesing(in.text, log.stream, &tableLEX, &idTableID);
		std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
		AllTimeTranslations += time_span.count();
		std::cout << ">>>>>>>>>> Лексический анализатор: \t" << time_span.count() << " сек." << std::endl << std::endl;

		LT::PrintTableLex(&tableLEX, parm.out);
#ifdef DEBUG
		MFST_TRACE_START									//	отладка
#endif // DEBUG

		//MFST::MFST mfst(tableLEX, GRB::getGreibach());	    //	автомат
		//
		//t1 = std::chrono::high_resolution_clock::now();
		//mfst.start();										//	старт синтаксического	анализа
		//t2 = std::chrono::high_resolution_clock::now();
		//time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
		//AllTimeTranslations += time_span.count();
		//std::cout << ">>>>>>>>>> Cинтаксический анализатор: \t" << time_span.count() << " сек." << std::endl << std::endl;

		//t1 = std::chrono::high_resolution_clock::now();
		//SA::SemanticAnalises(&tableLEX, &idTableID);
		//t2 = std::chrono::high_resolution_clock::now();
		//time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
		//AllTimeTranslations += time_span.count();
		//std::cout << ">>>>>>>>>> Семантический анализатор: \t" << time_span.count() << " сек." << std::endl << std::endl;
		
		t1 = std::chrono::high_resolution_clock::now();
		PN::seachingExpressions(tableLEX, idTableID);
		t2 = std::chrono::high_resolution_clock::now();
		time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
		AllTimeTranslations += time_span.count();
		std::cout << ">>>>>>>>>> Преобразование выражений: \t" << time_span.count() << " сек." << std::endl << std::endl;

		LT::PrintTableLex(&tableLEX, parm.out);

		t1 = std::chrono::high_resolution_clock::now();
		GTA::GeneratorASM(&tableLEX, &idTableID);
		t2 = std::chrono::high_resolution_clock::now();
		time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
		AllTimeTranslations += time_span.count();
		std::cout << ">>>>>>>>>> Генерация кода: \t\t" << time_span.count() << "сек." << std::endl << std::endl;

		IT::PrintIdTable(PARM_ID_DEFAULT_EXT, idTableID);
		IT::Delete(idTableID);
		LT::Delete(tableLEX);
		
		Log::Close(log);
		std::cout << ">>>>>>>>>> Трансляция выполнена за : \t" << AllTimeTranslations << " сек." << std::endl << std::endl;

#ifdef RULESPRINT
		//mfst.printRules();
#endif
	}
	catch (Error::ERROR e)
	{
		std::cout << "########## Ошибка, проверьте лог-файл ##########" << std::endl;
		Log::WriteError(log, e);
	};
	return 0;
}
