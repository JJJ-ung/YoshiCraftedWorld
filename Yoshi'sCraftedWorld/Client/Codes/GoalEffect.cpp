#include "stdafx.h"
#include "..\Headers\GoalEffect.h"
#include "Management.h"
#include "Light_Manager.h"
#include "Mesh_Dynamic.h"
#include "Yoshi_Stage.h"
#include "Yoshi_Stage_State.h"
#include "Road.h"

USING(Client)

CGoalEffect::CGoalEffect(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CGoalEffect::CGoalEffect(const CGoalEffect & rhs)
	:CGameObject(rhs)
{
}

HRESULT CGoalEffect::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CGoalEffect::Ready_GameObject(void * pArg)
{
	//m_iIndex = *(_int*)pArg;
	memcpy(&m_iIndex, pArg, sizeof(_int));

	if (FAILED(Add_Component()))
		return E_FAIL;

	//m_pTransformCom->SetUp_Rotation(_vec3(0.f, 1.f, 0.f), 180.f);
	m_pMeshCom->SetUp_Animation(1);


	return NOERROR;
}

_int CGoalEffect::Update_GameObject(_double TimeDelta)
{
	m_TimeDelta = TimeDelta;

	//if (!m_bAnimation)
	//{
	//	if (m_pMeshCom->Finish_Animation(0.1))
	//	{
	//		m_pMeshCom->SetUp_Animation(1);
	//		m_bAnimation = true;
	//	}
	//}
	_vec3 vPos, vScale;	_tchar m_szData01[255];

	if (m_iIndex == 0)
	{
		GetPrivateProfileString(_T("Position"), _T("X1"), NULL, m_szData01, 255, _T("../Bin/Data/Information/Magic.ini"));
		vPos.x = (_float)_tstof(m_szData01);
	}
	else
	{
		GetPrivateProfileString(_T("Position"), _T("X2"), NULL, m_szData01, 255, _T("../Bin/Data/Information/Magic.ini"));
		vPos.x = (_float)_tstof(m_szData01);
	}

	GetPrivateProfileString(_T("Position"), _T("Y"), NULL, m_szData01, 255, _T("../Bin/Data/Information/Magic.ini"));
	vPos.y = (_float)_tstof(m_szData01);

	GetPrivateProfileString(_T("Position"), _T("Z"), NULL, m_szData01, 255, _T("../Bin/Data/Information/Magic.ini"));
	vPos.z = (_float)_tstof(m_szData01);

	GetPrivateProfileString(_T("Scale"), _T("X"), NULL, m_szData01, 255, _T("../Bin/Data/Information/Magic.ini"));
	vScale.x = (_float)_tstof(m_szData01);

	GetPrivateProfileString(_T("Scale"), _T("Y"), NULL, m_szData01, 255, _T("../Bin/Data/Information/Magic.ini"));
	vScale.y = (_float)_tstof(m_szData01);

	GetPrivateProfileString(_T("Scale"), _T("Z"), NULL, m_szData01, 255, _T("../Bin/Data/Information/Magic.ini"));
	vScale.z = (_float)_tstof(m_szData01);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	m_pTransformCom->SetUp_Rotation(_vec3(0.f, 1.f, 0.f), D3DXToRadian(180.f));
	m_pTransformCom->SetUp_Scale(vScale);

	return _int();
}

_int CGoalEffect::LateUpdate_GameObject(_double TimeDelta)
{
	if (m_pRendererCom == nullptr)
		return -1;

	m_pRendererCom->Add_RenderList(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

HRESULT CGoalEffect::Render_GameObject()
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

HRESULT CGoalEffect::Add_Component()
{
	// For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, &CTransform::STATE(1.f, D3DXToRadian(90)))))
		return E_FAIL;

	// For.Com_Renderer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// For.Com_Mesh
	if (m_iIndex == 0)
	{
		if (FAILED(CGameObject::Add_Component(SCENE_TRAIN, L"Component_Mesh_GoalEffect0", L"Com_Mesh", (CComponent**)&m_pMeshCom)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(CGameObject::Add_Component(SCENE_TRAIN, L"Component_Mesh_GoalEffect1", L"Com_Mesh", (CComponent**)&m_pMeshCom)))
			return E_FAIL;
	}

	// For.Com_Shader
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Shader_Mesh", L"Com_Shader", (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CGoalEffect::SetUp_ConstantTable()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	_matrix			matView;

	m_pShaderCom->Set_Value("g_matWorld", &m_pTransformCom->Get_WorldMatrix(), sizeof(_matrix));
	m_pShaderCom->Set_Value("g_matView", matView = Get_Transform(D3DTS_VIEW), sizeof(_matrix));
	m_pShaderCom->Set_Value("g_matProj", Get_Transform(D3DTS_PROJECTION), sizeof(_matrix));

	return NOERROR;
}

CGameObject * CGoalEffect::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CGameObject*		pInstance = new CGoalEffect(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		MSG_BOX("Failed To Create CGameObject Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CGoalEffect::Clone_GameObject(void * pArg)
{
	CGameObject*		pInstance = new CGoalEffect(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		MSG_BOX("Failed To Create CGameObject Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGoalEffect::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pMeshCom);
	Safe_Release(m_pRendererCom);

	CGameObject::Free();
}
