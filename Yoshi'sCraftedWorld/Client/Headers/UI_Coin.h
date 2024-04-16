#pragma once

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CTransform;
class CBuffer_RcTex;
END

BEGIN(Client)
class CUI_Coin final : public CGameObject
{
public:
	enum UI_STATE { UI_IDLE, UI_IN, UI_SHOW, UI_OUT };
	enum COIN_TYPE { COIN_NORMAL, COIN_LARGE };

private:
	explicit CUI_Coin(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CUI_Coin(const CUI_Coin& rhs);
	virtual ~CUI_Coin() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int Update_GameObject(_double TimeDelta);
	virtual _int LateUpdate_GameObject(_double TimeDelta);
	virtual HRESULT Render_GameObject();

public:
	HRESULT Add_Coin(COIN_TYPE eType);

private:
	_int Update_Idle(_double TImeDelta);
	_int Update_In(_double TImeDelta);
	_int Update_Show(_double TImeDelta);
	_int Update_Out(_double TImeDelta);

private:
	CRenderer*		m_pRendererCom = nullptr;
	CBuffer_RcTex*	m_pBufferCom = nullptr;
	CShader*		m_pShaderCom = nullptr;
	CTexture*		m_pTextureCom = nullptr;
	CTransform*		m_pTransformCom = nullptr;

private:
	UI_STATE	m_eCurrState = UI_IDLE;
	_float			m_fX, m_fY, m_fSizeX, m_fSizeY;
	_uint			m_iCoinCnt = 0;
	wstring		m_strCoinCnt = L"";
	_float			m_fSpeed = 300.f;

private:
	system_clock::time_point m_tEnterTime;

private:
	HRESULT Add_Component();
	HRESULT SetUp_ConstantTable();

public:
	static CUI_Coin* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject(void* pArg);
	virtual void Free();
};
END