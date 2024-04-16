#pragma once

// �׸��⸦ �� �� �ִ� ���. (������ ����ġ��)
// �ؽ��ĸ� ���� ������ ��, �ش� �ؽ��ķκ��� ����Ÿ�ٿ� �ش��ϴ� ��ü�� ���´�.

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
	LPDIRECT3DTEXTURE9			m_pTexture = nullptr; // �ؽ���
	LPDIRECT3DSURFACE9			m_pSurface = nullptr; // ����Ÿ��.
	LPDIRECT3DSURFACE9			m_pOldSurface = nullptr; // �� ����Ÿ��.
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