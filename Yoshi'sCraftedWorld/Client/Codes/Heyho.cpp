#include "stdafx.h"
#include "..\Headers\Heyho.h"
#include "Management.h"
#include "Light_Manager.h"
#include "Mesh_Dynamic.h"
#include "Input_Device.h"
#include "Collider.h"
#include "Heyho_State.h"
#include "Yoshi_Stage.h"
#include "Yoshi_Tongue.h"
#include "Yoshi_Egg.h"
#include "Mouse_Manager.h"
#include "Yoshi_Stage_State.h"
#include "Frustum.h"
#include "Effect_Smoke.h"
#include "Effect_Star.h"

USING(Client)

CHeyho::CHeyho(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CHeyho::CHeyho(const CHeyho & rhs)
	:CGameObject(rhs)
{
}

HRESULT CHeyho::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CHeyho::Ready_GameObject(void * pArg)
{
	memcpy(&m_tInfo, pArg, sizeof(MONSTER_INFO));

	m_iRandom = rand() % 7;
	//m_iRandom = 0;

	if (FAILED(Add_Component()))
		return E_FAIL;

//	m_pTransformCom->SetUp_Rotation(_vec3(0.f, 1.f, 0.f), D3DXToRadian(180.f));
	
	m_vStartPos = m_tInfo.vPos;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_tInfo.vPos);
	m_pTransformCom->SetUp_Scale(m_tInfo.vScale);
	m_pTransformCom->SetUp_Rotation(CTransform::STATE_RIGHT, m_tInfo.vRoation.x);
	m_pTransformCom->SetUp_Rotation(CTransform::STATE_UP, m_tInfo.vRoation.y);
	m_pTransformCom->SetUp_Rotation(CTransform::STATE_LOOK, m_tInfo.vRoation.z);

	_vec3 vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);

	m_pState = new CStateMachine<CHeyho>(this);

	switch (m_tInfo.iOption3)
	{
	case 0:
		m_pState->SetCurrentState(CHeyho_Idle::Instance());
		break;
	case 1:
		m_pState->SetCurrentState(CHeyho_Walk::Instance());
		break;
	case 3:
		m_pState->SetCurrentState(CHeyho_Sleep::Instance());
		break;
	case 4:
		m_pState->SetCurrentState(CHeyho_Jump::Instance());
		break;
	case 5:
		m_pState->SetCurrentState(CHeyho_Look::Instance());
		break;
	default:
		m_pState->SetCurrentState(CHeyho_Idle::Instance());
		break;
	}
	m_pState->GetCurrentState()->Enter(this);

	CManagement* pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return -1;

	Safe_AddRef(pManagement);

	m_pYoshiTongue = (CYoshi_Tongue*)pManagement->Get_ObjectFromLayer(SCENE_TRAIN, L"Layer_Tongue", 0);

	Safe_Release(pManagement);

	return NOERROR;
}

_int CHeyho::Update_GameObject(_double TimeDelta)
{
	m_TimeDelta = TimeDelta;

	m_pState->Update();

	Set_HeadCollider();

	if (m_bTongueCollision && m_bMouthCollision)
	{
		CManagement* pManagement = CManagement::GetInstance();
		Safe_AddRef(pManagement);

		CYoshi_Stage* pPlayer = (CYoshi_Stage*)pManagement->Get_ObjectFromLayer(SCENE_TRAIN, L"Layer_Object", 0);
		pPlayer->Get_TongueCollision() = true;
		Safe_Release(pManagement);

		return OBJDEAD;
	}

	if (m_bHeadCollision)
	{
		CManagement* pManagement = CManagement::GetInstance();
		Safe_AddRef(pManagement);

		CYoshi_Stage* pPlayer = (CYoshi_Stage*)pManagement->Get_ObjectFromLayer(SCENE_TRAIN, L"Layer_Object", 0);
		pPlayer->Get_HeadCollision() = true;

		Safe_Release(pManagement);
	}
	else
	{
		if (m_bBodyCollision)
		{
			CManagement* pManagement = CManagement::GetInstance();
			Safe_AddRef(pManagement);

			CYoshi_Stage* pPlayer = (CYoshi_Stage*)pManagement->Get_ObjectFromLayer(SCENE_TRAIN, L"Layer_Object", 0);
			pPlayer->Get_KnockBack() = true;
			Safe_Release(pManagement);
		}
	}

	m_pColliderCom->Update_Collider();
	m_pHeadColliderCom->Update_Collider();
	m_pEggColliderCom->Update_Collider();

	if (m_bDead)
	{
		CManagement* pManagement = CManagement::GetInstance();
		_vec3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		for(int i = 0; i < 5; ++i)
			pManagement->Add_GameObjectToLayer(SCENE_TRAIN, L"Layer_Effect", L"GameObject_CEffect_Smoke", (void*)&CEffect_Smoke::EFFECT_INFO(m_pTransformCom->Get_State(CTransform::STATE_POSITION) + _vec3(0.f, 0.3f, 0.f), _vec3(-0.3f, -0.3f, 0.f), _vec3(0.3f, 0.3f, 0.f)));
		for (int i = 0; i < 5; ++i)
			pManagement->Add_GameObjectToLayer(SCENE_TRAIN, L"Layer_Effect", L"GameObject_CEffect_Star", (void*)&CEffect_Star::EFFECT_INFO(m_pTransformCom->Get_State(CTransform::STATE_POSITION) + _vec3(0.f, 0.3f, 0.f), _vec3(-0.7f, -0.7f, 0.f), _vec3(0.7f, 0.7f, 0.f), i));

		CSoundMgr::GetInstance()->PlaySound(L"Enemy_dead01.wav", CSoundMgr::MONSTER);
		return OBJDEAD;
	}

	return _int();
}

_int CHeyho::LateUpdate_GameObject(_double TimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	CManagement* pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return -1;

	Safe_AddRef(pManagement);

	if (m_pColliderCom->Collision_Sphere((CCollider*)pManagement->Get_Component(SCENE_TRAIN, L"Layer_Object", L"Com_Collider")))
		m_bBodyCollision = true;
	else
		m_bBodyCollision = false;

	if (m_pHeadColliderCom->Collision_Sphere((CCollider*)pManagement->Get_Component(SCENE_TRAIN, L"Layer_Object", L"Com_Collider")))
		m_bHeadCollision = true;
	else
		m_bHeadCollision = false;

	if (m_pYoshiTongue->Get_Render())
	{
		if (m_pColliderCom->Collision_Sphere((CCollider*)pManagement->Get_Component(SCENE_TRAIN, L"Layer_Tongue", L"Com_Collider")))
			m_bTongueCollision = true;
		else
			m_bTongueCollision = false;
	}

	if (m_pColliderCom->Collision_Sphere((CCollider*)pManagement->Get_Component(SCENE_TRAIN, L"Layer_Object", L"Com_MouthCollider")))
		m_bMouthCollision = true;
	else
		m_bMouthCollision = false;

	CYoshi_Stage* pPlayer = (CYoshi_Stage*)pManagement->Get_ObjectFromLayer(SCENE_TRAIN, L"Layer_Object");

	m_bEggCollision = false;
	for (auto& pEgg : *pPlayer->Get_ReleasedEgg())
	{
		if (m_pEggColliderCom->Collision_Sphere(pEgg->Get_Collider()))
		{
			m_bEggCollision = true;
			m_vDirection = pEgg->Get_ThrowDirection();
		}
	}

	if (pPlayer->GetFSM()->GetCurrentState() == CStage_ThrowIdle::Instance())
	{
		_vec3 vRayPos = CMouse_Manager::GetInstance()->Get_RayPos();
		_vec3 vRayDir = CMouse_Manager::GetInstance()->Get_RayDir();

		_float fDistance = 0.f;
		if (m_pEggColliderCom->Collision_Ray(vRayPos, vRayDir, fDistance))
			pPlayer->Set_Focus(m_pTransformCom, fDistance);
		else
		{
			if (pPlayer->Get_FocusTarget() == m_pTransformCom)
				pPlayer->Reset_Focus();
		}
	}

	if (true == m_pFrustumCom->Culling_OnWorldSpace(&m_pTransformCom->Get_State(CTransform::STATE_POSITION), 1.f))	
		m_pRendererCom->Add_RenderList(CRenderer::RENDER_NONALPHA, this);

	Safe_Release(pManagement);

	return _int();
}

HRESULT CHeyho::Render_GameObject()
{
	if (nullptr == m_pMeshCom ||
		nullptr == m_pShaderCom)
		return E_FAIL;

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
			if (m_iRandom == 0)
				m_pMeshCom->SetTexture_OnShader(pEffect, "g_BaseTexture", i, j, MESHTEXTURE::TYPE_DIFFUSE);
			else
				m_pTextureCom->SetUp_OnShader(pEffect, "g_BaseTexture");
			m_pMeshCom->SetMaterial_OnShader(pEffect, "g_Material", i, j);

			pEffect->CommitChanges();

			m_pMeshCom->Render_Mesh(i, j);
		}
	}

	pEffect->EndPass();
	pEffect->End();

	m_pColliderCom->Render_Collider();
	m_pHeadColliderCom->Render_Collider();
	m_pEggColliderCom->Render_Collider();

	Safe_Release(pEffect);

	return NOERROR;
}

void CHeyho::Set_HeadCollider()
{
	_matrix matWorld = m_pTransformCom->Get_WorldMatrix();
	_matrix matFrame = m_pMeshCom->Get_FrameMatrix("J_coin_B");

	m_matHeadCollider = matFrame * matWorld;

	_vec3 vRight, vUp, vLook, vPos;

	memcpy(&vRight, &m_matHeadCollider.m[0][0], sizeof(_vec3));
	memcpy(&vUp, &m_matHeadCollider.m[1][0], sizeof(_vec3));
	memcpy(&vLook, &m_matHeadCollider.m[2][0], sizeof(_vec3));
	memcpy(&vPos, &m_matHeadCollider.m[3][0], sizeof(_vec3));

	D3DXVec3Normalize(&vRight, &vRight);
	D3DXVec3Normalize(&vUp, &vUp);
	D3DXVec3Normalize(&vLook, &vLook);
	vPos += _vec3(0.f, 0.1f, 0.f);

	memcpy(&m_matHeadCollider.m[0][0], &vRight, sizeof(_vec3));
	memcpy(&m_matHeadCollider.m[1][0], &vUp, sizeof(_vec3));
	memcpy(&m_matHeadCollider.m[2][0], &vLook, sizeof(_vec3));
	memcpy(&m_matHeadCollider.m[3][0], &vPos, sizeof(_vec3));
}

HRESULT CHeyho::Add_Component()
{
	// For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, &CTransform::STATE(1.f, D3DXToRadian(90)))))
		return E_FAIL;

	// For.Com_Mesh
	list<const char*>	FrameList;
	FrameList.push_back("J_coin_B");
	if (FAILED(CGameObject::Add_Component(SCENE_TRAIN, L"Component_Mesh_Heyho", L"Com_Mesh", (CComponent**)&m_pMeshCom, &FrameList)))
		return E_FAIL;

	// For.Com_Renderer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// For.Com_Shader
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Shader_Mesh", L"Com_Shader", (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	// For.Com_Collider
	CCollider::COLLIDERDESC		ColliderDesc;
	ColliderDesc.vScale = _vec3(0.6f, 0.6f, 0.6f);
	ColliderDesc.vRotation_Angle = _vec3(0.f, 0.f, 0.f);
	ColliderDesc.vPosition = _vec3(0.0f, 0.2f, 0.0f);
	ColliderDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrixPointer();
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Collider_Sphere", L"Com_Collider", (CComponent**)&m_pColliderCom, &ColliderDesc)))
		return E_FAIL;

	// For.Com_HeadCollider
	CCollider::COLLIDERDESC		HeadColliderDesc;
	HeadColliderDesc.vScale = _vec3(0.8f, 0.8f, 0.8f);
	HeadColliderDesc.vRotation_Angle = _vec3(0.f, 0.f, 0.f);
	HeadColliderDesc.vPosition = _vec3(0.0f, 0.f, 0.0f);
	HeadColliderDesc.pParentMatrix = &m_matHeadCollider;
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Collider_Sphere", L"Com_HeadCollider", (CComponent**)&m_pHeadColliderCom, &HeadColliderDesc)))
		return E_FAIL;

	// For.Com_EggCollider
	CCollider::COLLIDERDESC		EggColliderDesc;
	EggColliderDesc.vScale = _vec3(0.8f, 0.8f, 0.8f);
	EggColliderDesc.vRotation_Angle = _vec3(0.f, 0.f, 0.f);
	EggColliderDesc.vPosition = _vec3(0.0f, 0.3f, 0.0f);
	EggColliderDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrixPointer();
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Collider_Sphere", L"Com_EggCollider", (CComponent**)&m_pEggColliderCom, &EggColliderDesc)))
		return E_FAIL;

	// For.Com_Frustum
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Frustum", L"Com_Frustum", (CComponent**)&m_pFrustumCom)))
		return E_FAIL;
	
	switch (m_iRandom)
	{
	case 1:
		if (FAILED(CGameObject::Add_Component(SCENE_TRAIN, L"Component_Heyho_Blue", L"Com_Texture", (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;
	case 2:
		if (FAILED(CGameObject::Add_Component(SCENE_TRAIN, L"Component_Heyho_Green", L"Com_Texture", (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;
	case 3:
		if (FAILED(CGameObject::Add_Component(SCENE_TRAIN, L"Component_Heyho_LightBlue", L"Com_Texture", (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;
	case 4:
		if (FAILED(CGameObject::Add_Component(SCENE_TRAIN, L"Component_Heyho_Orange", L"Com_Texture", (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;
	case 5:
		if (FAILED(CGameObject::Add_Component(SCENE_TRAIN, L"Component_Heyho_Pink", L"Com_Texture", (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;
	case 6:
		if (FAILED(CGameObject::Add_Component(SCENE_TRAIN, L"Component_Heyho_Yellow", L"Com_Texture", (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;
	default:
		break;
	}

	return NOERROR;
}

HRESULT CHeyho::SetUp_ConstantTable()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	_matrix			matView;

	m_pShaderCom->Set_Value("g_matWorld", &m_pTransformCom->Get_WorldMatrix(), sizeof(_matrix));
	m_pShaderCom->Set_Value("g_matView", matView = Get_Transform(D3DTS_VIEW), sizeof(_matrix));
	m_pShaderCom->Set_Value("g_matProj", Get_Transform(D3DTS_PROJECTION), sizeof(_matrix));

	return NOERROR;
}

CGameObject * CHeyho::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CGameObject*		pInstance = new CHeyho(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		MSG_BOX("Failed To Create CGameObject Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CHeyho::Clone_GameObject(void * pArg)
{
	CGameObject*		pInstance = new CHeyho(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		MSG_BOX("Failed To Create CGameObject Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHeyho::Free()
{
	Safe_Release(m_pFrustumCom);
	Safe_Release(m_pState);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pHeadColliderCom);
	Safe_Release(m_pEggColliderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pMeshCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTextureCom);

	CGameObject::Free();
}
