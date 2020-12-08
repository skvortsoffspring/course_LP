#pragma once
#pragma warning(disable : 4996)
//#include <list>
#include <tchar.h>
#include <clocale>
#include <limits>
#include <stack>
#include <string>
#include <tchar.h>
#include <ctime>
#include <cwchar>
#include <locale>
#include <fstream>
#include <iostream>
#include "Error.h"			//	обработка ошибок
#include "Parm.h"			//	обработка параметров
#include "Log.h"			//	работа с протоколом
#include "In.h"				//	ввод исходного файла
#include "DP.h"				//	обработка данных
#include "FST.h"
#include "LT.h"
#include "IT.h"
#include "PN.h"
#include "GTA.h"
#include "MFST.h"
#include "GRB.h"

//#define TEST1				//  проверка ERROR
//#define TEST2				//  проверка PARM
//#define TEST3				//  проверка IN
//#define TEST4				//  проверка IN другой тип ошибок
#define TEST5				//	проверка 