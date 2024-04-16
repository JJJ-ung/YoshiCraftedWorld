#pragma once

#include "Component.h"

class CTransform final : public CComponent
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

public:
	explicit CTransform(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CTransform() = default;

public:
	_vec3 Get_State(STATEINFO eStateInfo) const { return *((_vec3*)&m_matWorld.m[eStateInfo][0]); }
	_float Get_Scale(STATEINFO eStateInfo) { return D3DXVec3Length((_vec3*)&m_matWorld.m[eStateInfo][0]); }
	_matrix Get_WorldMatrix() const { return m_matWorld;}
	_matrix Get_WorldMatrixInv() const;

public:
	HRESULT Set_State(STATEINFO eStateInfo, const _vec3& vInfo);

public:
	HRESULT Ready_Component(STATE pArg);

public:
	HRESULT Go_Straight(_double TimeDelta); 
	HRESULT Back_Straight(_double TimeDelta);
	HRESULT Go_Left(_double TimeDelta);
	HRESULT Go_Right(_double TimeDelta);		
	HRESULT Rotation_Axis(const _vec3& vAxis, _double TimeDelta);
	HRESULT Rotation_Axis(STATEINFO eStateInfo, _double TimeDelta);
	HRESULT Rotation_LookAt(const _vec3& vTarget);
	HRESULT SetUp_Rotation(const _vec3& vAxis, _float fDegree);
	HRESULT SetUp_Rotation(STATEINFO eStateInfo, _float fDegree);
	HRESULT SetUp_Scale(const _vec3& vScale);
	HRESULT Goto_Target(const CTransform* pTargetTransform, _double TimeDelta);

public:
	STATE		m_State;
	_matrix		m_matWorld;	
	_vec3			m_vRotation;
	_vec3			m_vRadianRotation;

public:
	static CTransform* Create(LPDIRECT3DDEVICE9 pGraphic_Device, STATE pArg);
	virtual void Free();
};