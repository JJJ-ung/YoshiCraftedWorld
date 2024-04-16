#include "stdafx.h"
#include "..\Headers\TransformBridge.h"
#include "Management.h"
#include "Light_Manager.h"
#include "Mesh_Dynamic.h"
#include "Frustum.h"
#include "Mouse_Manager.h"
#include "MeshCollision.h"
#include "Yoshi_Stage.h"
#include "Yoshi_Stage_State.h"
#include "Yoshi_Egg.h"
#include "SteamTrain.h"

USING(Client)

CTransformBridge::CTransformBridge(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CTransformBridge::CTransformBridge(const CTransformBridge & rhs)
	:CGameObject(rhs)
{
}

HRESULT CTransformBridge::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CTransformBridge::Ready_GameObject(void * pArg)
{
	m_pTag = L"../Bin/Data/Gimmick/TownTrain/Inline/TransformBridge.ini";

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

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	m_pTransformCom->SetUp_Scale(vScale);
	m_pTransformCom->SetUp_Rotation(CTransform::STATE_RIGHT, vAngle.x);
	m_pTransformCom->SetUp_Rotation(CTransform::STATE_UP, vAngle.y);
	m_pTransformCom->SetUp_Rotation(CTransform::STATE_LOOK, vAngle.z);

	m_pMeshATransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	m_pMeshATransformCom->SetUp_Scale(_vec3(0.005f, 0.005f, 0.005f));
	m_pMeshATransformCom->SetUp_Rotation(CTransform::STATE_RIGHT, vAngle.x);
	m_pMeshATransformCom->SetUp_Rotation(CTransform::STATE_UP, vAngle.y);
	m_pMeshATransformCom->SetUp_Rotation(CTransform::STATE_LOOK, vAngle.z);

	m_pMeshBTransformCom->Set_State(CTransform::STATE_POSITION, _vec3(158.3f, 0.43f, 24.f));
	m_pMeshBTransformCom->SetUp_Scale(_vec3(0.005f, 0.005f, 0.005f));
	m_pMeshBTransformCom->SetUp_Rotation(CTransform::STATE_RIGHT, vAngle.x);
	m_pMeshBTransformCom->SetUp_Rotation(CTransform::STATE_UP, vAngle.y);
	m_pMeshBTransformCom->SetUp_Rotation(CTransform::STATE_LOOK, vAngle.z);

	m_pMeshCom->SetUp_Animation(5);
	m_pMeshCom->Set_AnimationSpeed(2.f);

	CManagement* pManagemet = CManagement::GetInstance();
	Safe_AddRef(pManagemet);

	m_pPlayer = (CYoshi_Stage*)pManagemet->Get_ObjectFromLayer(SCENE_TRAIN, L"Layer_Object");

	Safe_Release(pManagemet);

	return NOERROR;
}

_int CTransformBridge::Update_GameObject(_double TimeDelta)
{
	m_TimeDelta = TimeDelta;

	return _int();
}

_int CTransformBridge::LateUpdate_GameObject(_double TimeDelta)
{
	if (m_pRendererCom == nullptr)
		return -1;

	// 아직 열리지 않음 >> 충돌A 마우스 피킹
	if (!m_bPass)
	{
		if (!m_bOpen)
		{
			if (m_pPlayer->GetFSM()->GetCurrentState() == CStage_ThrowIdle::Instance())
			{
				_vec3 vRayPos = CMouse_Manager::GetInstance()->Get_RayPos();
				_vec3 vRayDir = CMouse_Manager::GetInstance()->Get_RayDir();

				_float fDistance = 0.f;
				if (m_pMeshA_CollisionCom->Collision_Mesh(vRayPos, vRayDir, fDistance))
				{
					//cout << "Focus" << endl;
					m_pPlayer->Set_Focus(m_pMeshATransformCom, fDistance, true);
				}
				else
				{
					if (m_pPlayer->Get_FocusTarget() == m_pMeshATransformCom)
						m_pPlayer->Reset_Focus();
				}
			}

			for (auto& pEgg : *m_pPlayer->Get_ReleasedEgg())
			{
				_vec3 vRayPos = pEgg->Get_EggTransform()->Get_State(CTransform::STATE_POSITION);
				_vec3 vRayDir = pEgg->Get_ThrowDirection();

				_float fDistance = 0.f;

				m_pMeshA_CollisionCom->Convert_Local(vRayPos, vRayDir);

				if (m_pMeshA_CollisionCom->Collision_Mesh(vRayPos, vRayDir, fDistance))
				{
					if (fDistance <= 0.8f)
					{
						// 열리는 처리
						m_bOpen = true;
						m_pMeshCom->SetUp_Animation(4);
						CManagement* pManagement = CManagement::GetInstance();
						Safe_AddRef(pManagement);
						CSteamTrain* pCamera = (CSteamTrain*)pManagement->Get_ObjectFromLayer(SCENE_TRAIN, L"Layer_SteamTrain");
						pCamera->Get_Stop() = false;
						Safe_Release(pManagement);
						CSoundMgr::GetInstance()->PlaySound(L"HouseOpen.wav", CSoundMgr::GIMMICKS);
					}
				}
			}
		}
		else
		{
			if (m_pMeshCom->Finish_Animation(0.48))
			{
				m_bPass = true;
				m_pMeshCom->SetUp_Animation(3);
			}
		}
	}
	else
	{
		if (true == m_pFrustumCom->Culling_OnWorldSpace(&m_pTransformCom->Get_State(CTransform::STATE_POSITION), 8.f))
			m_pPlayer->Add_CollisionList(m_pMeshB_CollisionCom);
	}

	if (true == m_pFrustumCom->Culling_OnWorldSpace(&m_pTransformCom->Get_State(CTransform::STATE_POSITION), 8.f))
		m_pRendererCom->Add_RenderList(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

HRESULT CTransformBridge::Render_GameObject()
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

	return NOERROR;
}

HRESULT CTransformBridge::Add_Component()
{
	// For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, &CTransform::STATE(1.f, D3DXToRadian(90)))))
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_MeshATransform", (CComponent**)&m_pMeshATransformCom, &CTransform::STATE(1.f, D3DXToRadian(90)))))
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_MeshBTransform", (CComponent**)&m_pMeshBTransformCom, &CTransform::STATE(1.f, D3DXToRadian(90)))))
		return E_FAIL;

	// For.Com_Renderer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// For.Com_Mesh
	if (FAILED(CGameObject::Add_Component(SCENE_TRAIN, L"Component_Mesh_TransformBridge", L"Com_Mesh", (CComponent**)&m_pMeshCom)))
		return E_FAIL;

	// For.Com_Shader
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Shader_Mesh", L"Com_Shader", (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	// For.Com_Frustum
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Frustum", L"Com_Frustum", (CComponent**)&m_pFrustumCom)))
		return E_FAIL;

	//// For.Com_CollMeshA
	//if (FAILED(CGameObject::Add_Component(SCENE_TRAIN, L"Component_Mesh_TransformBridge_CollA", L"Com_CollMeshA", (CComponent**)&m_pCollisionMeshA)))
	//	return E_FAIL;

	//// For.Com_CollMeshB
	//if (FAILED(CGameObject::Add_Component(SCENE_TRAIN, L"Component_Mesh_TransformBridge_CollB", L"Com_CollMeshB", (CComponent**)&m_pCollisionMeshB)))
	//	return E_FAIL;

	if (FAILED(D3DXLoadMeshFromX(L"../Bin/Resources/Gimmicks/TownTrain/TransformBridge/SM_TransformBridgeC_CollisionA.X", D3DXMESH_MANAGED, m_pGraphic_Device, nullptr, nullptr, nullptr, nullptr, &m_pCollisionMeshA)))
		return E_FAIL;

	if (FAILED(D3DXLoadMeshFromX(L"../Bin/Resources/Gimmicks/TownTrain/TransformBridge/SM_TransformBridgeC_CollisionB.X", D3DXMESH_MANAGED, m_pGraphic_Device, nullptr, nullptr, nullptr, nullptr, &m_pCollisionMeshB)))
		return E_FAIL;

	// For.Com_Collision
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Mesh_Collision", L"Com_CollisionA", (CComponent**)&m_pMeshA_CollisionCom, &CMeshCollision::COLLINFO(m_pMeshATransformCom, m_pCollisionMeshA/*->Get_Mesh()*/))))
		return E_FAIL;

	// For.Com_Collision
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Mesh_Collision", L"Com_CollisionB", (CComponent**)&m_pMeshB_CollisionCom, &CMeshCollision::COLLINFO(m_pMeshBTransformCom, m_pCollisionMeshB/*->Get_Mesh()*/))))
		return E_FAIL;

	return NOERROR;
}

HRESULT CTransformBridge::SetUp_ConstantTable()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	_matrix			matView;

	m_pShaderCom->Set_Value("g_matWorld", &m_pTransformCom->Get_WorldMatrix(), sizeof(_matrix));
	m_pShaderCom->Set_Value("g_matView", matView = Get_Transform(D3DTS_VIEW), sizeof(_matrix));
	m_pShaderCom->Set_Value("g_matProj", Get_Transform(D3DTS_PROJECTION), sizeof(_matrix));

	return NOERROR;
}

CGameObject * CTransformBridge::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CGameObject*		pInstance = new CTransformBridge(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		MSG_BOX("Failed To Create CGameObject Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTransformBridge::Clone_GameObject(void * pArg)
{
	CGameObject*		pInstance = new CTransformBridge(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		MSG_BOX("Failed To Create CGameObject Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTransformBridge::Free()
{
	Safe_Release(m_pFrustumCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pMeshCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pMeshATransformCom);
	Safe_Release(m_pMeshBTransformCom);
	Safe_Release(m_pMeshA_CollisionCom);
	Safe_Release(m_pMeshB_CollisionCom);
	Safe_Release(m_pCollisionMeshA);
	Safe_Release(m_pCollisionMeshB);

	CGameObject::Free();
}
