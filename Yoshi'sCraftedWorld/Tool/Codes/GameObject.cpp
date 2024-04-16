#include "stdafx.h"
#include "GameObject.h"
#include "Component.h"

CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device(pGraphic_Device)
{
}

_matrix CGameObject::Get_Transform(_D3DTRANSFORMSTATETYPE eType)
{
	_matrix	matTmp;
	
	m_pGraphic_Device->GetTransform(eType, &matTmp);

	return matTmp;
}

HRESULT CGameObject::Ready_GameObject()
{
	return NOERROR;
}

_int CGameObject::Update_GameObject(_double TimeDelta)
{
	return _int();
}

_int CGameObject::LateUpdate_GameObject(_double TimeDelta)
{
	return _int();
}

HRESULT CGameObject::Render_GameObject()
{
	return NOERROR;
}

HRESULT CGameObject::Add_Component(const _tchar * pComponentTag, CComponent * pComponent)
{
	if (nullptr != Find_Component(pComponentTag))
	{
		MSG_BOX("Component Already Exists");
		return E_FAIL;
	}

	if (nullptr == pComponent)
	{
		MSG_BOX("Component is Null");
		return E_FAIL;
	}

	m_Components.insert(COMPONENTS::value_type(pComponentTag, pComponent));	

	return NOERROR;
}

HRESULT CGameObject::Delete_Component(const _tchar * pComponentTag)
{
	CComponent* pComponent = Find_Component(pComponentTag);

	if (pComponent == nullptr)
	{
		MSG_BOX("Not Existing Component");
		return E_FAIL;
	}

	pComponent->Free();
	Safe_Delete(pComponent);
	m_Components.erase(pComponentTag);

	return NOERROR;
}

CComponent * CGameObject::Find_Component(const _tchar * pComponentTag)
{
	auto	iter = find_if(m_Components.begin(), m_Components.end(), CTag_Finder(pComponentTag));
	if (iter == m_Components.end())
		return nullptr;

	return iter->second;
}

void CGameObject::Free()
{
	for (auto& Pair : m_Components)
	{
		Pair.second->Free();
		Safe_Delete(Pair.second);
	}
	m_Components.clear();
}
