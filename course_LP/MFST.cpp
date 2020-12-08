#include "MFST.h"
#include "GRB.h"
#include <iostream>
#include <iomanip>
#include <stack>

MFST::MFSTState::MFSTState()
{
	lenta_position = 0;
	nrule = -1;
	nrulechain = -1;
};

MFST::MFSTState::MFSTState(short pposition, MFSTSTSTACK pst, short pnrulechain)
{
	lenta_position = pposition;
	st = pst;
	nrulechain = pnrulechain;
};

MFST::MFSTState::MFSTState(short pposition, MFSTSTSTACK  pst, short pnrule, short pnrulechain)
{
	lenta_position = pposition;
	st = pst;
	nrule = pnrule;
	nrulechain = pnrulechain;
};

MFST::MFST::MFST_Diagnosis::MFST_Diagnosis()
{
	lenta_position = -1;
	rc_step = SURPRISE;
	nrule = -1;
	nrule_chain = -1;
}

MFST::MFST::MFST_Diagnosis::MFST_Diagnosis(short plenta_position, RC_STEP prc_step, short pnrule, short pnrule_chain)
{
	lenta_position = plenta_position;
	rc_step = prc_step;
	nrule = pnrule;
	nrule_chain = pnrule_chain;
}

MFST::MFST::MFST()
{
	lenta = 0;
	lenta_size = lenta_position = 0;
}

MFST::MFST::MFST(LT::LexTable plex, GRB::Greibach pgrebach)
{
	grebach = pgrebach;
	lex = plex;
	lenta = new short[lenta_size = lex.size];

	for (int i = 0; i < lenta_size; i++)
		lenta[i] = GRB::Rule::Chain::T(lex.table[i].lexema[0]);

	lenta_position = 0;
	st.push(grebach.stbottomT);
	st.push(grebach.startN);
	nrulechain = -1;
}

MFST::MFST::RC_STEP MFST::MFST::step()
{
	RC_STEP rc = SURPRISE;
	if (lenta_position < lenta_size)
	{
		if(GRB::Rule::Chain::isN(st.top()))
		{
			GRB::Rule rule;
			if ((nrule = grebach.getRule(st.top(), rule)) >= 0)
			{
				GRB::Rule::Chain chain;
				if ((nrulechain = rule.getNextChain(lenta[lenta_position], chain, nrulechain + 1)) >= 0)
				{
					MFST_TRACE1
					savestate();
					st.pop();
					push_chain(chain);
					rc = NS_OK;
					MFST_TRACE2
				}
				else
				{
					MFST_TRACE4("TNS_NS_NORULECHAIN/NS_NORULE")
					savediagnosis(NS_NORULECHAIN); rc = resetstate() ? NS_NORULECHAIN : NS_NORULE;
				}
			}
			else
				rc = NS_ERROR;
		}
		else if (st.top() == lenta[lenta_position])
		{
			lenta_position++;
			st.pop();
			nrulechain = -1;
			rc = TS_OK;
			MFST_TRACE3
		}
		else
		{
			MFST_TRACE4("TS_NOK/TNS_NS_NORULECHAIN/NS_NORULE")
			rc = resetstate() ? TS_NOK : NS_NORULECHAIN;
		}
	}
	else
	{
		rc = LENTA_END;
		MFST_TRACE4("LENTA_END")
	}

	return rc;
}

bool MFST::MFST::push_chain(GRB::Rule::Chain chain)
{
	for (int i = chain.size - 1; i >= 0; i--)
		st.push(chain.nt[i]);
	return true;
}

bool MFST::MFST::savestate()
{
	storestate.push(MFSTState(lenta_position, st, nrule, nrulechain));
	MFST_TRACE6("SAVESTATE:", storestate.size())
	return true;
}

bool MFST::MFST::resetstate()
{
	bool rc = false;
	MFSTState state;
	
	if (rc = (storestate.size() > 0))
	{
		state = storestate.top();
		lenta_position = state.lenta_position;
		st = state.st;
		nrule = state.nrule;
		nrulechain = state.nrulechain;
		storestate.pop();

		MFST_TRACE5("RESTATE")
		MFST_TRACE2
	}

	return rc;
}

bool MFST::MFST::savediagnosis(RC_STEP prc_step)
{
	bool rc = false;
	short k = 0;

	while (k < MFST_DIAGN_NUMBER && lenta_position <= diagnosis[k].lenta_position)
		k++;

	if (rc = (k < MFST_DIAGN_NUMBER))
	{
		diagnosis[k] = MFST_Diagnosis(lenta_position, prc_step, nrule, nrulechain);
		for (int i = k + 1; i < MFST_DIAGN_NUMBER; i++)
			diagnosis[i].lenta_position = -1;
	}

	return rc;
}

bool MFST::MFST::start()
{
	bool rc = false;
	RC_STEP rc_step = SURPRISE;
	char buf[MFST_DIAGN_MAXSIZE];
	rc_step = step();
	while (rc_step == NS_OK || rc_step == NS_NORULECHAIN || rc_step == TS_OK || rc_step == TS_NOK)
		rc_step = step();

	switch (rc_step)
	{
		case LENTA_END:
		{
			MFST_TRACE4("------>LENTA_END")
				std::cout << "------------------------------------------------------------------------------------------   ------" << std::endl;
			sprintf_s(buf, MFST_DIAGN_MAXSIZE, "%d: всего строк %d, синтаксический анализ выполнен без ошибок", 0, lenta_size);
			std::cout << std::setw(4) << std::left << 0 << "всего строк " << lenta_size << ", синтаксический анализ выполнен без ошибок" << std::endl;
			rc = true;
			break;
		}

		case NS_NORULE:
		{
			MFST_TRACE4("------>NS_NORULE")
				std::cout << "------------------------------------------------------------------------------------------   ------" << std::endl;
			std::cout << getDiagnosis(0, buf) << std::endl;
			std::cout << getDiagnosis(1, buf) << std::endl;
			std::cout << getDiagnosis(2, buf) << std::endl;
			break;
		}

		case NS_NORULECHAIN:	MFST_TRACE4("------>NS_NORULECHAIN") break;
		case NS_ERROR:			MFST_TRACE4("------>NS_ERROR") break;
		case SURPRISE:			MFST_TRACE4("------>NS_SURPRISE") break;
	};

	return rc;
}

char* MFST::MFST::getCSt(char* buf)
{
	for (int i = (signed)st.size() - 1; i >= 0; --i)
	{
		short p = st[i];
		buf[st.size() - 1 - i] = GRB::Rule::Chain::alphabet_to_char(p);
	}

	buf[st.size()] = 0x00;

	return buf;
}

char* MFST::MFST::getCLenta(char* buf, short pos, short n)
{
	short i = 0, k = (pos + n < lenta_size) ? pos + n : lenta_size;

	for (i = pos; i < k; i++)
		buf[i - pos] = GRB::Rule::Chain::alphabet_to_char(lenta[i]);

	buf[i - pos] = 0x00 ;
	return buf;
}

char* MFST::MFST::getDiagnosis(short n, char* buf)
{
	char* rc = nullptr;
	int errid = 0;
	int lpos = -1;
	if (n < MFST_DIAGN_NUMBER && (lpos = diagnosis[n].lenta_position) >= 0)
	{
		errid = grebach.getRule(diagnosis[n].nrule).iderror;
		Error::ERROR err = Error::geterror(errid);
		sprintf_s(buf, MFST_DIAGN_MAXSIZE, "%d: строка %d,	%s", err.id, lex.table[lpos].sn, err.message);
		rc = buf;
	}
	return rc;
}

void MFST::MFST::printRules()
{
	MFSTState state;
	GRB::Rule rule;
	for (unsigned short i = 0; i < storestate.size(); i++)
	{
		state = storestate[i];
		rule = grebach.getRule(state.nrule);
		MFST_TRACE7
	}
}

bool MFST::MFST::savededucation()
{
	MFSTState state;
	GRB::Rule rule;
	deducation.nrules = new short[deducation.size = storestate.size()];
	deducation.nrulechains = new short[deducation.size];

	for (unsigned short i = 0; i < storestate.size(); i++)
	{
		state = storestate[i];
		deducation.nrules[i] = state.nrule; 
		deducation.nrulechains[i] = state.nrulechain;
	}
	return true;
}