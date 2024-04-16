#include "..\Headers\Component.h"

CComponent::CComponent(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device(pGraphic_Device)
	, m_isClone(false)
{
	_uint check = Safe_AddRef(m_pGraphic_Device);
}


CComponent::CComponent(const CComponent & rhs)
	: m_pGraphic_Device(rhs.m_pGraphic_Device)
	, m_isClone(true)
{
	_uint check = Safe_AddRef(m_pGraphic_Device);
}

_matrix CComponent::Get_Transform(_D3DTRANSFORMSTATETYPE eType)
{
	_matrix	matTmp;

	m_pGraphic_Device->GetTransform(eType, &matTmp);

	return matTmp;
}

HRESULT CComponent::Ready_Component_Prototype()
{
	return NOERROR;
}

HRESULT CComponent::Ready_Component(void* pArg)
{
	return NOERROR;
}

void CComponent::Free()
{
	Safe_Release(m_pGraphic_Device);
}
