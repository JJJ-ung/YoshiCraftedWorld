#include "..\Headers\Component_Manager.h"

IMPLEMENT_SINGLETON(CComponent_Manager)

CComponent_Manager::CComponent_Manager()
{

}

HRESULT CComponent_Manager::Reserve_Component_Manager(_uint iNumScene)
{
	if (nullptr != m_pPrototypes)
		return E_FAIL;

	m_pPrototypes = new PROTOTYPES[iNumScene];

	m_iNumScene = iNumScene;
	
	return NOERROR;
}

HRESULT CComponent_Manager::Add_Prototype(_uint iSceneID, wstring pPrototypeTag, CComponent * pPrototype)
{
	if (nullptr == m_pPrototypes ||
		m_iNumScene <= iSceneID ||
		nullptr == pPrototype)
		return E_FAIL;

	if (nullptr != Find_Prototype(iSceneID, pPrototypeTag))
		return E_FAIL;

	m_pPrototypes[iSceneID].insert(PROTOTYPES::value_type(pPrototypeTag, pPrototype));	

	return NOERROR;
}

CComponent * CComponent_Manager::Clone_Component(_uint iSceneID, wstring pPrototypeTag, void * pArg)
{
	if (nullptr == m_pPrototypes ||
		m_iNumScene <= iSceneID)
		return nullptr;

	CComponent*	pPrototype = Find_Prototype(iSceneID, pPrototypeTag);
	if (nullptr == pPrototype)
		return nullptr;

	return pPrototype->Clone_Component(pArg);	
}

HRESULT CComponent_Manager::Clear_Component_Manager(_uint iSceneID)
{
	if (m_iNumScene <= iSceneID)
		return E_FAIL;

	for (auto& Pair : m_pPrototypes[iSceneID])
	{
		Safe_Release(Pair.second);
	}

	m_pPrototypes[iSceneID].clear();

	return NOERROR;
}

CComponent * CComponent_Manager::Find_Prototype(_uint iSceneID, wstring pPrototypeTag)
{
	if (m_iNumScene <= iSceneID)
		return nullptr;

	auto	iter = m_pPrototypes[iSceneID].find(pPrototypeTag);

	if (iter == m_pPrototypes[iSceneID].end())
		return nullptr;

	return iter->second;
}

void CComponent_Manager::Free()
{
	for (size_t i = 0; i < m_iNumScene; ++i)
	{
		for (auto& Pair : m_pPrototypes[i])
			Safe_Release(Pair.second);
		m_pPrototypes[i].clear();		
	}
	Safe_Delete_Array(m_pPrototypes);	
}
