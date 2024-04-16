#pragma once

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
END

BEGIN(Client)

class CCamera_Free final : public CGameObject
{
public:
	typedef struct tagCameraDesc
	{
		_vec3	vEye, vAt, vUp;
		_float	fFovY, fAspect, fNear, fFar;
	}CAMERADESC;

private:
	explicit CCamera_Free(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CCamera_Free(const CCamera_Free& rhs);
	virtual ~CCamera_Free() = default;

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
	static CGameObject* Create(LPDIRECT3DDEVICE9 pGraphic_Device); // ������ü�� ����.
	virtual CGameObject* Clone_GameObject(void* pArg); // ��������� ��ü�� ����.
	virtual void Free();
};

END