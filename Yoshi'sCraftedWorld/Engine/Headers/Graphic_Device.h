#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CGraphic_Device : public CBase
{
	DECLARE_SINGLETON(CGraphic_Device)

public:
	enum WINMODE {MODE_FULL, MODE_WIN};

private:
	explicit	CGraphic_Device(void);
	virtual		~CGraphic_Device(void) = default;

public:
	LPDIRECT3DDEVICE9		Get_GraphicDev(void) { return m_pGraphicDev;  }

public:
	HRESULT		Ready_GraphicDev(HWND hWnd, 
								WINMODE eMode, 
								const _uint& iSizeX,
								const _uint& iSizeY,
								LPDIRECT3DDEVICE9* ppGraphicDev);

	void		Render_Begin(D3DXCOLOR Color);
	void		Render_End(void);

private:
	LPDIRECT3D9				m_pSDK = nullptr;
	LPDIRECT3DDEVICE9		m_pGraphicDev = nullptr;
public:
	virtual void Free(void);
};

END