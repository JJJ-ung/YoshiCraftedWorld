#include "stdafx.h"
#include "..\Headers\Coin.h"
#include "Management.h"
#include "Light_Manager.h"
#include "Collider.h"
#include "UI_Coin.h"
#include "Yoshi_Stage.h"
#include "Yoshi_Stage_State.h"
#include "Yoshi_Egg.h"
#include "Mouse_Manager.h"
#include "Frustum.h"

USING(Client)

CCoin::CCoin(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CCoin::CCoin(const CCoin & rhs)
	:CGameObject(rhs)
{
}

HRESULT CCoin::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CCoin::Ready_GameObject(void * pArg)
{
	COIN_INFO* p = (COIN_INFO*)pArg;

	m_tInfo.eSceneID = p->eSceneID;
	m_tInfo.pComponentTag = p->pComponentTag;
	m_tInfo.vPos = p->vPos;
	m_tInfo.vScale = p->vScale;
	m_tInfo.vAngle = p->vAngle;
	m_tInfo.eCoinType = p->eCoinType;
	m_tInfo.vDirection = p->vDirection;

	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_tInfo.vPos);
	m_pTransformCom->SetUp_Scale(m_tInfo.vScale);
	m_pTransformCom->SetUp_Rotation(CTransform::STATE_RIGHT, m_tInfo.vAngle.x);
	m_pTransformCom->SetUp_Rotation(CTransform::STATE_UP, m_tInfo.vAngle.y);
	m_pTransformCom->SetUp_Rotation(CTransform::STATE_LOOK, m_tInfo.vAngle.z);

	return NOERROR;
}

_int CCoin::Update_GameObject(_double TimeDelta)
{
	if (m_bDead)
	{
		return OBJDEAD;
	}

	if (m_tInfo.eCoinType == FALL_COIN)
	{
		_vec3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		vPos.y += m_fJumpPower * (_float)TimeDelta;
		m_fJumpPower -= m_fJumpAccel * (_float)TimeDelta;

		if (vPos.y < m_tInfo.vDirection.y)
		{
			vPos.y = m_tInfo.vDirection.y;
			m_tInfo.eCoinType = NORMAL_COIN;
		}

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
		m_pTransformCom->Move_Direction(m_tInfo.vDirection, TimeDelta * 0.4);
	}
	else
	{
		m_fAngle += (_float)TimeDelta * 4.f;
		if (m_fAngle >= 360.f)
			m_fAngle = 0.f;
		m_pTransformCom->SetUp_Rotation(_vec3(0.f, 1.f, 0.f), m_fAngle);
	}

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
	
	return _int();
}
 
_int CCoin::LateUpdate_GameObject(_double TimeDelta)
{
	if (m_pRendererCom == nullptr)
		return -1;

	CManagement* pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return -1;

	Safe_AddRef(pManagement);

	CYoshi_Stage* pPlayer = (CYoshi_Stage*)pManagement->Get_ObjectFromLayer(SCENE_TRAIN, L"Layer_Object");

	for (auto& pEgg : *pPlayer->Get_ReleasedEgg())
	{
		if (m_pColliderCom->Collision_Sphere(pEgg->Get_Collider()))
		{
			if (m_tInfo.eCoinType != FALL_COIN)
			{
				CUI_Coin* pUI = (CUI_Coin*)pManagement->Get_ObjectFromLayer(SCENE_TRAIN, L"Layer_CoinUI");
				if (m_tInfo.eCoinType == CCoin::NORMAL_COIN)
					pUI->Add_Coin(CUI_Coin::COIN_NORMAL);
				else if (m_tInfo.eCoinType == CCoin::LARGE_COIN)
					pUI->Add_Coin(CUI_Coin::COIN_LARGE);

				m_bDead = true;
			}
		}
	}

	if (m_pColliderCom->Collision_Sphere((CCollider*)pManagement->Get_Component(SCENE_TRAIN, L"Layer_Object", L"Com_Collider")))
	{
		CUI_Coin* pUI = (CUI_Coin*)pManagement->Get_ObjectFromLayer(SCENE_TRAIN, L"Layer_CoinUI");
		if (m_tInfo.eCoinType == CCoin::NORMAL_COIN)
			pUI->Add_Coin(CUI_Coin::COIN_NORMAL);
		else if (m_tInfo.eCoinType == CCoin::LARGE_COIN)
			pUI->Add_Coin(CUI_Coin::COIN_LARGE);

		m_bDead = true;
	}
	
	if (pPlayer->GetFSM()->GetCurrentState() == CStage_ThrowIdle::Instance())
	{
		_vec3 vRayPos = CMouse_Manager::GetInstance()->Get_RayPos();
		_vec3 vRayDir = CMouse_Manager::GetInstance()->Get_RayDir();

		_float fDistance = 0.f;
		if (m_pColliderCom->Collision_Ray(vRayPos, vRayDir, fDistance))
			pPlayer->Set_Focus(m_pTransformCom, fDistance);
		else
		{
			if (pPlayer->Get_FocusTarget() == m_pTransformCom)
				pPlayer->Reset_Focus();
		}
	}

	Safe_Release(pManagement);

	if (true == m_pFrustumCom->Culling_OnWorldSpace(&m_pTransformCom->Get_State(CTransform::STATE_POSITION), 1.f))
		m_pRendererCom->Add_RenderList(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

HRESULT CCoin::Render_GameObject()
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

HRESULT CCoin::Add_Component()
{
	// For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, &CTransform::STATE(1.f, D3DXToRadian(90)))))
		return E_FAIL;

	// For.Com_Renderer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// For.Com_Mesh
	if (FAILED(CGameObject::Add_Component(m_tInfo.eSceneID, m_tInfo.pComponentTag.c_str(), L"Com_Mesh", (CComponent**)&m_pMeshCom)))
		return E_FAIL;

	// For.Com_Shader
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Shader_Mesh", L"Com_Shader", (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	// For.Com_Collider
	CCollider::COLLIDERDESC		ColliderDesc;
	ColliderDesc.vScale = _vec3(1.f, 1.f, 1.f);
	ColliderDesc.vRotation_Angle = _vec3(0.f, 0.f, 0.f);
	ColliderDesc.vPosition = _vec3(0.0f, 0.0f, 0.0f);
	ColliderDesc.pParentMatrix = &m_matCollider;
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Collider_Sphere", L"Com_Collider", (CComponent**)&m_pColliderCom, &ColliderDesc)))
		return E_FAIL;

	// For.Com_Frustum
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Frustum", L"Com_Frustum", (CComponent**)&m_pFrustumCom)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CCoin::SetUp_ConstantTable()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	_matrix			matView;

	m_pShaderCom->Set_Value("g_matWorld", &m_pTransformCom->Get_WorldMatrix(), sizeof(_matrix));
	m_pShaderCom->Set_Value("g_matView", matView = Get_Transform(D3DTS_VIEW), sizeof(_matrix));
	m_pShaderCom->Set_Value("g_matProj", Get_Transform(D3DTS_PROJECTION), sizeof(_matrix));

	return NOERROR;
}

CGameObject * CCoin::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CGameObject*		pInstance = new CCoin(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		MSG_BOX("Failed To Create CGameObject Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCoin::Clone_GameObject(void * pArg)
{
	CGameObject*		pInstance = new CCoin(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		MSG_BOX("Failed To Create CGameObject Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCoin::Free()
{
	Safe_Release(m_pFrustumCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pMeshCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pColliderCom);

	CGameObject::Free();
}
