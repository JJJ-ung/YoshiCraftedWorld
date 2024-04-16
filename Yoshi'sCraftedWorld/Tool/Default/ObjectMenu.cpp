// ObjectMenu.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "ObjectMenu.h"
#include "afxdialogex.h"
#include "FileInfo.h"
#include "MapObject.h"
#include "Transform.h"
#include "Object_Manager.h"
#include "Graphic_Device.h"
#include "Input_Device.h"
#include "Mouse.h"

// CObjectMenu 대화 상자입니다.

IMPLEMENT_DYNAMIC(CObjectMenu, CDialogEx)

CObjectMenu::CObjectMenu(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_OBJECTMENU, pParent)
	, m_fPosX(0.f)
	, m_fPosY(0.f)
	, m_fPosZ(0.f)
	, m_fRotX(0.f)
	, m_fRotY(0.f)
	, m_fRotZ(0.f)
	, m_fScaleX(0.005f)
	, m_fScaleY(0.005f)
	, m_fScaleZ(0.005f)
{
}

CObjectMenu::~CObjectMenu()
{
}

void CObjectMenu::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO1, m_RenderMode[0]);
	DDX_Control(pDX, IDC_RADIO4, m_RenderMode[1]);
	DDX_Control(pDX, IDC_RADIO5, m_PickMode[0]);
	DDX_Control(pDX, IDC_RADIO6, m_PickMode[1]);
	DDX_Control(pDX, IDC_TREE1, m_MeshList);
	DDX_Control(pDX, IDC_LIST1, m_ObjectList);
	DDX_Text(pDX, IDC_EDIT2, m_fPosX);
	DDX_Text(pDX, IDC_EDIT3, m_fPosY);
	DDX_Text(pDX, IDC_EDIT4, m_fPosZ);
	DDX_Text(pDX, IDC_EDIT6, m_fRotX);
	DDX_Text(pDX, IDC_EDIT10, m_fRotY);
	DDX_Text(pDX, IDC_EDIT11, m_fRotZ);
	DDX_Text(pDX, IDC_EDIT7, m_fScaleX);
	DDX_Text(pDX, IDC_EDIT12, m_fScaleY);
	DDX_Text(pDX, IDC_EDIT13, m_fScaleZ);
	DDX_Control(pDX, IDC_COMBO2, m_CollisionType);
}


BEGIN_MESSAGE_MAP(CObjectMenu, CDialogEx)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, &CObjectMenu::OnDeltaposPosX)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN6, &CObjectMenu::OnDeltaposPosY)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN7, &CObjectMenu::OnDeltaposPosZ)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN8, &CObjectMenu::OnDeltaposRotX)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN9, &CObjectMenu::OnDeltaposRotY)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN10, &CObjectMenu::OnDeltaposRotZ)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN11, &CObjectMenu::OnDeltaposScaleX)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN12, &CObjectMenu::OnDeltaposScaleY)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN13, &CObjectMenu::OnDeltaposScaleZ)
	ON_BN_CLICKED(IDC_RADIO1, &CObjectMenu::OnBnClickedRenderSolid)
	ON_BN_CLICKED(IDC_RADIO4, &CObjectMenu::OnBnClickedRenderWire)
	ON_LBN_SELCHANGE(IDC_LIST1, &CObjectMenu::OnLbnSelchangeObjList)
	ON_BN_CLICKED(IDC_BUTTON1, &CObjectMenu::OnBnClickedAdjust)
	ON_BN_CLICKED(IDC_BUTTON3, &CObjectMenu::OnBnClickedDelete)
	ON_BN_CLICKED(IDC_BUTTON4, &CObjectMenu::OnBnClickedSave)
	ON_BN_CLICKED(IDC_BUTTON5, &CObjectMenu::OnBnClickedLoad)
END_MESSAGE_MAP()


// CObjectMenu 메시지 처리기입니다.


BOOL CObjectMenu::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_RenderMode[0].SetCheck(TRUE);
	m_PickMode[0].SetCheck(TRUE);

	m_CollisionType.AddString(L"No Collision");
	m_CollisionType.AddString(L"Egg Collision - Interact");
	m_CollisionType.AddString(L"Egg Collision - Break");
	m_CollisionType.AddString(L"Player Collision - Default");
	m_CollisionType.AddString(L"Player Collision - One Way Floor");
	m_CollisionType.AddString(L"Item - Coin");
	m_CollisionType.AddString(L"Item - CoinL");
	m_CollisionType.AddString(L"Object - Pipe");
	m_CollisionType.AddString(L"No Culling");

	m_CollisionType.SetCurSel(0);

	CString strFolderPath = CFileInfo::GetFolderPath(L"\\Client\\Bin\\Resources\\");
	CFileInfo::DirRootExtraction(strFolderPath.GetString(), m_MeshList, L".X");

	m_pObjectMgr = CObject_Manager::GetInstance();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CObjectMenu::OnDeltaposPosX(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CMapObject* pObj = dynamic_cast<CMapObject*>(m_pObjectMgr->Get_Object(CObject_Manager::OBJECT, m_ObjectList.GetCurSel()));
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


void CObjectMenu::OnDeltaposPosY(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CMapObject* pObj = dynamic_cast<CMapObject*>(m_pObjectMgr->Get_Object(CObject_Manager::OBJECT, m_ObjectList.GetCurSel()));
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


void CObjectMenu::OnDeltaposPosZ(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CMapObject* pObj = dynamic_cast<CMapObject*>(m_pObjectMgr->Get_Object(CObject_Manager::OBJECT, m_ObjectList.GetCurSel()));
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


void CObjectMenu::OnDeltaposRotX(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CMapObject* pObj = dynamic_cast<CMapObject*>(m_pObjectMgr->Get_Object(CObject_Manager::OBJECT, m_ObjectList.GetCurSel()));
	if (pObj == nullptr)
		return;

	float fAddValue = 1.f;

	if (pNMUpDown->iDelta < 0)
	{
		m_fRotX += fAddValue;
		if (m_fRotX > 360)
			m_fRotX = 0;
	}
	else
	{
		if (m_fRotX <= 0)
			m_fRotX = 359;
		else
			m_fRotX -= fAddValue;
	}

	pObj->m_pTransformCom->SetUp_Rotation(CTransform::STATE_RIGHT, m_fRotX);
	//pObj->m_pTransformCom->SetUp_Rotation(_vec3(1.f, 0.f, 0.f), m_fRotX);

	UpdateData(FALSE);
	*pResult = 0;
}


void CObjectMenu::OnDeltaposRotY(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CMapObject* pObj = dynamic_cast<CMapObject*>(m_pObjectMgr->Get_Object(CObject_Manager::OBJECT, m_ObjectList.GetCurSel()));
	if (pObj == nullptr)
		return;

	float fAddValue = 1.f;

	if (pNMUpDown->iDelta < 0)
	{
		m_fRotY += fAddValue;
		if (m_fRotY > 360)
			m_fRotY = 0;
	}
	else
	{
		if (m_fRotY <= 0)
			m_fRotY = 359;
		else
			m_fRotY -= fAddValue;
	}

	pObj->m_pTransformCom->SetUp_Rotation(CTransform::STATE_UP, m_fRotY);
	//pObj->m_pTransformCom->SetUp_Rotation(_vec3(0.f, 1.f, 0.f), m_fRotX);

	UpdateData(FALSE);
	*pResult = 0;
}


void CObjectMenu::OnDeltaposRotZ(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CMapObject* pObj = dynamic_cast<CMapObject*>(m_pObjectMgr->Get_Object(CObject_Manager::OBJECT, m_ObjectList.GetCurSel()));
	if (pObj == nullptr)
		return;

	float fAddValue = 1.f;

	if (pNMUpDown->iDelta < 0)
	{
		m_fRotZ += fAddValue;
		if (m_fRotZ > 360)
			m_fRotZ = 0;
	}
	else
	{
		if (m_fRotZ <= 0)
			m_fRotZ = 359;
		else
			m_fRotZ -= fAddValue;
	}

	pObj->m_pTransformCom->SetUp_Rotation(CTransform::STATE_LOOK, m_fRotZ);
//	pObj->m_pTransformCom->SetUp_Rotation(_vec3(0.f, 0.f, 1.f), m_fRotX);

	UpdateData(FALSE);
	*pResult = 0;
}


void CObjectMenu::OnDeltaposScaleX(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CMapObject* pObj = dynamic_cast<CMapObject*>(m_pObjectMgr->Get_Object(CObject_Manager::OBJECT, m_ObjectList.GetCurSel()));
	if (pObj == nullptr)
		return;

	if (pNMUpDown->iDelta < 0.f)
		m_fScaleX += 0.01f;
	else
		m_fScaleX -= 0.01f;

	pObj->m_pTransformCom->SetUp_Scale(_vec3(m_fScaleX, m_fScaleY, m_fScaleZ));

	UpdateData(FALSE);
	*pResult = 0;
}


void CObjectMenu::OnDeltaposScaleY(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CMapObject* pObj = dynamic_cast<CMapObject*>(m_pObjectMgr->Get_Object(CObject_Manager::OBJECT, m_ObjectList.GetCurSel()));
	if (pObj == nullptr)
		return;

	if (pNMUpDown->iDelta < 0.f)
		m_fScaleY += 0.01f;
	else
		m_fScaleY -= 0.01f;

	pObj->m_pTransformCom->SetUp_Scale(_vec3(m_fScaleX, m_fScaleY, m_fScaleZ));

	UpdateData(FALSE);
	*pResult = 0;
}


void CObjectMenu::OnDeltaposScaleZ(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CMapObject* pObj = dynamic_cast<CMapObject*>(m_pObjectMgr->Get_Object(CObject_Manager::OBJECT, m_ObjectList.GetCurSel()));
	if (pObj == nullptr)
		return;

	if (pNMUpDown->iDelta < 0.f)
		m_fScaleZ += 0.01f;
	else
		m_fScaleZ -= 0.01f;

	pObj->m_pTransformCom->SetUp_Scale(_vec3(m_fScaleX, m_fScaleY, m_fScaleZ));

	UpdateData(FALSE);
	*pResult = 0;
}

CString CObjectMenu::GetObjectPath()
{
	HTREEITEM hItem = m_MeshList.GetSelectedItem();
	CString strMeshName = m_MeshList.GetItemText(hItem);

	CString strFilePath = L"";

	if (hItem == NULL)
		return strFilePath;
	if (m_MeshList.ItemHasChildren(hItem))
		return strFilePath;

	strFilePath = m_MeshList.GetItemText(hItem);
	while (true)
	{
		if (m_MeshList.GetParentItem(hItem) == NULL)
			break;

		HTREEITEM hParentItem = m_MeshList.GetParentItem(hItem);
		CString strParentPath = m_MeshList.GetItemText(hParentItem);
		strFilePath = strParentPath + L"/" + strFilePath;
		hItem = hParentItem;
	}

	strFilePath = L"../../Client/Bin/Resources/"+ strFilePath;
	// Environment /
	return strFilePath;
}


void CObjectMenu::OnBnClickedRenderSolid()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pObjectMgr->Set_ObjectsWire(false);
}


void CObjectMenu::OnBnClickedRenderWire()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pObjectMgr->Set_ObjectsWire(true);
}


void CObjectMenu::OnLbnSelchangeObjList()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CMapObject* pObj = dynamic_cast<CMapObject*>(m_pObjectMgr->Get_Object(CObject_Manager::OBJECT, m_ObjectList.GetCurSel()));
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

	m_pObjectMgr->Reset_ShaderPath(CObject_Manager::OBJECT, 1);
	pObj->m_iShaderPass = 2;

	m_CollisionType.SetCurSel(pObj->m_eCollisionType);

	UpdateData(FALSE);
}


void CObjectMenu::OnBnClickedAdjust()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CMapObject* pObj = dynamic_cast<CMapObject*>(m_pObjectMgr->Get_Object(CObject_Manager::OBJECT, m_ObjectList.GetCurSel()));
	if (pObj == nullptr)
		return;

	pObj->m_pTransformCom->Set_State(CTransform::STATE_POSITION, _vec3(m_fPosX, m_fPosY, m_fPosZ));
	pObj->m_pTransformCom->SetUp_Scale(_vec3(m_fScaleX, m_fScaleY, m_fScaleZ));
	pObj->m_pTransformCom->SetUp_Rotation(CTransform::STATE_RIGHT, m_fRotX);
	pObj->m_pTransformCom->SetUp_Rotation(CTransform::STATE_UP, m_fRotY);
	pObj->m_pTransformCom->SetUp_Rotation(CTransform::STATE_LOOK, m_fRotZ);
	pObj->m_pTransformCom->m_vRotation = _vec3(m_fRotX, m_fRotY, m_fRotZ);
	pObj->m_eCollisionType = (COLLISION_TYPE)m_CollisionType.GetCurSel();

	UpdateData(FALSE);
}


void CObjectMenu::OnBnClickedDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	int i = m_ObjectList.GetCurSel();
	if (i == -1)
		return;

	if (FAILED(m_pObjectMgr->Delete_Object(CObject_Manager::OBJECT, i)))
		return;

	m_ObjectList.DeleteString(i);

	if (i >= 0)
		m_ObjectList.SetCurSel(i);


	m_fPosX = 0.f;
	m_fPosY = 0.f;
	m_fPosZ = 0.f;

	m_fScaleX = 0.005f;
	m_fScaleY = 0.005f;
	m_fScaleZ = 0.005f;

	m_fRotX = 0.f;
	m_fRotY = 0.f;
	m_fRotZ = 0.f;

	m_CollisionType.SetCurSel(0);

	UpdateData(FALSE);
}


void CObjectMenu::OnBnClickedSave()
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
	lstrcat(szCurPath, L"\\Client\\Bin\\Data\\Object");

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

		for (auto& pObj : m_pObjectMgr->m_GameObjects[CObject_Manager::OBJECT])
		{
			CTransform* pTransform = dynamic_cast<CMapObject*>(pObj)->m_pTransformCom;
			fout << dynamic_cast<CMapObject*>(pObj)->m_strFilePath.GetString() << endl;
			fout << pTransform->Get_State(CTransform::STATE_POSITION).x << " "
				<< pTransform->Get_State(CTransform::STATE_POSITION).y << " "
				<< pTransform->Get_State(CTransform::STATE_POSITION).z << " "
				<< pTransform->Get_Scale(CTransform::STATE_RIGHT) << " "
				<< pTransform->Get_Scale(CTransform::STATE_UP) << " "
				<< pTransform->Get_Scale(CTransform::STATE_LOOK) << " "
				<< pTransform->m_vRotation.x << " " 
				<< pTransform->m_vRotation.y << " "
				<< pTransform->m_vRotation.z << " "
				<< (_uint)dynamic_cast<CMapObject*>(pObj)->m_eCollisionType <<
				endl;
		}

		fout.close();
		dynamic_cast<CMouse*>(m_pObjectMgr->Get_Object(CObject_Manager::MOUSE, 0))->m_bClick = true;
	}
}


void CObjectMenu::OnBnClickedLoad()
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
	lstrcat(szCurPath, L"\\Client\\Bin\\Data\\Object");

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

		m_ObjectList.ResetContent();

		if (FAILED(m_pObjectMgr->Reset_Content(CObject_Manager::OBJECT)))
			return;

		wstring strPath;
		_vec3 vPos, vScale, vAngle;
		_uint eType = 0;
		LPDIRECT3DDEVICE9 pGraphic_Device = CGraphic_Device::GetInstance()->Get_GraphicDev();
		CMapObject* pObj = nullptr;
		
		while (true)
		{
			fin >> strPath >> vPos.x >> vPos.y >> vPos.z >> vScale.x >> vScale.y >> vScale.z >> vAngle.x >> vAngle.y >> vAngle.z >> eType;

			if (fin.eof())
				break;

			pObj = CMapObject::Create(pGraphic_Device, strPath.c_str(), vPos, eType);
			if (pObj == nullptr)
				break;
			pObj->m_pTransformCom->SetUp_Scale(vScale);
			pObj->m_pTransformCom->SetUp_Rotation(CTransform::STATE_RIGHT, vAngle.x);
			pObj->m_pTransformCom->SetUp_Rotation(CTransform::STATE_UP, vAngle.y);
			pObj->m_pTransformCom->SetUp_Rotation(CTransform::STATE_LOOK, vAngle.z);
			//pObj->m_pTransformCom->SetUp_Rotation(_vec3(1.f, 0.f, 0.f), vAngle.x);
			//pObj->m_pTransformCom->SetUp_Rotation(_vec3(0.f, 1.f, 0.f), vAngle.y);
			//pObj->m_pTransformCom->SetUp_Rotation(_vec3(0.f, 0.f, 1.f), vAngle.z);


			if (FAILED(m_pObjectMgr->Add_GameObject(CObject_Manager::OBJECT, pObj)))
				break;

			CString strMeshName = strPath.c_str();
			strMeshName = strMeshName.Right(strMeshName.GetLength() - strMeshName.ReverseFind('/') - 1);
			m_ObjectList.AddString(strMeshName);
		}

		fin.close();
		dynamic_cast<CMouse*>(m_pObjectMgr->Get_Object(CObject_Manager::MOUSE, 0))->m_bClick = true;
	}

	return;
}
