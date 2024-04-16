#pragma once

#include "Defines.h"
#include "GameObject.h"

BEGIN(Client)

class CBack_Logo;
class CLoading_Image;
class CLoading_Screen : public CGameObject
{
public:
	enum LOAD_TYPE { LOAD_IN, LOAD_PROGRESS, LOAD_OUT, LOAD_END };

public:
	typedef struct tagLoadingScreen
	{
		tagLoadingScreen() {}
		tagLoadingScreen(SCENEID _eID, SCENEID _eBeforeScene, LOAD_TYPE _eType)
		: eScene(_eID), eBeforeScene(_eBeforeScene), eType(_eType)	{}
	public:
		SCENEID eScene;
		SCENEID eBeforeScene;
		LOAD_TYPE eType;
	}LOADING_SCREEN;

private:
	explicit CLoading_Screen(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CLoading_Screen(const CLoading_Screen& rhs);
	virtual ~CLoading_Screen() = default;

public:
	_bool Get_IsAnimationEnd() { return m_bAnimationEnd; }

public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int Update_GameObject(_double TimeDelta);
	virtual _int LateUpdate_GameObject(_double TimeDelta);
	virtual HRESULT Render_GameObject();

private:
	LOADING_SCREEN m_tInfo;
	_bool m_bAnimationEnd = false;
	_bool m_bCheckAnimation_In = false;
	_bool m_bCheckAnimation_Out = false;
	_bool m_bCheckAnimation_Out_End = false;
	_bool m_bSound = false;

private:
	CLoading_Image* m_pLoadingImage;
	CLoading_Image* m_pLoadingBase[2];
	CBack_Logo* m_pLoadingAnimation;
	system_clock::time_point m_tEnterTime;

private:
	HRESULT Add_Loading_In();
	HRESULT Add_Loading_Progress();
	HRESULT Add_Loading_Out();

public:
	static CGameObject* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject(void* pArg);
	virtual void Free();
};

END