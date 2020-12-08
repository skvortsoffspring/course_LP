#include "stdafx.h" 
#include <string.h>

namespace Parm
{
	PARM getparm(int argc, _TCHAR* argv[])
	{
		if (argc == 1)
			throw ERROR_THROW(100);
		if (argc > 4)
			throw ERROR_THROW(101);

		PARM parm;
		

		//-------------ptr-for-strs-----------------
		wchar_t* ptr_in = nullptr;
		wchar_t* ptr_out = nullptr;
		wchar_t* ptr_log = nullptr;
		//------------------------------------------

		for (int i = 1, in = 0; i < argc; i++) {

			if (wcslen(argv[i]) > PARM_MAX_SIZE)
				throw ERROR_THROW(104);

			if (!ptr_in)
			{
				if (ptr_in = wcsstr(argv[i], PARM_IN))
				{
					wcscpy_s(parm.in, (ptr_in + wcslen(PARM_IN)));
					continue;
				}
			}

			if (!ptr_out)
			{
				if (ptr_out = wcsstr(argv[i], PARM_OUT))
				{
					wcscpy_s(parm.out, (ptr_out + wcslen(PARM_OUT)));
					wcscat_s(parm.out, PARM_OUT_DEFAULT_EXT);
					continue;
				}
			}

			if (!ptr_log)
			{
				if (ptr_log = wcsstr(argv[i], PARM_LOG))
				{
					wcscpy_s(parm.log, (ptr_log + wcslen(PARM_LOG)));
					wcscat_s(parm.log, PARM_LOG_DEFAULT_EXT);
					continue;
				}
			}

		}

		if (!ptr_in)
			throw ERROR_THROW(100);

		if (!ptr_out)
		{
			wcscpy_s(parm.out, (ptr_in + wcslen(PARM_IN)));
			wcscat_s(parm.out, PARM_OUT_DEFAULT_EXT);
		}

		if (!ptr_log)
		{
			wcscpy_s(parm.log, (ptr_in + wcslen(PARM_IN)));
			wcscat_s(parm.log, PARM_LOG_DEFAULT_EXT);
		}

		return parm;
	}
}
