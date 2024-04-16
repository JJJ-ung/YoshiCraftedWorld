#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CNavigationMenu 대화 상자입니다.

class CNavigationMenu : public CDialogEx
{
	DECLARE_DYNAMIC(CNavigationMenu)

public:
	CNavigationMenu(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CNavigationMenu();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NAVIGATIONMENU };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();

public:
	afx_msg void OnDeltaposPosX(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposPosY(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposPosZ(NMHDR *pNMHDR, LRESULT *pResult);

public:
	afx_msg void OnBnClickedSetGroupName();

public:
	CButton	m_PickMode[2];

public:
	CComboBox m_NavigationType;
	CString m_strPathType;
	CTreeCtrl m_NavigationList;
	CString m_strGroupTag;

public:
	float m_fPosX;
	float m_fPosY;
	float m_fPosZ;

public:
	CButton m_RenderPlayer;
	CButton m_RenderMonster;
	CButton m_RenderTrain;

};
