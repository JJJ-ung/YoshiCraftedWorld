#include "stdafx.h"
#include "..\Headers\TrainB.h"
#include "Management.h"
#include "Light_Manager.h"
#include "Mesh_Dynamic.h"
#include "Frustum.h"
#include "Input_Device.h"
#include "SteamTrain.h"
#include "Collider.h"
#include "Yoshi_Stage.h"
#include "MeshCollision.h"

USING(Client)

CTrainB::CTrainB(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CTrainB::CTrainB(const CTrainB & rhs)
	:CGameObject(rhs)
{
}

HRESULT CTrainB::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CTrainB::Ready_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	CManagement* pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return E_FAIL;
	Safe_AddRef(pManagement);

	m_pPlayer = (CYoshi_Stage*)pManagement->Get_ObjectFromLayer(SCENE_TRAIN, L"Layer_Object");

	CTransform* pTargetTransform = (CTransform*)pManagement->Get_Component(SCENE_TRAIN, L"Layer_SteamTrain", L"Com_Transform");
	m_pTrain = (CSteamTrain*)pManagement->Get_ObjectFromLayer(SCENE_TRAIN, L"Layer_SteamTrain");
	_vec3 vDir = m_pTrain->Get_Look();
	vDir *= -5.7f;

	_vec3 vPos = pTargetTransform->Get_State(CTransform::STATE_POSITION) + vDir;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);

	Safe_Release(pManagement);

	m_pTransformCom->Rotation_LookAt(pTargetTransform->Get_State(CTransform::STATE_POSITION));
	m_vLook = pTargetTransform->Get_State(CTransform::STATE_POSITION) - m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	//

	m_vecCurrPath = m_pTrain->Get_CurrPath();

	m_iterTarget = m_vecCurrPath.begin();
	++m_iterTarget;

	m_pMeshCom->SetUp_Animation(0);

	return NOERROR;
}

_int CTrainB::Update_GameObject(_double TimeDelta)
{
	m_TimeDelta = TimeDelta;

	if (m_pTrain->Get_Stop())
		return 0;

	m_fSpeed += 5.f * (_float)TimeDelta;
	if (m_fSpeed > 5.f)
		m_fSpeed = 5.f;

	_vec3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_vec3 vDistance = (*m_iterTarget) - vPos;

	_float fDistance = D3DXVec3Length(&vDistance);

	if (fDistance < 0.1f)
	{
		if (m_iterTarget != m_vecCurrPath.end())
			++m_iterTarget;
		if (m_iterTarget == m_vecCurrPath.end())
		{
			m_iterTarget = m_vecCurrPath.begin();
			m_iterTarget += m_vecCurrPath.size() - 1;
		}
	}

	D3DXVec3Lerp(&m_vLook, &m_vLook, &(*D3DXVec3Normalize(&vDistance, &vDistance)), (_float)TimeDelta * 10.f);
	_vec3 vLook = vPos + m_vLook * 5.f /** 10.f*/;

	m_pTransformCom->Move_Direction(*D3DXVec3Normalize(&vDistance, &vDistance), (_float)TimeDelta * m_fSpeed);
	m_pTransformCom->Rotation_LookAt(vLook);

	m_pColliderCom->Update_Collider();

	return _int();
}

_int CTrainB::LateUpdate_GameObject(_double TimeDelta)
{
	if (m_pRendererCom == nullptr)
		return -1;

	if (m_pTrain->Get_IsEnd() == false)
	{
		CManagement* pManagement = CManagement::GetInstance();
		if (nullptr == pManagement)
			return -1;

		Safe_AddRef(pManagement);

		if (m_pColliderCom->Collision_Sphere((CCollider*)pManagement->Get_Component(SCENE_TRAIN, L"Layer_Object", L"Com_Collider")))
			m_pPlayer->Get_OnTrain(false) = true;
		else
			m_pPlayer->Get_OnTrain(false) = false;

		Safe_Release(pManagement);
	}
	else
	{
		m_pPlayer->Get_OnTrain(false) = false;
	}

	if (true == m_pFrustumCom->Culling_OnWorldSpace(&m_pTransformCom->Get_State(CTransform::STATE_POSITION), 8.f))
		m_pRendererCom->Add_RenderList(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

HRESULT CTrainB::Render_GameObject()
{
	if (nullptr == m_pMeshCom ||
		nullptr == m_pShaderCom)
		return E_FAIL;

	if(m_pTrain->Get_Animation())
		m_pMeshCom->Play_Animation(m_TimeDelta);

	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if (nullptr == pEffect)
		return E_FAIL;

	Safe_AddRef(pEffect);

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	for (size_t i = 0; i < m_pMeshCom->Get_NumMeshContainer(); ++i)
	{
		m_pMeshCom->Update_SkinnedMesh(i);

		_ulong	dwNumMaterials = m_pMeshCom->Get_NumMaterials(i);

		for (size_t j = 0; j < dwNumMaterials; ++j)
		{
			m_pMeshCom->SetTexture_OnShader(pEffect, "g_BaseTexture", i, j, MESHTEXTURE::TYPE_DIFFUSE);
			m_pMeshCom->SetMaterial_OnShader(pEffect, "g_Material", i, j);

			pEffect->CommitChanges();

			m_pMeshCom->Render_Mesh(i, j);
		}
	}

	pEffect->EndPass();
	pEffect->End();
	Safe_Release(pEffect);

	m_pColliderCom->Render_Collider();

	return NOERROR;
}

HRESULT CTrainB::Change_CurrVec(vector<_vec3>& vPath)
{
	m_vecCurrPath = vPath;

	_vec3 vDir = m_pTrain->Get_Look();
	vDir *= -5.7f;

	_vec3 vPos = m_vecCurrPath[0] + vDir;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);

	CManagement* pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return E_FAIL;
	Safe_AddRef(pManagement);
	CTransform* pTargetTransform = (CTransform*)pManagement->Get_Component(SCENE_TRAIN, L"Layer_SteamTrain", L"Com_Transform");
	m_pTransformCom->Rotation_LookAt(pTargetTransform->Get_State(CTransform::STATE_POSITION));
	m_vLook = pTargetTransform->Get_State(CTransform::STATE_POSITION) - m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	Safe_Release(pManagement);

	m_iterTarget = m_vecCurrPath.begin();
	++m_iterTarget;

	return NOERROR;
}

HRESULT CTrainB::Add_Component()
{
	// For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, &CTransform::STATE(1.f, D3DXToRadian(90)))))
		return E_FAIL;

	// For.Com_Renderer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// For.Com_Mesh
	if (FAILED(CGameObject::Add_Component(SCENE_TRAIN, L"Component_Mesh_TrainB", L"Com_Mesh", (CComponent**)&m_pMeshCom)))
		return E_FAIL;

	// For.Com_Shader
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Shader_Mesh", L"Com_Shader", (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	// For.Com_Frustum
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Frustum", L"Com_Frustum", (CComponent**)&m_pFrustumCom)))
		return E_FAIL;

	// For.Com_Collider
	CCollider::COLLIDERDESC		ColliderDesc;
	ColliderDesc.vScale = _vec3(2.f, 2.f, 2.f);
	ColliderDesc.vRotation_Angle = _vec3(0.f, 0.f, 0.f);
	ColliderDesc.vPosition = _vec3(0.f, 2.f, 0.35f);
	ColliderDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrixPointer();
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Collider_Sphere", L"Com_Collider", (CComponent**)&m_pColliderCom, &ColliderDesc)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CTrainB::SetUp_ConstantTable()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	_matrix			matView;

	m_pShaderCom->Set_Value("g_matWorld", &m_pTransformCom->Get_WorldMatrix(), sizeof(_matrix));
	m_pShaderCom->Set_Value("g_matView", matView = Get_Transform(D3DTS_VIEW), sizeof(_matrix));
	m_pShaderCom->Set_Value("g_matProj", Get_Transform(D3DTS_PROJECTION), sizeof(_matrix));

	return NOERROR;
}

CGameObject * CTrainB::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CGameObject*		pInstance = new CTrainB(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		MSG_BOX("Failed To Create CGameObject Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTrainB::Clone_GameObject(void * pArg)
{
	CGameObject*		pInstance = new CTrainB(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		MSG_BOX("Failed To Create CGameObject Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTrainB::Free()
{
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pFrustumCom);
	Safe_Release(m_pMeshCom);

	CGameObject::Free();
}
