#pragma once

#include "Engine_Defines.h"

BEGIN(Engine)

class ENGINE_DLL CBase abstract
{
protected:
	explicit CBase();
	virtual ~CBase() = default;

public:
	_ulong AddRef(); 
	_ulong Release();  

private:
	unsigned long m_dwRefCnt = 0;

public:
	virtual void Free() = 0;	
};

END
