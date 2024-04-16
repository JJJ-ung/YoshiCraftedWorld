#include "..\Headers\Transform.h"

CTransform::CTransform(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{

}

CTransform::CTransform(const CTransform & rhs)
	: CComponent(rhs)
	, m_matWorld(rhs.m_matWorld)
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

HRESULT CTransform::Ready_Component_Prototype()
{
	D3DXMatrixIdentity(&m_matWorld);

	return NOERROR;
}

HRESULT CTransform::Ready_Component(void * pArg)
{
	memcpy(&m_State, pArg, sizeof(STATE));

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

HRESULT CTransform::Go_Up(_double TimeDelta)
{
	_vec3		vPosition = Get_State(CTransform::STATE_POSITION);
	_vec3		vUp = Get_State(CTransform::STATE_UP);

	vPosition += *D3DXVec3Normalize(&vUp, &vUp) * m_State.fSpeedPerSec * _float(TimeDelta);

	Set_State(CTransform::STATE_POSITION, vPosition);

	return NOERROR;
}

HRESULT CTransform::Go_Down(_double TimeDelta)
{
	_vec3		vPosition = Get_State(CTransform::STATE_POSITION);
	_vec3		vUp = Get_State(CTransform::STATE_UP);

	vPosition -= *D3DXVec3Normalize(&vUp, &vUp) * m_State.fSpeedPerSec * _float(TimeDelta);

	Set_State(CTransform::STATE_POSITION, vPosition);

	return NOERROR;
}

HRESULT CTransform::Move_Direction(_vec3 vDir, _double TimeDelta)
{
	_vec3		vPosition = Get_State(CTransform::STATE_POSITION);

	vPosition += *D3DXVec3Normalize(&vDir, &vDir) * m_State.fSpeedPerSec * _float(TimeDelta);

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

HRESULT CTransform::Rotation_Look(const _vec3 & vTarget)
{
	_vec3	vUp = _vec3(0.f, 1.f, 0.f);
	_vec3	vRight = *D3DXVec3Cross(&vRight, &vUp, &vTarget);
	_vec3	vLook = *D3DXVec3Cross(&vLook, &vRight, &vUp);

	Set_State(CTransform::STATE_RIGHT, *D3DXVec3Normalize(&vRight, &vRight) * Get_Scale(CTransform::STATE_RIGHT));
	Set_State(CTransform::STATE_UP, *D3DXVec3Normalize(&vUp, &vUp) * Get_Scale(CTransform::STATE_UP));
	Set_State(CTransform::STATE_LOOK, *D3DXVec3Normalize(&vLook, &vLook) * Get_Scale(CTransform::STATE_LOOK));

	return NOERROR;
}

HRESULT CTransform::Rotation_OrbitTarget(const _vec3 & vTarget, const _vec3& vAxis, float fDistance, _double TimeDelta)
{
	_vec3 vPos, vLook;

	vPos = vTarget;
	vLook = Get_State(CTransform::STATE_LOOK);
	vLook *= -1.f;

	vPos += vLook * fDistance;
	Set_State(CTransform::STATE_POSITION, vPos);

	Rotation_Axis(vAxis, TimeDelta);

	return NOERROR;
}

HRESULT CTransform::SetUp_Rotation(const _vec3 & vAxis, _float fRadian)
{
	_matrix		matRot;
	D3DXMatrixRotationAxis(&matRot, &vAxis, fRadian);

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

HRESULT CTransform::Goto_Target(const _vec3 & vTargetVector, _double TimeDelta)
{
	_vec3	vSourPos = Get_State(CTransform::STATE_POSITION);

	_vec3	vDirection = vTargetVector - vSourPos;

	vSourPos += *D3DXVec3Normalize(&vDirection, &vDirection) * m_State.fSpeedPerSec * _float(TimeDelta);

	Rotation_LookAt(vTargetVector);

	Set_State(CTransform::STATE_POSITION, vSourPos);

	return NOERROR;
}

HRESULT CTransform::Follow_Target(const CTransform * pTargetTransform, float fDistance, _double TimeDelta)
{
	_vec3	vSourPos = Get_State(CTransform::STATE_POSITION);
	_vec3	vDestPos = pTargetTransform->Get_State(CTransform::STATE_POSITION);

	_vec3	vDirection = vDestPos - vSourPos;
	vDirection *= -1.f;

	vDestPos += *D3DXVec3Normalize(&vDirection, &vDirection) * fDistance;
	Set_State(CTransform::STATE_POSITION, vDestPos);

	//Rotation_LookAt(vDestPos);

	return NOERROR;
}


_bool CTransform::Rotation_Degree(_double TimeDelta, _double TimeLimit, _float fMin, _float fMax)
{
	m_AccTime += TimeDelta;
	m_fProgress = float(m_AccTime / TimeLimit);

	SetUp_Rotation(_vec3(0.f, 1.f, 0.f), Lerp_Degree(fMin, fMax, m_fProgress));

	if (m_fProgress >= 1.f)
	{
		m_AccTime = 0.0;
		m_fProgress = 0.f;
		SetUp_Rotation(_vec3(0.f, 1.f, 0.f), fMax);
		return true;
	}
	else
		return false;
}

_float CTransform::Lerp_Degree(_float fMin, _float fMax, _float fProgress)
{
	_float fOut = (fMax - fMin) * fProgress + fMin;
	return fOut;
}

CTransform * CTransform::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTransform*		pInstance = new CTransform(pGraphic_Device);

	if (FAILED(pInstance->Ready_Component_Prototype()))
	{
		MSG_BOX("Failed To Create CTransform Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CTransform::Clone_Component(void * pArg)
{
	CTransform*		pInstance = new CTransform(*this);

	if (FAILED(pInstance->Ready_Component(pArg)))
	{
		MSG_BOX("Failed To Create CTransform Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTransform::Free()
{
	CComponent::Free();
}
