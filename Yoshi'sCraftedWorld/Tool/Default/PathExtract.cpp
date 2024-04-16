// PathExtract.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "PathExtract.h"
#include "afxdialogex.h"

#include "FileInfo.h"
#include "Mouse.h"
#include "Object_Manager.h"

// CPathExtract 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPathExtract, CDialogEx)

CPathExtract::CPathExtract(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PATHEXTRACT, pParent)
{

}

CPathExtract::~CPathExtract()
{
}

void CPathExtract::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_FileType);
	DDX_Control(pDX, IDC_LIST1, m_PathList);
}


BEGIN_MESSAGE_MAP(CPathExtract, CDialogEx)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON1, &CPathExtract::OnBnClickedSave)
END_MESSAGE_MAP()


// CPathExtract 메시지 처리기입니다.


BOOL CPathExtract::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_FileType.AddString(L" Mesh | .X ");
	m_FileType.AddString(L" Texture | .png ");
	m_FileType.AddString(L" Texture | .tga ");

	m_FileType.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CPathExtract::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CDialog::OnDropFiles(hDropInfo);

	int iCount = DragQueryFile(hDropInfo, -1, nullptr, 0);

	TCHAR szFullPath[256] = L"";

	m_PathList.ResetContent();

	if (m_FileType.GetCurSel() == 0)
	{
		for (int i = 0; i < iCount; ++i)
		{
			CString strFilePath, strFileName;
			DragQueryFile(hDropInfo, i, szFullPath, 256);
			CFileInfo::DirMeshInfoExtraction(CString(szFullPath), strFilePath, strFileName, m_PathList);
		}
	}

	SetHorizontalScroll();
}


void CPathExtract::SetHorizontalScroll()
{
	int iCX = 0;
	CSize size;
	CString strName = L"";

	CDC* pDC = m_PathList.GetDC();

	for (int i = 0; i < m_PathList.GetCount(); ++i)
	{
		m_PathList.GetText(i, strName);
		size = pDC->GetTextExtent(strName);

		if (iCX < size.cx)
			iCX = size.cx;
	}

	m_PathList.ReleaseDC(pDC);

	if (iCX > m_PathList.GetHorizontalExtent())
	{
		m_PathList.SetHorizontalExtent(iCX);
	}
}

void CPathExtract::OnBnClickedSave()
{
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
		lstrcat(szCurPath, L"\\Client\\Bin\\Data\\PathExtract");

		Dlg.m_ofn.lpstrInitialDir = szCurPath;

		if (IDOK == Dlg.DoModal())
		{
			dynamic_cast<CMouse*>(CObject_Manager::GetInstance()->Get_Object(CObject_Manager::MOUSE, 0))->m_bClick = false;

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

			int iCount = m_PathList.GetCount();

			for (int i = 0; i < iCount; ++i)
			{
				CString strOut = L"";
				m_PathList.GetText(i, strOut);
				fout << strOut.GetString() << endl;
			}

			fout.close();
			dynamic_cast<CMouse*>(CObject_Manager::GetInstance()->Get_Object(CObject_Manager::MOUSE, 0))->m_bClick = true;
		}
	}
}