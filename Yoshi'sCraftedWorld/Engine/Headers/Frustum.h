#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CFrustum final : public CComponent
{
private:
	explicit CFrustum(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CFrustum(const CFrustum& rhs);
	virtual ~CFrustum() = default;

public:
	HRESULT Ready_Component_Prototype();
	HRESULT Ready_Component(void* pArg);
	_bool Culling_OnWorldSpace(const _vec3* pPosition, _float fRange = 0.0f); // 객체의 위치와 절두체의 비교. 

private:
	_vec3			m_vPoint[8];
	D3DXPLANE		m_Plane[6];

private:
	_bool isIn_Frustum(const D3DXPLANE* pPlane, const _vec3* pPoint, _float fRange);

public:
	static CFrustum* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone_Component(void* pArg);
	virtual void Free();
};

END