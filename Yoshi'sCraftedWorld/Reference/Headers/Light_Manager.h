#pragma once

#include "Base.h"

// 여러개의 빛을 보관나다.

BEGIN(Engine)

class CLight;
class ENGINE_DLL CLight_Manager final : public CBase
{
	DECLARE_SINGLETON(CLight_Manager)
private:
	explicit CLight_Manager();
	virtual ~CLight_Manager() = default;
public:
	const D3DLIGHT9& Get_Light(_uint iIndex);
public:
	HRESULT Add_Light(LPDIRECT3DDEVICE9 pGraphic_Device, const D3DLIGHT9& LightInfo);
	HRESULT Render_Light(LPD3DXEFFECT pEffect);
private:
	list<CLight*>			m_LightList;
	typedef list<CLight*>	LIGHTLIST;
public:
	virtual void Free();
};

END