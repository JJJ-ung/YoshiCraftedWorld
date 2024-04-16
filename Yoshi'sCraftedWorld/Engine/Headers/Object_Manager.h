#pragma once

#include "Base.h"

BEGIN(Engine)

class CLayer;
class CComponent;
class CGameObject;
class CObject_Manager final : public CBase
{
	DECLARE_SINGLETON(CObject_Manager)

private:
	explicit CObject_Manager();
	virtual ~CObject_Manager() = default;

public:
	CComponent* Get_Component(_uint iSceneID, const _tchar* pLayerTag, const _tchar* pComponentTag, _uint iIndex);

public:
	HRESULT Reserve_Object_Manager(_uint iNumScene);
	HRESULT Add_Prototype(const _tchar* pPrototypeTag, CGameObject* pPrototype);
	HRESULT Add_GameObjectToLayer(_uint iSceneID, const _tchar* pLayerTag, const _tchar* pPrototypeTag, void* pArg);

public:
	CGameObject* Get_ObjectFromLayer(_uint iSceneID, const _tchar* pLayerTag, _uint iIndex);
	HRESULT Delete_ObjectFromLayer(_uint iSceneID, const _tchar* pLayerTag, _uint iIndex);
	list<CGameObject*>& Get_Layer(_uint iSceneID, const _tchar* pLayerTag);

public:
	_int Update_Object_Manager(_double TimeDelta);
	_int LateUpdate_Object_Manager(_double TimeDelta);
	HRESULT Clear_Object_Manager(_uint iSceneID);

private:
	unordered_map<const _tchar*, CGameObject*>			m_Prototypes;
	typedef unordered_map<const _tchar*, CGameObject*>	PROTOTYPES;

private:
	unordered_map<const _tchar*, CLayer*>*		m_pLayers = nullptr;
	typedef unordered_map<const _tchar*, CLayer*>	LAYERS;

	_uint		m_iNumScene = 0;

private:
	CGameObject* Find_Prototype(const _tchar* pPrototypeTag);
	CLayer* Find_Layer(_uint iSceneID, const _tchar* pLayerTag);

public:
	virtual void Free();
};

END