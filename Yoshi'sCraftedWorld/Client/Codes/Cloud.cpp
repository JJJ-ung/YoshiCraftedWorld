#include "stdafx.h"
#include "..\Headers\Cloud.h"
#include "Management.h"
#include "Light_Manager.h"
#include "Mesh_Dynamic.h"
#include "Frustum.h"
#include "Collider.h"
#include "Yoshi_Stage.h"
#include "Yoshi_Stage_State.h"
#include "Yoshi_Egg.h"
#include "Mouse_Manager.h"
#include "Coin.h"
#include "Effect_Smoke.h"

USING(Client)

CCloud::CCloud(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CCloud::CCloud(const CCloud & rhs)
	:CGameObject(rhs)
{
}

HRESULT CCloud::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CCloud::Ready_GameObject(void * pArg)
{
	m_pTag = (_tchar*)pArg;

	if (FAILED(Add_Component()))
		return E_FAIL;

	_tchar m_szData01[255];
	_vec3 vPos, vScale, vAngle;

	GetPrivateProfileString(_T("Position"), _T("X"), NULL, m_szData01, 255, m_pTag);
	vPos.x = (_float) _tstof(m_szData01);
	GetPrivateProfileString(_T("Position"), _T("Y"), NULL, m_szData01, 255, m_pTag);
	vPos.y = (_float)_tstof(m_szData01);
	GetPrivateProfileString(_T("Position"), _T("Z"), NULL, m_szData01, 255, m_pTag);
	vPos.z = (_float)_tstof(m_szData01);

	GetPrivateProfileString(_T("Scale"), _T("X"), NULL, m_szData01, 255, m_pTag);
	vScale.x = (_float)_tstof(m_szData01);
	GetPrivateProfileString(_T("Scale"), _T("Y"), NULL, m_szData01, 255, m_pTag);
	vScale.y = (_float)_tstof(m_szData01);
	GetPrivateProfileString(_T("Scale"), _T("Z"), NULL, m_szData01, 255, m_pTag);
	vScale.z = (_float)_tstof(m_szData01);

	GetPrivateProfileString(_T("Rotation"), _T("X"), NULL, m_szData01, 255, m_pTag);
	vAngle.x = (_float)_tstof(m_szData01);
	GetPrivateProfileString(_T("Rotation"), _T("Y"), NULL, m_szData01, 255, m_pTag);
	vAngle.y = (_float)_tstof(m_szData01);
	GetPrivateProfileString(_T("Rotation"), _T("Z"), NULL, m_szData01, 255, m_pTag);
	vAngle.z = (_float)_tstof(m_szData01);

	GetPrivateProfileString(_T("Option"), _T("Type"), NULL, m_szData01, 255, m_pTag);
	m_eOption = (CLOUD_OPTION)_ttoi(m_szData01);

	GetPrivateProfileString(_T("Option"), _T("Option"), NULL, m_szData01, 255, m_pTag);
	m_iIndex = _ttoi(m_szData01);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	m_pTransformCom->SetUp_Scale(vScale);
	m_pTransformCom->SetUp_Rotation(CTransform::STATE_RIGHT, vAngle.x);
	m_pTransformCom->SetUp_Rotation(CTransform::STATE_UP, vAngle.y);
	m_pTransformCom->SetUp_Rotation(CTransform::STATE_LOOK, vAngle.z);

	if (m_eOption == CCloud::APPEAR)
	{
		CSoundMgr::GetInstance()->PlaySound(L"EggFlowerAppear.wav", CSoundMgr::GIMMICKS);
		m_pMeshCom->SetUp_Animation(5);
	}
	else
		m_pMeshCom->SetUp_Animation(3);

	return NOERROR;
}

_int CCloud::Update_GameObject(_double TimeDelta)
{
	m_TimeDelta = TimeDelta;

	if (m_eOption == CCloud::APPEAR)
	{
		if (m_pMeshCom->Finish_Animation())
		{
			m_eOption = CCloud::COIN;
			m_pMeshCom->SetUp_Animation(3);
		}
	}

	if (m_bHit)
	{
		if (!m_bSound)
		{
			CSoundMgr::GetInstance()->PlaySound(L"SD_PON6.wav", CSoundMgr::GIMMICKS);
			m_bSound = true;
		}
		m_pMeshCom->SetUp_Animation(0);

		m_fAngle += (_float)TimeDelta * 700.f;
		m_pTransformCom->SetUp_Rotation(_vec3(0.f, 1.f, 0.f), D3DXToRadian(m_fAngle));
		
		if (m_fAngle >= 540.f)
		{
			CManagement* pManagement = CManagement::GetInstance();
			_vec3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			for (int i = 0; i < 5; ++i)
				pManagement->Add_GameObjectToLayer(SCENE_TRAIN, L"Layer_Effect", L"GameObject_CEffect_Smoke", (void*)&CEffect_Smoke::EFFECT_INFO(m_pTransformCom->Get_State(CTransform::STATE_POSITION) + _vec3(0.f, 0.3f, 0.f), _vec3(-0.3f, -0.3f, 0.f), _vec3(0.3f, 0.3f, 0.f)));
			return OBJDEAD;
		}
	}

	m_pColliderCom->Update_Collider();

	return _int();
}

_int CCloud::LateUpdate_GameObject(_double TimeDelta)
{
	if (m_pRendererCom == nullptr)
		return -1;

	CManagement* pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return -1;

	Safe_AddRef(pManagement);

	CYoshi_Stage* pPlayer = (CYoshi_Stage*)pManagement->Get_ObjectFromLayer(SCENE_TRAIN, L"Layer_Object");

	if (m_eOption != CCloud::APPEAR)
	{
		for (auto& pEgg : *pPlayer->Get_ReleasedEgg())
		{
			if (m_pColliderCom->Collision_Sphere(pEgg->Get_Collider()) && !m_bHit)
			{
				m_bHit = true;
				m_tHitTime = system_clock::now();

				if (m_eOption == CCloud::STAIRS)
				{
					if (m_iIndex == 0)
						pManagement->Add_GameObjectToLayer(SCENE_TRAIN, L"Layer_Stairs", L"GameObject_CStairs", L"../Bin/Data/Gimmick/TownTrain/Inline/Stairs0.ini");
					else if (m_iIndex == 5)
						pManagement->Add_GameObjectToLayer(SCENE_TRAIN, L"Layer_Stairs", L"GameObject_CStairs", L"../Bin/Data/Gimmick/TownTrain/Inline/Stairs1.ini");
				}
				else if (m_eOption == CCloud::HEART)
				{
					_vec3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
					_vec3 vScale = _vec3(0.005f, 0.005f, 0.005f);
					_vec3 vAngle = _vec3(0.f, 0.f, 0.f);
					_vec3 vDir;
					for (size_t i = 0; i < 5; ++i)
					{
						GetRandomVector(&vDir, &_vec3(-1.f, 0.f, -1.f), &_vec3(1.f, 0.f, 1.f));
						vDir.y = vPos.y - 2.f;

						if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, L"Layer_Coin", L"GameObject_CCoin", (void*)&CCoin::COIN_INFO(L"Coin.X", SCENE_TRAIN, vPos, vScale, vAngle, CCoin::FALL_COIN, vDir))))
							return E_FAIL;
					}
				}
				else if (m_eOption == CCloud::COIN)
				{
					_vec3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
					_vec3 vScale = _vec3(0.005f, 0.005f, 0.005f);
					_vec3 vAngle = _vec3(0.f, 0.f, 0.f);
					_vec3 vDir;
					for (size_t i = 0; i < 5; ++i)
					{
						GetRandomVector(&vDir, &_vec3(-1.f, 0.f, -1.f), &_vec3(1.f, 0.f, 1.f));
						vDir.y = vPos.y - 2.f;

						if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, L"Layer_Coin", L"GameObject_CCoin", (void*)&CCoin::COIN_INFO(L"Coin.X", SCENE_TRAIN, vPos, vScale, vAngle, CCoin::FALL_COIN, vDir))))
							return E_FAIL;
					}
				}
			}
		}
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

	if (true == m_pFrustumCom->Culling_OnWorldSpace(&m_pTransformCom->Get_State(CTransform::STATE_POSITION), 3.f))
		m_pRendererCom->Add_RenderList(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

HRESULT CCloud::Render_GameObject()
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

_float CCloud::GetRandomFloat(float lowBound, float highBound)
{
	if (lowBound >= highBound) // bad input
		return lowBound;

	// get random float in [0, 1] interval
	float f = (rand() % 10000) * 0.0001f;

	// return float in [lowBound, highBound] interval. 
	return (f * (highBound - lowBound)) + lowBound;
}

void CCloud::GetRandomVector(D3DXVECTOR3 * out, D3DXVECTOR3 * min, D3DXVECTOR3 * max)
{
	out->x = GetRandomFloat(min->x, max->x);
	out->y = GetRandomFloat(min->y, max->y);
	out->z = GetRandomFloat(min->z, max->z);
}

HRESULT CCloud::Add_Component()
{
	// For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, &CTransform::STATE(1.f, D3DXToRadian(90)))))
		return E_FAIL;

	// For.Com_Renderer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// For.Com_Mesh
	if (FAILED(CGameObject::Add_Component(SCENE_TRAIN, L"Component_Mesh_HatenaCloud", L"Com_Mesh", (CComponent**)&m_pMeshCom)))
		return E_FAIL;

	// For.Com_Shader
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Shader_Mesh", L"Com_Shader", (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	// For.Com_Frustum
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Frustum", L"Com_Frustum", (CComponent**)&m_pFrustumCom)))
		return E_FAIL;

	// For.Com_Collider
	CCollider::COLLIDERDESC		ColliderDesc;
	ColliderDesc.vScale = _vec3(1.f, 1.f, 1.f);
	ColliderDesc.vRotation_Angle = _vec3(0.f, 0.f, 0.f);
	ColliderDesc.vPosition = _vec3(0.0f, 0.0f, 0.0f);
	ColliderDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrixPointer();
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Collider_Sphere", L"Com_Collider", (CComponent**)&m_pColliderCom, &ColliderDesc)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CCloud::SetUp_ConstantTable()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	_matrix			matView;

	m_pShaderCom->Set_Value("g_matWorld", &m_pTransformCom->Get_WorldMatrix(), sizeof(_matrix));
	m_pShaderCom->Set_Value("g_matView", matView = Get_Transform(D3DTS_VIEW), sizeof(_matrix));
	m_pShaderCom->Set_Value("g_matProj", Get_Transform(D3DTS_PROJECTION), sizeof(_matrix));

	return NOERROR;
}

CGameObject * CCloud::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CGameObject*		pInstance = new CCloud(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		MSG_BOX("Failed To Create CGameObject Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCloud::Clone_GameObject(void * pArg)
{
	CGameObject*		pInstance = new CCloud(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		MSG_BOX("Failed To Create CGameObject Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCloud::Free()
{
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pFrustumCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pMeshCom);
	Safe_Release(m_pRendererCom);

	CGameObject::Free();
}
