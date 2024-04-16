#pragma once

#include "Defines.h"
#include "Scene.h"

BEGIN(Client)

class CLoading_Screen;
class CScene_Train : public CScene
{
public:
	explicit CScene_Train(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CScene_Train() = default;

public:
	virtual HRESULT Ready_Scene();
	virtual _int Update_Scene(_double TimeDelta);
	virtual HRESULT Render_Scene();

private:
	HRESULT Ready_LightInfo();
	HRESULT Ready_GameObject_Prototype();
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag);
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Object(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Monster(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Flower(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Gimmick(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Trigger(const _tchar* pLayerTag);
	HRESULT Ready_Layer_UI(const _tchar* pLayerTag);

public:
	HRESULT Add_StaticObjectFromFile(const _tchar* pFilePath, const _tchar* pLayerTag);
	HRESULT Add_FlowerFromFile(const _tchar* pFilePath, const _tchar* pLayerTag);
	HRESULT Add_MonsterFromFile(const _tchar* pFilePath, const _tchar* pLayerTag);

private:
	CLoading_Screen* m_pLoadingScreen;
	_bool m_bLoadScreen = true;

public:
	static CScene_Train* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free();
};

END