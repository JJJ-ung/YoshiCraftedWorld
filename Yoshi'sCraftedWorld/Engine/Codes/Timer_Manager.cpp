#include "..\Headers\Timer_Manager.h"	
#include "Timer.h"

IMPLEMENT_SINGLETON(CTimer_Manager)

CTimer_Manager::CTimer_Manager()
{
}

_double CTimer_Manager::Get_TimeDelta(const _tchar * pTimerTag)
{
	CTimer* pTimer = Find_Timer(pTimerTag);

	if (nullptr == pTimer)
		return 0.0;

	return pTimer->Get_TimeDelta();
}

HRESULT CTimer_Manager::Add_Timer(const _tchar * pTimerTag)
{
	CTimer* pTimer = Find_Timer(pTimerTag);

	if (nullptr != pTimer)
		return E_FAIL;

	pTimer = CTimer::Create();
	if (nullptr == pTimer)
		return E_FAIL;

	m_mapTimers.insert(MAPTIMERS::value_type(pTimerTag, pTimer));

	return NOERROR;
}

_double CTimer_Manager::Compute_Timer(const _tchar * pTimerTag)
{
	CTimer* pTimer = Find_Timer(pTimerTag);

	if (nullptr == pTimer)
		return 0.f;

	return pTimer->Compute_TimeDelta();	
}

CTimer * CTimer_Manager::Find_Timer(const _tchar * pTimerTag)
{	
	auto iter = find_if(m_mapTimers.begin(), m_mapTimers.end(), CTag_Finder(pTimerTag));

	if(iter == m_mapTimers.end())		
		return nullptr;

	return iter->second;
}

void CTimer_Manager::Free()
{
	for (auto& Pair : m_mapTimers)
	{
		Safe_Release(Pair.second);
	}
	m_mapTimers.clear();
}

