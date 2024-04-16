#pragma once
#include "afxwin.h"


// CPathExtract 대화 상자입니다.

class CPathExtract : public CDialogEx
{
	DECLARE_DYNAMIC(CPathExtract)

public:
	CPathExtract(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPathExtract();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PATHEXTRACT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();

public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedSave();

public:
	void SetHorizontalScroll();

public:
	CComboBox m_FileType;
	CListBox m_PathList;
};
