#include "stdafx.h" 

namespace Error
{
	/************************************************************************/
	/*				|	0	- 99	|	системные ошибки					*/
	/*				|---------------|---------------------------------------*/
	/* серии ошибок |	100	- 109	|	ошибки параметров					*/
	/*				|---------------|---------------------------------------*/
	/*				|	110	- 119	|	ошибки открытия и чтения файлов		*/
	/*				|---------------|---------------------------------------*/
	/*				|	120	- 129	|	ошибки при анализе лексем			*/
	/************************************************************************/

	ERROR errors[ERROR_MAX_ENTRY] = //таблица ошибок
	{
		ERROR_ENTRY(0, "Недопустимый код ошибки"),
		ERROR_ENTRY(1, "Системный сбой"),
		ERROR_ENTRY_NODEF(2), ERROR_ENTRY_NODEF(3), ERROR_ENTRY_NODEF(4), ERROR_ENTRY_NODEF(5),
		ERROR_ENTRY_NODEF(6), ERROR_ENTRY_NODEF(7), ERROR_ENTRY_NODEF(8), ERROR_ENTRY_NODEF(9),
		ERROR_ENTRY_NODEF10(10), ERROR_ENTRY_NODEF10(20), ERROR_ENTRY_NODEF10(30), ERROR_ENTRY_NODEF10(40), ERROR_ENTRY_NODEF10(50),
		ERROR_ENTRY_NODEF10(60), ERROR_ENTRY_NODEF10(70), ERROR_ENTRY_NODEF10(80), ERROR_ENTRY_NODEF10(90),
		ERROR_ENTRY(100, "Параметр -in должен быть задан"),
		ERROR_ENTRY(101, "Ошибка, параметров слишком много"), ERROR_ENTRY_NODEF(102),ERROR_ENTRY_NODEF(103),
		ERROR_ENTRY(104, "Превышена длина входного параметра"),
		ERROR_ENTRY_NODEF(105), ERROR_ENTRY_NODEF(106), ERROR_ENTRY_NODEF(107),
		ERROR_ENTRY_NODEF(108), ERROR_ENTRY_NODEF(109),
		ERROR_ENTRY(110, "Ошибка при открытии файла с исходным кодом (-in)"),
		ERROR_ENTRY(111, "Недопустимый символ в исходном файле (-in)"),
		ERROR_ENTRY(112, "Ошибка при создании файла протокола(-log)"),
		ERROR_ENTRY(113,"Ошибка при открытии файла для вывода"),
		ERROR_ENTRY(114,"Ошибка,при чтении (отуствует парный символ (') для строки)"),
		ERROR_ENTRY(115, "Ошибка создания файла для генерации" ),
		ERROR_ENTRY_NODEF(116),
		ERROR_ENTRY_NODEF(117),
		ERROR_ENTRY_NODEF(118),
		ERROR_ENTRY_NODEF(119),
		ERROR_ENTRY(120,"Превышен размер таблицы лексем"),
		ERROR_ENTRY(121,""),
		ERROR_ENTRY(122,"Превышен размер таблицы идентификаторов"),
		ERROR_ENTRY(123,"Превышена длина строки"),
		ERROR_ENTRY(124,"Пропущено ключевое слово для функции"),
		ERROR_ENTRY(125,"Переобъявление функции"),
		ERROR_ENTRY(126,"Переобъявление переменной"),
		ERROR_ENTRY(127,"Не указан тип для функции"),
		ERROR_ENTRY(128,"Не распознана лексема"),
		ERROR_ENTRY(129,"Неверный диапазон типа byte"),
		ERROR_ENTRY(130,"Неверный диапазон типа integer"),
		ERROR_ENTRY(131,"Неверный диапазон типа float"),
		ERROR_ENTRY(132,"Литерал не соотвествует, типу переменной"),
		ERROR_ENTRY(133,"Необъявленная переменная"),
		ERROR_ENTRY(134,"Название внешней функции превышает допустимый размер"),
		ERROR_ENTRY(135,"Отсуствует закрывающая скобка в последней функции, см. строку"),
		ERROR_ENTRY(136,"Отсуствует открывающая скобка в последней функции, см. строку"),
		ERROR_ENTRY(137,"Переменная не может быть глобальной"),	
		ERROR_ENTRY(138,"Неприсвоено значение константе"),
		ERROR_ENTRY(139,"Константе нельзя присвоить значение"),
		ERROR_ENTRY(140,"Сравнение строк невозможно в условном операторе"),
		ERROR_ENTRY(141,"Неверное количество параметров при вызове функции"),
		ERROR_ENTRY(142,"Фактические параметры не совпадают при вызове функции"),
		ERROR_ENTRY(143,"Нет точки входа main"),
		ERROR_ENTRY(144,"Неверный тип возвращаемого параметра"),
		ERROR_ENTRY(145,"Присваивать какое-либо значение можно только переменной"),
		ERROR_ENTRY(146,"В программе может быть только одна точка входа(main)"),
		ERROR_ENTRY(147,"Отсутствует лексема окончания блока для main"),
		ERROR_ENTRY(148,"Арифметические действия со строками запрещены"),
		ERROR_ENTRY(149,"Функции не могут возвращать строки"),
		ERROR_ENTRY(150,"В выражении не может быть строк"),
		ERROR_ENTRY(151,""),
		ERROR_ENTRY(152,""),
		ERROR_ENTRY(153,""),
		ERROR_ENTRY(154,""),
		ERROR_ENTRY(155,""),
		ERROR_ENTRY(156,""),
		ERROR_ENTRY(157,""),
		ERROR_ENTRY(158,""),
		ERROR_ENTRY(159,""),
		ERROR_ENTRY_NODEF10(160), ERROR_ENTRY_NODEF10(170), ERROR_ENTRY_NODEF10(180), ERROR_ENTRY_NODEF10(190),
		ERROR_ENTRY_NODEF100(200), ERROR_ENTRY_NODEF100(300), ERROR_ENTRY_NODEF100(400), ERROR_ENTRY_NODEF100(500),
		ERROR_ENTRY(600,"Неверная структура программы"),
		ERROR_ENTRY(601,"Ошибочный оператор"),
		ERROR_ENTRY(602,"Ошибка в выражении"),
		ERROR_ENTRY(603,"Ошибка в параметрах функции"),
		ERROR_ENTRY(604,"Ошибка параметрах функции"),
		ERROR_ENTRY(605,"Ошибка в параметрах вызываемой функции"),
		ERROR_ENTRY(606,"Ошибка, массив может содержать только литералы"),
		ERROR_ENTRY(607,"Ошибка, размер массив дожен указываться только литералом или константой"),
		ERROR_ENTRY(608,"Ошибка в блоке else"),
		ERROR_ENTRY_NODEF100(700), ERROR_ENTRY_NODEF100(800), ERROR_ENTRY_NODEF100(900)
	};

	ERROR geterror(int id) {

		if (id < 0 || id > ERROR_MAX_ENTRY) id = 0;		//если вне диапозона то ошибка 0

		ERROR* ptr = new ERROR;
		ptr->id = id;
		strcpy_s(ptr->message, errors[id].message);
		return *ptr;
	};

	Error::ERROR geterrorline(int id, int line)
	{
		if (id < 0 || id > ERROR_MAX_ENTRY) id = 0;		//если вне диапозона то ошибка 0

		ERROR* ptr = new ERROR;

		ptr->id = id;
		ptr->inext.line = line;
		strcpy_s(ptr->message, errors[id].message);

		return *ptr;
	}

	Error::ERROR geterrorin(int id, int line, int col)
	{
		if (id < 0 || id > ERROR_MAX_ENTRY) id = 0;		//если вне диапозона то ошибка 0

		ERROR* ptr = new ERROR;

		ptr->id = id;
		ptr->inext.line = line;
		ptr->inext.col = col;
		strcpy_s(ptr->message, errors[id].message);
		
		return *ptr;
	}

}
