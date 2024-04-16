#include "stdafx.h"
#include "..\Headers\TrainPlatform.h"
#include "Management.h"
#include "Light_Manager.h"
#include "Mesh_Dynamic.h"
#include "Frustum.h"
#include "Yoshi_Stage.h"
#include "MeshCollision.h"
#include "Open_FlowerUI.h"

USING(Client)

CTrainPlatform::CTrainPlatform(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CTrainPlatform::CTrainPlatform(const CTrainPlatform & rhs)
	:CGameObject(rhs)
{
}

HRESULT CTrainPlatform::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CTrainPlatform::Ready_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	_vec3 vPos, vScale, vAngle;

	vPos = _vec3(184.18f, 0.f, 34.1f);
	vScale = _vec3(1.f, 1.f, 1.f);
	vAngle = _vec3(0.f, 180.f, 0.f);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	m_pTransformCom->SetUp_Scale(vScale);
	m_pTransformCom->SetUp_Rotation(CTransform::STATE_RIGHT, vAngle.x);
	m_pTransformCom->SetUp_Rotation(CTransform::STATE_UP, vAngle.y);
	m_pTransformCom->SetUp_Rotation(CTransform::STATE_LOOK, vAngle.z);

	m_pCollisionTransform->Set_State(CTransform::STATE_POSITION, vPos);
	m_pCollisionTransform->SetUp_Scale(_vec3(0.005f, 0.005f, 0.005f));
	m_pCollisionTransform->SetUp_Rotation(CTransform::STATE_RIGHT, 0.f);
	m_pCollisionTransform->SetUp_Rotation(CTransform::STATE_UP, 0.f);
	m_pCollisionTransform->SetUp_Rotation(CTransform::STATE_LOOK, 0.f);

	m_pMeshCom->SetUp_Animation(m_iAnimationIndex);

	CManagement* pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return E_FAIL;
	Safe_AddRef(pManagement);

	m_pPlayer = (CYoshi_Stage*)pManagement->Get_ObjectFromLayer(SCENE_TRAIN, L"Layer_Object", 0);
	if (m_pPlayer == nullptr)
		return E_FAIL;

	Safe_Release(pManagement);

	return NOERROR;
}

_int CTrainPlatform::Update_GameObject(_double TimeDelta)
{
	m_TimeDelta = TimeDelta;

	switch (m_eState)
	{
	case Client::CTrainPlatform::START:
		Update_Start(TimeDelta);
		break;
	case Client::CTrainPlatform::CUTSCENE:
		Update_CutScene(TimeDelta);
		break;
	case Client::CTrainPlatform::IDLE:
		Update_Idle(TimeDelta);
		break;
	}

	return _int();
}

_int CTrainPlatform::LateUpdate_GameObject(_double TimeDelta)
{
	if (m_pRendererCom == nullptr)
		return -1;

	if (true == m_pFrustumCom->Culling_OnWorldSpace(&m_pTransformCom->Get_State(CTransform::STATE_POSITION), 3.f))
		m_pRendererCom->Add_RenderList(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

HRESULT CTrainPlatform::Render_GameObject()
{
	if (nullptr == m_pMeshCom ||
		nullptr == m_pShaderCom)
		return E_FAIL;

	if(m_bPlayAnimation)
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

	return NOERROR;
}

HRESULT CTrainPlatform::Change_State(PLATFORM_STATE eState)
{
	if (m_eState != eState)
		m_eState = eState;

	if (eState == CUTSCENE)
	{
		if (m_iAnimationIndex == 3)
			m_iAnimationIndex = 0;
		else if (m_iAnimationIndex == 1)
		{
			m_pMeshCom->Set_AnimationSpeed(2.f);
			m_iAnimationIndex = 2;
			CSoundMgr::GetInstance()->PlaySound(L"SD_GACYA_APPLAUSE_M01.wav", CSoundMgr::EFFECT);
		}
	}

	m_pMeshCom->SetUp_Animation(m_iAnimationIndex);

	return NOERROR;
}

_int CTrainPlatform::Update_Start(_double TimeDelta)
{
	return _int();
}

_int CTrainPlatform::Update_Idle(_double TimeDelta)
{
	if (m_iAnimationIndex == 3)
	{
		if (true == m_pFrustumCom->Culling_OnWorldSpace(&m_pTransformCom->Get_State(CTransform::STATE_POSITION), 3.f))
		{
			m_pPlayer->Add_CollisionList(m_pCollisionCom);
			m_pPlayer->Add_HeadCollisionList(m_pCollisionCom);
		}
	}

	return _int();
}

_int CTrainPlatform::Update_CutScene(_double TimeDelta)
{
	if (m_pMeshCom->Finish_Animation(0.5))
	{
		if (m_iAnimationIndex == 0)
			m_iAnimationIndex = 1;
		else if (m_iAnimationIndex == 2)
		{
			CManagement* pManagement = CManagement::GetInstance();
			Safe_AddRef(pManagement);
			if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, L"Layer_Trigger", L"GameObject_CTrigger", (_tchar*)L"ZTrigger10_0")))
				return E_FAIL;

			COpen_FlowerUI* pUI = (COpen_FlowerUI*)pManagement->Get_ObjectFromLayer(SCENE_TRAIN, L"Layer_FlowerOpenUI");
			pUI->Get_CurrentFrame() = 1.0;

			Safe_Release(pManagement);
			m_iAnimationIndex = 3;
		}

		m_pMeshCom->SetUp_Animation(m_iAnimationIndex);
		Change_State(CTrainPlatform::IDLE);
	}

	return _int();
}


HRESULT CTrainPlatform::Add_Component()
{
	// For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, &CTransform::STATE(1.f, D3DXToRadian(90)))))
		return E_FAIL;

	// For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_CoillisionTransform", (CComponent**)&m_pCollisionTransform, &CTransform::STATE(1.f, D3DXToRadian(90)))))
		return E_FAIL;

	// For.Com_Renderer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// For.Com_Mesh
	if (FAILED(CGameObject::Add_Component(SCENE_TRAIN, L"Component_Mesh_TrainPlatform", L"Com_Mesh", (CComponent**)&m_pMeshCom)))
		return E_FAIL;

	// For.Com_Shader
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Shader_Mesh", L"Com_Shader", (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	// For.Com_Frustum
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Frustum", L"Com_Frustum", (CComponent**)&m_pFrustumCom)))
		return E_FAIL;

	if (FAILED(D3DXLoadMeshFromX(L"../Bin/Resources/Gimmicks/TownTrain/TrainPlatform/TrainPlatform_Collision.X", D3DXMESH_MANAGED, m_pGraphic_Device, nullptr, nullptr, nullptr, nullptr, &m_pCollisionMesh)))
		return E_FAIL;

	// For.Com_Collision
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Mesh_Collision", L"Com_Collision", (CComponent**)&m_pCollisionCom, &CMeshCollision::COLLINFO(m_pCollisionTransform, m_pCollisionMesh))))
		return E_FAIL;

	return NOERROR;
}

HRESULT CTrainPlatform::SetUp_ConstantTable()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	_matrix			matView;

	m_pShaderCom->Set_Value("g_matWorld", &m_pTransformCom->Get_WorldMatrix(), sizeof(_matrix));
	m_pShaderCom->Set_Value("g_matView", matView = Get_Transform(D3DTS_VIEW), sizeof(_matrix));
	m_pShaderCom->Set_Value("g_matProj", Get_Transform(D3DTS_PROJECTION), sizeof(_matrix));

	return NOERROR;
}

CGameObject * CTrainPlatform::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CGameObject*		pInstance = new CTrainPlatform(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		MSG_BOX("Failed To Create CGameObject Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTrainPlatform::Clone_GameObject(void * pArg)
{
	CGameObject*		pInstance = new CTrainPlatform(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		MSG_BOX("Failed To Create CGameObject Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTrainPlatform::Free()
{
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pFrustumCom);
	Safe_Release(m_pMeshCom);
	Safe_Release(m_pCollisionMesh);
	Safe_Release(m_pCollisionTransform);
	Safe_Release(m_pCollisionCom);

	CGameObject::Free();
}
