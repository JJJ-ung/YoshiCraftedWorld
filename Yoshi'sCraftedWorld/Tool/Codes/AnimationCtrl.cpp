#include "stdafx.h"
#include "..\Headers\AnimationCtrl.h"

CAnimationCtrl::CAnimationCtrl(LPDIRECT3DDEVICE9 pGraphic_Device, LPD3DXANIMATIONCONTROLLER pAnimationCtrl)
	: m_pGraphic_Device(pGraphic_Device)
	, m_pAnimationCtrl(pAnimationCtrl)
{
}

HRESULT CAnimationCtrl::Ready_Animation()
{
	return NOERROR;
}

HRESULT CAnimationCtrl::SetUp_Animation(_uint iIndex)
{
	if (nullptr == m_pAnimationCtrl)
		return E_FAIL;

	if (m_iOldAniIndex == iIndex)
		return NOERROR;

	LPD3DXANIMATIONSET		pAS = nullptr;

	if (FAILED(m_pAnimationCtrl->GetAnimationSet(iIndex, &pAS)))
		return E_FAIL;

	m_Period = pAS->GetPeriod();

	m_iNewTrack = m_iCurrentTrack == 0 ? 1 : 0;

	m_pAnimationCtrl->SetTrackAnimationSet(m_iNewTrack, pAS);

	m_pAnimationCtrl->UnkeyAllTrackEvents(m_iCurrentTrack);
	m_pAnimationCtrl->UnkeyAllTrackEvents(m_iNewTrack);

	m_pAnimationCtrl->KeyTrackEnable(m_iCurrentTrack, FALSE, m_TimeDeltaAcc + m_DelayTime);
	m_pAnimationCtrl->KeyTrackSpeed(m_iCurrentTrack, m_AnimationSpeed, m_TimeDeltaAcc, 0.2, D3DXTRANSITION_LINEAR);
	m_pAnimationCtrl->KeyTrackWeight(m_iCurrentTrack, m_Weight, m_TimeDeltaAcc, m_DelayTime, D3DXTRANSITION_LINEAR);

	m_pAnimationCtrl->SetTrackEnable(m_iNewTrack, TRUE);
	m_pAnimationCtrl->KeyTrackSpeed(m_iNewTrack, m_AnimationSpeed, m_TimeDeltaAcc, m_DelayTime, D3DXTRANSITION_LINEAR);
	m_pAnimationCtrl->KeyTrackWeight(m_iNewTrack, (1.f - m_Weight), m_TimeDeltaAcc, m_DelayTime, D3DXTRANSITION_LINEAR);

	m_pAnimationCtrl->SetTrackPosition(m_iNewTrack, 0.0);

	m_pAnimationCtrl->ResetTime();

	m_TimeDeltaAcc = 0.0;

	m_iOldAniIndex = iIndex;

	m_iCurrentTrack = m_iNewTrack;

	return NOERROR;
}

HRESULT CAnimationCtrl::Play_Animation(_double TimeDelta)
{
	if (nullptr == m_pAnimationCtrl)
		return E_FAIL;

	m_pAnimationCtrl->AdvanceTime(TimeDelta, nullptr);

	m_TimeDeltaAcc += TimeDelta;

	return NOERROR;
}

_bool CAnimationCtrl::Finish_Animation()
{
	D3DXTRACK_DESC		Track_Desc;
	ZeroMemory(&Track_Desc, sizeof(D3DXTRACK_DESC));

	m_pAnimationCtrl->GetTrackDesc(m_iCurrentTrack, &Track_Desc);

	if (m_Period <= Track_Desc.Position)
		return true;
	else
		return false;
}

_bool CAnimationCtrl::Finish_Animation(_double TimeLimit)
{
	D3DXTRACK_DESC		Track_Desc;
	ZeroMemory(&Track_Desc, sizeof(D3DXTRACK_DESC));

	m_pAnimationCtrl->GetTrackDesc(m_iCurrentTrack, &Track_Desc);

	if (m_Period - TimeLimit <= Track_Desc.Position)
		return true;
	else
		return false;
}

CAnimationCtrl * CAnimationCtrl::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LPD3DXANIMATIONCONTROLLER pAnimationCtrl)
{
	CAnimationCtrl*		pInstance = new CAnimationCtrl(pGraphic_Device, pAnimationCtrl);

	if (FAILED(pInstance->Ready_Animation()))
	{
		MSG_BOX("Failed To Create CAnimationCtrl Instance");
		Safe_Delete(pInstance);
	}

	return pInstance;
}

void CAnimationCtrl::Free()
{
	Safe_Release(m_pAnimationCtrl);
}
