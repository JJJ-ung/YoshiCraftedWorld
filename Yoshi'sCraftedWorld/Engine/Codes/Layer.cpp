#include "..\Headers\Layer.h"
#include "GameObject.h"

CLayer::CLayer()
{
}

CComponent * CLayer::Get_Component(const _tchar * pComponentTag, _uint iIndex)
{
	if (m_ObjectList.size() <= iIndex)
		return nullptr;

	auto	iter = m_ObjectList.begin();

	for (size_t i = 0; i < iIndex; ++i)
		++iter;

	return (*iter)->Get_Component(pComponentTag);
}

HRESULT CLayer::Add_GameObject(CGameObject * pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_ObjectList.push_back(pGameObject);

	return NOERROR;
}

_int CLayer::Update_Layer(_double TimeDelta)
{
	_int		iState = 0;

	for (auto& iter = m_ObjectList.begin(); iter != m_ObjectList.end(); )
	{
		if (nullptr != (*iter))
		{
			iState = (*iter)->Update_GameObject(TimeDelta);
			if (0x80000000 & iState)
				return iState;
			if (iState == 1)
			{
				Safe_Release(*iter);
				iter = m_ObjectList.erase(iter);
			}
			else
				++iter;
		}
	}
	 
	return _int(iState);
}

_int CLayer::LateUpdate_Layer(_double TimeDelta)
{
	_int		iState = 0;

	for (auto& pGameObject : m_ObjectList)
	{
		if (nullptr != pGameObject)
		{
			iState = pGameObject->LateUpdate_GameObject(TimeDelta);
			if (0x80000000 & iState)
				return iState;
		}
	}

	return _int(iState);
}

CGameObject * CLayer::Get_ObjectFromLayer(_uint iIndex)
{
	if (iIndex > m_ObjectList.size())
		return nullptr;

	auto& iter = m_ObjectList.begin();

	for (_uint i = 0; i < iIndex; ++i)
		++iter;

	return (*iter);
}

HRESULT CLayer::Delete_ObjectFromLayer(_uint iIndex)
{
	if (iIndex > m_ObjectList.size())
		return E_FAIL;

	auto& iter = m_ObjectList.begin();

	for (_uint i = 0; i < iIndex; ++i)
		++iter;

	Safe_Release(*iter);
	m_ObjectList.erase(iter);

	return NOERROR;
}

CLayer * CLayer::Create()
{
	return new CLayer;
}

void CLayer::Free()
{
	for (auto& pGameObject : m_ObjectList)
		Safe_Release(pGameObject);

	m_ObjectList.clear();
}

