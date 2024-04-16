#include "..\Headers\Light_Manager.h"
#include "Light.h"

IMPLEMENT_SINGLETON(CLight_Manager)

CLight_Manager::CLight_Manager()
{

}

const D3DLIGHT9 & CLight_Manager::Get_Light(_uint iIndex)
{
	auto	iter = m_LightList.begin();

	for (size_t i = 0; i < iIndex; ++i)
		++iter;

	return (*iter)->Get_Light();	
}

HRESULT CLight_Manager::Add_Light(LPDIRECT3DDEVICE9 pGraphic_Device, const D3DLIGHT9 & LightInfo)
{
	CLight*		pLight = CLight::Create(pGraphic_Device, LightInfo);
	if (nullptr == pLight)
		return E_FAIL;

	m_LightList.push_back(pLight);

	return NOERROR;
}

HRESULT CLight_Manager::Render_Light(LPD3DXEFFECT pEffect)
{
	for (auto& pLight : m_LightList)
	{
		if (nullptr != pLight)
			pLight->Render_Light(pEffect);
	}

	return NOERROR;
}

void CLight_Manager::Free()
{
	for (auto& pLight : m_LightList)
		Safe_Release(pLight);
	m_LightList.clear();
}
