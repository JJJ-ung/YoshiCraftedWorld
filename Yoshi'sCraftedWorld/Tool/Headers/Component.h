#pragma once

class CComponent abstract
{
public:
	explicit CComponent(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CComponent() = default;

public:
	virtual HRESULT Ready_Component(void* pArg);

public:
	LPDIRECT3DDEVICE9		m_pGraphic_Device = nullptr;

public:
	virtual void Free();
};