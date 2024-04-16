#include "..\Headers\Collider.h"
#include "Shader.h"
#include "Transform.h"

CCollider::CCollider(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)	
{

}

CCollider::CCollider(const CCollider & rhs)
	: CComponent(rhs)
	, m_pMesh(rhs.m_pMesh)
	, m_pShader(rhs.m_pShader)
	, m_pParentMatrix(rhs.m_pParentMatrix)
	, m_isColl(rhs.m_isColl)
	, m_eBoxType(rhs.m_eBoxType)
{
	Safe_AddRef(m_pShader);
	Safe_AddRef(m_pMesh);	
}

_vec3 CCollider::Get_ColliderPos()
{
	_vec3 vPos;
	D3DXVec3TransformCoord(&vPos, &m_pTransform->Get_State(CTransform::STATE_POSITION), &m_RenderingMatrix);
	return vPos;
}

HRESULT CCollider::Ready_Component_Prototype(COLLIDERTYPE eType, BOXTYPE eBoxType)
{
	HRESULT		hr; 

	switch (eType)
	{
	case CCollider::TYPE_BOX:
		hr = Ready_BoundingBoxMesh();
		break;
	case CCollider::TYPE_SPHERE:
		hr = Ready_SphereMesh();
		break;
	}

	if (FAILED(hr))
		return E_FAIL;

	m_pShader = CShader::Create(m_pGraphic_Device, L"../Bin/Shader/Shader_Collider.fx");
	if (nullptr == m_pShader)
		return E_FAIL;

	m_eBoxType = eBoxType;

	m_isColl = false;

	return NOERROR;
}

HRESULT CCollider::Ready_Component(void * pArg)
{
	m_pTransform = CTransform::Create(m_pGraphic_Device);
	if (nullptr == m_pTransform)
		return E_FAIL;

	COLLIDERDESC*	pColliderDesc = (COLLIDERDESC*)pArg;

	if (nullptr == pArg)
		return E_FAIL;

	m_pTransform->SetUp_Scale(pColliderDesc->vScale);
	m_pTransform->SetUp_Rotation(_vec3(1.f, 0.f, 0.f), D3DXToRadian(pColliderDesc->vRotation_Angle.x));
	m_pTransform->SetUp_Rotation(_vec3(0.f, 1.f, 0.f), D3DXToRadian(pColliderDesc->vRotation_Angle.y));
	m_pTransform->SetUp_Rotation(_vec3(0.f, 0.f, 1.f), D3DXToRadian(pColliderDesc->vRotation_Angle.z));
	m_pTransform->Set_State(CTransform::STATE_POSITION, pColliderDesc->vPosition);

	m_pParentMatrix = pColliderDesc->pParentMatrix;

	// For.AABB
	void*		pVertices = nullptr;

	m_pMesh->LockVertexBuffer(0, &pVertices);

	D3DXComputeBoundingBox((_vec3*)pVertices, m_pMesh->GetNumVertices(), D3DXGetFVFVertexSize(m_pMesh->GetFVF()), &m_vMin, &m_vMax);

	m_pMesh->UnlockVertexBuffer();

	// For.OBB	
	ZeroMemory(&m_OBBDesc, sizeof(OBBDESC));

	m_OBBDesc.vPoint[0] = _vec3(m_vMin.x, m_vMax.y, m_vMin.z);
	m_OBBDesc.vPoint[1] = _vec3(m_vMax.x, m_vMax.y, m_vMin.z);
	m_OBBDesc.vPoint[2] = _vec3(m_vMax.x, m_vMin.y, m_vMin.z);
	m_OBBDesc.vPoint[3] = _vec3(m_vMin.x, m_vMin.y, m_vMin.z);

	m_OBBDesc.vPoint[4] = _vec3(m_vMin.x, m_vMax.y, m_vMax.z);
	m_OBBDesc.vPoint[5] = _vec3(m_vMax.x, m_vMax.y, m_vMax.z);
	m_OBBDesc.vPoint[6] = _vec3(m_vMax.x, m_vMin.y, m_vMax.z);
	m_OBBDesc.vPoint[7] = _vec3(m_vMin.x, m_vMin.y, m_vMax.z);

	// For.Sphere
	m_fRadius = pColliderDesc->vScale.x * 0.5f;

	return NOERROR;
}

HRESULT CCollider::Compute_BoundingSphere(LPD3DXMESH pMesh)
{
	if (pMesh == nullptr)
		return E_FAIL;

	BYTE* pV = NULL;
	LPDIRECT3DVERTEXBUFFER9		pVB = nullptr;
	pMesh->GetVertexBuffer(&pVB);
	pVB->Lock(0, 0, (void**)&pV, 0);

	_vec3 vPos;
	if (FAILED(D3DXComputeBoundingSphere((_vec3*)pV, pMesh->GetNumVertices(), pMesh->GetNumBytesPerVertex(), &vPos, &m_fRadius)))
		return E_FAIL;

	D3DXVec3TransformCoord(&vPos, &vPos, &m_RenderingMatrix);

	pVB->Unlock();
	Safe_Release(pVB);
	Safe_Release(pMesh);

	return NOERROR;
}

_int CCollider::Update_Collider()
{
	m_RenderingMatrix = nullptr == m_pParentMatrix
		? m_pTransform->Get_WorldMatrix() : m_pTransform->Get_WorldMatrix() * *m_pParentMatrix;

	if (BOX_AABB == m_eBoxType)
		Remove_Rotation(&m_RenderingMatrix);

	return _int();
}

HRESULT CCollider::Render_Collider()
{
	if (!g_bRenderCollider)
		return NOERROR;

	if (nullptr == m_pMesh || 
		nullptr == m_pShader || 
		nullptr == m_pTransform)
		return E_FAIL;

	LPD3DXEFFECT pEffect = m_pShader->Get_EffectHandle();
	if (nullptr == pEffect)
		return E_FAIL;

	Safe_AddRef(pEffect);

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);		
	
	m_pMesh->DrawSubset(0);

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);

	return NOERROR;
}

_bool CCollider::Collision_AABB(CCollider * pTarget)
{
	if (BOX_AABB != m_eBoxType ||
		BOX_AABB != pTarget->m_eBoxType)
		return false;

	_vec3		vSourMin, vSourMax;
	_vec3		vDestMin, vDestMax;

	D3DXVec3TransformCoord(&vSourMin, &m_vMin, &m_RenderingMatrix);
	D3DXVec3TransformCoord(&vSourMax, &m_vMax, &m_RenderingMatrix);

	D3DXVec3TransformCoord(&vDestMin, &pTarget->m_vMin, &pTarget->m_RenderingMatrix);
	D3DXVec3TransformCoord(&vDestMax, &pTarget->m_vMax, &pTarget->m_RenderingMatrix);

	m_isColl = false;

	pTarget->m_isColl = false;

	// x축선상에서(너비가) 겹쳤니?
	if (max(vSourMin.x, vDestMin.x) > min(vSourMax.x, vDestMax.x))
		return false;

	if (max(vSourMin.y, vDestMin.y) > min(vSourMax.y, vDestMax.y))
		return false;

	if (max(vSourMin.z, vDestMin.z) > min(vSourMax.z, vDestMax.z))
		return false;

	m_isColl = true;
	pTarget->m_isColl = true;
	
	return _bool(true);
}

_bool CCollider::Collision_OBB(CCollider * pTarget)
{
	m_isColl = false;

	Compute_OBB();
	pTarget->Compute_OBB();

	OBBDESC		OBB[2] = { m_OBBDesc, pTarget->m_OBBDesc };

	_float		fDistance[3];

	_vec3		vCenterAxis = OBB[1].vCenterPoint - OBB[0].vCenterPoint;

	for (size_t i = 0; i < 2; ++i)
	{
		for (size_t j = 0; j < 3; ++j)
		{
			fDistance[0] = fabs(D3DXVec3Dot(&vCenterAxis, &OBB[i].vAlignAxis[j]));

			fDistance[1] = fabs(D3DXVec3Dot(&OBB[0].vProjAxis[0], &OBB[i].vAlignAxis[j]))
				+ fabs(D3DXVec3Dot(&OBB[0].vProjAxis[1], &OBB[i].vAlignAxis[j]))
				+ fabs(D3DXVec3Dot(&OBB[0].vProjAxis[2], &OBB[i].vAlignAxis[j]));

			fDistance[2] = fabs(D3DXVec3Dot(&OBB[1].vProjAxis[0], &OBB[i].vAlignAxis[j]))
				+ fabs(D3DXVec3Dot(&OBB[1].vProjAxis[1], &OBB[i].vAlignAxis[j]))
				+ fabs(D3DXVec3Dot(&OBB[1].vProjAxis[2], &OBB[i].vAlignAxis[j]));

			if (fDistance[0] > fDistance[1] + fDistance[2])
				return false;
		}
	}

	m_isColl = true;
	
	return _bool(true);
}

_bool CCollider::Collision_Sphere(CCollider * pTarget)
{
	m_isColl = false;
	pTarget->m_isColl = false;

	_vec3 vSrcCenter, vDstCenter;
	
	memcpy(&vSrcCenter, &m_RenderingMatrix.m[3][0], sizeof(_vec3));
	memcpy(&vDstCenter, &pTarget->m_RenderingMatrix.m[3][0], sizeof(_vec3));

	_vec3 vDistance = vDstCenter - vSrcCenter;

	_float fDistSum = sqrtf((vDistance.x * vDistance.x) + (vDistance.y * vDistance.y) + (vDistance.z * vDistance.z));

	if (fDistSum > m_fRadius + pTarget->m_fRadius)
		return false;
	else
	{
		m_isColl = true;
		pTarget->m_isColl = true;
		return true;
	}
}

_bool CCollider::Collision_Ray(_vec3 vRayPos, _vec3 vRayDir, _float& fDistance)
{
	m_isColl = false;

	_vec3 vSpherePos;
	memcpy(&vSpherePos, &m_RenderingMatrix.m[3][0], sizeof(_vec3));

	_vec3 q = vSpherePos - vRayPos;
	_float c = D3DXVec3Length(&q);
	_float v = D3DXVec3Dot(&q, D3DXVec3Normalize(&vRayDir, &vRayDir));
	_float d = m_fRadius * m_fRadius - (c*c - v*v);

	if (d < 0.f)
		return false;

	fDistance = v - sqrt(d);
	m_isColl = true;
	return true;
}

HRESULT CCollider::Ready_SphereMesh()
{
	if (FAILED(D3DXCreateSphere(m_pGraphic_Device, 0.5f, 20, 20, &m_pMesh, &m_pAdjacency)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CCollider::Ready_BoundingBoxMesh()
{
	if (FAILED(D3DXCreateBox(m_pGraphic_Device, 1.f, 1.f, 1.f, &m_pMesh, &m_pAdjacency)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CCollider::SetUp_ConstantTable()
{
	if (nullptr == m_pShader || 
		nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pShader->Set_Value("g_matWorld", &m_RenderingMatrix, sizeof(_matrix));
	m_pShader->Set_Value("g_matView", &Get_Transform(D3DTS_VIEW), sizeof(_matrix));
	m_pShader->Set_Value("g_matProj", &Get_Transform(D3DTS_PROJECTION), sizeof(_matrix));
	m_pShader->Get_EffectHandle()->SetBool("g_isColl", m_isColl);

	return NOERROR;
}

HRESULT CCollider::Remove_Rotation(_matrix* pMatrix)
{
	_matrix		matTransform = *pMatrix;

	_vec3		vRight(1.f, 0.f, 0.f), vUp(0.f, 1.f, 0.f), vLook(0.f, 0.f, 1.f);

	_float		fRightScale = D3DXVec3Length((_vec3*)&matTransform.m[0][0]);
	_float		fUpScale = D3DXVec3Length((_vec3*)&matTransform.m[1][0]);
	_float		fLookScale = D3DXVec3Length((_vec3*)&matTransform.m[2][0]);

	vRight *= fRightScale;
	vUp *= fUpScale;
	vLook *= fLookScale;

	memcpy(&matTransform.m[0][0], &vRight, sizeof(_vec3));
	memcpy(&matTransform.m[1][0], &vUp, sizeof(_vec3));
	memcpy(&matTransform.m[2][0], &vLook, sizeof(_vec3));

	*pMatrix = matTransform;

	return NOERROR;
}

HRESULT CCollider::Compute_OBB()
{
	_vec3	vWorldPoint[8];


	for (size_t i = 0; i < 8; ++i)
		D3DXVec3TransformCoord(&vWorldPoint[i], &m_OBBDesc.vPoint[i], &m_RenderingMatrix);	

	m_OBBDesc.vCenterPoint = (vWorldPoint[0] + vWorldPoint[6]) * 0.5f;

	m_OBBDesc.vAlignAxis[0] = vWorldPoint[2] - vWorldPoint[3];
	m_OBBDesc.vAlignAxis[1] = vWorldPoint[0] - vWorldPoint[3];
	m_OBBDesc.vAlignAxis[2] = vWorldPoint[7] - vWorldPoint[3];

	for (size_t i = 0; i < 3; ++i)
		D3DXVec3Normalize(&m_OBBDesc.vAlignAxis[i], &m_OBBDesc.vAlignAxis[i]);
	

	m_OBBDesc.vProjAxis[0] = (vWorldPoint[5] + vWorldPoint[2]) * 0.5f - m_OBBDesc.vCenterPoint;
	m_OBBDesc.vProjAxis[1] = (vWorldPoint[5] + vWorldPoint[0]) * 0.5f - m_OBBDesc.vCenterPoint;
	m_OBBDesc.vProjAxis[2] = (vWorldPoint[5] + vWorldPoint[7]) * 0.5f - m_OBBDesc.vCenterPoint;

	return NOERROR;
}



CCollider * CCollider::Create(LPDIRECT3DDEVICE9 pGraphic_Device, COLLIDERTYPE eType, BOXTYPE eBoxType)
{
	CCollider*		pInstance = new CCollider(pGraphic_Device);

	if (FAILED(pInstance->Ready_Component_Prototype(eType, eBoxType)))
	{
		MSG_BOX("Failed To Create CCollider Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CCollider::Clone_Component(void * pArg)
{
	CCollider*		pInstance = new CCollider(*this);

	if (FAILED(pInstance->Ready_Component(pArg)))
	{
		MSG_BOX("Failed To Create CCollider Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCollider::Free()
{
	Safe_Release(m_pShader);
	Safe_Release(m_pTransform);
	Safe_Release(m_pMesh);

	CComponent::Free();
}
