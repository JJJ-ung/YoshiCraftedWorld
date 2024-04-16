#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTexture final : public CComponent
{
public:
	enum TEXTURETYPE {TYPE_GENERAL, TYPE_CUBE};

protected:
	explicit CTexture(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTexture(const CTexture& rhs);
	virtual ~CTexture(void) = default;

public:
	virtual HRESULT Ready_Component_Prototype(const _tchar* pPath, TEXTURETYPE eType, const _uint& iCnt);		
	virtual HRESULT Ready_Component(void* pArg);	

public:
	HRESULT SetUp_OnShader(LPD3DXEFFECT pEffect, const char* pConstantName, _uint iIndex = 0);
	
private:	
	vector<IDirect3DBaseTexture9*>			m_vecTexture;
	typedef vector<IDirect3DBaseTexture9*>	VECTEXTURE;

public:
	virtual CComponent*	Clone_Component(void* pArg);
	static CTexture* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pPath,  TEXTURETYPE eType, const _uint& iCnt = 1);
	virtual void Free(void);
};

END
