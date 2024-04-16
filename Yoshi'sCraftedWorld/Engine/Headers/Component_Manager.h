#pragma once

#include "Base.h"
#include "Shader.h"
#include "Texture.h"
#include "Renderer.h"
#include "Transform.h"
#include "Mesh_Static.h"
#include "Buffer_RcTex.h"
#include "Buffer_Terrain.h"

BEGIN(Engine)

class CComponent_Manager final : public CBase
{
	DECLARE_SINGLETON(CComponent_Manager)
private:
	explicit CComponent_Manager();
	virtual ~CComponent_Manager() = default;
public:
	HRESULT Reserve_Component_Manager(_uint iNumScene);
	HRESULT Add_Prototype(_uint iSceneID, wstring pPrototypeTag, CComponent* pPrototype);
	CComponent* Clone_Component(_uint iSceneID, wstring pPrototypeTag, void* pArg);
	HRESULT Clear_Component_Manager(_uint iSceneID);
private:
	unordered_map<wstring, CComponent*>*			m_pPrototypes = nullptr;
	typedef unordered_map<wstring, CComponent*>	PROTOTYPES;

	_uint			m_iNumScene = 0;
private:
	CComponent* Find_Prototype(_uint iSceneID, wstring pPrototypeTag);

public:
	virtual void Free();
};

END