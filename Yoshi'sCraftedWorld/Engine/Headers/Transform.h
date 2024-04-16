#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform final : public CComponent
{
public:
	enum STATEINFO { STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POSITION };

public:
	typedef struct tagState
	{
	public:
		tagState() {  }
		tagState(_float InSpeedPerSec, _float InRadianPerSec) 
			: fSpeedPerSec(InSpeedPerSec), fRadianPerSec(InRadianPerSec) {}

		_float	fSpeedPerSec;
		_float	fRadianPerSec;
	}STATE;
	 
	explicit CTransform(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CTransform(const CTransform& rhs);
	virtual ~CTransform() = default;

public:
	void Set_SpeedPerSec(_float fSpeed) { m_State.fSpeedPerSec = fSpeed; }
	void Set_RadianPerSec(_float fRadian) { m_State.fRadianPerSec = fRadian; }

public:
	_vec3 Get_State(STATEINFO eStateInfo) const { return *((_vec3*)&m_matWorld.m[eStateInfo][0]); }
	_float Get_Scale(STATEINFO eStateInfo) { return D3DXVec3Length((_vec3*)&m_matWorld.m[eStateInfo][0]); }
	_matrix Get_WorldMatrix() const {	return m_matWorld;}
	const _matrix* Get_WorldMatrixPointer() const {	return &m_matWorld;	}
	_matrix Get_WorldMatrixInv() const;
	void Set_Matrix(_matrix mat) { m_matWorld = mat; }

public:
	HRESULT Set_State(STATEINFO eStateInfo, const _vec3& vInfo);

public:
	HRESULT Ready_Component_Prototype();
	HRESULT Ready_Component(void* pArg);

public:
	HRESULT Go_Straight(_double TimeDelta); 
	HRESULT Back_Straight(_double TimeDelta);
	HRESULT Go_Left(_double TimeDelta);
	HRESULT Go_Right(_double TimeDelta);		
	HRESULT Go_Up(_double TimeDelta);
	HRESULT Go_Down(_double TimeDelta);
	HRESULT Move_Direction(_vec3 vDir, _double TimeDelta);

public:
	HRESULT Rotation_Axis(const _vec3& vAxis, _double TimeDelta);
	HRESULT Rotation_LookAt(const _vec3& vTarget);
	HRESULT Rotation_Look(const _vec3& vTarget);
	HRESULT Rotation_OrbitTarget(const _vec3& vTarget, const _vec3& vAxis, float fDistance, _double TimeDelta);
	HRESULT SetUp_Rotation(const _vec3& vAxis, _float fRadian);
	HRESULT SetUp_Rotation(STATEINFO eStateInfo, _float fDegree);

public:
	HRESULT SetUp_Scale(const _vec3& vScale);

public:
	HRESULT Goto_Target(const CTransform* pTargetTransform, _double TimeDelta);
	HRESULT Goto_Target(const _vec3& vTargetVector, _double TimeDelta);
	HRESULT Follow_Target(const CTransform* pTargetTransform, float fDistance, _double TimeDelta);

public:
	_bool Rotation_Degree(_double TimeDelta, _double TimeLimit, _float fMin, _float fMax);
	_float Lerp_Degree(_float fMin, _float fMax, _float fProgress);

private:
	STATE		m_State;
	_matrix		m_matWorld;	
	_vec3			m_vRotation;
	_float			m_fProgress = 0.f;
	_double		m_AccTime = 0.0;

public:
	static CTransform* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone_Component(void* pArg);
	virtual void Free();
};

END