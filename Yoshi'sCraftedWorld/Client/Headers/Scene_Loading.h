#pragma once

#include "Defines.h"
#include "Scene.h"

BEGIN(Client)

class CLoading;
class CScene_Loading final : public CScene
{
private:
	explicit CScene_Loading(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CScene_Loading() = default;

public:
	virtual HRESULT Ready_Scene(SCENEID eBeforeScene, SCENEID eNextScene);
	virtual _int Update_Scene(_double TimeDelta);
	virtual HRESULT Render_Scene();

private:
	CLoading*		m_pLoading = nullptr;
	SCENEID			m_eBeforeScene;
	SCENEID			m_eNextScene;

private:
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);

public:
	static CScene_Loading* Create(LPDIRECT3DDEVICE9 pGraphic_Device, SCENEID eBeforeScene, SCENEID eNextScene);
	virtual void Free();
};

END