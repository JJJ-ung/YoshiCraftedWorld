#pragma once

class CAnimationCtrl final
{
public:
	enum ANIMATION_SET { ANI_DEFAULT = 9999999 };

public:
	explicit CAnimationCtrl(LPDIRECT3DDEVICE9 pGraphic_Device, LPD3DXANIMATIONCONTROLLER pAnimationCtrl);
	virtual ~CAnimationCtrl() = default;

public:
	HRESULT Set_DelayTime(_double Time) { m_DelayTime = Time;  return NOERROR; }
	HRESULT Set_Weight(_float Weight) { m_Weight = Weight;  return NOERROR; }
	HRESULT Set_AnimationSpeed(_float Speed) { m_AnimationSpeed = Speed; return NOERROR; }

public:
	HRESULT Ready_Animation();
	HRESULT SetUp_Animation(_uint iIndex);
	HRESULT Play_Animation(_double TimeDelta);
	_bool Finish_Animation();
	_bool Finish_Animation(_double TimeLimit);

public:
	LPDIRECT3DDEVICE9			m_pGraphic_Device = nullptr;
	LPD3DXANIMATIONCONTROLLER	m_pAnimationCtrl = nullptr;

public:
	_uint		m_iOldAniIndex = ANI_DEFAULT;
	_uint		m_iCurrentTrack = 0;
	_uint		m_iNewTrack = 1;
	_double		m_TimeDeltaAcc = 0.0;
	_double		m_Period = 0.0;

public:
	_double m_DelayTime = 0.2;
	_float m_Weight = 0.1f;
	_float m_AnimationSpeed = 1.f;

public:
	static CAnimationCtrl* Create(LPDIRECT3DDEVICE9 pGraphic_Device, LPD3DXANIMATIONCONTROLLER	pAnimationCtrl);
	virtual void Free();
};
