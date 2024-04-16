#include "stdafx.h"
#include "..\Headers\Camera_Title.h"
#include "Management.h"
#include "Input_Device.h"

USING(Client)

CCamera_Title::CCamera_Title(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CCamera_Title::CCamera_Title(const CCamera_Title & rhs)
	: CGameObject(rhs)
{
}

HRESULT CCamera_Title::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CCamera_Title::Ready_GameObject(void * pArg)
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

_int CCamera_Title::Update_GameObject(_double TimeDelta)
{
	if (nullptr == m_pTransformCom)
		return -1;

	CInput_Device* pInputDevice = CInput_Device::GetInstance();

	Safe_AddRef(pInputDevice);

	if (pInputDevice->Key_Pressing(KEY_D))
	{
		m_bFront = true;
		m_fSpeed -= 0.1f;
		if (m_fSpeed < -15.f)
			m_fSpeed = -15.f;
	}
	else if (pInputDevice->Key_Pressing(KEY_A))
	{
		m_bFront = false;
		m_fSpeed += 0.1f;
		if (m_fSpeed > 18.f)
			m_fSpeed = 18.f;
	}
	else
	{
		if (m_bFront)
		{
			m_fSpeed += 0.1f;
			if (m_fSpeed > -3.f)
				m_fSpeed = -3.f;
		}
		else
		{
			m_fSpeed -= 0.1f;
			if (m_fSpeed < -3.f)
			{
				m_fSpeed = -3.f;
				m_bFront = true;
			}
		}
	}

	Safe_Release(pInputDevice);

	m_pTransformCom->Set_RadianPerSec(D3DXToRadian(m_fSpeed));

	if (FAILED(m_pTransformCom->Rotation_OrbitTarget(m_CameraDesc.vAt, _vec3(0.f, 1.f, 0.f), 13.f, TimeDelta)))
		return -1;

	return _int();
}

_int CCamera_Title::LateUpdate_GameObject(_double TimeDelta)
{
	if (nullptr == m_pTransformCom ||
		nullptr == m_pGraphic_Device)
		return -1;

	_matrix			matTmp;

	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &m_pTransformCom->Get_WorldMatrixInv());
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, D3DXMatrixPerspectiveFovLH(&matTmp, m_CameraDesc.fFovY, m_CameraDesc.fAspect, m_CameraDesc.fNear, m_CameraDesc.fFar));

	return _int();
}

HRESULT CCamera_Title::Render_GameObject()
{
	return NOERROR;
}

HRESULT CCamera_Title::Add_Component()
{
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, &CTransform::STATE(5.0f, D3DXToRadian(-3.f)))))
		return E_FAIL;

	return NOERROR;
}

CGameObject * CCamera_Title::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCamera_Title*		pInstance = new CCamera_Title(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		MSG_BOX("Failed To Create CCamera_Title Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCamera_Title::Clone_GameObject(void * pArg)
{
	CCamera_Title*		pInstance = new CCamera_Title(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		MSG_BOX("Failed To Create CCamera_Free Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Title::Free()
{
	Safe_Release(m_pTransformCom);

	CGameObject::Free();
}
