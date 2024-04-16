#pragma once

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CFrustum;
class CRenderer;
class CTransform;
class CBuffer_RcTex;
END

BEGIN(Client)

class COpen_FlowerUI final : public CGameObject
{
private:
	explicit COpen_FlowerUI(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit COpen_FlowerUI(const COpen_FlowerUI& rhs);
	virtual ~COpen_FlowerUI() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int Update_GameObject(_double TimeDelta);
	virtual _int LateUpdate_GameObject(_double TimeDelta);
	virtual HRESULT Render_GameObject();

	_double& Get_CurrentFrame() { return m_dCurrentFrame; }

private:
	CRenderer*			m_pRendererCom = nullptr;
	CShader*				m_pShaderCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CBuffer_RcTex*	m_pBufferCom = nullptr;
	CFrustum*			m_pFrustumCom = nullptr;

private:
	_double					m_dCurrentFrame = 0.0;

private:
	HRESULT Add_Component();

public:
	static CGameObject* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject(void* pArg);
	virtual void Free();
};

END