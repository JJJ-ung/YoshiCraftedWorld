#include "..\Headers\Base.h"

USING(Engine)

CBase::CBase()
	: m_dwRefCnt(0)
{
}

_ulong CBase::AddRef()
{
	return _ulong(++m_dwRefCnt);
}

_ulong CBase::Release()
{
	if (0 == m_dwRefCnt)
	{
		Free();
		delete this;
		return 0;
	}

	else
		return m_dwRefCnt--;	
}

