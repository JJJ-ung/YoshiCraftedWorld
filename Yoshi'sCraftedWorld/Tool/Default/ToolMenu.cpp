// ToolMenu.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "ToolMenu.h"
#include "Input_Device.h"

// CToolMenu

IMPLEMENT_DYNCREATE(CToolMenu, CFormView)

CToolMenu::CToolMenu()
	: CFormView(IDD_TOOLMENU)
{

}

CToolMenu::~CToolMenu()
{
	Safe_Delete(m_pMain);
	Safe_Delete(m_pObjects);
	Safe_Delete(m_pGimmicks);
	Safe_Delete(m_pNavigation);
	Safe_Delete(m_pAnimation);
	Safe_Delete(m_pParticle);
}

void CToolMenu::DoDataExchange(CDataExchange* pDX)
{ 
	DDX_Control(pDX, IDC_TAB1, m_TabCtrl);
}

BEGIN_MESSAGE_MAP(CToolMenu, CFormView)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CToolMenu::OnTcnSelchangeTabCtrl)
END_MESSAGE_MAP()


// CToolMenu �����Դϴ�.

#ifdef _DEBUG
void CToolMenu::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CToolMenu::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CToolMenu �޽��� ó�����Դϴ�.


void CToolMenu::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	CSize scrollSize(0, 0);
	SetScrollSizes(MM_TEXT, scrollSize);

	m_MainFont.CreatePointFont(100, L"������������� Regular");
	GetDlgItem(IDC_TAB1)->SetFont(&m_MainFont);

	m_TabCtrl.InsertItem(0, L"MAIN");
	m_TabCtrl.InsertItem(1, L"OBJECTS");
	m_TabCtrl.InsertItem(2, L"GIMMICKS");
	m_TabCtrl.InsertItem(3, L"NAVIGATION");
	m_TabCtrl.InsertItem(4, L"COLLIDER");
	m_TabCtrl.InsertItem(5, L"PARTICLE");

	m_TabCtrl.SetCurSel(0);

	CRect rect;
	m_TabCtrl.GetWindowRect(&rect);

	m_pMain = new CMainMenu;
	m_pMain->Create(IDD_MAINMENU, &m_TabCtrl);
	m_pMain->MoveWindow(0, 25, rect.Width(), rect.Height());

	m_pObjects = new CObjectMenu;
	m_pObjects->Create(IDD_OBJECTMENU, &m_TabCtrl);
	m_pObjects->MoveWindow(0, 25, rect.Width(), rect.Height());

	m_pGimmicks = new CGimmickMenu;
	m_pGimmicks->Create(IDD_GIMMICKMENU, &m_TabCtrl);
	m_pGimmicks->MoveWindow(0, 25, rect.Width(), rect.Height());

	m_pNavigation = new CNavigationMenu;
	m_pNavigation->Create(IDD_NAVIGATIONMENU, &m_TabCtrl);
	m_pNavigation->MoveWindow(0, 25, rect.Width(), rect.Height());

	m_pAnimation = new CAnimationMenu;
	m_pAnimation->Create(IDD_ANIMATIONMENU, &m_TabCtrl);
	m_pAnimation->MoveWindow(0, 25, rect.Width(), rect.Height());

	m_pParticle = new CParticleMenu;
	m_pParticle->Create(IDD_PARTICLEMENU, &m_TabCtrl);
	m_pParticle->MoveWindow(0, 25, rect.Width(), rect.Height());

	m_pMain->ShowWindow(SW_SHOW);
	m_pWnd = m_pMain;
}


void CToolMenu::OnTcnSelchangeTabCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	if (m_pWnd != nullptr)
	{
		m_pWnd->ShowWindow(SW_HIDE);
		m_pWnd = nullptr;
	}

	int iSel = m_TabCtrl.GetCurSel();

	switch (iSel)
	{
	case 0:
		m_pMain->ShowWindow(SW_SHOW);
		m_pWnd = m_pMain;
		CInput_Device::GetInstance()->m_bLock_KeyBoard = false;
		break;
	case 1:
		m_pObjects->ShowWindow(SW_SHOW);
		m_pWnd = m_pObjects;
		CInput_Device::GetInstance()->m_bLock_KeyBoard = false;
		break;
	case 2:
		m_pGimmicks->ShowWindow(SW_SHOW);
		m_pWnd = m_pGimmicks;
		CInput_Device::GetInstance()->m_bLock_KeyBoard = false;
		break;
	case 3:
		m_pNavigation->ShowWindow(SW_SHOW);
		m_pWnd = m_pNavigation;
		CInput_Device::GetInstance()->m_bLock_KeyBoard = false;
		break;
	case 4:
		m_pAnimation->ShowWindow(SW_SHOW);
		m_pWnd = m_pAnimation;
		CInput_Device::GetInstance()->m_bLock_KeyBoard = true;
		break;
	case 5:
		m_pParticle->ShowWindow(SW_SHOW);
		m_pWnd = m_pParticle;
		CInput_Device::GetInstance()->m_bLock_KeyBoard = true;
		break;
	}

	*pResult = 0;
}
