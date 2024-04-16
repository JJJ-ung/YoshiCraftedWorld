#include "..\Headers\MeshCollision.h"
#include "Transform.h"

CMeshCollision::CMeshCollision(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{
}

CMeshCollision::CMeshCollision(const CMeshCollision & rhs)
	:CComponent(rhs)
	, m_isColl(rhs.m_isColl)
{
}

HRESULT CMeshCollision::Ready_Component_Prototype()
{
	return NOERROR;
}

HRESULT CMeshCollision::Ready_Component(void * pArg)
{
	COLLINFO* p = (COLLINFO*)pArg;
	m_pCollisionMesh = p->pMesh;
	m_pTargetTransform = p->pTargetTransform;
	m_bCheckChange = p->bChange;

	if (m_pCollisionMesh == nullptr)
		return E_FAIL;

	return NOERROR;
}

_int CMeshCollision::Collision_Mesh(_vec3 vRayPos, _vec3 vRayDir, _float& fDistance)
{
	D3DXIntersect(m_pCollisionMesh, &vRayPos, &vRayDir, &m_isColl, &m_iFaceIndex, &fU, &fV, &fDistance, nullptr, nullptr);
	m_vPoint = vRayPos + (fDistance * vRayDir);

	return m_isColl;
}

HRESULT CMeshCollision::Convert_Local(_vec3 vMousePos, _vec3 & vRayPos, _vec3 & vRayDir)
{
	// 투영 -> 뷰스페이스
	_vec3 vMouse = vMousePos;
	_matrix		matProj;
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, 0, &matProj);
	D3DXVec3TransformCoord(&vMouse, &vMouse, &matProj);

	vRayPos = _vec3(0.f, 0.f, 0.f);
	vRayDir = vMouse - vRayPos;

	// 뷰스페이스 -> 월드
	_matrix		matView;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, 0, &matView);
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);

	// 월드 -> 로컬
	_matrix		matWorldInv;
	if (m_bCheckChange)
	{
		_matrix matWorld = m_pTargetTransform->Get_WorldMatrix();
		_vec3		vRight, vUp, vLook;

		memcpy(vRight, &matWorld.m[0][0], sizeof(_vec3));
		memcpy(vUp, &matWorld.m[1][0], sizeof(_vec3));
		memcpy(vLook, &matWorld.m[2][0], sizeof(_vec3));

		vRight = *D3DXVec3Normalize(&vRight, &vRight) * 0.005f;
		vUp = *D3DXVec3Normalize(&vUp, &vUp) * 0.005f;
		vLook = *D3DXVec3Normalize(&vLook, &vLook) * 0.005f;

		memcpy(&matWorld.m[0][0], vRight, sizeof(_vec3));
		memcpy(&matWorld.m[1][0], vUp, sizeof(_vec3));
		memcpy(&matWorld.m[2][0], vLook, sizeof(_vec3));

		matWorldInv = *D3DXMatrixInverse(&matWorldInv, nullptr, &matWorld);

		return NOERROR;
	}
	else
	{
		matWorldInv = m_pTargetTransform->Get_WorldMatrixInv();
	}

	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matWorldInv);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matWorldInv);

	return NOERROR;
}

HRESULT CMeshCollision::Convert_Local(_vec3 & vRayPos, _vec3 & vRayDir)
{
	// 월드 -> 로컬
	_matrix		matWorldInv;
	if (m_bCheckChange)
	{
		_matrix matWorld = m_pTargetTransform->Get_WorldMatrix();
		_vec3		vRight, vUp, vLook;

		memcpy(vRight, &matWorld.m[0][0], sizeof(_vec3));
		memcpy(vUp, &matWorld.m[1][0], sizeof(_vec3));
		memcpy(vLook, &matWorld.m[2][0], sizeof(_vec3));

		vRight = *D3DXVec3Normalize(&vRight, &vRight) * 0.005f;
		vUp = *D3DXVec3Normalize(&vUp, &vUp) * 0.005f;
		vLook = *D3DXVec3Normalize(&vLook, &vLook) * 0.005f;

		memcpy(&matWorld.m[0][0], vRight, sizeof(_vec3));
		memcpy(&matWorld.m[1][0], vUp, sizeof(_vec3));
		memcpy(&matWorld.m[2][0], vLook, sizeof(_vec3));

		matWorldInv = *D3DXMatrixInverse(&matWorldInv, nullptr, &matWorld);
	}
	else
	{
		matWorldInv = m_pTargetTransform->Get_WorldMatrixInv();
	}
//	matWorldInv = m_pTargetTransform->Get_WorldMatrixInv();
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matWorldInv);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matWorldInv);

	return NOERROR;
}

_vec3 CMeshCollision::Convert_World(_vec3 vPoint)
{
	if (m_bCheckChange)
	{
		_matrix matWorld = m_pTargetTransform->Get_WorldMatrix();
		_vec3		vRight, vUp, vLook;

		memcpy(vRight, &matWorld.m[0][0], sizeof(_vec3));
		memcpy(vUp, &matWorld.m[1][0], sizeof(_vec3));
		memcpy(vLook, &matWorld.m[2][0], sizeof(_vec3));

		vRight = *D3DXVec3Normalize(&vRight, &vRight) * 0.005f;
		vUp = *D3DXVec3Normalize(&vUp, &vUp) * 0.005f;
		vLook = *D3DXVec3Normalize(&vLook, &vLook) * 0.005f;

		memcpy(&matWorld.m[0][0], vRight, sizeof(_vec3));
		memcpy(&matWorld.m[1][0], vUp, sizeof(_vec3));
		memcpy(&matWorld.m[2][0], vLook, sizeof(_vec3));

		return *D3DXVec3TransformCoord(&vPoint, &vPoint, &matWorld);
	}

	return *D3DXVec3TransformCoord(&vPoint, &vPoint, &m_pTargetTransform->Get_WorldMatrix());
}

CMeshCollision * CMeshCollision::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMeshCollision*		pInstance = new CMeshCollision(pGraphic_Device);

	if (FAILED(pInstance->Ready_Component_Prototype()))
	{
		MSG_BOX("Failed To Create CMeshCollision Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CMeshCollision::Clone_Component(void * pArg)
{
	CMeshCollision*		pInstance = new CMeshCollision(*this);

	if (FAILED(pInstance->Ready_Component(pArg)))
	{
		MSG_BOX("Failed To Create CMeshCollision Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMeshCollision::Free()
{
	Safe_Release(m_pCollisionMesh);

	CComponent::Free();
}
