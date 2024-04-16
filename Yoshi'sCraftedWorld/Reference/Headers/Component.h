#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CComponent abstract : public CBase
{
protected:
	explicit CComponent(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CComponent(const CComponent& rhs);
	virtual ~CComponent() = default;

public:
	_matrix Get_Transform(_D3DTRANSFORMSTATETYPE eType);

public:
	virtual HRESULT Ready_Component_Prototype();
	virtual HRESULT Ready_Component(void* pArg);

protected:
	LPDIRECT3DDEVICE9		m_pGraphic_Device = nullptr;
	_bool					m_isClone = false;

public:
	virtual CComponent* Clone_Component(void* pArg) = 0;
	virtual void Free();
};

END