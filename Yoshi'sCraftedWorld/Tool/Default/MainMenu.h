#pragma once
#include "afxwin.h"
#include "afxcolorbutton.h"
#include "afxcmn.h"

#include "PathExtract.h"

// CMainMenu 대화 상자입니다.

class CMainMenu : public CDialogEx
{
	DECLARE_DYNAMIC(CMainMenu)

public:
	CMainMenu(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMainMenu();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAINMENU };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();

public:
	afx_msg void OnDeltaposFov(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposX(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposZ(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposItv(NMHDR *pNMHDR, LRESULT *pResult);

public:
	afx_msg void OnBnClickedApplyFOV();
	afx_msg void OnBnClickedApplySize();

public:
	afx_msg void OnBnClickedRenderWire();
	afx_msg void OnBnClickedRenderSolid();
	afx_msg void OnBnClickedApplyColor();

public:
	afx_msg void OnBnClickedRenderTerrain();
	afx_msg void OnBnClickedRenderNavigation();
	afx_msg void OnBnClickedRenderObjects();
	afx_msg void OnBnClickedRenderGimmicks();

public:
	afx_msg void OnBnClickedOpenPathExtract();

public:
	UINT m_iFov;
	UINT m_iTerrainX;
	UINT m_iTerrainZ;
	UINT m_iTerrainItv;

public:
	CButton m_TerrainRender[2];
	UINT m_iColorR;
	UINT m_iColorG;
	UINT m_iColorB;

public:
	CButton m_AutoSave;

public:
	CButton m_RenderTerrain;
	CButton m_RenderNavigation;
	CButton m_RenderObject;
	CButton m_RenderGimmicks;

public:
	CPathExtract m_PathExtract;
};
