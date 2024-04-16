
// Tool.h : Tool 응용 프로그램에 대한 주 헤더 파일
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'stdafx.h'를 포함합니다."
#endif

#include "resource.h"       // 주 기호입니다.
#include "ToolView.h"

// CToolApp:
// 이 클래스의 구현에 대해서는 Tool.cpp을 참조하십시오.
//

class CMouse;
class CTerrain;
class CCamera;
class CInput_Device;
class CObject_Manager;
class CToolApp : public CWinAppEx
{
public:
	CToolApp();


// 재정의입니다.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

public:
	_int Update_MainApp(_double TimeDelta);
	HRESULT Render_MainApp();

public:
	HRESULT Ready_DefaultSetting();

	// 구현입니다.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnIdle(LONG lCount);

public:
	LPDIRECT3DDEVICE9		m_pGraphic_Device = nullptr;

public:
	CObject_Manager*			m_pObject_Manager = nullptr;
	CInput_Device*					m_pInput_Device = nullptr;
	CCamera*							m_pCamera = nullptr;
	CMouse*								m_pMouse = nullptr;
	CTerrain*								m_pTerrain = nullptr;
};

extern CToolApp theApp;
extern CToolApp* g_pMainApp;
