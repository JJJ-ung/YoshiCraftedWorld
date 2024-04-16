#pragma once

#include "Defines.h"
#include "Scene.h"

BEGIN(Client)

class CLoading;
class CScene_Logo final : public CScene
{
private:
	explicit CScene_Logo(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CScene_Logo() = default;

public:
	virtual HRESULT Ready_Scene();
	virtual _int Update_Scene(_double TimeDelta);
	virtual HRESULT Render_Scene();

private:
	CLoading*		m_pLoading = nullptr;

private:
	HRESULT Ready_GameObject_Prototype();
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);

public:
	static CScene_Logo* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free();
};

END