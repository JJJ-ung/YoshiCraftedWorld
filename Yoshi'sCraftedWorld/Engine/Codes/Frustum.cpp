#include "..\Headers\Frustum.h"

CFrustum::CFrustum(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{

}

CFrustum::CFrustum(const CFrustum & rhs)
	: CComponent(rhs)	
{
	memcpy(m_vPoint, rhs.m_vPoint, sizeof(_vec3) * 8);
}

HRESULT CFrustum::Ready_Component_Prototype()
{
	// 투영스페이스 상의 절두체를 구성한다.
	m_vPoint[0] = _vec3(-1.f, 1.f, 0.f);
	m_vPoint[1] = _vec3(1.f, 1.f, 0.f);
	m_vPoint[2] = _vec3(1.f, -1.f, 0.f);
	m_vPoint[3] = _vec3(-1.f, -1.f, 0.f);

	m_vPoint[4] = _vec3(-1.f, 1.f, 1.f);
	m_vPoint[5] = _vec3(1.f, 1.f, 1.f);
	m_vPoint[6] = _vec3(1.f, -1.f, 1.f);
	m_vPoint[7] = _vec3(-1.f, -1.f, 1.f);

	return NOERROR;
}

HRESULT CFrustum::Ready_Component(void * pArg)
{
	return NOERROR;
}

_bool CFrustum::Culling_OnWorldSpace(const _vec3 * pPosition, _float fRange)
{
	_vec3	vPoint[8];


	// 투영스페이스상의 정점 여덟개.
	memcpy(vPoint, m_vPoint, sizeof(_vec3) * 8);

	_matrix	matProj = Get_Transform(D3DTS_PROJECTION);
	D3DXMatrixInverse(&matProj, nullptr, &matProj);

	// 뷰스페이스 상의 정점 여덟개.
	for (size_t i = 0; i < 8; ++i)	
		D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], &matProj);

	_matrix	matView = Get_Transform(D3DTS_VIEW);
	D3DXMatrixInverse(&matView, nullptr, &matView);

	// 월드스페이스 상의 정점 여덟개.
	for (size_t i = 0; i < 8; ++i)
		D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], &matView);

	// 평면을 구성하자.
	// +x
	D3DXPlaneFromPoints(&m_Plane[0], &vPoint[1], &vPoint[5], &vPoint[6]);
	// -x
	D3DXPlaneFromPoints(&m_Plane[1], &vPoint[4], &vPoint[0], &vPoint[3]);

	// +y
	D3DXPlaneFromPoints(&m_Plane[2], &vPoint[1], &vPoint[0], &vPoint[4]);
	// -y
	D3DXPlaneFromPoints(&m_Plane[3], &vPoint[3], &vPoint[2], &vPoint[6]);

	// +z
	D3DXPlaneFromPoints(&m_Plane[4], &vPoint[5], &vPoint[4], &vPoint[7]);
	// -z
	D3DXPlaneFromPoints(&m_Plane[5], &vPoint[0], &vPoint[1], &vPoint[2]);

	return isIn_Frustum(m_Plane, pPosition, fRange);	
}



_bool CFrustum::isIn_Frustum(const D3DXPLANE * pPlane, const _vec3 * pPoint, _float fRange)
{
	for (size_t i = 0; i < 6; ++i)	
	{
		// 	ax + by +cz + d
		if (fRange < D3DXPlaneDotCoord(&pPlane[i], pPoint))
			return false;
	}

	return _bool(true);
}

CFrustum * CFrustum::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CFrustum*		pInstance = new CFrustum(pGraphic_Device);

	if (FAILED(pInstance->Ready_Component_Prototype()))
	{
		MSG_BOX("Failed To Create CFrustum Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}


CComponent * CFrustum::Clone_Component(void * pArg)
{
	CFrustum*		pInstance = new CFrustum(*this);

	if (FAILED(pInstance->Ready_Component(pArg)))
	{
		MSG_BOX("Failed To Create CFrustum Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFrustum::Free()
{
	CComponent::Free();
}
