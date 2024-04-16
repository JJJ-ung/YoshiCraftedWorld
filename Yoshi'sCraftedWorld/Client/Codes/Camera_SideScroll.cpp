#include "stdafx.h"
#include "..\Headers\Camera_SideScroll.h"

#include "Transform.h"
#include "Management.h"
#include "Yoshi_Stage.h"

USING(Client)

CCamera_SideScroll::CCamera_SideScroll(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CCamera_SideScroll::CCamera_SideScroll(const CCamera_SideScroll & rhs)
	:CGameObject(rhs)
{
}

HRESULT CCamera_SideScroll::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CCamera_SideScroll::Ready_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	memcpy(&m_CameraDesc, pArg, sizeof(CCamera_SideScroll::CAMERADESC));

	_vec3 vPlayerPos = m_CameraDesc.pTargetTransform->Get_State(CTransform::STATE_POSITION) + m_CameraDesc.vPlayerOffset;
	_vec3 vCameraPos = vPlayerPos + m_CameraDesc.vMove + m_CameraDesc.vCameraOffset;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCameraPos);

	_vec3 vLook = vPlayerPos - vCameraPos;
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, *D3DXVec3Normalize(&vLook, &vLook));

	_vec3	vRight = *D3DXVec3Cross(&vRight, &_vec3(0.f, 1.f, 0.f), &vLook);
	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, *D3DXVec3Normalize(&vRight, &vRight));

	_vec3	vUp = *D3DXVec3Cross(&vUp, &vLook, &vRight);
	m_pTransformCom->Set_State(CTransform::STATE_UP, *D3DXVec3Normalize(&vUp, &vUp));

	return NOERROR;
}

_int CCamera_SideScroll::Update_GameObject(_double TimeDelta)
{
	m_fProgress = (_float)TimeDelta * 4.f;

	if (m_bCutSceneStart)
	{
		m_fProgress = (_float)TimeDelta * 1.5f;

		system_clock::duration temp = system_clock::now() - m_tEnterTime;
		if (duration_cast<milliseconds>(temp) > m_CutSceneTime)
		{
			m_bCutSceneStart = false;
			CManagement* pManagement = CManagement::GetInstance();
			Safe_AddRef(pManagement);
			CYoshi_Stage* pPlayer = (CYoshi_Stage*)pManagement->Get_ObjectFromLayer(SCENE_TRAIN, L"Layer_Object");
			pPlayer->Get_CutScene() = false;
			Safe_Release(pManagement);
		}
	}

	m_CameraDesc.vCameraOffset.x = m_CameraDesc.pTargetTransform->Get_State(CTransform::STATE_LOOK).x;

	_vec3 vTargetPos;
	if (m_bRenewTarget)
	{
		vTargetPos = m_CameraDesc.pTargetTransform->Get_State(CTransform::STATE_POSITION) + m_CameraDesc.vPlayerOffset + m_CameraDesc.vMove + m_CameraDesc.vCameraOffset;
		vTargetPos.y = m_pTransformCom->Get_State(CTransform::STATE_POSITION).y;
	}
	else
	{
		vTargetPos = m_CameraDesc.pTargetTransform->Get_State(CTransform::STATE_POSITION) + m_CameraDesc.vPlayerOffset + m_CameraDesc.vMove + m_CameraDesc.vCameraOffset;
	}

	if (m_bBridgeTrigger)
		vTargetPos += _vec3(3.f, 1.f, -2.f);
	else if (m_bStart)
		vTargetPos += _vec3(-1.f, -1.5f, 2.5f);
	else if (m_bCutSceneStart)
		vTargetPos = _vec3(184.18f, 1.2f, 34.6f) + m_CameraDesc.vPlayerOffset + m_CameraDesc.vMove;
	else if (m_bEnd)
	{
		m_fRotate -= (_float)TimeDelta/* * 10*/ * 0.1f;
		if (m_fRotate < -0.16f)
			m_fRotate = -0.16f;

		vTargetPos = m_CameraDesc.pTargetTransform->Get_State(CTransform::STATE_POSITION) + _vec3(0.f, 0.71f, -3.75f);
		m_pTransformCom->SetUp_Rotation(_vec3(1.f, 0.f, 0.f), m_fRotate);
	}
	else if (m_bEndCam)
	{
		m_fRotate2 += (_float)TimeDelta;
		if (m_fRotate2 > 0.5f)
			m_fRotate2 = 0.5f;

		m_pTransformCom->SetUp_Rotation(_vec3(0.f, 1.f, 0.f), m_fRotate2);

		vTargetPos += _vec3(-2.f, -1.3f, 0.f);
	}

	_vec3 vOut;
	D3DXVec3Lerp(&vOut, &m_pTransformCom->Get_State(CTransform::STATE_POSITION), &vTargetPos, m_fProgress);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vOut);

	return _int();
}

_int CCamera_SideScroll::LateUpdate_GameObject(_double TimeDelta)
{
	if (nullptr == m_pTransformCom ||
		nullptr == m_pGraphic_Device)
		return -1;

	_matrix			matTmp;

	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &m_pTransformCom->Get_WorldMatrixInv());
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, D3DXMatrixPerspectiveFovLH(&matTmp, m_CameraDesc.fFovY, _float(g_nWinCX) / g_nWinCY, 0.2f, 500.f));

	return _int();
}

HRESULT CCamera_SideScroll::Render_GameObject()
{
	return NOERROR;
}

void CCamera_SideScroll::Renewal_Target()
{
}

HRESULT CCamera_SideScroll::Add_Component()
{
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, &CTransform::STATE(3.0f, D3DXToRadian(10.0f)))))
		return E_FAIL;

	return NOERROR;
}

CGameObject * CCamera_SideScroll::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCamera_SideScroll*		pInstance = new CCamera_SideScroll(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		MSG_BOX("Failed To Create CCamera_SideScroll Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCamera_SideScroll::Clone_GameObject(void * pArg)
{
	CCamera_SideScroll*		pInstance = new CCamera_SideScroll(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		MSG_BOX("Failed To Create CCamera_SideScroll Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_SideScroll::Free()
{
	Safe_Release(m_pTransformCom);

	CGameObject::Free();
}
