#include "stdafx.h"
#include "..\Headers\EndingCredit.h"
#include "Management.h"
#include "FontManager.h"

USING(Client)

CEndingCredit::CEndingCredit(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CEndingCredit::CEndingCredit(const CEndingCredit & rhs)
	: CGameObject(rhs)
{
}

HRESULT CEndingCredit::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CEndingCredit::Ready_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_fX = 960.f;
	m_fY = 1800.f;
	m_fSizeX = 600.f;
	m_fSizeY = 2000.f;

	return NOERROR;
}

_int CEndingCredit::Update_GameObject(_double TimeDelta)
{
	m_fY -= (_float)TimeDelta * 100.f;

	return _int(0);
}

_int CEndingCredit::LateUpdate_GameObject(_double TimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	if (FAILED(m_pRendererCom->Add_RenderList(CRenderer::RENDER_UI, this)))
		return -1;

	return _int();
}

HRESULT CEndingCredit::Render_GameObject()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pTextureCom ||
		nullptr == m_pBufferCom)
		return E_FAIL;

	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if (nullptr == pEffect)
		return E_FAIL;

	Safe_AddRef(pEffect);

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);

	return NOERROR;
}

HRESULT CEndingCredit::Add_Component()
{
	// For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, &CTransform::STATE(5.f, D3DXToRadian(360)))))
		return E_FAIL;

	// For.Com_Renderer	
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// For.Com_Buffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Buffer_RcTex", L"Com_Buffer", (CComponent**)&m_pBufferCom)))
		return E_FAIL;

	// For.Com_Shader
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Shader_UI", L"Com_Shader", (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	// For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_TRAIN, L"Component_EndingCredit", L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CEndingCredit::SetUp_ConstantTable()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pTextureCom)
		return E_FAIL;

	m_pTransformCom->SetUp_Scale(_vec3(m_fSizeX, m_fSizeY, 1.f));

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _vec3(m_fX - (g_nWinCX >> 1), (g_nWinCY >> 1) - m_fY, 0.f));

	m_pShaderCom->Set_Value("g_matWorld", &m_pTransformCom->Get_WorldMatrix(), sizeof(_matrix));

	_matrix			matView;
	m_pShaderCom->Set_Value("g_matView", D3DXMatrixIdentity(&matView), sizeof(_matrix));

	_matrix			matProj;
	D3DXMatrixOrthoLH(&matProj, g_nWinCX, g_nWinCY, 0.f, 1.f);

	m_pShaderCom->Set_Value("g_matProj", &matProj, sizeof(_matrix));

	m_pTextureCom->SetUp_OnShader(m_pShaderCom->Get_EffectHandle(), "g_BaseTexture", 0);

	return NOERROR;
}

CEndingCredit * CEndingCredit::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CEndingCredit*		pInstance = new CEndingCredit(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		MSG_BOX("Failed To Create CGameObject Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CEndingCredit::Clone_GameObject(void * pArg)
{
	CEndingCredit*		pInstance = new CEndingCredit(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		MSG_BOX("Failed To Create CGameObject Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEndingCredit::Free()
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);

	CGameObject::Free();
}
