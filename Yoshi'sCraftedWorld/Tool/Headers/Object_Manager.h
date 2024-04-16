#pragma once

class CTerrain;
class CGameObject;
class CObject_Manager final
{
	DECLARE_SINGLETON(CObject_Manager)

public:
	enum OBJECT_TYPE { MOUSE, BACKGROUND, NAVIGATION, OBJECT, GIMMICK, UNIT, PARTICLE, TRIGGER, OBJ_END };

public:
	explicit CObject_Manager();
	virtual ~CObject_Manager() = default;

public:
	HRESULT Update_Object_Manager(_double TimeDelta);
	HRESULT LateUpdate_Object_Manager(_double TimeDelta);
	HRESULT Render_Object_Manager();

public:
	HRESULT Add_GameObject(OBJECT_TYPE eType, CGameObject* pObject);

public:
	HRESULT Set_ObjectsWire(bool bWire);
	HRESULT Set_GimmicksWire(bool bWire);

public:
	CGameObject*	Get_Object(OBJECT_TYPE eType, _uint iIndex);
	HRESULT				Delete_Object(OBJECT_TYPE eType, _uint iIndex);
	HRESULT				Reset_Content(OBJECT_TYPE eType);

public:
	HRESULT				Reset_ShaderPath(OBJECT_TYPE eType, int iPass);

public:
	vector<CGameObject*>					m_GameObjects[OBJ_END];
	typedef vector<CGameObject*>	GAMEOBJECTS;

public:
	virtual void Free();

public:
	CTerrain* m_pTerrain = nullptr;
	bool	m_bRender[OBJ_END];
};