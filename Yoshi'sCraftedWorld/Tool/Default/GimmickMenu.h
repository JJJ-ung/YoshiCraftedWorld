#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "Object_Manager.h"

// CGimmickMenu 대화 상자입니다.

class CGimmickMenu : public CDialogEx
{
	DECLARE_DYNAMIC(CGimmickMenu)

public:
	CGimmickMenu(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CGimmickMenu();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GIMMICKMENU };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();

public:
	afx_msg void OnBnClickedAdjust();
	afx_msg void OnBnClickedDelete();
	afx_msg void OnBnClickedReset();

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
	afx_msg void OnDeltaposSpinTiggerX(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinTiggerY(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinTiggerZ(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinTriggerSize(NMHDR *pNMHDR, LRESULT *pResult);

public:
	afx_msg void OnBnClickedAdjustAnimation();
	afx_msg void OnLbnSelchangeGimmickList();
	afx_msg void OnCbnSelchangeGimmickType();

public:
	afx_msg void OnBnClickedAddTrigger();

public:
	afx_msg void OnEnSetfocusTriggerKey();
	afx_msg void OnEnKillfocusTriggerKey();

public:
	afx_msg void OnBnClickedDeleteTrigger();
	afx_msg void OnBnClickedAdjustKey();
	afx_msg void OnLbnSelchangeTriggerList();

public:
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedLoad();
	afx_msg void OnBnClickedSaveTrigger();
	afx_msg void OnBnClickedLoadTrigger();

public:
	CString	GetObjectPath();
	CString	GetObjectName();

public:
	CObject_Manager* m_pObjectMgr = nullptr;

public:
	CButton	m_RenderMode[2];
	CButton	m_PickMode[3];

public:
	CTreeCtrl m_MeshList;
	CListBox m_GimmickList;

public:
	CComboBox m_GimmickType;

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

public:
	UINT m_iAnimationKey;

public:
	CListBox m_TriggerList;

public:
	CString m_strTriggerKey;
	float m_fTriggerX;
	float m_fTriggerY;
	float m_fTriggerZ;
	float m_fTriggerSize;
	afx_msg void OnBnClickedSaveTriggerInline();
	afx_msg void OnBnClickedResetTrigger();
	UINT m_iOption;
	afx_msg void OnBnClickedAdjustOption();
	afx_msg void OnBnClickedSaveInline();
	afx_msg void OnBnClickedSaveAllInline();
};
