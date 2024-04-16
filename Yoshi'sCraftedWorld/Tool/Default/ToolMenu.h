#pragma once
#include "afxcmn.h"

#include "MainMenu.h"
#include "ObjectMenu.h"
#include "GimmickMenu.h"
#include "NavigationMenu.h"
#include "AnimationMenu.h"
#include "ParticleMenu.h"

// CToolMenu 폼 뷰입니다.

class CToolMenu : public CFormView
{
	DECLARE_DYNCREATE(CToolMenu)

protected:
	CToolMenu();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CToolMenu();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TOOLMENU };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();

public:
	afx_msg void OnTcnSelchangeTabCtrl(NMHDR *pNMHDR, LRESULT *pResult);

public:
	CFont m_MainFont;

public:
	CTabCtrl m_TabCtrl;

public:
	CWnd*							m_pWnd;
	CMainMenu*				m_pMain;
	CObjectMenu*			m_pObjects;
	CGimmickMenu*		m_pGimmicks;
	CNavigationMenu*	m_pNavigation;
	CAnimationMenu*	m_pAnimation;
	CParticleMenu*			m_pParticle;
};


