#pragma once
#include "afxwin.h"


// CParticleMenu ��ȭ �����Դϴ�.

class CParticleMenu : public CDialogEx
{
	DECLARE_DYNAMIC(CParticleMenu)

public:
	CParticleMenu(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CParticleMenu();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PARTICLEMENU };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();

public:
	afx_msg void OnBnClickedAddSystem();
	afx_msg void OnBnClickedDeleteSystem();
	afx_msg void OnBnClickedAddEmitter();
	afx_msg void OnBnClickedDeleteEmitter();
	afx_msg void OnBnClickedResetEmitter();
	afx_msg void OnBnClickedCopyEmitter();

public:
	CString m_strParticleSystem;
	CListBox m_ParticleSystemList;
	CString m_strEmitter;
	CListBox m_EmitterList;
};
