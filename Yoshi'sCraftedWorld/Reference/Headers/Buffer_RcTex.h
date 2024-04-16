#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CBuffer_RcTex final : public CVIBuffer
{
private:
	explicit CBuffer_RcTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBuffer_RcTex(const CBuffer_RcTex& rhs);
	virtual ~CBuffer_RcTex(void) = default;

public:
	virtual HRESULT Ready_Component_Prototype();
	virtual HRESULT Ready_Component(void* pArg);
	virtual HRESULT Render_Buffer(void);

public:
	static CBuffer_RcTex* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone_Component(void* pArg);
	virtual void Free(void);
};

END

