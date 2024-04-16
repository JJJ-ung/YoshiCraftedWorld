#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "afxcolorbutton.h"


// CAnimationMenu ��ȭ �����Դϴ�.

class CUnit;
class CAnimationMenu : public CDialogEx
{
	DECLARE_DYNAMIC(CAnimationMenu)

public:
	CAnimationMenu(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CAnimationMenu();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ANIMATIONMENU };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
