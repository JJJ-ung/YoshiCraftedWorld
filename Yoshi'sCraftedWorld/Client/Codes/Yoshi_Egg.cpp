#include "stdafx.h"
#include "..\Headers\Yoshi_Egg.h"

#include "Management.h"
#include "Light_Manager.h"
#include "Mesh_Dynamic.h"
#include "Input_Device.h"
#include "Collider.h"
#include "Mouse_Manager.h"

USING(Client)

CYoshi_Egg::CYoshi_Egg(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CYoshi_Egg::CYoshi_Egg(const CYoshi_Egg & rhs)
	: CGameObject(rhs)
{
}

HRESULT CYoshi_Egg::Change_Mode(EGG_MODE eMode, _vec3 vTarget)
{
	if (m_eCurrMode != eMode)
		m_eCurrMode = eMode;

	if (eMode == CYoshi_Egg::RELEASE)
		Convert_World();
	else if (eMode == CYoshi_Egg::FOCUS)
	{
		m_vTarget = vTarget;
		m_vThrowDirection = m_vTarget - m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		D3DXVec3Normalize(&m_vThrowDirection, &m_vThrowDirection);
		_float fAngle = 0.f;
		fAngle = D3DXVec3Dot(&_vec3(0.f, 1.f, 0.f), &m_vThrowDirection);
		fAngle = acos(fAngle);
		m_pTransformCom->SetUp_Rotation(_vec3(0.f, 0.f, 1.f), -fAngle);
		m_eCurrMode = CYoshi_Egg::RELEASE;
	}

	return NOERROR;
}

HRESULT CYoshi_Egg::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CYoshi_Egg::Ready_GameObject(EGGINFO tEggInfo)
{
	memcpy(&m_tEggInfo, &tEggInfo, sizeof(EGGINFO));

	if (FAILED(Add_Component()))
		return E_FAIL;

	if (FAILED(Compute_ParentMatrix()))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_tEggInfo.pPlayerTransform->Get_State(CTransform::STATE_POSITION));

	m_pTransformCom->SetUp_Scale(_vec3(0.005f, 0.005f, 0.005f));

	return NOERROR;
}

_int CYoshi_Egg::Update_GameObject(_double TimeDelta)
{
	m_pTransformCom->SetUp_Scale(_vec3(0.005f, 0.005f, 0.005f));

	switch (m_eCurrMode)
	{
	case Client::CYoshi_Egg::IDLE:
		return Update_IdleMode(TimeDelta);
	case Client::CYoshi_Egg::MOVE:
		return Update_MoveMode(TimeDelta);
	case Client::CYoshi_Egg::THROW:
		return Update_ThrowMode(TimeDelta);
	case CYoshi_Egg::RELEASE:
		return Update_ReleaseMode(TimeDelta);
	case CYoshi_Egg::RELEASE_DEAD:
	{
		CSoundMgr::GetInstance()->PlaySound(L"SD_PON1.wav", CSoundMgr::EGG);
		return OBJDEAD;
	}
	}

	return 0;
}

_int CYoshi_Egg::LateUpdate_GameObject(_double TimeDelta)
{
	if (m_pRendererCom == nullptr)
		return -1;

	m_pRendererCom->Add_RenderList(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

HRESULT CYoshi_Egg::Render_GameObject()
{
	if (nullptr == m_pMeshCom ||
		nullptr == m_pShaderCom)
		return E_FAIL;

	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if (nullptr == pEffect)
		return E_FAIL;

	Safe_AddRef(pEffect);

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	_ulong	dwNumMaterials = m_pMeshCom->Get_NumMaterials();

	for (size_t i = 0; i < dwNumMaterials; ++i)
	{
		m_pMeshCom->SetTexture_OnShader(pEffect, "g_BaseTexture", i);
		m_pMeshCom->SetMaterial_OnShader(pEffect, "g_Material", i);

		pEffect->CommitChanges();

		m_pMeshCom->Render_Mesh(i);
	}

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);

	m_pColliderCom->Render_Collider();

	return NOERROR;
}

_int CYoshi_Egg::Update_IdleMode(_double TimeDelta)
{
	m_fProgress = (_float)TimeDelta * 5.f;

	_vec3 vTargetPos, vPlayerPos;
	vTargetPos = m_tEggInfo.pTargetTransform->Get_State(CTransform::STATE_POSITION);
	vPlayerPos = m_tEggInfo.pPlayerTransform->Get_State(CTransform::STATE_POSITION);
	vTargetPos.x += -(m_tEggInfo.pPlayerTransform->Get_State(CTransform::STATE_LOOK)).x * 0.8f;
	vTargetPos.y = m_tEggInfo.pTargetTransform->Get_State(CTransform::STATE_POSITION).y;
	
	//vTargetPos.y = vPlayerPos.y + 0.4f;

	if (vTargetPos.y <= vPlayerPos.y + 0.4f)
		vTargetPos.y = vPlayerPos.y + 0.4f;

	_vec3 vOut;
	D3DXVec3Lerp(&vOut, &m_pTransformCom->Get_State(CTransform::STATE_POSITION), &vTargetPos, m_fProgress);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vOut);
	m_matCollider = m_pTransformCom->Get_WorldMatrix();

	_vec3 vRight, vUp, vLook;

	memcpy(&vRight, &m_matCollider.m[0][0], sizeof(_vec3));
	memcpy(&vUp, &m_matCollider.m[1][0], sizeof(_vec3));
	memcpy(&vLook, &m_matCollider.m[2][0], sizeof(_vec3));

	D3DXVec3Normalize(&vRight, &vRight);
	D3DXVec3Normalize(&vUp, &vUp);
	D3DXVec3Normalize(&vLook, &vLook);

	memcpy(&m_matCollider.m[0][0], &vRight, sizeof(_vec3));
	memcpy(&m_matCollider.m[1][0], &vUp, sizeof(_vec3));
	memcpy(&m_matCollider.m[2][0], &vLook, sizeof(_vec3));

	m_pColliderCom->Update_Collider();

	return NOERROR;
}

_int CYoshi_Egg::Update_MoveMode(_double TimeDelta)
{
	m_fProgress = (_float)TimeDelta * 20.f;

	_matrix			matFrame = m_pParentMeshCom->Get_FrameMatrix("J_egg");
	_matrix			matParent = m_pParentTransformCom->Get_WorldMatrix();

	_matrix			matTarget = matFrame * matParent;

	_vec3 vTargetPos;
	memcpy(vTargetPos, &matTarget.m[3][0], sizeof(_vec3));

	_vec3 vOut;
	D3DXVec3Lerp(&vOut, &m_pTransformCom->Get_State(CTransform::STATE_POSITION), &vTargetPos, m_fProgress);

	_vec3 vLength = vOut - m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float fLength = D3DXVec3Length(&vLength);

	if (fLength <= 0.01)
	{
		if (FAILED(Change_Mode(CYoshi_Egg::THROW)))
			return E_FAIL;
		return NOERROR;
	}

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vOut);
	m_matCollider = m_pTransformCom->Get_WorldMatrix();

	_vec3 vRight, vUp, vLook;

	memcpy(&vRight, &m_matCollider.m[0][0], sizeof(_vec3));
	memcpy(&vUp, &m_matCollider.m[1][0], sizeof(_vec3));
	memcpy(&vLook, &m_matCollider.m[2][0], sizeof(_vec3));

	D3DXVec3Normalize(&vRight, &vRight);
	D3DXVec3Normalize(&vUp, &vUp);
	D3DXVec3Normalize(&vLook, &vLook);

	memcpy(&m_matCollider.m[0][0], &vRight, sizeof(_vec3));
	memcpy(&m_matCollider.m[1][0], &vUp, sizeof(_vec3));
	memcpy(&m_matCollider.m[2][0], &vLook, sizeof(_vec3));

	m_pColliderCom->Update_Collider();

	return NOERROR;
}

_int CYoshi_Egg::Update_ThrowMode(_double TimeDelta)
{
	m_fProgress = (_float)TimeDelta * 20.f;

	_matrix			matFrame = m_pParentMeshCom->Get_FrameMatrix("J_egg");
	_matrix			matParent = m_pParentTransformCom->Get_WorldMatrix();

	_matrix			matTarget = matFrame * matParent;

	_vec3 vTargetPos;
	memcpy(vTargetPos, &matTarget.m[3][0], sizeof(_vec3));

	_vec3 vOut;
	D3DXVec3Lerp(&vOut, &m_pTransformCom->Get_State(CTransform::STATE_POSITION), &vTargetPos, m_fProgress);

	_vec3 vLength = vOut - m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float fLength = D3DXVec3Length(&vLength);

	if (fLength <= 0.01)
	{
		if (FAILED(Change_Mode(CYoshi_Egg::THROW)))
			return E_FAIL;
		return NOERROR;
	}

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vOut);
	m_matCollider = m_pTransformCom->Get_WorldMatrix();

	_vec3 vRight, vUp, vLook;

	memcpy(&vRight, &m_matCollider.m[0][0], sizeof(_vec3));
	memcpy(&vUp, &m_matCollider.m[1][0], sizeof(_vec3));
	memcpy(&vLook, &m_matCollider.m[2][0], sizeof(_vec3));

	D3DXVec3Normalize(&vRight, &vRight);
	D3DXVec3Normalize(&vUp, &vUp);
	D3DXVec3Normalize(&vLook, &vLook);

	memcpy(&m_matCollider.m[0][0], &vRight, sizeof(_vec3));
	memcpy(&m_matCollider.m[1][0], &vUp, sizeof(_vec3));
	memcpy(&m_matCollider.m[2][0], &vLook, sizeof(_vec3));

	m_pColliderCom->Update_Collider();

	return NOERROR;
}

_int CYoshi_Egg::Update_ReleaseMode(_double TimeDelta)
{
	_vec3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	m_pTransformCom->Move_Direction(m_vThrowDirection, TimeDelta);

	_vec3 vDistance = m_tEggInfo.pPlayerTransform->Get_State(CTransform::STATE_POSITION) - vPos;
	if (D3DXVec3Length(&vDistance) > 15.f)
		Change_Mode(CYoshi_Egg::RELEASE_DEAD);

	_matrix			matWorld = m_pTransformCom->Get_WorldMatrix();

	m_matCollider = m_pTransformCom->Get_WorldMatrix();

	_vec3 vRight, vUp, vLook;

	memcpy(&vRight, &m_matCollider.m[0][0], sizeof(_vec3));
	memcpy(&vUp, &m_matCollider.m[1][0], sizeof(_vec3));
	memcpy(&vLook, &m_matCollider.m[2][0], sizeof(_vec3));

	D3DXVec3Normalize(&vRight, &vRight);
	D3DXVec3Normalize(&vUp, &vUp);
	D3DXVec3Normalize(&vLook, &vLook);

	memcpy(&m_matCollider.m[0][0], &vRight, sizeof(_vec3));
	memcpy(&m_matCollider.m[1][0], &vUp, sizeof(_vec3));
	memcpy(&m_matCollider.m[2][0], &vLook, sizeof(_vec3));

	m_pColliderCom->Update_Collider();

	return NOERROR;
}

HRESULT CYoshi_Egg::Convert_World()
{
	_vec3 vRayPos = CMouse_Manager::GetInstance()->Get_RayPos();
	_vec3 vRayDir = CMouse_Manager::GetInstance()->Get_RayDir();

	_vec3 vEggPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_vec3 vNormal = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	vNormal.z *= 100.f;

	_vec3 vRayTarget = vRayPos + (vRayDir * 100.f);

	D3DXPLANE Plane;
	D3DXPlaneFromPointNormal(&Plane, &vEggPos, &vNormal);
	D3DXPlaneIntersectLine(&m_vTarget, &Plane, &vRayPos, &vRayTarget);

	m_vThrowDirection = m_vTarget - m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	D3DXVec3Normalize(&m_vThrowDirection, &m_vThrowDirection);

	_float fAngle = 0.f;
	fAngle = D3DXVec3Dot(&_vec3(0.f, 1.f, 0.f), &m_vThrowDirection);
	fAngle = acos(fAngle);

	m_pTransformCom->SetUp_Rotation(_vec3(0.f, 0.f, 1.f), -fAngle);

	return NOERROR;
}

HRESULT CYoshi_Egg::Add_Component()
{
	// For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, &CTransform::STATE(10.f, D3DXToRadian(90)))))
		return E_FAIL;

	// For.Com_Renderer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// For.Com_Mesh
	if (FAILED(CGameObject::Add_Component(SCENE_TRAIN, L"Component_Mesh_Egg", L"Com_Mesh", (CComponent**)&m_pMeshCom)))
		return E_FAIL;

	// For.Com_Shader
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Shader_Mesh", L"Com_Shader", (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	// For.Com_Collider
	CCollider::COLLIDERDESC		ColliderDesc;
	ColliderDesc.vScale = _vec3(0.5f, 0.5f, 0.5f);
	ColliderDesc.vRotation_Angle = _vec3(0.f, 0.f, 0.f);
	ColliderDesc.vPosition = _vec3(0.0f, 0.0f, 0.0f);
	ColliderDesc.pParentMatrix = &m_matCollider;
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Collider_Sphere", L"Com_Collider", (CComponent**)&m_pColliderCom, &ColliderDesc)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CYoshi_Egg::SetUp_ConstantTable()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	_matrix			matView, matOut;
	_matrix			matWorld = m_pTransformCom->Get_WorldMatrix();

	if (m_eCurrMode == CYoshi_Egg::THROW)
	{
		_matrix			matFrame = m_pParentMeshCom->Get_FrameMatrix("J_egg");
		_matrix			matParent = m_pParentTransformCom->Get_WorldMatrix();
		matOut = matWorld * matFrame * matParent;
	}
	else
		matOut = matWorld;

	m_pShaderCom->Set_Value("g_matWorld", &m_pTransformCom->Get_WorldMatrix(), sizeof(_matrix));
	m_pShaderCom->Set_Value("g_matView", matView = Get_Transform(D3DTS_VIEW), sizeof(_matrix));
	m_pShaderCom->Set_Value("g_matProj", Get_Transform(D3DTS_PROJECTION), sizeof(_matrix));

	return NOERROR;
}

HRESULT CYoshi_Egg::Compute_ParentMatrix()
{
	CManagement* pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return E_FAIL;
	Safe_AddRef(pManagement);

	m_pParentTransformCom = m_tEggInfo.pPlayerTransform;
	if (nullptr == m_pParentTransformCom)
		return E_FAIL;

	m_pParentMeshCom = (CMesh_Dynamic*)pManagement->Get_Component(SCENE_TRAIN, L"Layer_Object", L"Com_Mesh");
	if (nullptr == m_pParentMeshCom)
		return E_FAIL;

	Safe_Release(pManagement);

	return NOERROR;
}

CYoshi_Egg * CYoshi_Egg::Create(LPDIRECT3DDEVICE9 pGraphic_Device, EGGINFO tEggInfo)
{
	CYoshi_Egg*		pInstance = new CYoshi_Egg(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject(tEggInfo)))
	{
		MSG_BOX("Failed To Create CGameObject Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CYoshi_Egg::Clone_GameObject(void * pArg)
{
	CGameObject*		pInstance = new CYoshi_Egg(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		MSG_BOX("Failed To Create CGameObject Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CYoshi_Egg::Free()
{
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pMeshCom);
	Safe_Release(m_pRendererCom);

	CGameObject::Free();
}
