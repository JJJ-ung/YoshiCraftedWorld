#pragma once

#include "Base.h"

BEGIN(Engine)

class CComponent;
class ENGINE_DLL CGameObject abstract : public CBase
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CGameObject(const CGameObject& rhs);
	virtual ~CGameObject() = default;

public:
	_matrix Get_Transform(_D3DTRANSFORMSTATETYPE eType);
	CComponent* Get_Component(const _tchar* pComponentTag);
	_float Get_ViewZ();

public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int Update_GameObject(_double TimeDelta);
	virtual _int LateUpdate_GameObject(_double TimeDelta);
	virtual HRESULT Render_GameObject();

protected:
	LPDIRECT3DDEVICE9		m_pGraphic_Device = nullptr;

protected:
	unordered_map<const _tchar*, CComponent*>			m_Components;
	typedef unordered_map<const _tchar*, CComponent*>	COMPONENTS;

protected:
	_float		m_fViewZ = 0.f;

protected:
	HRESULT Add_Component(_uint iSceneID, const _tchar* pPrototypeTag, const _tchar* pComponentTag, CComponent** ppOut, void* pArg = nullptr);
	HRESULT Compute_ViewZ(const _vec3* pWorldPos);

private:
	CComponent* Find_Component(const _tchar* pComponentTag);

public:
	virtual CGameObject* Clone_GameObject(void* pArg) = 0;
	virtual void Free();
};

END