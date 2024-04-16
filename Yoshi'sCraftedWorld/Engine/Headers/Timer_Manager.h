#pragma once

#include "Base.h"

BEGIN(Engine)

class CTimer;
class ENGINE_DLL CTimer_Manager final : public CBase
{
	DECLARE_SINGLETON(CTimer_Manager)
private:
	explicit CTimer_Manager();
	virtual ~CTimer_Manager() = default;

public:
	_double Get_TimeDelta(const _tchar* pTimerTag);

public:
	HRESULT Add_Timer(const _tchar* pTimerTag);
	_double Compute_Timer(const _tchar* pTimerTag);

private:
	map<const _tchar*, CTimer*>			m_mapTimers;
	typedef map<const _tchar*, CTimer*>	MAPTIMERS;

private:
	CTimer*	Find_Timer(const _tchar * pTimerTag);

protected:
	virtual void Free();
};

END