
// Tool.cpp : ���� ���α׷��� ���� Ŭ���� ������ �����մϴ�.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "Tool.h"
#include "MainFrm.h"

#include "ToolDoc.h"
#include "Graphic_Device.h"
#include "Object_Manager.h"
#include "Input_Device.h"

#include "Terrain.h"
#include "Camera.h"
#include "Mouse.h"

#include "ToolMenu.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CToolApp
HWND g_hWnd;

BEGIN_MESSAGE_MAP(CToolApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CToolApp::OnAppAbout)
	// ǥ�� ������ ���ʷ� �ϴ� ���� ����Դϴ�.
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// ǥ�� �μ� ���� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// CToolApp ����

CToolApp::CToolApp()
{
	// �ٽ� ���� ������ ����
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// ���� ���α׷��� ���� ��� ��Ÿ�� ������ ����Ͽ� ������ ���(/clr):
	//     1) �� �߰� ������ �ٽ� ���� ������ ������ ����� �۵��ϴ� �� �ʿ��մϴ�.
	//     2) ������Ʈ���� �����Ϸ��� System.Windows.Forms�� ���� ������ �߰��ؾ� �մϴ�.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: �Ʒ� ���� ���α׷� ID ���ڿ��� ���� ID ���ڿ��� �ٲٽʽÿ�(����).
	// ���ڿ��� ���� ����: CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("Tool.AppID.NoVersion"));

	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	// InitInstance�� ��� �߿��� �ʱ�ȭ �۾��� ��ġ�մϴ�.
}

// ������ CToolApp ��ü�Դϴ�.

CToolApp theApp;
CToolApp* g_pMainApp;

// CToolApp �ʱ�ȭ

BOOL CToolApp::InitInstance()
{
	// ���� ���α׷� �Ŵ��佺Ʈ�� ComCtl32.dll ���� 6 �̻��� ����Ͽ� ���־� ��Ÿ����
	// ����ϵ��� �����ϴ� ���, Windows XP �󿡼� �ݵ�� InitCommonControlsEx()�� �ʿ��մϴ�. 
	// InitCommonControlsEx()�� ������� ������ â�� ���� �� �����ϴ�.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ���� ���α׷����� ����� ��� ���� ��Ʈ�� Ŭ������ �����ϵ���
	// �� �׸��� �����Ͻʽÿ�.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	// OLE ���̺귯���� �ʱ�ȭ�մϴ�.
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// RichEdit ��Ʈ���� ����Ϸ���  AfxInitRichEdit2()�� �־�� �մϴ�.	
	// AfxInitRichEdit2();

	// ǥ�� �ʱ�ȭ
	// �̵� ����� ������� �ʰ� ���� ���� ������ ũ�⸦ ���̷���
	// �Ʒ����� �ʿ� ���� Ư�� �ʱ�ȭ
	// ��ƾ�� �����ؾ� �մϴ�.
	// �ش� ������ ����� ������Ʈ�� Ű�� �����Ͻʽÿ�.
	// TODO: �� ���ڿ��� ȸ�� �Ǵ� ������ �̸��� ����
	// ������ �������� �����ؾ� �մϴ�.
	SetRegistryKey(_T("���� ���� ���α׷� �����翡�� ������ ���� ���α׷�"));
	LoadStdProfileSettings(4);  // MRU�� �����Ͽ� ǥ�� INI ���� �ɼ��� �ε��մϴ�.


	// ���� ���α׷��� ���� ���ø��� ����մϴ�.  ���� ���ø���
	//  ����, ������ â �� �� ������ ���� ������ �մϴ�.
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CToolDoc),
		RUNTIME_CLASS(CMainFrame),       // �� SDI ������ â�Դϴ�.
		RUNTIME_CLASS(CToolView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// ǥ�� �� ���, DDE, ���� ���⿡ ���� ������� ���� �м��մϴ�.
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);



	// ����ٿ� ������ ����� ����ġ�մϴ�.
	// ���� ���α׷��� /RegServer, /Register, /Unregserver �Ǵ� /Unregister�� ���۵� ��� FALSE�� ��ȯ�մϴ�.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	m_pMainWnd->SetWindowTextW(L"Yoshi's Crafted World");

	// â �ϳ��� �ʱ�ȭ�Ǿ����Ƿ� �̸� ǥ���ϰ� ������Ʈ�մϴ�.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	g_pMainApp = this;

	if (FAILED(Ready_DefaultSetting()))
	{
		MSG_BOX("Failed : Ready_DefaultSetting");
		return FALSE;
	}

#include <crtdbg.h>

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	return TRUE;
}

int CToolApp::ExitInstance()
{
	//TODO: �߰��� �߰� ���ҽ��� ó���մϴ�.
	AfxOleTerm(FALSE);

	CObject_Manager::GetInstance()->Free();
	CObject_Manager::GetInstance()->DestroyInstance();

	CCamera::GetInstance()->Free();
	CCamera::GetInstance()->DestroyInstance();

	CInput_Device::GetInstance()->Free();
	CInput_Device::GetInstance()->DestroyInstance();

	Safe_Release(m_pGraphic_Device);
	CGraphic_Device::GetInstance()->DestroyInstance();

	return CWinAppEx::ExitInstance();
}

_int CToolApp::Update_MainApp(_double TimeDelta)
{
	m_pInput_Device->Set_InputDev();
	m_pCamera->Update_Camera(TimeDelta);
	m_pObject_Manager->Update_Object_Manager(TimeDelta);
	m_pCamera->LateUpdate_Camera(TimeDelta);
	m_pObject_Manager->LateUpdate_Object_Manager(TimeDelta);

	return 0;
}

HRESULT CToolApp::Render_MainApp()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_ARGB(255, 255, 250, 250), 1.f, 0);
	m_pGraphic_Device->BeginScene();

	m_pObject_Manager->Render_Object_Manager();

	m_pGraphic_Device->EndScene();
	m_pGraphic_Device->Present(nullptr, nullptr, 0, nullptr);

	return NOERROR;
}

HRESULT CToolApp::Ready_DefaultSetting()
{
	CGraphic_Device* pGraphicDev = CGraphic_Device::GetInstance();
	m_pInput_Device = CInput_Device::GetInstance();

	if (FAILED(pGraphicDev->Ready_GraphicDev(g_hWnd, CGraphic_Device::MODE_WIN, WINCX, WINCY, &m_pGraphic_Device)))
		return E_FAIL;

	if (FAILED(m_pInput_Device->Ready_InputDev(AfxGetInstanceHandle(), g_hWnd)))
		return E_FAIL;

	m_pObject_Manager = CObject_Manager::GetInstance();
	if (m_pObject_Manager == nullptr)
		return E_FAIL;

	_matrix temp, matView, matProj;
	D3DXMatrixIdentity(&temp);
	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &temp);

	m_pCamera = CCamera::GetInstance();
	m_pCamera->Ready_Camera(m_pGraphic_Device, CCamera::CAMERADESC(D3DXVECTOR3(3.f, 7.f, -5.f), D3DXVECTOR3(3.f, 0.f, 0.f), D3DXToRadian(60.f)));
	
	m_pTerrain = CTerrain::Create(m_pGraphic_Device);
	m_pObject_Manager->Add_GameObject(CObject_Manager::BACKGROUND, m_pTerrain);
	m_pObject_Manager->m_pTerrain = m_pTerrain;

	m_pMouse = CMouse::Create(m_pGraphic_Device);
	m_pObject_Manager->Add_GameObject(CObject_Manager::MOUSE, m_pMouse);

	return NOERROR;
}

// CToolApp �޽��� ó����


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// ��ȭ ���ڸ� �����ϱ� ���� ���� ���α׷� ����Դϴ�.
void CToolApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CToolApp �޽��� ó����


BOOL CToolApp::OnIdle(LONG lCount)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	if (this->m_pMainWnd->IsIconic())
		return FALSE;
	else
	{
		Update_MainApp(0.0);
		Render_MainApp();
	}

	return TRUE;
}
