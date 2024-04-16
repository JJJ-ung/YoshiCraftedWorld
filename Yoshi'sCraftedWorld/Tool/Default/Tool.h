
// Tool.h : Tool ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.
#include "ToolView.h"

// CToolApp:
// �� Ŭ������ ������ ���ؼ��� Tool.cpp�� �����Ͻʽÿ�.
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


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

public:
	_int Update_MainApp(_double TimeDelta);
	HRESULT Render_MainApp();

public:
	HRESULT Ready_DefaultSetting();

	// �����Դϴ�.
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
