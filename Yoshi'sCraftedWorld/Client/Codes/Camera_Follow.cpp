#include "stdafx.h"
#include "..\Headers\Camera_Follow.h"
#include "Transform.h"

USING(Client)

CCamera_Follow::CCamera_Follow(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CCamera_Follow::CCamera_Follow(const CCamera_Follow & rhs)
	:CGameObject(rhs)
{
}

HRESULT CCamera_Follow::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CCamera_Follow::Ready_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	memcpy(&m_CameraDesc, pArg, sizeof(CAMERADESC));

	_vec3 vTargetPos = m_CameraDesc.pTargetTransform->Get_State(CTransform::STATE_POSITION);
	_vec3 vTargetDir = m_CameraDesc.pTargetTransform->Get_State(CTransform::STATE_LOOK);

	m_CameraDesc.vEye = vTargetDir * (-1.f);
	m_CameraDesc.vEye += vTargetPos;
	m_CameraDesc.vAt = vTargetPos;

	_vec3 vLook = m_CameraDesc.vAt - m_CameraDesc.vEye;
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, vLook);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_CameraDesc.vEye);

	m_pTransformCom->Rotation_LookAt(vTargetPos);

	return NOERROR;
}

_int CCamera_Follow::Update_GameObject(_double TimeDelta)
{
	if (nullptr == m_pTransformCom)
		return -1;

	//m_pTransformCom->Goto_Target(vTarget, TimeDelta);
	//m_pTransformCom->Follow_Target(m_CameraDesc.pTargetTransform, m_CameraDesc.fDistance, TimeDelta);
	_vec3 vTargetPos = m_CameraDesc.pTargetTransform->Get_State(CTransform::STATE_POSITION);
	_vec3 vTargetDir = m_CameraDesc.pTargetTransform->Get_State(CTransform::STATE_LOOK);

	m_CameraDesc.vEye = vTargetDir * (-1.f);
	m_CameraDesc.vEye += vTargetPos;
	m_CameraDesc.vAt = vTargetPos;

	_vec3 vLook = m_CameraDesc.vAt - m_CameraDesc.vEye;
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, vLook);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_CameraDesc.vEye);

	m_pTransformCom->Rotation_LookAt(vTargetPos);

	return _int();
}

_int CCamera_Follow::LateUpdate_GameObject(_double TimeDelta)
{
	if (nullptr == m_pTransformCom ||
		nullptr == m_pGraphic_Device)
		return -1;

	_matrix			matTmp;

	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &m_pTransformCom->Get_WorldMatrixInv());
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, D3DXMatrixPerspectiveFovLH(&matTmp, m_CameraDesc.fFovY, m_CameraDesc.fAspect, m_CameraDesc.fNear, m_CameraDesc.fFar));

	return _int();
}

HRESULT CCamera_Follow::Render_GameObject()
{
	return NOERROR;
}

HRESULT CCamera_Follow::Add_Component()
{
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, &CTransform::STATE(3.0f, D3DXToRadian(10.0f)))))
		return E_FAIL;


	return NOERROR;
}

CGameObject * CCamera_Follow::Create(LPDIRECT3DDEVICE9 pGraphic_Device)                           
{
	CCamera_Follow*		pInstance = new CCamera_Follow(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		MSG_BOX("Failed To Create CCamera_Follow Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCamera_Follow::Clone_GameObject(void * pArg)
{
	CCamera_Follow*		pInstance = new CCamera_Follow(*this);
	                 
	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		MSG_BOX("Failed To Create CCamera_Free Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Follow::Free()
{
	Safe_Release(m_pTransformCom);

	CGameObject::Free();
}
