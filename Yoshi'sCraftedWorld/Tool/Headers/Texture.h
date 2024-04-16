#pragma once

#include "Component.h"

class CTexture final : public CComponent
{
public:
	enum TEXTURETYPE {TYPE_GENERAL, TYPE_CUBE};

public:
	explicit CTexture(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTexture(void) = default;

public:
	virtual HRESULT Ready_Component(const _tchar* pPath, TEXTURETYPE eType, const _uint& iCnt);

public:
	HRESULT SetUp_OnShader(LPD3DXEFFECT pEffect, const char* pConstantName, _uint iIndex = 0);
	
private:	
	vector<IDirect3DBaseTexture9*>				m_vecTexture;
	typedef vector<IDirect3DBaseTexture9*>	VECTEXTURE;

public:
	static CTexture* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pPath,  TEXTURETYPE eType, const _uint& iCnt = 1);
	virtual void Free(void);
};