#pragma once

#include "Defines.h"
#include "Scene.h"

BEGIN(Client)

class CLoading;
class CScene_Title final : public CScene
{
private:
	explicit CScene_Title(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CScene_Title() = default;

public:
	virtual HRESULT Ready_Scene();
	virtual _int Update_Scene(_double TimeDelta);
	virtual HRESULT Render_Scene();

private:
	CLoading*		m_pLoading = nullptr;
	_bool m_bTransition = false;

private:
	HRESULT Ready_LightInfo();
	HRESULT Ready_GameObject_Prototype();
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag);
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);
	HRESULT Ready_Layer_UI(const _tchar* pLayerTag);

public:
	HRESULT Add_StaticObjectFromFile(const _tchar* pFilePath, const _tchar* pLayerTag);

public:
	static CScene_Title* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free();
};

END