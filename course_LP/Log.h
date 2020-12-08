#pragma once
#include "stdafx.h"
#include "In.h"	
namespace Log // –абота с протоколом
{
    struct LOG // протокол
    {
        wchar_t logfile[PARM_MAX_SIZE];
        std::fstream* stream;
    };



    static const LOG INITLOG = { L"", NULL };       // структура дл€ начальной инициализации LOG
    LOG getlog(wchar_t logfile[]);                  // сформировать структуру LOG          
    void WriteLine(LOG log, const char* c, ...);    // вывести в протокол конкатенацию строк
    void WriteLine(LOG log, const wchar_t* c, ...); // вывести в протокол конкатенацию строк
    void WriteLog(LOG log);                         // вывести в протокол заголовок
    void WriteParm(LOG log, Parm::PARM parm);       // вывести в протокол информацию о входных параметрах
    void WriteIn(LOG log, In::IN in);               // вывести в протокол информацию о входном потоке
    void WriteError(LOG log, Error::ERROR error);   // вывести в протокол инфомацию об ошибке
    void Close(LOG log);	                        //закрыть протокол

}
    
