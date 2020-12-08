#include "FST.h"
#include <memory>

namespace FST
{
	RELATION::RELATION(char c, short ns)
	{
		symbol = c;
		nnode = ns;
	}
	NODE::NODE()
	{

		n_relation = 0;
		RELATION* relations = NULL;
	}
	NODE::NODE(short n, RELATION rel, ...)
	{
		n_relation = n;
		RELATION* ptr = &rel;
		relations = new RELATION[n];
		for (int i = 0; i < n; ++i, ptr++)relations[i] = *ptr;
	}
	FST::FST(char* s, short ns, NODE n, ...)
	{
		lenta = (unsigned char*)s;
		nstates = ns;
		nodes = new NODE[ns];
		NODE* ptr = &n;
		for (int i = 0; i < ns; ++i)nodes[i] = ptr[i];
		rstates = new short[ns];
		memset(rstates, 0xff, sizeof(short) * nstates);
		rstates[0] = 0;
		position = -1;
	}

	bool step(FST& fst, short*& rstates)						// îäèí øàã àâòîìàòà
	{
		bool rc = false;
		std::swap(rstates, fst.rstates);						// ñìåíà ìàññèâîâ 

		for (short i = 0; (i < fst.nstates) && !rc; i++)
		{
			if (rstates[i] == fst.position)
				for (short j = 0; j < fst.nodes[i].n_relation; j++)
				{
					if (fst.nodes[i].relations[j].symbol == fst.lenta[fst.position])
					{
						fst.rstates[fst.nodes[i].relations[j].nnode] = fst.position + 1;
						rc = true;
						break;
					};
				};
		};
		return rc;
	};

	bool execute(FST& fst)										// âûïîëíèòü ðàñïîçíàâàíèå öåïî÷êè
	{
		short* rstates = new short[fst.nstates];
		memset(rstates, 0xff, sizeof(short) * fst.nstates);
		short llenta = strlen((const char*)fst.lenta);
		bool rc = true;

		for (short i = 0; i < llenta && rc; i++)
		{
			fst.position++;										// ïðîäèíóëè ïîçèöèþ
			rc = step(fst, rstates);							// îäèí øàã àâòîìàòà
		}

		delete[] rstates;
		return (rc ? (fst.rstates[fst.nstates - 1] == llenta) : rc);
	}
};