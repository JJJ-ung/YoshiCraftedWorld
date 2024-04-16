#include "stdafx.h"
#include "..\Headers\Effect_Hit.h"
#include "Management.h"

CEffect_Hit::CEffect_Hit(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CEffect_Hit::CEffect_Hit(const CEffect_Hit & rhs)
	: CGameObject(rhs)
{
}

HRESULT CEffect_Hit::Ready_GameObject_Prototype()
{
	return E_NOTIMPL;
}

HRESULT CEffect_Hit::Ready_GameObject(void * pArg)
{
	return E_NOTIMPL;
}

_int CEffect_Hit::Update_GameObject(_double TimeDelta)
{
	return _int();
}

_int CEffect_Hit::LateUpdate_GameObject(_double TimeDelta)
{
	return _int();
}

HRESULT CEffect_Hit::Render_GameObject()
{
	return E_NOTIMPL;
}

HRESULT CEffect_Hit::Add_Component()
{
	return E_NOTIMPL;
}

HRESULT CEffect_Hit::SetUp_ConstantTable()
{
	return E_NOTIMPL;
}

CEffect_Hit * CEffect_Hit::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	return nullptr;
}

CGameObject * CEffect_Hit::Clone_GameObject(void * pArg)
{
	return nullptr;
}

void CEffect_Hit::Free()
{
}
