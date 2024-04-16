#pragma once

#include "Base.h"

BEGIN(Engine)

class CTimer final : public CBase
{
private:
	explicit CTimer();
	virtual ~CTimer() = default;

public:
	_double Get_TimeDelta() const {	return m_TimeDelta; } 

public:
	HRESULT Ready_Timer();
	_double Compute_TimeDelta();

private:
	_double						m_TimeDelta = 0.f;

private:
	LARGE_INTEGER				m_FrameTime;
	LARGE_INTEGER				m_FixTime;
	LARGE_INTEGER				m_LastTime;
	LARGE_INTEGER				m_CpuTick;

public:
	static CTimer* Create();

protected:
	virtual void Free();
};

END