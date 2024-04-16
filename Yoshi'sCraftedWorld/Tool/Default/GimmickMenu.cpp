// GimmickMenu.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "GimmickMenu.h"
#include "afxdialogex.h"
#include "FileInfo.h"
#include "Gimmick.h"
#include "Transform.h"
#include "Trigger.h"
#include "Graphic_Device.h"
#include "Input_Device.h"
#include "Mouse.h"

// CGimmickMenu 대화 상자입니다.

IMPLEMENT_DYNAMIC(CGimmickMenu, CDialogEx)

CGimmickMenu::CGimmickMenu(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_GIMMICKMENU, pParent)
	, m_fPosX(0.f)
	, m_fPosY(0.f)
	, m_fPosZ(0.f)
	, m_fRotX(0.f)
	, m_fRotY(0.f)
	, m_fRotZ(0.f)
	, m_fScaleX(1.f)
	, m_fScaleY(1.f)
	, m_fScaleZ(1.f)
	, m_iAnimationKey(0)
	, m_strTriggerKey(_T(""))
	, m_fTriggerX(0)
	, m_fTriggerY(0)
	, m_fTriggerZ(0)
	, m_fTriggerSize(1.f)
	, m_iOption(0)
{

}

CGimmickMenu::~CGimmickMenu()
{
}

void CGimmickMenu::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO1, m_RenderMode[0]);
	DDX_Control(pDX, IDC_RADIO4, m_RenderMode[1]);
	DDX_Control(pDX, IDC_RADIO14, m_PickMode[0]);
	DDX_Control(pDX, IDC_RADIO15, m_PickMode[1]);
	DDX_Control(pDX, IDC_TREE1, m_MeshList);
	DDX_Control(pDX, IDC_COMBO1, m_GimmickType);
	DDX_Text(pDX, IDC_EDIT2, m_fPosX);
	DDX_Text(pDX, IDC_EDIT3, m_fPosY);
	DDX_Text(pDX, IDC_EDIT4, m_fPosZ);
	DDX_Text(pDX, IDC_EDIT6, m_fRotX);
	DDX_Text(pDX, IDC_EDIT10, m_fRotY);
	DDX_Text(pDX, IDC_EDIT11, m_fRotZ);
	DDX_Text(pDX, IDC_EDIT7, m_fScaleX);
	DDX_Text(pDX, IDC_EDIT12, m_fScaleY);
	DDX_Text(pDX, IDC_EDIT13, m_fScaleZ);
	DDX_Text(pDX, IDC_EDIT1, m_iAnimationKey);
	DDX_Text(pDX, IDC_EDIT19, m_strTriggerKey);
	DDX_Text(pDX, IDC_EDIT20, m_fTriggerX);
	DDX_Text(pDX, IDC_EDIT21, m_fTriggerY);
	DDX_Text(pDX, IDC_EDIT22, m_fTriggerZ);
	DDX_Text(pDX, IDC_EDIT23, m_fTriggerSize);
	DDX_Control(pDX, IDC_LIST5, m_TriggerList);
	DDX_Control(pDX, IDC_LIST2, m_GimmickList);
	DDX_Text(pDX, IDC_EDIT24, m_iOption);
}


BEGIN_MESSAGE_MAP(CGimmickMenu, CDialogEx)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, &CGimmickMenu::OnDeltaposPosX)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN6, &CGimmickMenu::OnDeltaposPosY)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN7, &CGimmickMenu::OnDeltaposPosZ)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN8, &CGimmickMenu::OnDeltaposRotX)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN9, &CGimmickMenu::OnDeltaposRotY)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN10, &CGimmickMenu::OnDeltaposRotZ)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN11, &CGimmickMenu::OnDeltaposScaleX)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN12, &CGimmickMenu::OnDeltaposScaleY)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN13, &CGimmickMenu::OnDeltaposScaleZ)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN15, &CGimmickMenu::OnDeltaposSpinTiggerX)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN16, &CGimmickMenu::OnDeltaposSpinTiggerY)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN17, &CGimmickMenu::OnDeltaposSpinTiggerZ)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN18, &CGimmickMenu::OnDeltaposSpinTriggerSize)
	ON_BN_CLICKED(IDC_BUTTON1, &CGimmickMenu::OnBnClickedAdjust)
	ON_BN_CLICKED(IDC_BUTTON6, &CGimmickMenu::OnBnClickedDelete)
	ON_BN_CLICKED(IDC_BUTTON11, &CGimmickMenu::OnBnClickedAdjustAnimation)
	ON_LBN_SELCHANGE(IDC_LIST2, &CGimmickMenu::OnLbnSelchangeGimmickList)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CGimmickMenu::OnCbnSelchangeGimmickType)
	ON_BN_CLICKED(IDC_BUTTON26, &CGimmickMenu::OnBnClickedAddTrigger)
	ON_EN_SETFOCUS(IDC_EDIT19, &CGimmickMenu::OnEnSetfocusTriggerKey)
	ON_EN_KILLFOCUS(IDC_EDIT19, &CGimmickMenu::OnEnKillfocusTriggerKey)
	ON_BN_CLICKED(IDC_BUTTON20, &CGimmickMenu::OnBnClickedDeleteTrigger)
	ON_BN_CLICKED(IDC_BUTTON42, &CGimmickMenu::OnBnClickedAdjustKey)
	ON_LBN_SELCHANGE(IDC_LIST5, &CGimmickMenu::OnLbnSelchangeTriggerList)
	ON_BN_CLICKED(IDC_BUTTON2, &CGimmickMenu::OnBnClickedReset)
	ON_BN_CLICKED(IDC_BUTTON12, &CGimmickMenu::OnBnClickedSave)
	ON_BN_CLICKED(IDC_BUTTON13, &CGimmickMenu::OnBnClickedLoad)
	ON_BN_CLICKED(IDC_BUTTON43, &CGimmickMenu::OnBnClickedSaveTrigger)
	ON_BN_CLICKED(IDC_BUTTON44, &CGimmickMenu::OnBnClickedLoadTrigger)
	ON_BN_CLICKED(IDC_BUTTON45, &CGimmickMenu::OnBnClickedSaveTriggerInline)
	ON_BN_CLICKED(IDC_BUTTON24, &CGimmickMenu::OnBnClickedResetTrigger)
	ON_BN_CLICKED(IDC_BUTTON18, &CGimmickMenu::OnBnClickedAdjustOption)
	ON_BN_CLICKED(IDC_BUTTON25, &CGimmickMenu::OnBnClickedSaveInline)
	ON_BN_CLICKED(IDC_BUTTON30, &CGimmickMenu::OnBnClickedSaveAllInline)
END_MESSAGE_MAP()


// CGimmickMenu 메시지 처리기입니다.


BOOL CGimmickMenu::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_RenderMode[0].SetCheck(TRUE);
	m_PickMode[0].SetCheck(TRUE);

	CString strFolderPath = CFileInfo::GetFolderPath(L"\\Client\\Bin\\Resources\\");
	CFileInfo::DirRootExtraction(strFolderPath.GetString(), m_MeshList, L".X");

	m_pObjectMgr = CObject_Manager::GetInstance();

	m_GimmickType.AddString(L"Dynamic Object");
	m_GimmickType.AddString(L"Egg Interact");

	m_GimmickType.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CGimmickMenu::OnDeltaposPosX(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CGimmick* pObj = dynamic_cast<CGimmick*>(m_pObjectMgr->Get_Object(CObject_Manager::GIMMICK, m_GimmickList.GetCurSel()));
	if (pObj == nullptr)
		return;

	float fAddValue = 0.1f;

	if (pNMUpDown->iDelta < 0.f)
		m_fPosX += fAddValue;
	else
		m_fPosX -= fAddValue;

	pObj->m_pTransformCom->Set_State(CTransform::STATE_POSITION, _vec3(m_fPosX, m_fPosY, m_fPosZ));

	UpdateData(FALSE);
	*pResult = 0;
}


void CGimmickMenu::OnDeltaposPosY(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CGimmick* pObj = dynamic_cast<CGimmick*>(m_pObjectMgr->Get_Object(CObject_Manager::GIMMICK, m_GimmickList.GetCurSel()));
	if (pObj == nullptr)
		return;

	float fAddValue = 0.1f;

	if (pNMUpDown->iDelta < 0.f)
		m_fPosY += fAddValue;
	else
		m_fPosY -= fAddValue;

	pObj->m_pTransformCom->Set_State(CTransform::STATE_POSITION, _vec3(m_fPosX, m_fPosY, m_fPosZ));

	UpdateData(FALSE);
	*pResult = 0;
}


void CGimmickMenu::OnDeltaposPosZ(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CGimmick* pObj = dynamic_cast<CGimmick*>(m_pObjectMgr->Get_Object(CObject_Manager::GIMMICK, m_GimmickList.GetCurSel()));
	if (pObj == nullptr)
		return;

	float fAddValue = 0.1f;

	if (pNMUpDown->iDelta < 0.f)
		m_fPosZ += fAddValue;
	else
		m_fPosZ -= fAddValue;

	pObj->m_pTransformCom->Set_State(CTransform::STATE_POSITION, _vec3(m_fPosX, m_fPosY, m_fPosZ));

	UpdateData(FALSE);
	*pResult = 0;
}


void CGimmickMenu::OnDeltaposRotX(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CGimmick* pObj = dynamic_cast<CGimmick*>(m_pObjectMgr->Get_Object(CObject_Manager::GIMMICK, m_GimmickList.GetCurSel()));
	if (pObj == nullptr)
		return;

	if (pNMUpDown->iDelta < 0)
	{
		++m_fRotX;
		if (m_fRotX > 360)
			m_fRotX = 0;
	}
	else
	{
		if (m_fRotX <= 0)
			m_fRotX = 359;
		else
			--m_fRotX;
	}

	pObj->m_pTransformCom->SetUp_Rotation(CTransform::STATE_RIGHT, m_fRotX);

	UpdateData(FALSE);
	*pResult = 0;
}


void CGimmickMenu::OnDeltaposRotY(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CGimmick* pObj = dynamic_cast<CGimmick*>(m_pObjectMgr->Get_Object(CObject_Manager::GIMMICK, m_GimmickList.GetCurSel()));
	if (pObj == nullptr)
		return;

	if (pNMUpDown->iDelta < 0)
	{
		++m_fRotY;
		if (m_fRotY > 360)
			m_fRotY = 0;
	}
	else
	{
		if (m_fRotY <= 0)
			m_fRotY = 359;
		else
			--m_fRotY;
	}

	pObj->m_pTransformCom->SetUp_Rotation(CTransform::STATE_UP, m_fRotY);

	UpdateData(FALSE);
	*pResult = 0;
}


void CGimmickMenu::OnDeltaposRotZ(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CGimmick* pObj = dynamic_cast<CGimmick*>(m_pObjectMgr->Get_Object(CObject_Manager::GIMMICK, m_GimmickList.GetCurSel()));
	if (pObj == nullptr)
		return;

	if (pNMUpDown->iDelta < 0)
	{
		++m_fRotZ;
		if (m_fRotZ > 360)
			m_fRotZ = 0;
	}
	else
	{
		if (m_fRotZ <= 0)
			m_fRotZ = 359;
		else
			--m_fRotZ;
	}

	pObj->m_pTransformCom->SetUp_Rotation(CTransform::STATE_LOOK, m_fRotZ);

	UpdateData(FALSE);
	*pResult = 0;
}


void CGimmickMenu::OnDeltaposScaleX(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CGimmick* pObj = dynamic_cast<CGimmick*>(m_pObjectMgr->Get_Object(CObject_Manager::GIMMICK, m_GimmickList.GetCurSel()));
	if (pObj == nullptr)
		return;

	if (pNMUpDown->iDelta < 0.f)
		m_fScaleX += 0.5f;
	else
		m_fScaleX -= 0.5f;

	pObj->m_pTransformCom->SetUp_Scale(_vec3(m_fScaleX, m_fScaleY, m_fScaleZ));

	UpdateData(FALSE);
	*pResult = 0;
}


void CGimmickMenu::OnDeltaposScaleY(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CGimmick* pObj = dynamic_cast<CGimmick*>(m_pObjectMgr->Get_Object(CObject_Manager::GIMMICK, m_GimmickList.GetCurSel()));
	if (pObj == nullptr)
		return;

	if (pNMUpDown->iDelta < 0.f)
		m_fScaleY += 0.5f;
	else
		m_fScaleY -= 0.5f;

	pObj->m_pTransformCom->SetUp_Scale(_vec3(m_fScaleX, m_fScaleY, m_fScaleZ));

	UpdateData(FALSE);
	*pResult = 0;
}


void CGimmickMenu::OnDeltaposScaleZ(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CGimmick* pObj = dynamic_cast<CGimmick*>(m_pObjectMgr->Get_Object(CObject_Manager::GIMMICK, m_GimmickList.GetCurSel()));
	if (pObj == nullptr)
		return;

	if (pNMUpDown->iDelta < 0.f)
		m_fScaleZ += 0.5f;
	else
		m_fScaleZ -= 0.5f;

	pObj->m_pTransformCom->SetUp_Scale(_vec3(m_fScaleX, m_fScaleY, m_fScaleZ));

	UpdateData(FALSE);
	*pResult = 0;
}

CString CGimmickMenu::GetObjectPath()
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

	strFilePath = L"../../Client/Bin/Resources/" + strFilePath;
	return strFilePath;
}

CString CGimmickMenu::GetObjectName()
{
	HTREEITEM hItem = m_MeshList.GetSelectedItem();

	return m_MeshList.GetItemText(hItem);
}


void CGimmickMenu::OnDeltaposSpinTiggerX(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CTrigger* pObj = dynamic_cast<CTrigger*>(m_pObjectMgr->Get_Object(CObject_Manager::TRIGGER, m_TriggerList.GetCurSel()));
	if (pObj == nullptr)
		return;

	float fAddValue = 0.1f;

	if (pNMUpDown->iDelta < 0.f)
		m_fTriggerX += fAddValue;
	else
		m_fTriggerX -= fAddValue;

	pObj->m_pTransformCom->Set_State(CTransform::STATE_POSITION, _vec3(m_fTriggerX, m_fTriggerY, m_fTriggerZ));

	UpdateData(FALSE);
	*pResult = 0;
}


void CGimmickMenu::OnDeltaposSpinTiggerY(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CTrigger* pObj = dynamic_cast<CTrigger*>(m_pObjectMgr->Get_Object(CObject_Manager::TRIGGER, m_TriggerList.GetCurSel()));
	if (pObj == nullptr)
		return;
	float fAddValue = 0.1f;

	if (pNMUpDown->iDelta < 0.f)
		m_fTriggerY += fAddValue;
	else
		m_fTriggerY -= fAddValue;

	pObj->m_pTransformCom->Set_State(CTransform::STATE_POSITION, _vec3(m_fTriggerX, m_fTriggerY, m_fTriggerZ));

	UpdateData(FALSE);
	*pResult = 0;
}


void CGimmickMenu::OnDeltaposSpinTiggerZ(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CTrigger* pObj = dynamic_cast<CTrigger*>(m_pObjectMgr->Get_Object(CObject_Manager::TRIGGER, m_TriggerList.GetCurSel()));
	if (pObj == nullptr)
		return;
	float fAddValue = 0.1f;

	if (pNMUpDown->iDelta < 0.f)
		m_fTriggerZ += fAddValue;
	else
		m_fTriggerZ -= fAddValue;

	pObj->m_pTransformCom->Set_State(CTransform::STATE_POSITION, _vec3(m_fTriggerX, m_fTriggerY, m_fTriggerZ));

	UpdateData(FALSE);
	*pResult = 0;
}


void CGimmickMenu::OnDeltaposSpinTriggerSize(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CTrigger* pObj = dynamic_cast<CTrigger*>(m_pObjectMgr->Get_Object(CObject_Manager::TRIGGER, m_TriggerList.GetCurSel()));
	if (pObj == nullptr)
		return;

	if (pNMUpDown->iDelta < 0.f)
		m_fTriggerSize += 0.1f;
	else
	{
		if(m_fTriggerSize > 0.f)
			m_fTriggerSize -= 0.1f;
	}

	pObj->Change_Radius(m_fTriggerSize);

	UpdateData(FALSE);
	*pResult = 0;
}


void CGimmickMenu::OnBnClickedAdjust()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	CGimmick* pObj = dynamic_cast<CGimmick*>(m_pObjectMgr->Get_Object(CObject_Manager::GIMMICK, m_GimmickList.GetCurSel()));
	if (pObj == nullptr)
		return;

	pObj->m_pTransformCom->Set_State(CTransform::STATE_POSITION, _vec3(m_fPosX, m_fPosY, m_fPosZ));
	pObj->m_pTransformCom->SetUp_Scale(_vec3(m_fScaleX, m_fScaleY, m_fScaleZ));
	pObj->m_pTransformCom->SetUp_Rotation(CTransform::STATE_RIGHT, m_fRotX);
	pObj->m_pTransformCom->SetUp_Rotation(CTransform::STATE_UP, m_fRotY);
	pObj->m_pTransformCom->SetUp_Rotation(CTransform::STATE_LOOK, m_fRotZ);
	pObj->m_pTransformCom->m_vRotation = _vec3(m_fRotX, m_fRotY, m_fRotZ);
	pObj->m_iType = m_GimmickType.GetCurSel();

	UpdateData(FALSE);
}


void CGimmickMenu::OnBnClickedDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	int i = m_GimmickList.GetCurSel();
	if (i == -1)
		return;

	if (FAILED(m_pObjectMgr->Delete_Object(CObject_Manager::GIMMICK, i)))
		return;

	m_GimmickList.DeleteString(i);

	if (i >= 0)
		m_GimmickList.SetCurSel(i);


	m_fPosX = 0.f;
	m_fPosY = 0.f;
	m_fPosZ = 0.f;

	m_fScaleX = 1.f;
	m_fScaleY = 1.f;
	m_fScaleZ = 1.f;

	m_fRotX = 0.f;
	m_fRotY = 0.f;
	m_fRotZ = 0.f;

	m_GimmickType.SetCurSel(0);

	UpdateData(FALSE);
}


void CGimmickMenu::OnBnClickedAdjustAnimation()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CGimmick* pObj = dynamic_cast<CGimmick*>(m_pObjectMgr->Get_Object(CObject_Manager::GIMMICK, m_GimmickList.GetCurSel()));
	if (pObj == nullptr)
		return;

	pObj->Set_Animation(m_iAnimationKey);

	UpdateData(FALSE);
}


void CGimmickMenu::OnLbnSelchangeGimmickList()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CGimmick* pObj = dynamic_cast<CGimmick*>(m_pObjectMgr->Get_Object(CObject_Manager::GIMMICK, m_GimmickList.GetCurSel()));
	if (pObj == nullptr)
		return;

	_vec3 vPos = pObj->m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	m_fPosX = vPos.x;
	m_fPosY = vPos.y;
	m_fPosZ = vPos.z;

	m_fScaleX = pObj->m_pTransformCom->Get_Scale(CTransform::STATE_RIGHT);
	m_fScaleY = pObj->m_pTransformCom->Get_Scale(CTransform::STATE_UP);
	m_fScaleZ = pObj->m_pTransformCom->Get_Scale(CTransform::STATE_LOOK);

	m_fRotX = pObj->m_pTransformCom->m_vRotation.x;
	m_fRotY = pObj->m_pTransformCom->m_vRotation.y;
	m_fRotZ = pObj->m_pTransformCom->m_vRotation.z;

	m_pObjectMgr->Reset_ShaderPath(CObject_Manager::GIMMICK, 1);
	pObj->m_iShaderPass = 2;

	m_iOption = pObj->m_iOption;
	m_iAnimationKey = pObj->m_iAnimation;

	m_GimmickType.SetCurSel(pObj->m_iType);

	UpdateData(FALSE);
}


void CGimmickMenu::OnCbnSelchangeGimmickType()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CGimmick* pObj = dynamic_cast<CGimmick*>(m_pObjectMgr->Get_Object(CObject_Manager::GIMMICK, m_GimmickList.GetCurSel()));
	if (pObj == nullptr)
		return;

	pObj->m_iType = m_GimmickType.GetCurSel();

	UpdateData(FALSE);
}

void CGimmickMenu::OnBnClickedAddTrigger()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	if (m_strTriggerKey == L"")
	{
		AfxMessageBox(L"Set Trigger Key");
		return;
	}

	CTrigger* pTrigger = CTrigger::Create(CGraphic_Device::GetInstance()->Get_GraphicDev(), _vec3(0.f, 0.f, 0.f), 1.f);
	if (pTrigger == nullptr)
		return;

	m_pObjectMgr->Add_GameObject(CObject_Manager::TRIGGER, pTrigger);

	CString strTriggerKey = L"";

	if (m_GimmickList.GetCurSel() != -1)
	{
		CGimmick* pGimmick = dynamic_cast<CGimmick*>(m_pObjectMgr->Get_Object(CObject_Manager::GIMMICK, m_GimmickList.GetCurSel()));
		if (pGimmick == nullptr)
			return;
		strTriggerKey += pGimmick->m_strFileName + L"_";
	}

	strTriggerKey += m_strTriggerKey;
	pTrigger->m_strTriggerKey = strTriggerKey;
	m_TriggerList.AddString(strTriggerKey);

	UpdateData(FALSE);
}


void CGimmickMenu::OnEnSetfocusTriggerKey()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CInput_Device::GetInstance()->m_bLock_KeyBoard = true;
}


void CGimmickMenu::OnEnKillfocusTriggerKey()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CInput_Device::GetInstance()->m_bLock_KeyBoard = false;
}


void CGimmickMenu::OnBnClickedDeleteTrigger()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	int i = m_TriggerList.GetCurSel();
	if (i == -1)
		return;

	if (FAILED(m_pObjectMgr->Delete_Object(CObject_Manager::TRIGGER, i)))
		return;

	m_TriggerList.DeleteString(i);

	if (i >= 0)
		m_TriggerList.SetCurSel(i);

	m_strTriggerKey = L"";

	m_fTriggerX = 0.f;
	m_fTriggerY = 0.f;
	m_fTriggerZ = 0.f;

	m_fTriggerSize = 1.f;

	UpdateData(FALSE);
}


void CGimmickMenu::OnBnClickedAdjustKey()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	int i = m_TriggerList.GetCurSel();
	if (i == -1)
		return;

	CTrigger* pTrigger = dynamic_cast<CTrigger*>(m_pObjectMgr->Get_Object(CObject_Manager::TRIGGER, m_TriggerList.GetCurSel()));
	if (pTrigger == nullptr)
		return;

	if (m_strTriggerKey == L"")
	{
		AfxMessageBox(L"Set Trigger Key");
		return;
	}

	m_TriggerList.DeleteString(i);
	m_TriggerList.InsertString(i, m_strTriggerKey);

	pTrigger->m_strTriggerKey = m_strTriggerKey;

	UpdateData(FALSE);
}


void CGimmickMenu::OnLbnSelchangeTriggerList()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CTrigger* pObj = dynamic_cast<CTrigger*>(m_pObjectMgr->Get_Object(CObject_Manager::TRIGGER, m_TriggerList.GetCurSel()));
	if (pObj == nullptr)
		return;

	_vec3 vPos = pObj->m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	m_fTriggerX = vPos.x;
	m_fTriggerY = vPos.y;
	m_fTriggerZ = vPos.z;

	m_fTriggerSize = pObj->m_fRadius;

	m_strTriggerKey = pObj->m_strTriggerKey;

	for (int i = 0; i < m_TriggerList.GetCount(); ++i)
	{
		dynamic_cast<CTrigger*>(m_pObjectMgr->Get_Object(CObject_Manager::TRIGGER, i))->m_Color = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);
	}

	pObj->m_Color = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);

	UpdateData(FALSE);
}


void CGimmickMenu::OnBnClickedReset()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int i = AfxMessageBox(L"Reset All Contents?", MB_YESNO | MB_ICONSTOP);

	if (i == IDYES)
	{
		UpdateData(TRUE);
		m_pObjectMgr->Reset_Content(CObject_Manager::GIMMICK);

		m_GimmickList.ResetContent();

		m_fPosX = 0.f;
		m_fPosY = 0.f;
		m_fPosZ = 0.f;

		m_fScaleX = 1.f;
		m_fScaleY = 1.f;
		m_fScaleZ = 1.f;

		m_fRotX = 0.f;
		m_fRotY = 0.f;
		m_fRotZ = 0.f;

		m_GimmickType.SetCurSel(0);
		UpdateData(FALSE);
	}
}


void CGimmickMenu::OnBnClickedSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog Dlg(
		FALSE,
		L"dat",
		L"",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Data Files(*.dat)|*.dat|Text Files(*.txt)|*.txt||",
		this);

	TCHAR szCurPath[256] = L"";
	GetCurrentDirectory(256, szCurPath);
	PathRemoveFileSpec(szCurPath);
	PathRemoveFileSpec(szCurPath);
	lstrcat(szCurPath, L"\\Client\\Bin\\Data\\Gimmick");

	Dlg.m_ofn.lpstrInitialDir = szCurPath;

	if (IDOK == Dlg.DoModal())
	{
		dynamic_cast<CMouse*>(m_pObjectMgr->Get_Object(CObject_Manager::MOUSE, 0))->m_bClick = false;

		CString path = Dlg.GetPathName();

		std::locale::global(std::locale("Korean"));
#ifdef _UNICODE
		wofstream fout;
#else
		ofstream fout;
#endif // _UNICODE

		fout.open(path.GetString());

		if (fout.fail())
			return;

		for (auto& pObj : m_pObjectMgr->m_GameObjects[CObject_Manager::GIMMICK])
		{
			CTransform* pTransform = dynamic_cast<CGimmick*>(pObj)->m_pTransformCom;
			fout << dynamic_cast<CGimmick*>(pObj)->m_strFilePath.GetString() << endl;
			fout << dynamic_cast<CGimmick*>(pObj)->m_strFileName.GetString() << endl;
			fout << pTransform->Get_State(CTransform::STATE_POSITION).x << " "
				<< pTransform->Get_State(CTransform::STATE_POSITION).y << " "
				<< pTransform->Get_State(CTransform::STATE_POSITION).z << " "
				<< pTransform->Get_Scale(CTransform::STATE_RIGHT) << " "
				<< pTransform->Get_Scale(CTransform::STATE_UP) << " "
				<< pTransform->Get_Scale(CTransform::STATE_LOOK) << " "
				<< pTransform->m_vRotation.x << " "
				<< pTransform->m_vRotation.y << " "
				<< pTransform->m_vRotation.z << " "
				<< (_uint)dynamic_cast<CGimmick*>(pObj)->m_iType << " "
				<< dynamic_cast<CGimmick*>(pObj)->m_iAnimation << " "
				<< dynamic_cast<CGimmick*>(pObj)->m_iOption <<
				endl;
		}

		fout.close();
		dynamic_cast<CMouse*>(m_pObjectMgr->Get_Object(CObject_Manager::MOUSE, 0))->m_bClick = true;
	}
}


void CGimmickMenu::OnBnClickedLoad()
{
	CFileDialog Dlg(
		TRUE,
		L"dat",
		L"",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Data Files(*.dat)|*.dat|Text Files(*.txt)|*.txt||",
		this);

	TCHAR szCurPath[256] = L"";
	GetCurrentDirectory(256, szCurPath);
	PathRemoveFileSpec(szCurPath);
	lstrcat(szCurPath, L"\\Client\\Bin\\Data\\Gimmick");

	Dlg.m_ofn.lpstrInitialDir = szCurPath;

	if (Dlg.DoModal() == IDOK)
	{
		dynamic_cast<CMouse*>(m_pObjectMgr->Get_Object(CObject_Manager::MOUSE, 0))->m_bClick = false;

		CString path = Dlg.GetPathName();

		std::locale::global(std::locale("Korean"));
#ifdef _UNICODE
		wifstream fin;
#else
		wifstream fin;
#endif // _UNICODE

		fin.open(path.GetString());

		if (fin.fail())
			return;

		m_GimmickList.ResetContent();

		if (FAILED(m_pObjectMgr->Reset_Content(CObject_Manager::GIMMICK)))
			return;

		wstring strPath, strName;
		_vec3 vPos, vScale, vAngle;
		_uint eType, iAnimation, iOption = 0;
		LPDIRECT3DDEVICE9 pGraphic_Device = CGraphic_Device::GetInstance()->Get_GraphicDev();
		CGimmick* pObj = nullptr;

		while (true)
		{
			fin >> strPath >> strName >>  vPos.x >> vPos.y >> vPos.z >> vScale.x >> vScale.y >> vScale.z >> vAngle.x >> vAngle.y >> vAngle.z >> eType >> iAnimation >> iOption;

			if (fin.eof())
				break;

			pObj = CGimmick::Create(pGraphic_Device, strPath.c_str(), strName.c_str(), vPos, eType, iOption);
			if (pObj == nullptr)
				break;
			pObj->m_pTransformCom->SetUp_Scale(vScale);
			pObj->m_pTransformCom->SetUp_Rotation(CTransform::STATE_RIGHT, vAngle.x);
			pObj->m_pTransformCom->SetUp_Rotation(CTransform::STATE_UP, vAngle.y);
			pObj->m_pTransformCom->SetUp_Rotation(CTransform::STATE_LOOK, vAngle.z);
			pObj->Set_Animation(iAnimation);

			if (FAILED(m_pObjectMgr->Add_GameObject(CObject_Manager::GIMMICK, pObj)))
				break;

			m_GimmickList.AddString(CString(strName.c_str()));
		}

		fin.close();
		dynamic_cast<CMouse*>(m_pObjectMgr->Get_Object(CObject_Manager::MOUSE, 0))->m_bClick = true;
	}

	return;
}


void CGimmickMenu::OnBnClickedSaveTrigger()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog Dlg(
		FALSE,
		L"dat",
		L"",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Data Files(*.dat)|*.dat|Text Files(*.txt)|*.txt||",
		this);

	TCHAR szCurPath[256] = L"";
	GetCurrentDirectory(256, szCurPath);
	PathRemoveFileSpec(szCurPath);
	PathRemoveFileSpec(szCurPath);
	lstrcat(szCurPath, L"\\Client\\Bin\\Data\\Trigger");

	Dlg.m_ofn.lpstrInitialDir = szCurPath;

	if (IDOK == Dlg.DoModal())
	{
		dynamic_cast<CMouse*>(m_pObjectMgr->Get_Object(CObject_Manager::MOUSE, 0))->m_bClick = false;

		CString path = Dlg.GetPathName();

		std::locale::global(std::locale("Korean"));
#ifdef _UNICODE
		wofstream fout;
#else
		ofstream fout;
#endif // _UNICODE

		fout.open(path.GetString());

		if (fout.fail())
			return;

		for (auto& pObj : m_pObjectMgr->m_GameObjects[CObject_Manager::TRIGGER])
		{
			CTransform* pTransform = dynamic_cast<CTrigger*>(pObj)->m_pTransformCom;
			fout << dynamic_cast<CTrigger*>(pObj)->m_strTriggerKey.GetString() << endl;
			fout << pTransform->Get_State(CTransform::STATE_POSITION).x << " "
				<< pTransform->Get_State(CTransform::STATE_POSITION).y << " "
				<< pTransform->Get_State(CTransform::STATE_POSITION).z << " "
				<< dynamic_cast<CTrigger*>(pObj)->m_fRadius <<
				endl;
		}

		fout.close();
		dynamic_cast<CMouse*>(m_pObjectMgr->Get_Object(CObject_Manager::MOUSE, 0))->m_bClick = true;
	}
}


void CGimmickMenu::OnBnClickedLoadTrigger()
{
	CFileDialog Dlg(
		TRUE,
		L"dat",
		L"",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Data Files(*.dat)|*.dat|Text Files(*.txt)|*.txt||",
		this);

	TCHAR szCurPath[256] = L"";
	GetCurrentDirectory(256, szCurPath);
	PathRemoveFileSpec(szCurPath);
	lstrcat(szCurPath, L"\\Client\\Bin\\Data\\Trigger");

	Dlg.m_ofn.lpstrInitialDir = szCurPath;

	if (Dlg.DoModal() == IDOK)
	{
		dynamic_cast<CMouse*>(m_pObjectMgr->Get_Object(CObject_Manager::MOUSE, 0))->m_bClick = false;

		CString path = Dlg.GetPathName();

		std::locale::global(std::locale("Korean"));
#ifdef _UNICODE
		wifstream fin;
#else
		wifstream fin;
#endif // _UNICODE

		fin.open(path.GetString());

		if (fin.fail())
			return;

		m_TriggerList.ResetContent();

		if (FAILED(m_pObjectMgr->Reset_Content(CObject_Manager::TRIGGER)))
			return;

		wstring strName;
		_vec3 vPos;
		_float fSize;
		LPDIRECT3DDEVICE9 pGraphic_Device = CGraphic_Device::GetInstance()->Get_GraphicDev();
		CTrigger* pObj = nullptr;

		while (true)
		{
			fin >> strName >> vPos.x >> vPos.y >> vPos.z >> fSize;

			if (fin.eof())
				break;

			pObj = CTrigger::Create(pGraphic_Device, vPos, fSize);
			if (pObj == nullptr)
				break;
			pObj->m_fRadius = fSize;

			if (FAILED(m_pObjectMgr->Add_GameObject(CObject_Manager::TRIGGER, pObj)))
				break;

			m_TriggerList.AddString(CString(strName.c_str()));
		}

		fin.close();
		dynamic_cast<CMouse*>(m_pObjectMgr->Get_Object(CObject_Manager::MOUSE, 0))->m_bClick = true;
	}

	return;
}


void CGimmickMenu::OnBnClickedSaveTriggerInline()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog Dlg(
		FALSE,
		L"ini",
		L"",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Inline Files(*.ini)|*.ini|Data Files(*.dat)|*.dat||",
		this);

	TCHAR szCurPath[256] = L"";
	GetCurrentDirectory(256, szCurPath);
	PathRemoveFileSpec(szCurPath);
	PathRemoveFileSpec(szCurPath);
	lstrcat(szCurPath, L"\\Client\\Bin\\Data\\Trigger");

	Dlg.m_ofn.lpstrInitialDir = szCurPath;

	if (IDOK == Dlg.DoModal())
	{
		dynamic_cast<CMouse*>(m_pObjectMgr->Get_Object(CObject_Manager::MOUSE, 0))->m_bClick = false;

		CString path = Dlg.GetPathName();

		std::locale::global(std::locale("Korean"));
#ifdef _UNICODE
		wofstream fout;
#else
		ofstream fout;
#endif // _UNICODE

		fout.open(path.GetString());

		if (fout.fail())
			return;

		for (auto& pObj : m_pObjectMgr->m_GameObjects[CObject_Manager::TRIGGER])
		{
			CTransform* pTransform = dynamic_cast<CTrigger*>(pObj)->m_pTransformCom;

			fout << L"[" << dynamic_cast<CTrigger*>(pObj)->m_strTriggerKey.GetString() << L"]" << endl;
			fout << L"PosX=" << pTransform->Get_State(CTransform::STATE_POSITION).x << endl;
			fout << L"PosY=" << pTransform->Get_State(CTransform::STATE_POSITION).y << endl;
			fout << L"PosZ=" << pTransform->Get_State(CTransform::STATE_POSITION).z << endl;
			fout << L"Radius=" << dynamic_cast<CTrigger*>(pObj)->m_fRadius * 2 << endl;
			fout << endl;
		}

		fout.close();
		dynamic_cast<CMouse*>(m_pObjectMgr->Get_Object(CObject_Manager::MOUSE, 0))->m_bClick = true;
	}
}


void CGimmickMenu::OnBnClickedResetTrigger()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int i = AfxMessageBox(L"Reset All Triggers?", MB_YESNO | MB_ICONSTOP);

	if (i == IDYES)
	{
		m_pObjectMgr->Reset_Content(CObject_Manager::TRIGGER);

		UpdateData(TRUE);

		m_TriggerList.ResetContent();

		m_fTriggerX = 0.f;
		m_fTriggerY = 0.f;
		m_fTriggerZ = 0.f;

		m_fTriggerSize = 1.f;

		m_strTriggerKey = L"";

		UpdateData(FALSE);
	}

}


void CGimmickMenu::OnBnClickedAdjustOption()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CGimmick* pObj = dynamic_cast<CGimmick*>(m_pObjectMgr->Get_Object(CObject_Manager::GIMMICK, m_GimmickList.GetCurSel()));
	if (pObj == nullptr)
		return;

	pObj->m_iOption = m_iOption;

	UpdateData(FALSE);
}


void CGimmickMenu::OnBnClickedSaveInline()
{
	if (m_GimmickList.GetCurSel() == -1)
	{
		AfxMessageBox(L"Select Gimmick");
		return;
	}

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog Dlg(
		FALSE,
		L"ini",
		L"",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Inline Files(*.ini)|*.ini|Data Files(*.dat)|*.dat||",
		this);

	TCHAR szCurPath[256] = L"";
	GetCurrentDirectory(256, szCurPath);
	PathRemoveFileSpec(szCurPath);
	PathRemoveFileSpec(szCurPath);
	lstrcat(szCurPath, L"\\Client\\Bin\\Data\\Gimmick");

	Dlg.m_ofn.lpstrInitialDir = szCurPath;

	if (IDOK == Dlg.DoModal())
	{
		dynamic_cast<CMouse*>(m_pObjectMgr->Get_Object(CObject_Manager::MOUSE, 0))->m_bClick = false;

		CGimmick * pObj = dynamic_cast<CGimmick*>(m_pObjectMgr->Get_Object(CObject_Manager::GIMMICK, m_GimmickList.GetCurSel()));
		if (pObj == nullptr)
			return;

		CString path = Dlg.GetPathName();

		std::locale::global(std::locale("Korean"));
#ifdef _UNICODE
		wofstream fout;
#else
		ofstream fout;
#endif // _UNICODE

		fout.open(path.GetString());

		if (fout.fail())
			return;

		CTransform* pTransform = dynamic_cast<CGimmick*>(pObj)->m_pTransformCom;

		fout << L"[Mesh]" << endl;
		fout << L"MeshName=" << pObj->m_strFileName.GetString() << endl;
		fout << L"MeshPath=" << pObj->m_strFilePath.GetString() << endl;
		fout << endl;
		fout << L"[Position]" << endl;
		fout << L"X=" << pTransform->Get_State(CTransform::STATE_POSITION).x << endl;
		fout << L"Y=" << pTransform->Get_State(CTransform::STATE_POSITION).y << endl;
		fout << L"Z=" << pTransform->Get_State(CTransform::STATE_POSITION).z << endl;
		fout << endl;
		fout << L"[Scale]" << endl;
		fout << L"X=" << pTransform->Get_Scale(CTransform::STATE_RIGHT) << endl;
		fout << L"Y=" << pTransform->Get_Scale(CTransform::STATE_UP) << endl;
		fout << L"Z=" << pTransform->Get_Scale(CTransform::STATE_LOOK) << endl;
		fout << endl;
		fout << L"[Rotation]" << endl;
		fout << L"X=" << pTransform->m_vRotation.x << endl;
		fout << L"Y=" << pTransform->m_vRotation.y << endl;
		fout << L"Z=" << pTransform->m_vRotation.z << endl;
		fout << endl;
		fout << L"[Option]" << endl;
		fout << L"Type=" << pObj->m_iType << endl;
		fout << L"Animation=" << pObj->m_iAnimation << endl;
		fout << L"Option=" << pObj->m_iOption << endl;

		fout.close();
		dynamic_cast<CMouse*>(m_pObjectMgr->Get_Object(CObject_Manager::MOUSE, 0))->m_bClick = true;
	}
}


void CGimmickMenu::OnBnClickedSaveAllInline()
{
	if (m_GimmickList.GetCount() == 0)
	{
		AfxMessageBox(L"Gimmick List is Empty");
		return;
	}

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog Dlg(
		FALSE,
		L"ini",
		L"GimmickName",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Inline Files(*.ini)|*.ini|Data Files(*.dat)|*.dat||",
		this);

	TCHAR szCurPath[256] = L"";
	GetCurrentDirectory(256, szCurPath);
	PathRemoveFileSpec(szCurPath);
	PathRemoveFileSpec(szCurPath);
	lstrcat(szCurPath, L"\\Client\\Bin\\Data\\Gimmick");

	Dlg.m_ofn.lpstrInitialDir = szCurPath;

	if (IDOK == Dlg.DoModal())
	{
		dynamic_cast<CMouse*>(m_pObjectMgr->Get_Object(CObject_Manager::MOUSE, 0))->m_bClick = false;

		CString path = Dlg.GetPathName();

		int i = path.ReverseFind('\\');
		path = path.Left(i);

		for (auto& pOriginObj : m_pObjectMgr->m_GameObjects[CObject_Manager::GIMMICK])
		{
			CGimmick* pObj = dynamic_cast<CGimmick*>(pOriginObj);

			std::locale::global(std::locale("Korean"));
#ifdef _UNICODE
			wofstream fout;
#else
			ofstream fout;
#endif // _UNICODE

			CString strName = pObj->m_strFileName;

			int j = strName.ReverseFind('.');
			strName = strName.Left(j);
			strName = L"\\" + strName + L".ini";
			CString strPath = path + strName;

			fout.open(strPath.GetString());

			if (fout.fail())
				return;

			CTransform* pTransform = dynamic_cast<CGimmick*>(pObj)->m_pTransformCom;

			fout << L"[Mesh]" << endl;
			fout << L"MeshName=" << pObj->m_strFileName.GetString() << endl;
			fout << L"MeshPath=" << pObj->m_strFilePath.GetString() << endl;
			fout << endl;
			fout << L"[Position]" << endl;
			fout << L"X=" << pTransform->Get_State(CTransform::STATE_POSITION).x << endl;
			fout << L"Y=" << pTransform->Get_State(CTransform::STATE_POSITION).y << endl;
			fout << L"Z=" << pTransform->Get_State(CTransform::STATE_POSITION).z << endl;
			fout << endl;
			fout << L"[Scale]" << endl;
			fout << L"X=" << pTransform->Get_Scale(CTransform::STATE_RIGHT) << endl;
			fout << L"Y=" << pTransform->Get_Scale(CTransform::STATE_UP) << endl;
			fout << L"Z=" << pTransform->Get_Scale(CTransform::STATE_LOOK) << endl;
			fout << endl;
			fout << L"[Rotation]" << endl;
			fout << L"X=" << pTransform->m_vRotation.x << endl;
			fout << L"Y=" << pTransform->m_vRotation.y << endl;
			fout << L"Z=" << pTransform->m_vRotation.z << endl;
			fout << endl;
			fout << L"[Option]" << endl;
			fout << L"Type=" << pObj->m_iType << endl;
			fout << L"Animation=" << pObj->m_iAnimation << endl;
			fout << L"Option=" << pObj->m_iOption << endl;

			fout.close();
			dynamic_cast<CMouse*>(m_pObjectMgr->Get_Object(CObject_Manager::MOUSE, 0))->m_bClick = true;
		}
	}
}