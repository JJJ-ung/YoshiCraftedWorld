#include "stdafx.h"
#include "..\Headers\UI_Coin.h"
#include "Management.h"
#include "FontManager.h"

USING(Client)

CUI_Coin::CUI_Coin(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CUI_Coin::CUI_Coin(const CUI_Coin & rhs)
	: CGameObject(rhs)
{
}

HRESULT CUI_Coin::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CUI_Coin::Ready_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_fX = -300.f;
	m_fY = 60.f;
	m_fSizeX = 70.f;
	m_fSizeY = 70.f;

	m_iCoinCnt = 0;
	m_strCoinCnt = to_wstring(m_iCoinCnt);

	return NOERROR;
}

_int CUI_Coin::Update_GameObject(_double TimeDelta)
{
	_int iResult = 0;

	switch (m_eCurrState)
	{
	case Client::CUI_Coin::UI_IDLE:
		iResult = Update_Idle(TimeDelta);
		break;
	case Client::CUI_Coin::UI_IN:
		iResult = Update_In(TimeDelta);
		break;
	case Client::CUI_Coin::UI_SHOW:
		iResult = Update_Show(TimeDelta);
		break;
	case Client::CUI_Coin::UI_OUT:
		iResult = Update_Out(TimeDelta);
		break;
	}

	return _int(iResult);
}

_int CUI_Coin::LateUpdate_GameObject(_double TimeDelta)
{
	if (m_eCurrState == CUI_Coin::UI_IDLE)
		return 0;

	if (nullptr == m_pRendererCom)
		return -1;

	if (FAILED(m_pRendererCom->Add_RenderList(CRenderer::RENDER_UI, this)))
		return -1;

	return _int();
}

HRESULT CUI_Coin::Render_GameObject()
{
	if (m_eCurrState == CUI_Coin::UI_IDLE)
		return NOERROR;

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

	CFontManager* pFontManager = CFontManager::GetInstance();
	Safe_AddRef(pFontManager);
	pFontManager->Render_Font(L"Font_CoinUI", m_strCoinCnt.c_str(), &_vec2(m_fX + 50.f, 30.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	Safe_Release(pFontManager);

	return NOERROR;
}

HRESULT CUI_Coin::Add_Coin(COIN_TYPE eType)
{
	if (m_eCurrState == UI_IDLE)
		m_eCurrState = UI_IN;

	m_tEnterTime = system_clock::now();

	if(eType == CUI_Coin::COIN_NORMAL)
		m_iCoinCnt += 1;
	if (eType == CUI_Coin::COIN_LARGE)
		m_iCoinCnt += 10;

	m_strCoinCnt = to_wstring(m_iCoinCnt);

	if (CSoundMgr::GetInstance()->PlaySound(L"PressStart.wav", CSoundMgr::COIN))
	{
		if (CSoundMgr::GetInstance()->PlaySound(L"PressStart.wav", CSoundMgr::COIN2))
			CSoundMgr::GetInstance()->PlaySound(L"PressStart.wav", CSoundMgr::EFFECT);
	}

	return NOERROR;
}

_int CUI_Coin::Update_Idle(_double TImeDelta)
{
	return _int();
}

_int CUI_Coin::Update_In(_double TImeDelta)
{
	m_fX += m_fSpeed * (_float)TImeDelta;
	m_fSpeed += 200.f * (_float)TImeDelta;

	if (m_fX >= 70.f)
	{
		m_fX = 70.f;
		m_eCurrState = CUI_Coin::UI_SHOW;
	}

	return _int();
}

_int CUI_Coin::Update_Show(_double TImeDelta)
{
	system_clock::duration temp = system_clock::now() - m_tEnterTime;
	if (duration_cast<seconds>(temp) > (seconds)2)
		m_eCurrState = CUI_Coin::UI_OUT;

	return _int();
}

_int CUI_Coin::Update_Out(_double TImeDelta)
{
	m_fX -= m_fSpeed *  (_float)TImeDelta;
	m_fSpeed -= 100.f * (_float)TImeDelta;

	if (m_fX <= -300.f)
	{
		m_fX = -300.f;
		m_eCurrState = CUI_Coin::UI_IDLE;
	}

	return _int();
}

HRESULT CUI_Coin::Add_Component()
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
	if (FAILED(CGameObject::Add_Component(SCENE_TRAIN, L"Component_Texture_CoinUI", L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CUI_Coin::SetUp_ConstantTable()
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

CUI_Coin * CUI_Coin::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUI_Coin*		pInstance = new CUI_Coin(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		MSG_BOX("Failed To Create CGameObject Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CUI_Coin::Clone_GameObject(void * pArg)
{
	CUI_Coin*		pInstance = new CUI_Coin(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		MSG_BOX("Failed To Create CGameObject Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Coin::Free()
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);

	CGameObject::Free();
}
