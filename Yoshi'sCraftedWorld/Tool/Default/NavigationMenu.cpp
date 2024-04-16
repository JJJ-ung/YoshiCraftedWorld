// NavigationMenu.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "NavigationMenu.h"
#include "afxdialogex.h"


// CNavigationMenu 대화 상자입니다.

IMPLEMENT_DYNAMIC(CNavigationMenu, CDialogEx)

CNavigationMenu::CNavigationMenu(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_NAVIGATIONMENU, pParent)
	, m_strGroupTag(_T("GroupTag"))
	, m_fPosX(0.f)
	, m_fPosY(0.f)
	, m_fPosZ(0.f)
	, m_strPathType(_T(""))
{

}

CNavigationMenu::~CNavigationMenu()
{
}

void CNavigationMenu::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO1, m_PickMode[0]);
	DDX_Control(pDX, IDC_RADIO4, m_PickMode[1]);
	DDX_Control(pDX, IDC_COMBO2, m_NavigationType);
	DDX_Control(pDX, IDC_TREE1, m_NavigationList);
	DDX_Text(pDX, IDC_EDIT1, m_strGroupTag);
	DDX_Text(pDX, IDC_EDIT2, m_fPosX);
	DDX_Text(pDX, IDC_EDIT14, m_fPosY);
	DDX_Text(pDX, IDC_EDIT15, m_fPosZ);
	DDX_Control(pDX, IDC_CHECK1, m_RenderPlayer);
	DDX_Control(pDX, IDC_CHECK2, m_RenderMonster);
	DDX_Control(pDX, IDC_CHECK3, m_RenderTrain);
}


BEGIN_MESSAGE_MAP(CNavigationMenu, CDialogEx)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, &CNavigationMenu::OnDeltaposPosX)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN6, &CNavigationMenu::OnDeltaposPosY)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN14, &CNavigationMenu::OnDeltaposPosZ)
	ON_BN_CLICKED(IDC_BUTTON14, &CNavigationMenu::OnBnClickedSetGroupName)
END_MESSAGE_MAP()


// CNavigationMenu 메시지 처리기입니다.


BOOL CNavigationMenu::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_PickMode[0].SetCheck(TRUE);

	m_NavigationType.AddString(L"Player Path");
	m_NavigationType.AddString(L"Monster Path");
	m_NavigationType.AddString(L"Train Path");

	m_NavigationType.SetCurSel(0);

	m_RenderPlayer.SetCheck(TRUE);
	m_RenderMonster.SetCheck(FALSE);
	m_RenderTrain.SetCheck(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CNavigationMenu::OnDeltaposPosX(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	float fAddValue = 0.1f;

	if (pNMUpDown->iDelta < 0.f)
		m_fPosX += fAddValue;
	else
		m_fPosX -= fAddValue;

	UpdateData(FALSE);
	*pResult = 0;
}


void CNavigationMenu::OnDeltaposPosY(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	float fAddValue = 0.1f;

	if (pNMUpDown->iDelta < 0.f)
		m_fPosY += fAddValue;
	else
		m_fPosY -= fAddValue;

	UpdateData(FALSE);
	*pResult = 0;
}


void CNavigationMenu::OnDeltaposPosZ(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	float fAddValue = 0.1f;

	if (pNMUpDown->iDelta < 0.f)
		m_fPosZ += fAddValue;
	else
		m_fPosZ -= fAddValue;

	UpdateData(FALSE);
	*pResult = 0;
}


void CNavigationMenu::OnBnClickedSetGroupName()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_NavigationList.GetSelectedItem() == nullptr)
		return;

	UpdateData(TRUE);

	UpdateData(FALSE);
}
