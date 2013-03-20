#include "DBCommandCallBack.h"

void DBCommandCallBack::CleanUp()
{
	m_bSignal = false;
}

void DBCommandCallBack::Signal()
{
	CSmartLock lock(m_lock);
	m_bSignal = true;
}

bool DBCommandCallBack::canExcute()
{
	CSmartLock lock(m_lock);
	return m_bSignal;
}
