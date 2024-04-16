#pragma once

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CBuffer_RcTex;
END

BEGIN(Client)

class CBack_Logo final : public CGameObject
{
public:
	typedef struct tagLogoInfo
	{
		tagLogoInfo() {}
		tagLogoInfo(wstring _pTag, SCENEID _eID,  _vec3 _vScale, _vec3 _vPos, _float _fCnt, _float _fSpeed)
			:strLogoKey(_pTag), eSceneID(_eID), vScale(_vScale), vPos(_vPos), fFrameCnt(_fCnt), fSpeed(_fSpeed) {}
	public:
		wstring				strLogoKey;
		SCENEID			eSceneID;
		_vec3					vScale;
		_vec3					vPos;
		_float					fFrameCnt;
		_float					fSpeed;
		_bool					bAnimationEnd = false;
	}LOGO_INFO;

private:
	explicit CBack_Logo(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CBack_Logo(const CBack_Logo& rhs);
	virtual ~CBack_Logo() = default;

public:
	LOGO_INFO& Get_Info() { return m_tLogoInfo; }
	_double& Get_CurrFrame() { return m_dCurrentFrame; }

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

private:
	_double m_dCurrentFrame = 0.0;
	LOGO_INFO m_tLogoInfo;

private:
	HRESULT Add_Component();

public:
	static CGameObject* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject(void* pArg);
	virtual void Free();
};

END