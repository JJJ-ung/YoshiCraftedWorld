// AnimationMenu.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "AnimationMenu.h"
#include "afxdialogex.h"
#include "FileInfo.h"

#include "Graphic_Device.h"
#include "Object_Manager.h"
#include "DynamicMesh.h"

#include "Unit.h"

// CAnimationMenu 대화 상자입니다.

IMPLEMENT_DYNAMIC(CAnimationMenu, CDialogEx)

CAnimationMenu::CAnimationMenu(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ANIMATIONMENU, pParent)
	, m_fRadius(0)
	, m_fCenterX(0)
	, m_fCenterY(0)
	, m_fCenterZ(0)
{

}

CAnimationMenu::~CAnimationMenu()
{
}

void CAnimationMenu::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_MeshList);
	DDX_Control(pDX, IDC_LIST2, m_ColliderList);
	DDX_Control(pDX, IDC_LIST3, m_BoneList);
	DDX_Text(pDX, IDC_EDIT1, m_fRadius);
	DDX_Text(pDX, IDC_EDIT2, m_fCenterX);
	DDX_Text(pDX, IDC_EDIT14, m_fCenterY);
	DDX_Text(pDX, IDC_EDIT15, m_fCenterZ);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON1, m_ColliderColor);
}


BEGIN_MESSAGE_MAP(CAnimationMenu, CDialogEx)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, &CAnimationMenu::OnDeltaposCenterX)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN6, &CAnimationMenu::OnDeltaposCenterY)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN14, &CAnimationMenu::OnDeltaposCenterZ)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE1, &CAnimationMenu::OnNMDblclkMeshList)
END_MESSAGE_MAP()


// CAnimationMenu 메시지 처리기입니다.


BOOL CAnimationMenu::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CString strFolderPath = CFileInfo::GetFolderPath(L"\\Client\\Bin\\Resources\\Characters");
	CFileInfo::DirRootExtraction(strFolderPath.GetString(), m_MeshList, L".X");

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CAnimationMenu::OnDeltaposCenterX(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	float fAddValue = 0.1f;

	if (pNMUpDown->iDelta < 0.f)
		m_fCenterX += fAddValue;
	else
		m_fCenterX -= fAddValue;

	UpdateData(FALSE);
	*pResult = 0;
}


void CAnimationMenu::OnDeltaposCenterY(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	float fAddValue = 0.1f;

	if (pNMUpDown->iDelta < 0.f)
		m_fCenterY += fAddValue;
	else
		m_fCenterY -= fAddValue;

	UpdateData(FALSE);
	*pResult = 0;
}


void CAnimationMenu::OnDeltaposCenterZ(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	float fAddValue = 0.1f;

	if (pNMUpDown->iDelta < 0.f)
		m_fCenterZ += fAddValue;
	else
		m_fCenterZ -= fAddValue;

	UpdateData(FALSE);
	*pResult = 0;
}


void CAnimationMenu::OnNMDblclkMeshList(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CString strFilePath = Get_ObjectPath();

	HTREEITEM hItem = m_MeshList.GetSelectedItem();
	CString strFileName = m_MeshList.GetItemText(hItem);

	if (m_pCurrentUnit != nullptr)
		CObject_Manager::GetInstance()->Delete_Object(CObject_Manager::UNIT, 0);

	m_pCurrentUnit = CUnit::Create(CGraphic_Device::GetInstance()->m_pGraphicDev, strFilePath.GetString(), strFileName.GetString(), _vec3(0.f, 0.f, 0.f));

	CObject_Manager::GetInstance()->Add_GameObject(CObject_Manager::UNIT, m_pCurrentUnit);

	*pResult = 0;
}

CString CAnimationMenu::Get_ObjectPath()
{
	HTREEITEM hItem = m_MeshList.GetSelectedItem();
	CString strMeshName = m_MeshList.GetItemText(hItem);

	CString strFilePath = L"";

	if (hItem == NULL)
		return strFilePath;
	if (m_MeshList.ItemHasChildren(hItem))
		return strFilePath;

	//strFilePath = m_MeshList.GetItemText(hItem);
	while (true)
	{
		if (m_MeshList.GetParentItem(hItem) == NULL)
			break;

		HTREEITEM hParentItem = m_MeshList.GetParentItem(hItem);
		CString strParentPath = m_MeshList.GetItemText(hParentItem);
		strFilePath = strParentPath + L"/" + strFilePath;
		hItem = hParentItem;
	}

	strFilePath = L"../../Client/Bin/Resources/Characters/" + strFilePath;

	return strFilePath;
}
