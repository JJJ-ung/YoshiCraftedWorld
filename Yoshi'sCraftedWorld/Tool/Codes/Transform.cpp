#include "stdafx.h"
#include "..\Headers\Transform.h"

CTransform::CTransform(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{

}

_matrix CTransform::Get_WorldMatrixInv() const
{
	_matrix		matTmp;

	return *D3DXMatrixInverse(&matTmp, nullptr, &m_matWorld);
}

HRESULT CTransform::Set_State(STATEINFO eStateInfo, const _vec3 & vInfo)
{
	memcpy(&m_matWorld.m[eStateInfo][0], &vInfo, sizeof(_vec3));

	return NOERROR;
}

HRESULT CTransform::Ready_Component(STATE pArg)
{
	D3DXMatrixIdentity(&m_matWorld);
	memcpy(&m_State, &pArg, sizeof(STATE));

	return NOERROR;
}

HRESULT CTransform::Go_Straight(_double TimeDelta)
{
	_vec3		vPosition = Get_State(CTransform::STATE_POSITION);
	_vec3		vLook = Get_State(CTransform::STATE_LOOK);	

	vPosition += *D3DXVec3Normalize(&vLook, &vLook) * m_State.fSpeedPerSec * _float(TimeDelta);

	Set_State(CTransform::STATE_POSITION, vPosition);

	return NOERROR;
}

HRESULT CTransform::Back_Straight(_double TimeDelta)
{
	_vec3		vPosition = Get_State(CTransform::STATE_POSITION);
	_vec3		vLook = Get_State(CTransform::STATE_LOOK);

	vPosition -= *D3DXVec3Normalize(&vLook, &vLook) * m_State.fSpeedPerSec * _float(TimeDelta);

	Set_State(CTransform::STATE_POSITION, vPosition);

	return NOERROR;
}

HRESULT CTransform::Go_Left(_double TimeDelta)
{
	_vec3		vPosition = Get_State(CTransform::STATE_POSITION);
	_vec3		vRight = Get_State(CTransform::STATE_RIGHT);

	vPosition -= *D3DXVec3Normalize(&vRight, &vRight) * m_State.fSpeedPerSec * _float(TimeDelta);

	Set_State(CTransform::STATE_POSITION, vPosition);

	return NOERROR;
}

HRESULT CTransform::Go_Right(_double TimeDelta)
{
	_vec3		vPosition = Get_State(CTransform::STATE_POSITION);
	_vec3		vRight = Get_State(CTransform::STATE_RIGHT);

	vPosition += *D3DXVec3Normalize(&vRight, &vRight) * m_State.fSpeedPerSec * _float(TimeDelta);

	Set_State(CTransform::STATE_POSITION, vPosition);

	return NOERROR;
}

HRESULT CTransform::Rotation_Axis(const _vec3 & vAxis, _double TimeDelta)
{
	_matrix		matRot;

	D3DXMatrixRotationAxis(&matRot, &vAxis, m_State.fRadianPerSec * _float(TimeDelta));

	_vec3		vRight, vUp, vLook;

	vRight = Get_State(CTransform::STATE_RIGHT);
	vUp = Get_State(CTransform::STATE_UP);
	vLook = Get_State(CTransform::STATE_LOOK);

	D3DXVec3TransformNormal(&vRight, &vRight, &matRot);
	D3DXVec3TransformNormal(&vUp, &vUp, &matRot);
	D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);

	return NOERROR;
}

HRESULT CTransform::Rotation_Axis(STATEINFO eStateInfo, _double TimeDelta)
{
	_matrix		matRotX, matRotY, matRotZ, matRot;
	_vec3		vRight, vUp, vLook;

	switch (eStateInfo)
	{
	case CTransform::STATE_RIGHT:
		m_vRotation.x += m_State.fRadianPerSec * _float(TimeDelta);
		if (m_vRotation.x > 360)
			m_vRotation.x = 0.f;
		break;
	case CTransform::STATE_UP:
		m_vRotation.y += m_State.fRadianPerSec * _float(TimeDelta);
		if (m_vRotation.y > 360)
			m_vRotation.y = 0.f;
		break;
	case CTransform::STATE_LOOK:
		m_vRotation.z += m_State.fRadianPerSec * _float(TimeDelta);
		if (m_vRotation.z > 360)
			m_vRotation.z = 0.f;
		break;
	default:
		return E_FAIL;
	}

	D3DXMatrixRotationAxis(&matRotX, &_vec3(1.f, 0.f, 0.f), D3DXToRadian(m_vRotation.x));
	D3DXMatrixRotationAxis(&matRotY, &_vec3(0.f, 1.f, 0.f), D3DXToRadian(m_vRotation.y));
	D3DXMatrixRotationAxis(&matRotZ, &_vec3(0.f, 0.f, 1.f), D3DXToRadian(m_vRotation.z));

	matRot = matRotX * matRotY * matRotZ;

	vRight = _vec3(1.f, 0.f, 0.f) * Get_Scale(CTransform::STATE_RIGHT);
	vUp = _vec3(0.f, 1.f, 0.f) * Get_Scale(CTransform::STATE_UP);
	vLook = _vec3(0.f, 0.f, 1.f) * Get_Scale(CTransform::STATE_LOOK);

	D3DXVec3TransformNormal(&vRight, &vRight, &matRot);
	D3DXVec3TransformNormal(&vUp, &vUp, &matRot);
	D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);

	return NOERROR;
}

HRESULT CTransform::Rotation_LookAt(const _vec3 & vTarget)
{
	_vec3	vSourPos = Get_State(CTransform::STATE_POSITION);

	_vec3	vDirection = vTarget - vSourPos;
	
	_vec3	vUp = _vec3(0.f, 1.f, 0.f);

	_vec3	vRight = *D3DXVec3Cross(&vRight, &vUp, &vDirection);

	_vec3	vLook = *D3DXVec3Cross(&vLook, &vRight, &vUp);
	
	Set_State(CTransform::STATE_RIGHT, *D3DXVec3Normalize(&vRight, &vRight) * Get_Scale(CTransform::STATE_RIGHT));
	Set_State(CTransform::STATE_UP, *D3DXVec3Normalize(&vUp, &vUp) * Get_Scale(CTransform::STATE_UP));
	Set_State(CTransform::STATE_LOOK, *D3DXVec3Normalize(&vLook, &vLook) * Get_Scale(CTransform::STATE_LOOK));

	return NOERROR;
}

HRESULT CTransform::SetUp_Rotation(const _vec3 & vAxis, _float fDegree)
{
	_matrix		matRot;

	if(vAxis == _vec3(1.f, 0.f, 0.f))
		m_vRotation.x = fDegree;
	if (vAxis == _vec3(0.f, 1.f, 0.f))
		m_vRotation.y = fDegree;
	if (vAxis == _vec3(0.f, 0.f, 1.f))
		m_vRotation.z = fDegree;

	D3DXMatrixRotationAxis(&matRot, &vAxis, D3DXToRadian(fDegree));

	_vec3		vRight, vUp, vLook;

	vRight = _vec3(1.f, 0.f, 0.f) * Get_Scale(CTransform::STATE_RIGHT);
	vUp = _vec3(0.f, 1.f, 0.f) * Get_Scale(CTransform::STATE_UP);
	vLook = _vec3(0.f, 0.f, 1.f) * Get_Scale(CTransform::STATE_LOOK);

	D3DXVec3TransformNormal(&vRight, &vRight, &matRot);
	D3DXVec3TransformNormal(&vUp, &vUp, &matRot);
	D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);

	return NOERROR;
}

HRESULT CTransform::SetUp_Rotation(STATEINFO eStateInfo, _float fDegree)
{
	_matrix		matRotX, matRotY, matRotZ, matRot;
	_vec3		vRight, vUp, vLook;

	switch (eStateInfo)
	{
	case CTransform::STATE_RIGHT:
		m_vRotation.x = fDegree;
		break;
	case CTransform::STATE_UP:
		m_vRotation.y = fDegree;
		break;
	case CTransform::STATE_LOOK:
		m_vRotation.z = fDegree;
		break;
	default:
		return E_FAIL;
	}

	D3DXMatrixRotationAxis(&matRotX, &_vec3(1.f, 0.f, 0.f), D3DXToRadian(m_vRotation.x));
	D3DXMatrixRotationAxis(&matRotY, &_vec3(0.f, 1.f, 0.f), D3DXToRadian(m_vRotation.y));
	D3DXMatrixRotationAxis(&matRotZ, &_vec3(0.f, 0.f, 1.f), D3DXToRadian(m_vRotation.z));
	//D3DXMatrixRotationAxis(&matRotX, &Get_State(CTransform::STATE_RIGHT), D3DXToRadian(m_vRotation.x));
	//D3DXMatrixRotationAxis(&matRotY, &Get_State(CTransform::STATE_UP), D3DXToRadian(m_vRotation.y));
	//D3DXMatrixRotationAxis(&matRotZ, &Get_State(CTransform::STATE_LOOK), D3DXToRadian(m_vRotation.z));

	matRot = matRotX * matRotY * matRotZ;

	vRight = _vec3(1.f, 0.f, 0.f) * Get_Scale(CTransform::STATE_RIGHT);
	vUp = _vec3(0.f, 1.f, 0.f) * Get_Scale(CTransform::STATE_UP);
	vLook = _vec3(0.f, 0.f, 1.f) * Get_Scale(CTransform::STATE_LOOK);

	D3DXVec3TransformNormal(&vRight, &vRight, &matRot);
	D3DXVec3TransformNormal(&vUp, &vUp, &matRot);
	D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);

	return NOERROR;
}

HRESULT CTransform::SetUp_Scale(const _vec3 & vScale)
{
	_vec3		vRight, vUp, vLook;	

	vRight = *D3DXVec3Normalize(&vRight, &Get_State(CTransform::STATE_RIGHT)) * vScale.x;
	vUp = *D3DXVec3Normalize(&vUp, &Get_State(CTransform::STATE_UP)) * vScale.y;
	vLook = *D3DXVec3Normalize(&vLook, &Get_State(CTransform::STATE_LOOK)) * vScale.z;

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);

	return NOERROR;
}

HRESULT CTransform::Goto_Target(const CTransform * pTargetTransform, _double TimeDelta)
{
	_vec3	vSourPos = Get_State(CTransform::STATE_POSITION);
	_vec3	vDestPos = pTargetTransform->Get_State(CTransform::STATE_POSITION);	

	_vec3	vDirection = vDestPos - vSourPos;

	vSourPos += *D3DXVec3Normalize(&vDirection, &vDirection) * m_State.fSpeedPerSec * _float(TimeDelta);

	Rotation_LookAt(vDestPos);

	Set_State(CTransform::STATE_POSITION, vSourPos);	

	return NOERROR;
}

CTransform * CTransform::Create(LPDIRECT3DDEVICE9 pGraphic_Device, STATE pArg)
{
	CTransform*		pInstance = new CTransform(pGraphic_Device);

	if (FAILED(pInstance->Ready_Component(pArg)))
	{
		MSG_BOX("Failed To Create CTransform Instance");
		Safe_Delete(pInstance);
	}

	return pInstance;
}

void CTransform::Free()
{
	CComponent::Free();
}
