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

class CEffect_Hit : public CGameObject
{
public:
	explicit CEffect_Hit(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CEffect_Hit(const CEffect_Hit& rhs);
	virtual ~CEffect_Hit() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int Update_GameObject(_double TimeDelta);
	virtual _int LateUpdate_GameObject(_double TimeDelta);
	virtual HRESULT Render_GameObject();

private:
	CRenderer*				m_pRendererCom = nullptr;
	CBuffer_RcTex*		m_pBufferCom = nullptr;
	CShader*					m_pShaderCom = nullptr;
	CTexture*					m_pTextureCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;

private:
	HRESULT Add_Component();
	HRESULT SetUp_ConstantTable();

public:
	static CEffect_Hit* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject(void* pArg);
	virtual void Free();
};

