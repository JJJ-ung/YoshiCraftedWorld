#pragma once
#include "Component.h"

class CStaticMesh :	public CComponent
{
public:
	explicit CStaticMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStaticMesh() = default;

public:
	virtual HRESULT Ready_Component(const _tchar* pPath);
	HRESULT Render_Mesh(LPD3DXEFFECT pEffect, const char* pConstant, int iPass);

public:
	LPD3DXMESH											m_pMesh;
	vector<D3DMATERIAL9>					m_vecMtrl;
	vector<LPDIRECT3DTEXTURE9>		m_vecTex;
	_ulong														m_dwMtrlNum;

public:
	static CStaticMesh* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pPath);
	void Free();
};

