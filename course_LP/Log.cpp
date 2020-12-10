#include "stdafx.h"


namespace Log
{
	LOG getlog(wchar_t logfile[])
	{

		LOG log;

		log.stream = new std::fstream;
		log.stream->open(logfile, std::ios::out);

		wcscpy_s(log.logfile, logfile);
		
		return log;
	}

	void WriteLine(LOG log, const char* c, ...)
	{
		const  char** ptrC = &c;

		for (; *ptrC != ""; ptrC++) {
			*(log.stream) << *ptrC;
		}

		*(log.stream) << std::endl;
	}

	void WriteLine(LOG log, const wchar_t* c, ...)
	{
		char temp[PARM_MAX_SIZE];
		const wchar_t** ptrC = &c;

		for (; *ptrC != L""; ptrC++) {
			wcstombs(temp, *ptrC, PARM_MAX_SIZE);
			*(log.stream) << temp;
		}
		*(log.stream) <<ENDL;
	}

	void WriteLog(LOG log)
	{
		char buffer[48];
		time_t rawtime;
		struct tm* timeinfo;
		time(&rawtime);
		timeinfo = localtime(&rawtime);
		strftime(buffer, 48, "%d-%m-%Y --%A-- %H:%M:%S ", timeinfo);

		*(log.stream) << "------------ Протокол -------------" <<ENDL << buffer <<ENDL <<ENDL;
		
	}

	void WriteParm(LOG structlog, Parm::PARM parm)
	{
		*(structlog.stream) << "------------ Параметры -------------" <<ENDL;

		//----------------------- wchar_t to char for in -----------------------
		char* in = new char[wcslen(parm.in) + 1];
		wcstombs(in, parm.in, wcslen(parm.in));
		in[wcslen(parm.in)] = STR_ENDL;

		char* parm_in = new char[wcslen(PARM_IN) + 1];
		wcstombs(parm_in, PARM_IN, wcslen(PARM_IN));
		parm_in[wcslen(PARM_IN)] = STR_ENDL;

		//----------------------- wchar_t to char for out -----------------------
		char* out = new char[wcslen(parm.out) + 1];
		wcstombs(out, parm.out, wcslen(parm.out));
		out[wcslen(parm.out)] = STR_ENDL;

		char* parm_out = new char[wcslen(PARM_OUT) + 1];
		wcstombs(parm_out, PARM_OUT, wcslen(PARM_OUT));
		parm_out[wcslen(PARM_OUT)] = STR_ENDL;

		//----------------------- wchar_t to char for log -----------------------
		char* log = new char[wcslen(parm.log) + 1];
		wcstombs(log, parm.log, wcslen(parm.log));
		log[wcslen(parm.log)] = STR_ENDL;

		char* parm_log = new char[wcslen(PARM_LOG) + 1];
		wcstombs(parm_log, PARM_LOG, wcslen(PARM_LOG));
		parm_log[wcslen(PARM_LOG)] = STR_ENDL;

		//----------------------- out file -----------------------
		*(structlog.stream) << parm_in	<< TAB << in  <<ENDL;
		*(structlog.stream) << parm_out << TAB << out <<ENDL;
		*(structlog.stream) << parm_log << TAB << log <<ENDL <<ENDL;


		delete[] in;
		delete[] out;
		delete[] log;
	}

	void WriteIn(LOG log, In::IN in)
	{
		
		*(log.stream) << "--------- Исходные данные ---------" <<ENDL;
		log.stream->width(32);
		log.stream->setf(std::ios::left);
		*(log.stream) << "Количество символов:";
		*(log.stream) << in.size <<ENDL;
		log.stream->width(32);
		*(log.stream) << "Проигнорировано символов:"; 
		*(log.stream) << in.ignor <<ENDL;
		log.stream->width(32);
		*(log.stream) << "Количество строк:";
		*(log.stream) << in.lines <<ENDL <<ENDL;
	}

	void Close(LOG log)
	{
		delete log.stream;
		log.stream = nullptr;
	}

	void WriteError(LOG log, Error::ERROR error)
	{
		if (log.stream) {
			*(log.stream) << "Тест содержит ошибки!" <<ENDL
				<< "Ошибка: " << error.id << SPACE << error.message <<ENDL;
			if(error.inext.line != -1)
				*(log.stream) << "Строка:" << SPACE << error.inext.line <<ENDL;
			if(error.inext.col != -1)
				*(log.stream) << "Позиция:" << SPACE << error.inext.col;
		}
		else
			std::cout << "Ошибка: " << error.id << SPACE << error.message <<ENDL;
	}
}