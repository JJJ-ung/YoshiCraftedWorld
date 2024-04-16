#include "stdafx.h"
#include "..\Headers\RollTape.h"
#include "Management.h"
#include "Light_Manager.h"
#include "Mesh_Dynamic.h"
#include "Yoshi_Stage.h"
#include "Yoshi_Stage_State.h"
#include "Road.h"

USING(Client)

CRollTape::CRollTape(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CRollTape::CRollTape(const CRollTape & rhs)
	:CGameObject(rhs)
{
}

HRESULT CRollTape::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CRollTape::Ready_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	_tchar m_szData01[255];
	_vec3 vPos, vScale, vAngle;

	GetPrivateProfileString(_T("Position"), _T("X"), NULL, m_szData01, 255, _T("../Bin/Data/Gimmick/TownTrain/Inline/RollTape.ini"));
	vPos.x = (_float) _tstof(m_szData01);
	GetPrivateProfileString(_T("Position"), _T("Y"), NULL, m_szData01, 255, _T("../Bin/Data/Gimmick/TownTrain/Inline/RollTape.ini"));
	vPos.y = (_float)_tstof(m_szData01);
	GetPrivateProfileString(_T("Position"), _T("Z"), NULL, m_szData01, 255, _T("../Bin/Data/Gimmick/TownTrain/Inline/RollTape.ini"));
	vPos.z = (_float)_tstof(m_szData01);

	GetPrivateProfileString(_T("Scale"), _T("X"), NULL, m_szData01, 255, _T("../Bin/Data/Gimmick/TownTrain/Inline/RollTape.ini"));
	vScale.x = (_float)_tstof(m_szData01);
	GetPrivateProfileString(_T("Scale"), _T("Y"), NULL, m_szData01, 255, _T("../Bin/Data/Gimmick/TownTrain/Inline/RollTape.ini"));
	vScale.y = (_float)_tstof(m_szData01);
	GetPrivateProfileString(_T("Scale"), _T("Z"), NULL, m_szData01, 255, _T("../Bin/Data/Gimmick/TownTrain/Inline/RollTape.ini"));
	vScale.z = (_float)_tstof(m_szData01);

	GetPrivateProfileString(_T("Rotation"), _T("X"), NULL, m_szData01, 255, _T("../Bin/Data/Gimmick/TownTrain/Inline/RollTape.ini"));
	vAngle.x = (_float)_tstof(m_szData01);
	GetPrivateProfileString(_T("Rotation"), _T("Y"), NULL, m_szData01, 255, _T("../Bin/Data/Gimmick/TownTrain/Inline/RollTape.ini"));
	vAngle.y = (_float)_tstof(m_szData01);
	GetPrivateProfileString(_T("Rotation"), _T("Z"), NULL, m_szData01, 255, _T("../Bin/Data/Gimmick/TownTrain/Inline/RollTape.ini"));
	vAngle.z = (_float)_tstof(m_szData01);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	m_pTransformCom->SetUp_Scale(vScale);
	m_pTransformCom->SetUp_Rotation(CTransform::STATE_RIGHT, vAngle.x);
	m_pTransformCom->SetUp_Rotation(CTransform::STATE_UP, vAngle.y);
	m_pTransformCom->SetUp_Rotation(CTransform::STATE_LOOK, vAngle.z);

	m_pMeshCom->SetUp_Animation(2);

	return NOERROR;
}

_int CRollTape::Update_GameObject(_double TimeDelta)
{
	m_TimeDelta = TimeDelta;

	CManagement* pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return -1;

	Safe_AddRef(pManagement);

	if (!m_bAnimationStart)
	{
		CYoshi_Stage* pPlayer = (CYoshi_Stage*)pManagement->Get_ObjectFromLayer(SCENE_TRAIN, L"Layer_Object");
		if (pPlayer->GetFSM()->GetCurrentState() == CStage_GameStart::Instance())
		{
			m_pMeshCom->SetUp_Animation(1);
			m_bAnimationStart = true;
		}

	}

	if (m_bAnimationStart)
	{
		if (m_pMeshCom->Finish_Animation(0.1f))
		{
			if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, L"Layer_BackGround", L"GameObject_Road", (void*)&CRoad::ROAD_INFO(L"PaperRoadAb.X", SCENE_TRAIN, _vec3(27.4f, 0.1f, 23.7f), _vec3(0.005f, 0.005f, 0.005f), _vec3(0.f, 0.f, 0.f)))))
				return E_FAIL;

			Safe_Release(pManagement);

			return OBJDEAD;
		}
	}

	Safe_Release(pManagement);

	return _int();
}

_int CRollTape::LateUpdate_GameObject(_double TimeDelta)
{
	if (m_pRendererCom == nullptr)
		return -1;

	m_pRendererCom->Add_RenderList(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

HRESULT CRollTape::Render_GameObject()
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
	pEffect->BeginPass(1);

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

HRESULT CRollTape::Add_Component()
{
	// For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, &CTransform::STATE(1.f, D3DXToRadian(90)))))
		return E_FAIL;

	// For.Com_Renderer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// For.Com_Mesh
	if (FAILED(CGameObject::Add_Component(SCENE_TRAIN, L"Component_Mesh_RollTape", L"Com_Mesh", (CComponent**)&m_pMeshCom)))
		return E_FAIL;

	// For.Com_Shader
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Shader_Mesh", L"Com_Shader", (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CRollTape::SetUp_ConstantTable()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	_matrix			matView;

	m_pShaderCom->Set_Value("g_matWorld", &m_pTransformCom->Get_WorldMatrix(), sizeof(_matrix));
	m_pShaderCom->Set_Value("g_matView", matView = Get_Transform(D3DTS_VIEW), sizeof(_matrix));
	m_pShaderCom->Set_Value("g_matProj", Get_Transform(D3DTS_PROJECTION), sizeof(_matrix));

	return NOERROR;
}

CGameObject * CRollTape::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CGameObject*		pInstance = new CRollTape(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		MSG_BOX("Failed To Create CGameObject Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CRollTape::Clone_GameObject(void * pArg)
{
	CGameObject*		pInstance = new CRollTape(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		MSG_BOX("Failed To Create CGameObject Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRollTape::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pMeshCom);
	Safe_Release(m_pRendererCom);

	CGameObject::Free();
}
