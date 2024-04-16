#pragma once
#include "afxwin.h"


// CPathExtract ��ȭ �����Դϴ�.

class CPathExtract : public CDialogEx
{
	DECLARE_DYNAMIC(CPathExtract)

public:
	CPathExtract(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CPathExtract();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PATHEXTRACT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
