#pragma once

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
END

BEGIN(Client)

class CCamera_SideScroll final : public CGameObject
{
public:
	typedef struct tagCameraDesc
	{
		CTransform* pTargetTransform;
		_vec2 vTargetViewPos;
		_float fFovY, fDistance;
		_vec3 vMove;
		_vec3 vPlayerOffset, vCameraOffset;
	}CAMERADESC;

private:
	explicit CCamera_SideScroll(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CCamera_SideScroll(const CCamera_SideScroll& rhs);
	virtual ~CCamera_SideScroll() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int Update_GameObject(_double TimeDelta);
	virtual _int LateUpdate_GameObject(_double TimeDelta);
	virtual HRESULT Render_GameObject();

public:
	void Renewal_Target();
	_bool& Get_RenewTarget() { return m_bRenewTarget; }
	_bool& Get_Trigger() { return m_bBridgeTrigger; }
	_bool& Get_Start() { return m_bStart; }
	_bool& Get_End() { return m_bEnd; }
	_bool& Get_EndCam() { return m_bEndCam; }
	_float& Get_Height() { return m_fHeight; }
	_vec3& Get_StartPos() { return m_vStartPos; }
	_bool& Get_CutScene(milliseconds Time) { m_CutSceneTime = Time; return m_bCutSceneStart; }
	system_clock::time_point& Get_EnterTime() { return m_tEnterTime; }

private:
	_float m_fProgress = 0.f;

private:
	CAMERADESC		m_CameraDesc;

private:
	CTransform*		m_pTransformCom = nullptr;
	_bool					m_bRenewTarget = false;
	_float					m_fHeight = 0.1f;
	_vec3					m_vStartPos;
	_vec3					m_vCameraPos;

	_bool					m_bBridgeTrigger = false;
	_bool					m_bStart = true;
	_bool					m_bEnd = false;
	_bool					m_bEndCam = false;
	_float					m_fRotate = 0.f;
	_float					m_fRotate2 = 0.f;

private:
	_bool					m_bCutSceneStart = false;

private:
	system_clock::time_point m_tEnterTime;
	milliseconds m_CutSceneTime;

private:
	HRESULT Add_Component();

public:
	static CGameObject* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject(void* pArg);
	virtual void Free();
};

END