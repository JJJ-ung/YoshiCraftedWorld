#pragma once

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
END

BEGIN(Client)

class CCamera_Title final : public CGameObject
{
public:
	typedef struct tagCameraDesc
	{
		_vec3	vEye, vAt, vUp;
		_float	fFovY, fAspect, fNear, fFar;
	}CAMERADESC;

private:
	explicit CCamera_Title(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CCamera_Title(const CCamera_Title& rhs);
	virtual ~CCamera_Title() = default;

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
	_float						m_fSpeed = -3.f;
	bool						m_bFront = true;

private:
	HRESULT Add_Component();

public:
	static CGameObject* Create(LPDIRECT3DDEVICE9 pGraphic_Device); // 원형객체를 생성.
	virtual CGameObject* Clone_GameObject(void* pArg); // 실제사용할 객체를 생성.
	virtual void Free();
};

END