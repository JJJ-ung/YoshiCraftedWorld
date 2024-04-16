#include "stdafx.h"
#include "..\Headers\Camera_Free.h"
#include "Management.h"
#include "Input_Device.h"


USING(Client)

CCamera_Free::CCamera_Free(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CCamera_Free::CCamera_Free(const CCamera_Free & rhs)
	: CGameObject(rhs)
{

}

HRESULT CCamera_Free::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CCamera_Free::Ready_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	memcpy(&m_CameraDesc, pArg, sizeof(CAMERADESC));

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_CameraDesc.vEye);

	_vec3	vLook = m_CameraDesc.vAt - m_CameraDesc.vEye;	
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, *D3DXVec3Normalize(&vLook, &vLook));

	_vec3	vRight = *D3DXVec3Cross(&vRight, &m_CameraDesc.vUp, &vLook);
	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, *D3DXVec3Normalize(&vRight, &vRight));

	_vec3	vUp = *D3DXVec3Cross(&vUp, &vLook, &vRight);
	m_pTransformCom->Set_State(CTransform::STATE_UP, *D3DXVec3Normalize(&vUp, &vUp));	

	return NOERROR;
}

_int CCamera_Free::Update_GameObject(_double TimeDelta)
{
	if (nullptr == m_pTransformCom)
		return -1;

	CInput_Device*		pInput_Instance = CInput_Device::GetInstance();
	if (nullptr == pInput_Instance)
		return -1;

	Safe_AddRef(pInput_Instance);

	if (pInput_Instance->Key_Pressing(KEY_A))
		m_pTransformCom->Go_Left(TimeDelta);

	if (pInput_Instance->Key_Pressing(KEY_D))
		m_pTransformCom->Go_Right(TimeDelta);

	if (pInput_Instance->Key_Pressing(KEY_W))
		m_pTransformCom->Go_Straight(TimeDelta);

	if (pInput_Instance->Key_Pressing(KEY_S))
		m_pTransformCom->Back_Straight(TimeDelta);

	_long	MouseMove = 0;

	if (MouseMove = pInput_Instance->Get_DIMouseMove(CInput_Device::DIMS_X))
		m_pTransformCom->Rotation_Axis(_vec3(0.f, 1.f, 0.f), MouseMove * TimeDelta);

	if (MouseMove = pInput_Instance->Get_DIMouseMove(CInput_Device::DIMS_Y))
		m_pTransformCom->Rotation_Axis(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), MouseMove * TimeDelta);

	Safe_Release(pInput_Instance);

	POINT		ptCursorPos = { g_nWinCX >> 1, g_nWinCY >> 1 };

	ClientToScreen(g_hWnd, &ptCursorPos);

	SetCursorPos(ptCursorPos.x, ptCursorPos.y);

	return _int();
}

_int CCamera_Free::LateUpdate_GameObject(_double TimeDelta)
{
	if (nullptr == m_pTransformCom || 
		nullptr == m_pGraphic_Device)
		return -1;

	_matrix			matTmp;

	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &m_pTransformCom->Get_WorldMatrixInv());
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, D3DXMatrixPerspectiveFovLH(&matTmp, m_CameraDesc.fFovY, m_CameraDesc.fAspect, m_CameraDesc.fNear, m_CameraDesc.fFar));

	return _int();
}

HRESULT CCamera_Free::Render_GameObject()
{
	return NOERROR;
}

HRESULT CCamera_Free::Add_Component()
{
	
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, &CTransform::STATE(3.0f, D3DXToRadian(10.0f)))))
		return E_FAIL;
	

	return NOERROR;
}

CGameObject * CCamera_Free::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCamera_Free*		pInstance = new CCamera_Free(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		MSG_BOX("Failed To Create CCamera_Free Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCamera_Free::Clone_GameObject(void * pArg)
{
	CCamera_Free*		pInstance = new CCamera_Free(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		MSG_BOX("Failed To Create CCamera_Free Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Free::Free()
{
	Safe_Release(m_pTransformCom);

	CGameObject::Free();
}
