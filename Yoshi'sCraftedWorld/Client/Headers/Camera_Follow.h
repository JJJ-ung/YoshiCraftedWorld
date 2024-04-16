#pragma once

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
END

BEGIN(Client)

class CCamera_Follow final : public CGameObject
{
public:
	typedef struct tagCameraDesc
	{
		CTransform* pTargetTransform;
		_vec3	vEye, vAt, vUp;
		_float	fFovY, fAspect, fNear, fFar;
		_float fDistance;
	}CAMERADESC;

private:
	explicit CCamera_Follow(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CCamera_Follow(const CCamera_Follow& rhs);
	virtual ~CCamera_Follow() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int Update_GameObject(_double TimeDelta);
	virtual _int LateUpdate_GameObject(_double TimeDelta);
	virtual HRESULT Render_GameObject();

private:
	CAMERADESC		m_CameraDesc;

private:
	CTransform*		m_pTransformCom = nullptr;

private:
	HRESULT Add_Component();

public:
	static CGameObject* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject(void* pArg);
	virtual void Free();
};

END