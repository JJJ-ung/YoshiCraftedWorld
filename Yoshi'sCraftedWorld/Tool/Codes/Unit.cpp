#include "stdafx.h"
#include "..\Headers\Unit.h"
#include "Shader.h"
#include "MainFrm.h"
#include "ToolMenu.h"
#include "DynamicMesh.h"

CUnit::CUnit(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

HRESULT CUnit::Ready_GameObject(const _tchar * pFilePath, const _tchar* pFileName, _vec3 vPos)
{
	if (FAILED(Add_Component(pFilePath, pFileName)))
		return E_FAIL;

	m_strFilePath = pFilePath;
	m_strFileName = pFileName;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	m_iShaderPass = 1;

	CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CToolMenu* pToolMenu = dynamic_cast<CToolMenu*>(pFrameWnd->m_WndSplit.GetPane(0, 0));
	//m_pEffectMenu = pToolMenu->m_pEffect;

	m_pMeshCom->SetUp_Animation(0);

	return NOERROR;
}

_int CUnit::Update_GameObject(_double TimeDelta)
{
	m_pMeshCom->Play_Animation(TimeDelta);

	return _int();
}

_int CUnit::LateUpdate_GameObject(_double TimeDelta)
{
	return _int();
}

HRESULT CUnit::Render_GameObject()
{
	if (nullptr == m_pMeshCom ||
		nullptr == m_pShaderCom)
		return E_FAIL;

	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if (nullptr == pEffect)
		return E_FAIL;

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

	return NOERROR;
}

HRESULT CUnit::Add_Component(const _tchar * pFilePath, const _tchar* pFileName)
{
	// For.Com_Transform
	m_pTransformCom = CTransform::Create(m_pGraphic_Device, CTransform::STATE(1.f, D3DXToRadian(90)));
	if (FAILED(CGameObject::Add_Component(L"Com_Transform", m_pTransformCom)))
		return E_FAIL;
	m_pTransformCom->SetUp_Scale(_vec3(0.005f, 0.005f, 0.005f));

	// For.Com_Buffer
	m_pMeshCom = CDynamicMesh::Create(m_pGraphic_Device, pFilePath, pFileName);
	if (FAILED(CGameObject::Add_Component(L"Com_Mesh", m_pMeshCom)))
		return E_FAIL;

	// For.Com_Shader
	m_pShaderCom = CShader::Create(m_pGraphic_Device, L"../Bin/Shader/Shader_Default.fx");
	if (FAILED(CGameObject::Add_Component(L"Com_Shader", m_pShaderCom)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CUnit::SetUp_ConstantTable()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	m_pShaderCom->Set_Value("g_matWorld", m_pTransformCom->Get_WorldMatrix(), sizeof(_matrix));
	m_pShaderCom->Set_Value("g_matView", Get_Transform(D3DTS_VIEW), sizeof(_matrix));
	m_pShaderCom->Set_Value("g_matProj", Get_Transform(D3DTS_PROJECTION), sizeof(_matrix));

	return NOERROR;
}

CUnit * CUnit::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar * pFilePath, const _tchar* pFileName, _vec3 vPos)
{
	CUnit*		pInstance = new CUnit(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(pFilePath, pFileName, vPos)))
	{
		MSG_BOX("Failed To Create CGameObject Instance");
		Safe_Delete(pInstance);
	}

	return pInstance;
}
