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
class CUI_SingleFlower;
class CUI_Flower final : public CGameObject
{
private:
	explicit CUI_Flower(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CUI_Flower(const CUI_Flower& rhs);
	virtual ~CUI_Flower() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int Update_GameObject(_double TimeDelta);
	virtual _int LateUpdate_GameObject(_double TimeDelta);
	virtual HRESULT Render_GameObject();

public:
	HRESULT Add_Flower(_uint iIndex);

private:
	CRenderer*		m_pRendererCom = nullptr;
	CBuffer_RcTex*	m_pBufferCom = nullptr;
	CShader*		m_pShaderCom = nullptr;
	CTexture*		m_pTextureCom = nullptr;
	CTransform*		m_pTransformCom = nullptr;

private:
	_float			m_fX, m_fY, m_fSizeX, m_fSizeY;
	list<CUI_SingleFlower*>  m_FlowerList;

private:
	_bool		m_bShow = false;
	system_clock::time_point m_tEnterTime;

private:
	HRESULT Add_Component();
	HRESULT SetUp_ConstantTable();

public:
	static CUI_Flower* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject(void* pArg);
	virtual void Free();
};
END