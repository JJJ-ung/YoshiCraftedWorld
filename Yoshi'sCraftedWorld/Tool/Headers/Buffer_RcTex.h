#pragma once

#include "VIBuffer.h"

class CBuffer_RcTex final : public CVIBuffer
{
public:
	explicit CBuffer_RcTex(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBuffer_RcTex(void) = default;

public:
	virtual HRESULT Ready_Component();
	virtual HRESULT Render_Buffer(void);

public:
	static CBuffer_RcTex* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void Free(void);
};