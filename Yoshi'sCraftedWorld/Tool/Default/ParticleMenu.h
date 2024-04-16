#pragma once
#include "afxwin.h"


// CParticleMenu 대화 상자입니다.

class CParticleMenu : public CDialogEx
{
	DECLARE_DYNAMIC(CParticleMenu)

public:
	CParticleMenu(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CParticleMenu();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PARTICLEMENU };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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
