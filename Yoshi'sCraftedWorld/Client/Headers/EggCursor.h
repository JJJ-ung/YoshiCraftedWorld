#pragma once

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CTransform;
class CInput_Device;
class CBuffer_RcTex;
END

BEGIN(Client)
class CEggCursor final : public CGameObject
{
public:
	explicit CEggCursor(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CEggCursor(const CEggCursor& rhs);
	virtual ~CEggCursor() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int Update_GameObject(_double TimeDelta);
	virtual _int LateUpdate_GameObject(_double TimeDelta);
	virtual HRESULT Render_GameObject();

public:
	void Set_Focus(_bool bFocus, _vec3 vPos);
	_bool& Get_IsFocus() { return m_bFocus; }

private:
	CRenderer*		m_pRendererCom = nullptr;
	CBuffer_RcTex*	m_pBufferCom = nullptr;
	CShader*		m_pShaderCom = nullptr;
	CTexture*		m_pTextureCom = nullptr;
	CTransform*		m_pTransformCom = nullptr;

private:
	_float			m_fX, m_fY, m_fSizeX, m_fSizeY;
	_bool			m_bEggCursor = false;
	_bool			m_bFocus = false;
	_float			m_fFrame = 0.f;

private:
	LPD3DXLINE	m_pLine = nullptr;
	_vec3								m_vTest[2];

private:
	HRESULT Add_Component();
	HRESULT SetUp_ConstantTable();

public:
	static CEggCursor* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject(void* pArg);
	virtual void Free();
};
END