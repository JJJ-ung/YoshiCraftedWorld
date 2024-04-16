#include "..\Headers\Object_Manager.h"
#include "Layer.h"
#include "GameObject.h"

IMPLEMENT_SINGLETON(CObject_Manager)

CObject_Manager::CObject_Manager()
{
}

CComponent * CObject_Manager::Get_Component(_uint iSceneID, const _tchar * pLayerTag, const _tchar * pComponentTag, _uint iIndex)
{
	if (m_iNumScene <= iSceneID ||
		nullptr == m_pLayers)
		return nullptr;

	CLayer*	pLayer = Find_Layer(iSceneID, pLayerTag);
	if (nullptr == pLayer)
		return nullptr;

	return pLayer->Get_Component(pComponentTag, iIndex);
}

HRESULT CObject_Manager::Reserve_Object_Manager(_uint iNumScene)
{
	if (nullptr != m_pLayers)
		return E_FAIL;

	m_pLayers = new LAYERS[iNumScene];

	m_iNumScene = iNumScene;

	return NOERROR;
}

HRESULT CObject_Manager::Add_Prototype(const _tchar* pPrototypeTag, CGameObject * pPrototype)
{
	if (nullptr == pPrototype)
		return E_FAIL;

	if (nullptr != Find_Prototype(pPrototypeTag))
		return E_FAIL;

	m_Prototypes.insert(PROTOTYPES::value_type(pPrototypeTag, pPrototype));

	return NOERROR;
}

HRESULT CObject_Manager::Add_GameObjectToLayer(_uint iSceneID, const _tchar * pLayerTag, const _tchar * pPrototypeTag, void* pArg)
{
	if (m_iNumScene <= iSceneID || 
		nullptr == m_pLayers)
		return E_FAIL;

	CGameObject*	pPrototype = Find_Prototype(pPrototypeTag);
	if (nullptr == pPrototype)
		return E_FAIL;

	CGameObject*	pGameObject = pPrototype->Clone_GameObject(pArg);
	if (nullptr == pGameObject)
		return E_FAIL;

	CLayer*	pLayer = Find_Layer(iSceneID, pLayerTag);

	if (nullptr == pLayer)
	{
		pLayer = CLayer::Create();

		pLayer->Add_GameObject(pGameObject);

		m_pLayers[iSceneID].insert(LAYERS::value_type(pLayerTag, pLayer));
	}

	else
		pLayer->Add_GameObject(pGameObject);

	return NOERROR;
}

CGameObject * CObject_Manager::Get_ObjectFromLayer(_uint iSceneID, const _tchar * pLayerTag, _uint iIndex)
{
	if (m_iNumScene <= iSceneID ||
		nullptr == m_pLayers)
		return nullptr;

	CGameObject* pOut = nullptr;
	CLayer*	pLayer = Find_Layer(iSceneID, pLayerTag);

	if (nullptr == pLayer)
		return nullptr;
	else
		return pLayer->Get_ObjectFromLayer(iIndex);
}

HRESULT CObject_Manager::Delete_ObjectFromLayer(_uint iSceneID, const _tchar * pLayerTag, _uint iIndex)
{
	if (m_iNumScene <= iSceneID ||
		nullptr == m_pLayers)
		return E_FAIL;

	CGameObject* pOut = nullptr;
	CLayer*	pLayer = Find_Layer(iSceneID, pLayerTag);

	return pLayer->Delete_ObjectFromLayer(iIndex);
}

list<CGameObject*>& CObject_Manager::Get_Layer(_uint iSceneID, const _tchar * pLayerTag)
{
	CLayer*	pLayer = Find_Layer(iSceneID, pLayerTag);

	return pLayer->Get_Layer();
}

_int CObject_Manager::Update_Object_Manager(_double TimeDelta)
{
	_int iState = 0;

	for (size_t i = 0; i < m_iNumScene; ++i)
	{
		for (auto& Pair : m_pLayers[i])
		{
			if (nullptr != Pair.second)
			{
				iState = Pair.second->Update_Layer(TimeDelta);
				if (0x80000000 & iState)
					return iState;
			}			
		}
	}

	return _int(iState);
}

_int CObject_Manager::LateUpdate_Object_Manager(_double TimeDelta)
{
	_int iState = 0;

	for (size_t i = 0; i < m_iNumScene; ++i)
	{
		for (auto& Pair : m_pLayers[i])
		{
			if (nullptr != Pair.second)
			{
				iState = Pair.second->LateUpdate_Layer(TimeDelta);
				if (0x80000000 & iState)
					return iState;
			}
		}
	}

	return _int(iState);
}

HRESULT CObject_Manager::Clear_Object_Manager(_uint iSceneID)
{
	if (m_iNumScene <= iSceneID)
		return E_FAIL;

	for (auto& Pair : m_pLayers[iSceneID])
	{
		Safe_Release(Pair.second);
	}

	m_pLayers[iSceneID].clear();

	return NOERROR;
}

CGameObject * CObject_Manager::Find_Prototype(const _tchar * pPrototypeTag)
{
	auto	iter = find_if(m_Prototypes.begin(), m_Prototypes.end(), CTag_Finder(pPrototypeTag));

	if (iter == m_Prototypes.end())
		return nullptr;

	return iter->second;
}

CLayer * CObject_Manager::Find_Layer(_uint iSceneID, const _tchar * pLayerTag)
{
	if (m_iNumScene <= iSceneID)
		return nullptr;

	auto	iter = find_if(m_pLayers[iSceneID].begin(), m_pLayers[iSceneID].end(), CTag_Finder(pLayerTag));

	if (iter == m_pLayers[iSceneID].end())
		return nullptr;

	return iter->second;
}

void CObject_Manager::Free()
{
	for (size_t i = 0; i < m_iNumScene; ++i)
	{
		for (auto& Pair : m_pLayers[i])		
			Safe_Release(Pair.second);
		m_pLayers[i].clear();		
	}

	Safe_Delete_Array(m_pLayers);

	for (auto& Pair : m_Prototypes)
		Safe_Release(Pair.second);

	m_Prototypes.clear();

}
