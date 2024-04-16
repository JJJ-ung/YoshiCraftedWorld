// ParticleMenu.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "ParticleMenu.h"
#include "afxdialogex.h"


// CParticleMenu ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CParticleMenu, CDialogEx)

CParticleMenu::CParticleMenu(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PARTICLEMENU, pParent)
	, m_strParticleSystem(_T(""))
	, m_strEmitter(_T(""))
{

}

CParticleMenu::~CParticleMenu()
{
}

void CParticleMenu::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, m_strParticleSystem);
	DDX_Control(pDX, IDC_LIST3, m_ParticleSystemList);
	DDX_Text(pDX, IDC_EDIT14, m_strEmitter);
	DDX_Control(pDX, IDC_LIST4, m_EmitterList);
}


BEGIN_MESSAGE_MAP(CParticleMenu, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON3, &CParticleMenu::OnBnClickedAddSystem)
	ON_BN_CLICKED(IDC_BUTTON4, &CParticleMenu::OnBnClickedDeleteSystem)
	ON_BN_CLICKED(IDC_BUTTON29, &CParticleMenu::OnBnClickedAddEmitter)
	ON_BN_CLICKED(IDC_BUTTON30, &CParticleMenu::OnBnClickedDeleteEmitter)
	ON_BN_CLICKED(IDC_BUTTON27, &CParticleMenu::OnBnClickedResetEmitter)
	ON_BN_CLICKED(IDC_BUTTON26, &CParticleMenu::OnBnClickedCopyEmitter)
END_MESSAGE_MAP()


// CParticleMenu �޽��� ó�����Դϴ�.


BOOL CParticleMenu::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CParticleMenu::OnBnClickedAddSystem()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	if (m_strParticleSystem == L"")
	{
		AfxMessageBox(L"Insert Particle System Tag");
		return;
	}

	m_ParticleSystemList.AddString(m_strParticleSystem);

	UpdateData(FALSE);
}


void CParticleMenu::OnBnClickedDeleteSystem()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	int iIndex = m_ParticleSystemList.GetCurSel();
	if (iIndex == -1)
		return;

	m_ParticleSystemList.DeleteString(iIndex);

	UpdateData(FALSE);
}


void CParticleMenu::OnBnClickedAddEmitter()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	if (m_strEmitter == L"")
	{
		AfxMessageBox(L"Insert Emitter Tag");
		return;
	}

	m_EmitterList.AddString(m_strEmitter);

	UpdateData(FALSE);
}


void CParticleMenu::OnBnClickedDeleteEmitter()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	int iIndex = m_EmitterList.GetCurSel();
	if (iIndex == -1)
		return;

	m_EmitterList.DeleteString(iIndex);

	UpdateData(FALSE);
}


void CParticleMenu::OnBnClickedResetEmitter()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CParticleMenu::OnBnClickedCopyEmitter()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}
