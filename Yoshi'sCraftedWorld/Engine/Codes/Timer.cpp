#include "..\Headers\Timer.h"

CTimer::CTimer()
{

}

HRESULT CTimer::Ready_Timer()
{
	QueryPerformanceCounter(&m_FrameTime);
	QueryPerformanceCounter(&m_FixTime);
	QueryPerformanceCounter(&m_LastTime);
	QueryPerformanceFrequency(&m_CpuTick);

	return NOERROR;
}

_double CTimer::Compute_TimeDelta()
{
	QueryPerformanceCounter(&m_FrameTime);

	m_TimeDelta = _double(m_FrameTime.QuadPart - m_FixTime.QuadPart) / _double(m_CpuTick.QuadPart);

	m_FixTime.QuadPart = m_FrameTime.QuadPart;

	if ((m_FrameTime.QuadPart - m_LastTime.QuadPart) > m_CpuTick.QuadPart)
	{
		QueryPerformanceFrequency(&m_CpuTick);
		m_LastTime = m_FrameTime;
	}

	return _double(m_TimeDelta);
}

CTimer * CTimer::Create()
{
	CTimer*		pInstance = new CTimer();

	if (FAILED(pInstance->Ready_Timer()))
	{
		MSG_BOX("CTimer Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTimer::Free()
{

}