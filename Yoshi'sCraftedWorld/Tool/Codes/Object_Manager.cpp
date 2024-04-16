#include "stdafx.h"
#include "..\Headers\Object_Manager.h"
#include "GameObject.h"

IMPLEMENT_SINGLETON(CObject_Manager)

CObject_Manager::CObject_Manager()
{
	for (size_t i = 0; i < OBJ_END; ++i)
		m_bRender[i] = true;
}

HRESULT CObject_Manager::Update_Object_Manager(_double TimeDelta)
{
	for (size_t i = 0; i < OBJ_END; ++i)
	{
		if (m_bRender[i])
		{
			for (auto& pObj : m_GameObjects[i])
			{
				if (FAILED(pObj->Update_GameObject(TimeDelta)))
					return E_FAIL;
			}
		}
	}

	return NOERROR;
}

HRESULT CObject_Manager::LateUpdate_Object_Manager(_double TimeDelta)
{
	for (size_t i = 0; i < OBJ_END; ++i)
	{
		if (m_bRender[i])
		{
			for (auto& pObj : m_GameObjects[i])
			{
				if (FAILED(pObj->LateUpdate_GameObject(TimeDelta)))
					return E_FAIL;
			}
		}
	}

	return NOERROR;
}

HRESULT CObject_Manager::Render_Object_Manager()
{
	for (size_t i = 0; i < OBJ_END; ++i)
	{
		if (m_bRender[i])
		{
			for (auto& pObj : m_GameObjects[i])
			{
				if (FAILED(pObj->Render_GameObject()))
					return E_FAIL;
			}
		}
	}

	return NOERROR;
}

HRESULT CObject_Manager::Add_GameObject(OBJECT_TYPE eType, CGameObject * pObject)
{
	if (pObject == nullptr)
		return E_FAIL;

	m_GameObjects[eType].emplace_back(pObject);

	return NOERROR;
}

HRESULT CObject_Manager::Set_ObjectsWire(bool bWire)
{
	if (bWire)
	{
		for (auto& pObj : m_GameObjects[OBJECT])
			pObj->m_iShaderPass = 0;
	}
	else
	{
		for (auto& pObj : m_GameObjects[OBJECT])
			pObj->m_iShaderPass = 1;
	}

	return NOERROR;
}

HRESULT CObject_Manager::Set_GimmicksWire(bool bWire)
{
	return NOERROR;
}

CGameObject * CObject_Manager::Get_Object(OBJECT_TYPE eType, _uint iIndex)
{
	if (iIndex > m_GameObjects[eType].size())
		return nullptr;

	return m_GameObjects[eType][iIndex];
}

HRESULT CObject_Manager::Delete_Object(OBJECT_TYPE eType, _uint iIndex)
{
	if (iIndex > m_GameObjects[eType].size())
		return E_FAIL;

	auto iter = m_GameObjects[eType].begin();
	iter += iIndex;
	(*iter)->Free();
	Safe_Delete(*iter);
	m_GameObjects[eType].erase(iter);

	return NOERROR;
}

HRESULT CObject_Manager::Reset_Content(OBJECT_TYPE eType)
{
	if (m_GameObjects[eType].empty())
		return NOERROR;

	for (auto& pObj : m_GameObjects[eType])
	{
		pObj->Free();
		Safe_Delete(pObj);
	}
	m_GameObjects[eType].clear();

	return NOERROR;
}

HRESULT CObject_Manager::Reset_ShaderPath(OBJECT_TYPE eType, int iPass)
{
	for (auto& pObj : m_GameObjects[eType])
		pObj->m_iShaderPass = iPass;

	return NOERROR;
}

void CObject_Manager::Free()
{
	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for (auto& pObj : m_GameObjects[i])
		{
			pObj->Free();
			Safe_Delete(pObj);
		}

		m_GameObjects[i].clear();
	}
}
