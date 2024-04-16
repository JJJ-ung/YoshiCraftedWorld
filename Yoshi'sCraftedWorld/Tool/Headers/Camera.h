#pragma once
#include "Transform.h"
#include "Input_Device.h"

class CCamera
{
public:
	typedef struct tagCameraDesc
	{
		tagCameraDesc() {}
		tagCameraDesc(_vec3 Eye, _vec3 At, _float Fov)
			:vEye(Eye), vAt(At), fFovY(Fov)
		{
			vUp = _vec3(0.f, 1.f, 0.f);
			fAspect = float(WINCX) / WINCY;
			fNear = 0.1f;
			fFar = 1000.f;
		}
		_vec3	vEye, vAt, vUp;
		_float	fFovY, fAspect, fNear, fFar;
	}CAMERADESC;

	DECLARE_SINGLETON(CCamera)

public:
	CCamera();
	~CCamera() = default;

public:
	HRESULT Ready_Camera(LPDIRECT3DDEVICE9 pGraphic_Device, CAMERADESC tDesc);
	_int Update_Camera(_double TimeDelta);
	_int LateUpdate_Camera(_double TimeDelta);

public:
	void Key_Input();
	void Mouse_Move();

public:
	HRESULT Add_Component();
	void Free();

public:
	LPDIRECT3DDEVICE9		m_pGraphic_Device = nullptr;
	CInput_Device*					m_pInput_Device = nullptr;
	CAMERADESC					m_CameraDesc;
	CTransform*						m_pTransformCom = nullptr;

public:
	bool m_bClick = false;
	bool m_bMouseFix = false;
	float m_fSpeed;

public:
	D3DXMATRIX						m_matView, m_matProj;
};

