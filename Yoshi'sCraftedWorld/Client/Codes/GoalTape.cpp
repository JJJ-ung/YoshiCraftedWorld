#include "stdafx.h"
#include "..\Headers\GoalTape.h"
#include "Management.h"
#include "Light_Manager.h"
#include "Mesh_Dynamic.h"
#include "Frustum.h"
#include "Input_Device.h"
#include "Collider.h"
#include "Yoshi_Stage.h"
#include "Yoshi_Stage_State.h"

USING(Client)

CGoalTape::CGoalTape(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CGoalTape::CGoalTape(const CGoalTape & rhs)
	:CGameObject(rhs)
{
}

HRESULT CGoalTape::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CGoalTape::Ready_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	_tchar m_szData01[255];
	_vec3 vPos, vScale, vAngle;

	GetPrivateProfileString(_T("Position"), _T("X"), NULL, m_szData01, 255, _T("../Bin/Data/Gimmick/TownTrain/Inline/GoalTape.ini"));
	vPos.x = (_float)_tstof(m_szData01);
	GetPrivateProfileString(_T("Position"), _T("Y"), NULL, m_szData01, 255, _T("../Bin/Data/Gimmick/TownTrain/Inline/GoalTape.ini"));
	vPos.y = (_float)_tstof(m_szData01);
	GetPrivateProfileString(_T("Position"), _T("Z"), NULL, m_szData01, 255, _T("../Bin/Data/Gimmick/TownTrain/Inline/GoalTape.ini"));
	vPos.z = (_float)_tstof(m_szData01);

	GetPrivateProfileString(_T("Scale"), _T("X"), NULL, m_szData01, 255, _T("../Bin/Data/Gimmick/TownTrain/Inline/GoalTape.ini"));
	vScale.x = (_float)_tstof(m_szData01);
	GetPrivateProfileString(_T("Scale"), _T("Y"), NULL, m_szData01, 255, _T("../Bin/Data/Gimmick/TownTrain/Inline/GoalTape.ini"));
	vScale.y = (_float)_tstof(m_szData01);
	GetPrivateProfileString(_T("Scale"), _T("Z"), NULL, m_szData01, 255, _T("../Bin/Data/Gimmick/TownTrain/Inline/GoalTape.ini"));
	vScale.z = (_float)_tstof(m_szData01);

	GetPrivateProfileString(_T("Rotation"), _T("X"), NULL, m_szData01, 255, _T("../Bin/Data/Gimmick/TownTrain/Inline/GoalTape.ini"));
	vAngle.x = (_float)_tstof(m_szData01);
	GetPrivateProfileString(_T("Rotation"), _T("Y"), NULL, m_szData01, 255, _T("../Bin/Data/Gimmick/TownTrain/Inline/GoalTape.ini"));
	vAngle.y = (_float)_tstof(m_szData01);
	GetPrivateProfileString(_T("Rotation"), _T("Z"), NULL, m_szData01, 255, _T("../Bin/Data/Gimmick/TownTrain/Inline/GoalTape.ini"));
	vAngle.z = (_float)_tstof(m_szData01);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	m_pTransformCom->SetUp_Scale(vScale);
	m_pTransformCom->SetUp_Rotation(CTransform::STATE_RIGHT, vAngle.x);
	m_pTransformCom->SetUp_Rotation(CTransform::STATE_UP, vAngle.y);
	m_pTransformCom->SetUp_Rotation(CTransform::STATE_LOOK, vAngle.z);

	m_pMeshCom->SetUp_Animation(2);

	//Change_CurrVec();

	return NOERROR;
}

_int CGoalTape::Update_GameObject(_double TimeDelta)
{
	m_TimeDelta = TimeDelta;

	if (m_bEnd)
		return 0;

	if (m_bPass)
	{
		if (!m_bSound)
		{
			CSoundMgr::GetInstance()->StopAll();
			CSoundMgr::GetInstance()->PlaySound(L"Clear.wav", CSoundMgr::BGM);
			m_bSound = true;
		}
		if (m_pMeshCom->Finish_Animation(0.2f))
		{
			m_pMeshCom->SetUp_Animation(0);
			m_bEnd = true;
		}
	}

	m_pColliderCom->Update_Collider();

	return _int();
}

_int CGoalTape::LateUpdate_GameObject(_double TimeDelta)
{
	if (m_pRendererCom == nullptr)
		return -1;

	if (!m_bPass)
	{
		CManagement* pManagement = CManagement::GetInstance();
		if (nullptr == pManagement)
			return -1;

		Safe_AddRef(pManagement);

		if (m_pColliderCom->Collision_Sphere((CCollider*)pManagement->Get_Component(SCENE_TRAIN, L"Layer_Object", L"Com_Collider")))
		{
			m_bPass = true;
			m_pMeshCom->SetUp_Animation(1);

			CYoshi_Stage* pPlayer = (CYoshi_Stage*)pManagement->Get_ObjectFromLayer(SCENE_TRAIN, L"Layer_Object");
			pPlayer->GetFSM()->ChangeState(CStage_GoToGoalPoint::Instance());
		}

		Safe_Release(pManagement);
	}

	if (true == m_pFrustumCom->Culling_OnWorldSpace(&m_pTransformCom->Get_State(CTransform::STATE_POSITION), 8.f))
		m_pRendererCom->Add_RenderList(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

HRESULT CGoalTape::Render_GameObject()
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

HRESULT CGoalTape::Add_Component()
{
	// For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, &CTransform::STATE(1.f, D3DXToRadian(90)))))
		return E_FAIL;

	// For.Com_Renderer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// For.Com_Mesh
	if (FAILED(CGameObject::Add_Component(SCENE_TRAIN, L"Component_Mesh_GoalTape", L"Com_Mesh", (CComponent**)&m_pMeshCom)))
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
	ColliderDesc.vPosition = _vec3(0.f, 0.f, 0.f);
	ColliderDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrixPointer();
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Collider_Sphere", L"Com_Collider", (CComponent**)&m_pColliderCom, &ColliderDesc)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CGoalTape::SetUp_ConstantTable()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	_matrix			matView;

	m_pShaderCom->Set_Value("g_matWorld", &m_pTransformCom->Get_WorldMatrix(), sizeof(_matrix));
	m_pShaderCom->Set_Value("g_matView", matView = Get_Transform(D3DTS_VIEW), sizeof(_matrix));
	m_pShaderCom->Set_Value("g_matProj", Get_Transform(D3DTS_PROJECTION), sizeof(_matrix));

	return NOERROR;
}

CGameObject * CGoalTape::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CGameObject*		pInstance = new CGoalTape(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		MSG_BOX("Failed To Create CGameObject Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CGoalTape::Clone_GameObject(void * pArg)
{
	CGameObject*		pInstance = new CGoalTape(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		MSG_BOX("Failed To Create CGameObject Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGoalTape::Free()
{
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pFrustumCom);
	Safe_Release(m_pMeshCom);
	Safe_Release(m_pColliderCom);

	CGameObject::Free();
}
