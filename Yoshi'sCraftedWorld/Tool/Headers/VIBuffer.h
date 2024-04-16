#pragma once

#include "Component.h"

class CVIBuffer abstract : public CComponent
{
public:
	explicit CVIBuffer(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CVIBuffer(void) = default;	

public:
	virtual HRESULT Ready_Component();
	virtual HRESULT Render_Buffer(void) = 0;

protected:
	LPDIRECT3DVERTEXBUFFER9		m_pVB = nullptr;
	LPDIRECT3DINDEXBUFFER9		m_pIB = nullptr;
	_ulong		m_dwVtxSize = 0; 
	_ulong 		m_dwVtxCnt = 0;
	_ulong 		m_dwTriCnt = 0;
	_ulong 		m_dwVtxFVF = 0;
	_ulong 		m_dwIdxSize = 0;
	D3DFORMAT	m_IdxFmt;

public:
	virtual void Free(void);
};
