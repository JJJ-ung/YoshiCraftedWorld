#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "afxcolorbutton.h"


// CAnimationMenu 대화 상자입니다.

class CUnit;
class CAnimationMenu : public CDialogEx
{
	DECLARE_DYNAMIC(CAnimationMenu)

public:
	CAnimationMenu(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CAnimationMenu();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ANIMATIONMENU };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();

public:
	afx_msg void OnDeltaposCenterX(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposCenterY(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposCenterZ(NMHDR *pNMHDR, LRESULT *pResult);

public:
	afx_msg void OnNMDblclkMeshList(NMHDR *pNMHDR, LRESULT *pResult);

public:
	CString Get_ObjectPath();

public:
	CTreeCtrl m_MeshList;
	CListBox m_ColliderList;
	CListBox m_BoneList;
	CMFCColorButton m_ColliderColor;

public:
	float m_fRadius;

public:
	float m_fCenterX;
	float m_fCenterY;
	float m_fCenterZ;

public:
	CUnit* m_pCurrentUnit = nullptr;
};
