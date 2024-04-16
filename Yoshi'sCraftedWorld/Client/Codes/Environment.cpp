#include "stdafx.h"
#include "..\Headers\Environment.h"

#include "Management.h"
#include "Light_Manager.h"
#include "Frustum.h"

USING(Client)


CEnvironment::CEnvironment(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CEnvironment::CEnvironment(const CEnvironment & rhs)
	:CGameObject(rhs)
{
}

HRESULT CEnvironment::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CEnvironment::Ready_GameObject(void * pArg)
{
	ENVIRONMENT_INFO* p = (ENVIRONMENT_INFO*)pArg;

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

_int CEnvironment::Update_GameObject(_double TimeDelta)
{
	return _int();
}

_int CEnvironment::LateUpdate_GameObject(_double TimeDelta)
{
	if (m_pRendererCom == nullptr)
		return -1;

	if (m_tEnvironmentInfo.bCull)
	{
		if (true == m_pFrustumCom->Culling_OnWorldSpace(&m_pTransformCom->Get_State(CTransform::STATE_POSITION), 1.f))
			m_pRendererCom->Add_RenderList(CRenderer::RENDER_NONALPHA, this);
	}
	else
	{
		if (true == m_pFrustumCom->Culling_OnWorldSpace(&m_pTransformCom->Get_State(CTransform::STATE_POSITION), 80.f))
			m_pRendererCom->Add_RenderList(CRenderer::RENDER_NONALPHA, this);
	}

	return _int();
}

HRESULT CEnvironment::Render_GameObject()
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

	return NOERROR;
}

HRESULT CEnvironment::Add_Component()
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

	// For.Com_Frustum
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Frustum", L"Com_Frustum", (CComponent**)&m_pFrustumCom)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CEnvironment::SetUp_ConstantTable()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	_matrix			matView;

	m_pShaderCom->Set_Value("g_matWorld", &m_pTransformCom->Get_WorldMatrix(), sizeof(_matrix));
	m_pShaderCom->Set_Value("g_matView", matView = Get_Transform(D3DTS_VIEW), sizeof(_matrix));
	m_pShaderCom->Set_Value("g_matProj", Get_Transform(D3DTS_PROJECTION), sizeof(_matrix));

	return NOERROR;
}

CGameObject * CEnvironment::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CGameObject*		pInstance = new CEnvironment(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		MSG_BOX("Failed To Create CGameObject Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CEnvironment::Clone_GameObject(void * pArg)
{
	CGameObject*		pInstance = new CEnvironment(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		MSG_BOX("Failed To Create CGameObject Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEnvironment::Free()
{
	Safe_Release(m_pFrustumCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pMeshCom);
	Safe_Release(m_pRendererCom);

	CGameObject::Free();
}
