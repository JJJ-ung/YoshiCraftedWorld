#pragma once

#include "Defines.h"
#include "Base.h"
#include "Graphic_Device.h"
#include "Input_Device.h"

BEGIN(Engine)
class CRenderer;
class CManagement;
END

BEGIN(Client)

class CMouse_Manager;
class CMainApp final : public CBase
{
private:
	explicit CMainApp();
	virtual ~CMainApp() = default;

public:
	HRESULT Ready_MainApp();
	_int Update_MainApp(_double TimeDelta);
	HRESULT Render_MainApp();

public:
	HRESULT Ready_DefaultSetting(CGraphic_Device::WINMODE eMode, const _uint& iSizeX, const _uint& iSizeY);
	HRESULT Ready_Start_Scene(SCENEID eSceneID);
	HRESULT Ready_Component_Prototype();

private:
	LPDIRECT3DDEVICE9 m_pGraphic_Device = nullptr;

private:
	CManagement* m_pManagement = nullptr;
	CInput_Device*	m_pInput_Instance = nullptr;
	CRenderer*			m_pRenderer = nullptr;
	CMouse_Manager* m_pMouse_Manager = nullptr;

private:
	_tchar					m_szFPS[MAX_PATH] = L"";
	_uint						m_iRenderCall = 0;
	_double					m_TimerAcc = 0.0;

public:
	static CMainApp* Create();
	virtual void Free();
};

END