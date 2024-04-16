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
class CEndingCredit final : public CGameObject
{
private:
	explicit CEndingCredit(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CEndingCredit(const CEndingCredit& rhs);
	virtual ~CEndingCredit() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int Update_GameObject(_double TimeDelta);
	virtual _int LateUpdate_GameObject(_double TimeDelta);
	virtual HRESULT Render_GameObject();

private:
	CRenderer*		m_pRendererCom = nullptr;
	CBuffer_RcTex*	m_pBufferCom = nullptr;
	CShader*		m_pShaderCom = nullptr;
	CTexture*		m_pTextureCom = nullptr;
	CTransform*		m_pTransformCom = nullptr;

private:
	_float			m_fX, m_fY, m_fSizeX, m_fSizeY;

private:
	HRESULT Add_Component();
	HRESULT SetUp_ConstantTable();

public:
	static CEndingCredit* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject(void* pArg);
	virtual void Free();
};
END