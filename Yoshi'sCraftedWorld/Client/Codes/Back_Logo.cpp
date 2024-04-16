#include "stdafx.h"
#include "..\Headers\Back_Logo.h"
#include "Management.h"

USING(Client)

CBack_Logo::CBack_Logo(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CBack_Logo::CBack_Logo(const CBack_Logo & rhs)
	: CGameObject(rhs)
{

}

HRESULT CBack_Logo::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CBack_Logo::Ready_GameObject(void* pArg)
{
	LOGO_INFO* p = (LOGO_INFO*)pArg;

	m_tLogoInfo.eSceneID = p->eSceneID;
	m_tLogoInfo.strLogoKey = p->strLogoKey;
	m_tLogoInfo.vScale = p->vScale;
	m_tLogoInfo.vPos = p->vPos;
	m_tLogoInfo.fFrameCnt = p->fFrameCnt;
	m_tLogoInfo.fSpeed = p->fSpeed;

	if (FAILED(Add_Component()))
		return E_FAIL;

	return NOERROR;
}

_int CBack_Logo::Update_GameObject(_double TimeDelta)
{
	m_dCurrentFrame += TimeDelta * m_tLogoInfo.fSpeed;
	if (m_dCurrentFrame > (double)m_tLogoInfo.fFrameCnt)
	{
		m_dCurrentFrame = 0.f;
		m_tLogoInfo.bAnimationEnd = true;
	}

	return _int();
}

_int CBack_Logo::LateUpdate_GameObject(_double TimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	m_pRendererCom->Add_RenderList(CRenderer::RENDER_UI, this);

	return _int();
}

HRESULT CBack_Logo::Render_GameObject()
{
	if (nullptr == m_pBufferCom ||
		nullptr == m_pShaderCom ||
		nullptr == m_pTextureCom)
		return E_FAIL;


	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	if (nullptr == pEffect)
		return E_FAIL;

	Safe_AddRef(pEffect);

	_matrix		matTmp, matScale, matTrans;

	D3DXMatrixScaling(&matScale, m_tLogoInfo.vScale.x, m_tLogoInfo.vScale.y, m_tLogoInfo.vScale.z);
	D3DXMatrixTranslation(&matTrans, m_tLogoInfo.vPos.x, m_tLogoInfo.vPos.y, m_tLogoInfo.vPos.z);

	matTmp = matScale * matTrans;

	pEffect->SetMatrix("g_matWorld", &matTmp);
	pEffect->SetMatrix("g_matView", D3DXMatrixIdentity(&matTmp));
	pEffect->SetMatrix("g_matProj", D3DXMatrixIdentity(&matTmp));

	if (FAILED(m_pTextureCom->SetUp_OnShader(pEffect, "g_BaseTexture", (_uint)m_dCurrentFrame)))
		return E_FAIL;

	pEffect->Begin(nullptr, 0);

	pEffect->BeginPass(0);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);

	return NOERROR;
}

HRESULT CBack_Logo::Add_Component()
{
	// For.Com_Renderer	
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// For.Com_Buffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Buffer_RcTex", L"Com_Buffer", (CComponent**)&m_pBufferCom)))
		return E_FAIL;

	// For.Com_Shader
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Shader_Default", L"Com_Shader", (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	// For.Com_Texture
	if (FAILED(CGameObject::Add_Component(m_tLogoInfo.eSceneID, m_tLogoInfo.strLogoKey.c_str(), L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return NOERROR;
}

CGameObject * CBack_Logo::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CGameObject*		pInstance = new CBack_Logo(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		MSG_BOX("Failed To Create CGameObject Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBack_Logo::Clone_GameObject(void* pArg)
{
	CGameObject*		pInstance = new CBack_Logo(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		MSG_BOX("Failed To Create CGameObject Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBack_Logo::Free()
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pBufferCom);
	Safe_Release(m_pRendererCom);

	CGameObject::Free();
}
