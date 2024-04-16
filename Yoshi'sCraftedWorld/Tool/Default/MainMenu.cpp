// MainMenu.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MainMenu.h"
#include "afxdialogex.h"
#include "FileInfo.h"
#include "Object_Manager.h"
#include "Camera.h"
#include "Terrain.h"

// CMainMenu 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMainMenu, CDialogEx)

CMainMenu::CMainMenu(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MAINMENU, pParent)
	, m_iFov(60)
	, m_iTerrainX(10)
	, m_iTerrainZ(10)
	, m_iTerrainItv(1)
	, m_iColorR(100)
	, m_iColorG(100)
	, m_iColorB(100)
{

}

CMainMenu::~CMainMenu()
{
}

void CMainMenu::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_iFov);
	DDX_Text(pDX, IDC_EDIT2, m_iTerrainX);
	DDX_Text(pDX, IDC_EDIT3, m_iTerrainZ);
	DDX_Text(pDX, IDC_EDIT4, m_iTerrainItv);
	DDX_Control(pDX, IDC_RADIO2, m_TerrainRender[0]);
	DDX_Control(pDX, IDC_RADIO3, m_TerrainRender[1]);
	DDX_Text(pDX, IDC_EDIT9, m_iColorR);
	DDX_Text(pDX, IDC_EDIT7, m_iColorG);
	DDX_Text(pDX, IDC_EDIT8, m_iColorB);
	DDX_Control(pDX, IDC_CHECK5, m_AutoSave);
	DDX_Control(pDX, IDC_CHECK1, m_RenderTerrain);
	DDX_Control(pDX, IDC_CHECK3, m_RenderNavigation);
	DDX_Control(pDX, IDC_CHECK2, m_RenderObject);
	DDX_Control(pDX, IDC_CHECK4, m_RenderGimmicks);
}


BEGIN_MESSAGE_MAP(CMainMenu, CDialogEx)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CMainMenu::OnDeltaposFov)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, &CMainMenu::OnDeltaposX)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN3, &CMainMenu::OnDeltaposZ)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN4, &CMainMenu::OnDeltaposItv)
	ON_BN_CLICKED(IDC_CHECK1, &CMainMenu::OnBnClickedRenderTerrain)
	ON_BN_CLICKED(IDC_CHECK3, &CMainMenu::OnBnClickedRenderNavigation)
	ON_BN_CLICKED(IDC_CHECK2, &CMainMenu::OnBnClickedRenderObjects)
	ON_BN_CLICKED(IDC_CHECK4, &CMainMenu::OnBnClickedRenderGimmicks)
	ON_BN_CLICKED(IDC_RADIO2, &CMainMenu::OnBnClickedRenderWire)
	ON_BN_CLICKED(IDC_RADIO3, &CMainMenu::OnBnClickedRenderSolid)
	ON_BN_CLICKED(IDC_BUTTON22, &CMainMenu::OnBnClickedApplyColor)
	ON_BN_CLICKED(IDC_BUTTON17, &CMainMenu::OnBnClickedApplyFOV)
	ON_BN_CLICKED(IDC_BUTTON21, &CMainMenu::OnBnClickedApplySize)
	ON_BN_CLICKED(IDC_BUTTON23, &CMainMenu::OnBnClickedOpenPathExtract)
END_MESSAGE_MAP()


// CMainMenu 메시지 처리기입니다.


BOOL CMainMenu::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_TerrainRender[0].SetCheck(TRUE);

	m_RenderTerrain.SetCheck(TRUE);
	m_RenderNavigation.SetCheck(TRUE);
	m_RenderObject.SetCheck(TRUE);
	m_RenderGimmicks.SetCheck(TRUE);

	if (nullptr == m_PathExtract.GetSafeHwnd())
		m_PathExtract.Create(IDD_PATHEXTRACT);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CMainMenu::OnDeltaposFov(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	if (pNMUpDown->iDelta < 0)
	{
		if (m_iFov < 175)
			m_iFov += 5;
	}
	else
	{
		if (m_iFov > 10)
			m_iFov -= 5;
	}

	UpdateData(FALSE);
	*pResult = 0;
}


void CMainMenu::OnDeltaposX(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	if (pNMUpDown->iDelta < 0)
		++m_iTerrainX;
	else
	{
		if (m_iTerrainX > 1)
			--m_iTerrainX;
	}

	UpdateData(FALSE);
	*pResult = 0;
}


void CMainMenu::OnDeltaposZ(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	if (pNMUpDown->iDelta < 0)
		++m_iTerrainZ;
	else
	{
		if (m_iTerrainZ > 1)
			--m_iTerrainZ;
	}

	UpdateData(FALSE);
	*pResult = 0;
}


void CMainMenu::OnDeltaposItv(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	if (pNMUpDown->iDelta < 0)
		++m_iTerrainItv;
	else
	{
		if (m_iTerrainItv > 1)
			--m_iTerrainItv;
	}

	UpdateData(FALSE);
	*pResult = 0;
}


void CMainMenu::OnBnClickedApplyFOV()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CCamera::GetInstance()->m_CameraDesc.fFovY = D3DXToRadian(m_iFov);

	UpdateData(FALSE);
}


void CMainMenu::OnBnClickedApplySize()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CTerrain* pTerrain = dynamic_cast<CTerrain*>(CObject_Manager::GetInstance()->m_GameObjects[CObject_Manager::BACKGROUND][0]);
	pTerrain->Change_TerrainSize(m_iTerrainX, m_iTerrainZ, m_iTerrainItv);

	UpdateData(FALSE);
}


void CMainMenu::OnBnClickedRenderWire()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CTerrain* pTerrain = dynamic_cast<CTerrain*>(CObject_Manager::GetInstance()->m_GameObjects[CObject_Manager::BACKGROUND][0]);
	pTerrain->m_iShaderPass = 0;

	UpdateData(FALSE);
}


void CMainMenu::OnBnClickedRenderSolid()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CTerrain* pTerrain = dynamic_cast<CTerrain*>(CObject_Manager::GetInstance()->m_GameObjects[CObject_Manager::BACKGROUND][0]);
	pTerrain->m_iShaderPass = 1;

	UpdateData(FALSE);
}


void CMainMenu::OnBnClickedApplyColor()
{
	UpdateData(TRUE);

	CTerrain* pTerrain = dynamic_cast<CTerrain*>(CObject_Manager::GetInstance()->m_GameObjects[CObject_Manager::BACKGROUND][0]);
	pTerrain->Change_TerrainColor(1.f, (float)(m_iColorR / 255.f), (float)(m_iColorG / 255.f), (float)(m_iColorB / 255.f));

	UpdateData(FALSE);
}


void CMainMenu::OnBnClickedRenderTerrain()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CObject_Manager::GetInstance()->m_bRender[CObject_Manager::BACKGROUND] = m_RenderTerrain.GetCheck();

	UpdateData(FALSE);
}


void CMainMenu::OnBnClickedRenderNavigation()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CObject_Manager::GetInstance()->m_bRender[CObject_Manager::NAVIGATION] = m_RenderTerrain.GetCheck();

	UpdateData(FALSE);
}


void CMainMenu::OnBnClickedRenderObjects()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CObject_Manager::GetInstance()->m_bRender[CObject_Manager::OBJECT] = m_RenderTerrain.GetCheck();

	UpdateData(FALSE);
}


void CMainMenu::OnBnClickedRenderGimmicks()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CObject_Manager::GetInstance()->m_bRender[CObject_Manager::GIMMICK] = m_RenderTerrain.GetCheck();
	CObject_Manager::GetInstance()->m_bRender[CObject_Manager::UNIT] = m_RenderTerrain.GetCheck();

	UpdateData(FALSE);
}



void CMainMenu::OnBnClickedOpenPathExtract()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_PathExtract.ShowWindow(SW_SHOW);
}
