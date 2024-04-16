
// Tool.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
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
	// 표준 파일을 기초로 하는 문서 명령입니다.
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// 표준 인쇄 설정 명령입니다.
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// CToolApp 생성

CToolApp::CToolApp()
{
	// 다시 시작 관리자 지원
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// 응용 프로그램을 공용 언어 런타임 지원을 사용하여 빌드한 경우(/clr):
	//     1) 이 추가 설정은 다시 시작 관리자 지원이 제대로 작동하는 데 필요합니다.
	//     2) 프로젝트에서 빌드하려면 System.Windows.Forms에 대한 참조를 추가해야 합니다.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: 아래 응용 프로그램 ID 문자열을 고유 ID 문자열로 바꾸십시오(권장).
	// 문자열에 대한 서식: CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("Tool.AppID.NoVersion"));

	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}

// 유일한 CToolApp 개체입니다.

CToolApp theApp;
CToolApp* g_pMainApp;

// CToolApp 초기화

BOOL CToolApp::InitInstance()
{
	// 응용 프로그램 매니페스트가 ComCtl32.dll 버전 6 이상을 사용하여 비주얼 스타일을
	// 사용하도록 지정하는 경우, Windows XP 상에서 반드시 InitCommonControlsEx()가 필요합니다. 
	// InitCommonControlsEx()를 사용하지 않으면 창을 만들 수 없습니다.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 응용 프로그램에서 사용할 모든 공용 컨트롤 클래스를 포함하도록
	// 이 항목을 설정하십시오.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	// OLE 라이브러리를 초기화합니다.
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// RichEdit 컨트롤을 사용하려면  AfxInitRichEdit2()가 있어야 합니다.	
	// AfxInitRichEdit2();

	// 표준 초기화
	// 이들 기능을 사용하지 않고 최종 실행 파일의 크기를 줄이려면
	// 아래에서 필요 없는 특정 초기화
	// 루틴을 제거해야 합니다.
	// 해당 설정이 저장된 레지스트리 키를 변경하십시오.
	// TODO: 이 문자열을 회사 또는 조직의 이름과 같은
	// 적절한 내용으로 수정해야 합니다.
	SetRegistryKey(_T("로컬 응용 프로그램 마법사에서 생성된 응용 프로그램"));
	LoadStdProfileSettings(4);  // MRU를 포함하여 표준 INI 파일 옵션을 로드합니다.


	// 응용 프로그램의 문서 템플릿을 등록합니다.  문서 템플릿은
	//  문서, 프레임 창 및 뷰 사이의 연결 역할을 합니다.
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CToolDoc),
		RUNTIME_CLASS(CMainFrame),       // 주 SDI 프레임 창입니다.
		RUNTIME_CLASS(CToolView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// 표준 셸 명령, DDE, 파일 열기에 대한 명령줄을 구문 분석합니다.
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);



	// 명령줄에 지정된 명령을 디스패치합니다.
	// 응용 프로그램이 /RegServer, /Register, /Unregserver 또는 /Unregister로 시작된 경우 FALSE를 반환합니다.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	m_pMainWnd->SetWindowTextW(L"Yoshi's Crafted World");

	// 창 하나만 초기화되었으므로 이를 표시하고 업데이트합니다.
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
	//TODO: 추가한 추가 리소스를 처리합니다.
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

// CToolApp 메시지 처리기


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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

// 대화 상자를 실행하기 위한 응용 프로그램 명령입니다.
void CToolApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CToolApp 메시지 처리기


BOOL CToolApp::OnIdle(LONG lCount)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	if (this->m_pMainWnd->IsIconic())
		return FALSE;
	else
	{
		Update_MainApp(0.0);
		Render_MainApp();
	}

	return TRUE;
}
