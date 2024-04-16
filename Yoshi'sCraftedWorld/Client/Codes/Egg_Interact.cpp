#include "stdafx.h"
#include "..\Headers\Egg_Interact.h"

#include "Management.h"
#include "Light_Manager.h"

#include "Yoshi_Stage.h"
#include "Yoshi_Stage_State.h"
#include "Yoshi_Egg.h"
#include "Collider.h"
#include "Mouse_Manager.h"

#include "Frustum.h"

USING(Client)


CEgg_Interact::CEgg_Interact(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CEgg_Interact::CEgg_Interact(const CEgg_Interact & rhs)
	:CGameObject(rhs)
{
}

HRESULT CEgg_Interact::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CEgg_Interact::Ready_GameObject(void * pArg)
{
	EGGINTERACT_INFO* p = (EGGINTERACT_INFO*)pArg;

	m_tEnvironmentInfo.eSceneID = p->eSceneID;
	m_tEnvironmentInfo.pComponentTag = p->pComponentTag;
	m_tEnvironmentInfo.vPos = p->vPos;
	m_tEnvironmentInfo.vScale = p->vScale;
	m_tEnvironmentInfo.vAngle = p->vAngle;

	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_tEnvironmentInfo.vPos);
	m_pTransformCom->SetUp_Scale(m_tEnvironmentInfo.vScale);
	m_pTransformCom->SetUp_Rotation(CTransform::STATE_RIGHT, m_tEnvironmentInfo.vAngle.x);
	m_pTransformCom->SetUp_Rotation(CTransform::STATE_UP, m_tEnvironmentInfo.vAngle.y);
	m_pTransformCom->SetUp_Rotation(CTransform::STATE_LOOK, m_tEnvironmentInfo.vAngle.z);

	return NOERROR;
}

_int CEgg_Interact::Update_GameObject(_double TimeDelta)
{
	if (m_bObjectHit)
	{
		if (!m_bSound)
		{
			CSoundMgr::GetInstance()->PlaySound(L"PAPER_IMPACT01.wav", CSoundMgr::GIMMICKS);
			m_bSound = true;
		}

		_vec3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		vPos.y += m_fJumpPower * (_float)TimeDelta;
		m_fJumpPower -= m_fJumpAccel * (_float)TimeDelta;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);

		m_pTransformCom->Move_Direction(m_vDirection, TimeDelta);

		if (m_fJumpAccel == -30.f)
			return OBJDEAD;
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

_int CEgg_Interact::LateUpdate_GameObject(_double TimeDelta)
{
	if (m_pRendererCom == nullptr)
		return -1;

	if (true == m_pFrustumCom->Culling_OnWorldSpace(&m_pTransformCom->Get_State(CTransform::STATE_POSITION), 1.f))
	{
		CManagement* pManagemet = CManagement::GetInstance();
		Safe_AddRef(pManagemet);

		CYoshi_Stage* pPlayer = (CYoshi_Stage*)pManagemet->Get_ObjectFromLayer(SCENE_TRAIN, L"Layer_Object");

		if (pPlayer->GetFSM()->GetCurrentState() == CStage_ThrowIdle::Instance())
		{
			_vec3 vRayPos = CMouse_Manager::GetInstance()->Get_RayPos();
			_vec3 vRayDir = CMouse_Manager::GetInstance()->Get_RayDir();

			_float fDistance = 0.f;
			if (m_pColliderCom->Collision_Ray(vRayPos, vRayDir, fDistance))
				pPlayer->Set_Focus(m_pTransformCom, fDistance, true);
			else
			{
				if (pPlayer->Get_FocusTarget() == m_pTransformCom)
					pPlayer->Reset_Focus();
			}
		}

		for (auto& pEgg : *pPlayer->Get_ReleasedEgg())
		{
			if (!m_bObjectHit && m_pColliderCom->Collision_Sphere(pEgg->Get_Collider()))
			{
				//pEgg->Change_Mode(CYoshi_Egg::RELEASE_DEAD);
				m_bObjectHit = true;
			}
		}

		Safe_Release(pManagemet);

		m_pRendererCom->Add_RenderList(CRenderer::RENDER_NONALPHA, this);
	}

	return _int();
}

HRESULT CEgg_Interact::Render_GameObject()
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

HRESULT CEgg_Interact::Add_Component()
{
	// For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, &CTransform::STATE(1.f, D3DXToRadian(90)))))
		return E_FAIL;

	// For.Com_Renderer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// For.Com_Mesh
	if (FAILED(CGameObject::Add_Component(m_tEnvironmentInfo.eSceneID, m_tEnvironmentInfo.pComponentTag.c_str(), L"Com_Mesh", (CComponent**)&m_pMeshCom)))
		return E_FAIL;

	// For.Com_Shader
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Shader_Mesh", L"Com_Shader", (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	// For.Com_Collider
	CCollider::COLLIDERDESC		ColliderDesc;
	ColliderDesc.vScale = _vec3(2.f, 2.f, 2.f);
	ColliderDesc.vRotation_Angle = _vec3(0.f, 0.f, 0.f);
	ColliderDesc.vPosition = _vec3(0.0f, 0.5f, 0.0f);
	ColliderDesc.pParentMatrix = &m_matCollider;
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Collider_Sphere", L"Com_Collider", (CComponent**)&m_pColliderCom, &ColliderDesc)))
		return E_FAIL;

	// For.Com_Frustum
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Frustum", L"Com_Frustum", (CComponent**)&m_pFrustumCom)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CEgg_Interact::SetUp_ConstantTable()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	_matrix			matView;

	m_pShaderCom->Set_Value("g_matWorld", &m_pTransformCom->Get_WorldMatrix(), sizeof(_matrix));
	m_pShaderCom->Set_Value("g_matView", matView = Get_Transform(D3DTS_VIEW), sizeof(_matrix));
	m_pShaderCom->Set_Value("g_matProj", Get_Transform(D3DTS_PROJECTION), sizeof(_matrix));

	return NOERROR;
}

CGameObject * CEgg_Interact::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CGameObject*		pInstance = new CEgg_Interact(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		MSG_BOX("Failed To Create CGameObject Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CEgg_Interact::Clone_GameObject(void * pArg)
{
	CGameObject*		pInstance = new CEgg_Interact(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		MSG_BOX("Failed To Create CGameObject Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEgg_Interact::Free()
{
	Safe_Release(m_pFrustumCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pMeshCom);
	Safe_Release(m_pRendererCom);

	CGameObject::Free();
}
