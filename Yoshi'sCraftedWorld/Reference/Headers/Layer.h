#pragma once

#include "Base.h"

BEGIN(Engine)

class CComponent;
class CGameObject;
class CLayer final : public CBase
{
private:
	explicit CLayer();
	virtual ~CLayer() = default;

public:
	CComponent* Get_Component(const _tchar* pComponentTag, _uint iIndex);

public:
	HRESULT Add_GameObject(CGameObject* pGameObject);
	_int Update_Layer(_double TimeDelta);
	_int LateUpdate_Layer(_double TimeDelta);
	CGameObject* Get_ObjectFromLayer(_uint iIndex);
	HRESULT Delete_ObjectFromLayer(_uint iIndex);

	list<CGameObject*>& Get_Layer() { return m_ObjectList; }

private:
	list<CGameObject*>			m_ObjectList;
	typedef list<CGameObject*>	OBJECTLIST;

public:
	static CLayer* Create();
	virtual void Free();
};

END