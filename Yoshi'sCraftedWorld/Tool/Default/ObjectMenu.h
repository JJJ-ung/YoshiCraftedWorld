#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CObjectMenu 대화 상자입니다.

class CObject_Manager;
class CObjectMenu : public CDialogEx
{
	DECLARE_DYNAMIC(CObjectMenu)

public:
	CObjectMenu(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CObjectMenu();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OBJECTMENU };
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
	afx_msg void OnDeltaposRotX(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposRotY(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposRotZ(NMHDR *pNMHDR, LRESULT *pResult);

public:
	afx_msg void OnDeltaposScaleX(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposScaleY(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposScaleZ(NMHDR *pNMHDR, LRESULT *pResult);

public:
	afx_msg void OnBnClickedRenderSolid();
	afx_msg void OnBnClickedRenderWire();

public:
	afx_msg void OnLbnSelchangeObjList();
	afx_msg void OnBnClickedAdjust();
	afx_msg void OnBnClickedDelete();

public:
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedLoad();

public:
	CObject_Manager*	m_pObjectMgr = nullptr;

public:
	CString	GetObjectPath();

public:
	CButton	m_RenderMode[2];
	CButton	m_PickMode[2];
	CComboBox m_CollisionType;

public:
	CTreeCtrl m_MeshList;
	CListBox m_ObjectList;

public:
	float m_fPosX;
	float m_fPosY;
	float m_fPosZ;

public:
	float m_fRotX;
	float m_fRotY;
	float m_fRotZ;

public:
	float m_fScaleX;
	float m_fScaleY;
	float m_fScaleZ;
};
