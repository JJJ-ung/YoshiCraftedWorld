#include "stdafx.h"
#include "..\Headers\Camera.h"

IMPLEMENT_SINGLETON(CCamera)

CCamera::CCamera()
{
}

HRESULT CCamera::Ready_Camera(LPDIRECT3DDEVICE9 pGraphic_Device, CAMERADESC tDesc)
{
	m_pGraphic_Device = pGraphic_Device;
	m_pInput_Device = CInput_Device::GetInstance();
	m_CameraDesc = tDesc;

	//if (FAILED(Add_Component()))
	//	return E_FAIL;

	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_CameraDesc.vEye);

	//_vec3	vLook = m_CameraDesc.vAt - m_CameraDesc.vEye;
	//m_pTransformCom->Set_State(CTransform::STATE_LOOK, *D3DXVec3Normalize(&vLook, &vLook));

	//_vec3	vRight = *D3DXVec3Cross(&vRight, &m_CameraDesc.vUp, &vLook);
	//m_pTransformCom->Set_State(CTransform::STATE_RIGHT, *D3DXVec3Normalize(&vRight, &vRight));

	//_vec3	vUp = *D3DXVec3Cross(&vUp, &vLook, &vRight);
	//m_pTransformCom->Set_State(CTransform::STATE_UP, *D3DXVec3Normalize(&vUp, &vUp));

	m_fSpeed = 0.005f;

	return NOERROR;
}

_int CCamera::Update_Camera(_double TimeDelta)
{
	//if (nullptr == m_pTransformCom)
	//	return -1;

	//if (GetKeyState(VK_SHIFT) & 0x8000)
	//	m_pTransformCom->m_State.fSpeedPerSec = 15.f;
	//else
	//	m_pTransformCom->m_State.fSpeedPerSec = 8.f;

	//if (GetKeyState(VK_LEFT) & 0x8000)
	//	m_pTransformCom->Go_Left(0.001f);

	//if (GetKeyState(VK_RIGHT) & 0x8000)
	//	m_pTransformCom->Go_Right(0.001f);

	//if (GetKeyState(VK_UP) & 0x8000)
	//	m_pTransformCom->Go_Straight(0.001f);

	//if (GetKeyState(VK_DOWN) & 0x8000)
	//	m_pTransformCom->Back_Straight(0.001f);

	Key_Input();
	if (m_bMouseFix)
	{
		Mouse_Move();
		POINT	ptMouse{ WINCX >> 1, WINCY >> 1 };
		ClientToScreen(g_hWnd, &ptMouse);
		SetCursorPos(ptMouse.x, ptMouse.y);
	}

	D3DXMatrixLookAtLH(&m_matView, &m_CameraDesc.vEye, &m_CameraDesc.vAt, &m_CameraDesc.vUp);

	return _int();
}

_int CCamera::LateUpdate_Camera(_double TimeDelta)
{
	//if (nullptr == m_pTransformCom ||
	//	nullptr == m_pGraphic_Device)
	//	return -1;

	//_matrix			matTmp;

	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &m_matView);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, D3DXMatrixPerspectiveFovLH(&m_matProj, m_CameraDesc.fFovY, m_CameraDesc.fAspect, m_CameraDesc.fNear, m_CameraDesc.fFar));

	return _int();
}

void CCamera::Key_Input()
{
	D3DXMATRIX	matCamWorld;
	D3DXMatrixInverse(&matCamWorld, 0, &m_matView);

	if (m_pInput_Device->Key_Pressing(KEY_SHIFT))
		m_fSpeed = 0.08f;
	else
		m_fSpeed = 0.03f;

	if (m_pInput_Device->Key_Pressing(KEY_W))
	{
		D3DXVECTOR3	vLook;
		memcpy(&vLook, &matCamWorld.m[2][0], sizeof(D3DXVECTOR3));

		D3DXVECTOR3	vLength = *D3DXVec3Normalize(&vLook, &vLook) * m_fSpeed;

		m_CameraDesc.vEye += vLength;
		m_CameraDesc.vAt += vLength;
	}

	if (m_pInput_Device->Key_Pressing(KEY_S))
	{
		D3DXVECTOR3	vLook;
		memcpy(&vLook, &matCamWorld.m[2][0], sizeof(D3DXVECTOR3));

		D3DXVECTOR3	vLength = *D3DXVec3Normalize(&vLook, &vLook) * m_fSpeed;

		m_CameraDesc.vEye -= vLength;
		m_CameraDesc.vAt -= vLength;
	}

	if (m_pInput_Device->Key_Pressing(KEY_D))
	{
		D3DXVECTOR3	vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(D3DXVECTOR3));

		D3DXVECTOR3	vLength = *D3DXVec3Normalize(&vRight, &vRight) * m_fSpeed;

		m_CameraDesc.vEye += vLength;
		m_CameraDesc.vAt += vLength;
	}

	if (m_pInput_Device->Key_Pressing(KEY_A))
	{
		D3DXVECTOR3	vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(D3DXVECTOR3));

		D3DXVECTOR3	vLength = *D3DXVec3Normalize(&vRight, &vRight) * m_fSpeed;

		m_CameraDesc.vEye -= vLength;
		m_CameraDesc.vAt -= vLength;
	}

	if (m_pInput_Device->Key_Pressing(KEY_Q))
	{
		m_CameraDesc.vEye.y += m_fSpeed;
		m_CameraDesc.vAt.y += m_fSpeed;
	}

	if (m_pInput_Device->Key_Pressing(KEY_E))
	{
		m_CameraDesc.vEye.y -= m_fSpeed;
		m_CameraDesc.vAt.y -= m_fSpeed;
	}

	if (m_pInput_Device->Key_Pressing(KEY_SPACE))
	{
		if (true == m_bClick)
			return;

		m_bClick = true;

		if (true == m_bMouseFix)
			m_bMouseFix = false;
		else
			m_bMouseFix = true;
	}
	else
		m_bClick = false;

	if (false == m_bMouseFix)
		return;
}

void CCamera::Mouse_Move()
{
	if (!m_bMouseFix)
		return;

	D3DXMATRIX	matCamWorld;
	D3DXMatrixInverse(&matCamWorld, 0, &m_matView);

	long	dwMouse = 0;

	if (dwMouse = m_pInput_Device->Get_DIMouseMove(CInput_Device::DIMS_Y))
	{
		D3DXVECTOR3 vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(D3DXVECTOR3));

		D3DXVECTOR3	vLook = m_CameraDesc.vAt - m_CameraDesc.vEye;
		D3DXMATRIX	matAxisRot;

		D3DXMatrixRotationAxis(&matAxisRot, &vRight, D3DXToRadian(dwMouse / 10.f));

		D3DXVec3TransformNormal(&vLook, &vLook, &matAxisRot);

		m_CameraDesc.vAt = m_CameraDesc.vEye + vLook;
	}

	if (dwMouse = m_pInput_Device->Get_DIMouseMove(CInput_Device::DIMS_X))
	{
		D3DXVECTOR3	vUp = D3DXVECTOR3(0.f, 1.f, 0.f);

		D3DXVECTOR3	vLook = m_CameraDesc.vAt - m_CameraDesc.vEye;
		D3DXMATRIX	matAxisRot;

		D3DXMatrixRotationAxis(&matAxisRot, &vUp, D3DXToRadian(dwMouse / 10.f));

		D3DXVec3TransformNormal(&vLook, &vLook, &matAxisRot);

		m_CameraDesc.vAt = m_CameraDesc.vEye + vLook;
	}
}

HRESULT CCamera::Add_Component()
{
	m_pTransformCom = CTransform::Create(m_pGraphic_Device, CTransform::STATE(5.0f, D3DXToRadian(90.0f)));

	if (m_pTransformCom == nullptr)
		return E_FAIL;

	return NOERROR;
}

void CCamera::Free()
{
	Safe_Delete(m_pTransformCom);
}
