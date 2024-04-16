#pragma once

#include "Base.h"
#include "Object_Manager.h"
#include "Component_Manager.h"

BEGIN(Engine)

class CScene;
class ENGINE_DLL CManagement final : public CBase
{
	DECLARE_SINGLETON(CManagement)

private:
	explicit CManagement();
	virtual ~CManagement() = default;

public:
	CComponent* Get_Component(_uint iSceneID, const _tchar* pLayerTag, const _tchar* pComponentTag, _uint iIndex = 0);

public:
	HRESULT Ready_Engine(_uint iNumScene);
	HRESULT SetUp_CurrentScene(CScene* pScene);
	_int Update_Management(_double TimeDelta);
	HRESULT Render_Management();
	HRESULT Clear_Scene(_uint iNumScene);

public: // For.Object_Manager
	HRESULT Add_GameObject_Prototype(const _tchar* pPrototypeTag, CGameObject* pPrototype);
	HRESULT Add_GameObjectToLayer(_uint iSceneID, const _tchar* pLayerTag, const _tchar* pPrototypeTag, void* pArg = nullptr);

public: // For.Component_Manager
	HRESULT Add_Component_Prototype(_uint iSceneID, const wstring pPrototypeTag, CComponent* pPrototype);
	CComponent* Clone_Component(_uint iSceneID, const wstring pPrototypeTag, void* pArg = nullptr);

public:
	CGameObject* Get_ObjectFromLayer(_uint iSceneID, const _tchar* pLayerTag, _uint iIndex = 0);
	list<CGameObject*>& Get_Layer(_uint iSceneID, const _tchar* pLayerTag);
	HRESULT Delete_ObjectFromLayer(_uint iSceneID, const _tchar* pLayerTag, _uint iIndex);

public:
	static _ulong Release_Engine();

private:
	CScene*				m_pCurrentScene = nullptr;
	CObject_Manager*	m_pObject_Manager = nullptr;
	CComponent_Manager*	m_pComponent_Manager = nullptr;

public:
	virtual void Free();
};

END