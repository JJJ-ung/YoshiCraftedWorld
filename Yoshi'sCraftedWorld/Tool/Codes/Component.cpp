#include "stdafx.h"
#include "..\Headers\Component.h"

CComponent::CComponent(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device(pGraphic_Device)
{
}

HRESULT CComponent::Ready_Component(void* pArg)
{
	return NOERROR;
}

void CComponent::Free()
{
}
