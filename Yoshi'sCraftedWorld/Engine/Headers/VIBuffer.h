#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer abstract : public CComponent
{
protected:
	explicit CVIBuffer(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CVIBuffer(const CVIBuffer& rhs);
	virtual ~CVIBuffer(void) = default;	

public:
	virtual HRESULT Ready_Component_Prototype();
	virtual HRESULT Ready_Component(void* pArg);
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
	virtual CComponent* Clone_Component(void* pArg) = 0;
	virtual void Free(void);
};

END
