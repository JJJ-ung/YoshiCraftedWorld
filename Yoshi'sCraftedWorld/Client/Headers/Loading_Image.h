#pragma once

#include "Defines.h"
#include "GameObject.h"
#include "Renderer.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CTransform;
class CBuffer_RcTex;
END

BEGIN(Client)
class CLoading_Image : public CGameObject
{
public:
	typedef struct tagLoadImageInfo
	{
		tagLoadImageInfo() {}
		tagLoadImageInfo(wstring _strKey, _uint _iType, bool _bTop)
		: strKey(_strKey), iType(_iType), bTop(_bTop) {}
	public:
		wstring strKey;
		_uint iType;
		_uint iFrame;
		_bool bTop;
		_bool bAnimation = false;
	}LOADING_IMAGE;

public:
	explicit CLoading_Image(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CLoading_Image(const CLoading_Image& rhs);
	virtual ~CLoading_Image() = default;

public:
	_bool GetAnimationEnd() { return m_tInfo.bAnimation; }
	void SetStartAnimation(_bool bStart) { m_bStartAnimation = bStart; }

public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int Update_GameObject(_double TimeDelta);
	virtual _int LateUpdate_GameObject(_double TimeDelta);
	virtual HRESULT Render_GameObject();

private:
	_int Update_LoadIn(_double TimeDelta);
	_int Update_LoadOut_Up(_double TimeDelta);
	_int Update_LoadOut_Down(_double TimeDelta);

private:
	CRenderer*		m_pRendererCom = nullptr;
	CBuffer_RcTex*	m_pBufferCom = nullptr;
	CShader*		m_pShaderCom = nullptr;
	CTexture*		m_pTextureCom = nullptr;
	CTransform*	m_pTransformCom = nullptr;

private:
	LOADING_IMAGE m_tInfo;
	_bool m_bStartAnimation = false;
	_float m_fSpeed = 1.3f;

private:
	HRESULT Add_Component();

public:
	static CGameObject* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject(void* pArg);
	virtual void Free();
};
END