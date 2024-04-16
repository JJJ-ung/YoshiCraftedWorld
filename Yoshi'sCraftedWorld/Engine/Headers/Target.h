#pragma once

// 그리기를 할 수 있는 대상. (일종의 스케치북)
// 텍스쳐를 먼저 생성한 후, 해당 텍스쳐로부터 렌더타겟에 해당하는 객체를 얻어온다.

#include "Base.h"

BEGIN(Engine)

class CTarget final : public CBase
{
private:
	explicit CTarget(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CTarget() = default;
public:
	HRESULT Ready_Target(_uint iSizeX, _uint iSizeY, D3DFORMAT eFormat, D3DXCOLOR Color);
	HRESULT SetUp_OnShader(LPD3DXEFFECT pEffect, D3DXHANDLE hHandle);
	HRESULT SetUp_OnGraphicDev(_uint iIndex);
	HRESULT Release_OnGraphicDev(_uint iIndex);
	HRESULT Clear_Target();
public:
	HRESULT Ready_Debug_Buffer(_float fX, _float fY, _float fSizeX, _float fSizeY);
	HRESULT Render_Debug_Buffer();
private:
	LPDIRECT3DDEVICE9			m_pGraphic_Device = nullptr;
	LPDIRECT3DTEXTURE9			m_pTexture = nullptr; // 텍스쳐
	LPDIRECT3DSURFACE9			m_pSurface = nullptr; // 렌더타겟.
	LPDIRECT3DSURFACE9			m_pOldSurface = nullptr; // 전 렌더타겟.
private:
	D3DXCOLOR					m_ClearColor;
private:
	LPDIRECT3DVERTEXBUFFER9		m_pVB = nullptr;
	LPDIRECT3DINDEXBUFFER9		m_pIB = nullptr;
public:
	static CTarget* Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iSizeX, _uint iSizeY, D3DFORMAT eFormat, D3DXCOLOR Color);
	virtual void Free();
};

END