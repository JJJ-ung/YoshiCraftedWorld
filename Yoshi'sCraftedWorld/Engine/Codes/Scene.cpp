#include "..\Headers\Scene.h"


CScene::CScene(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device(pGraphic_Device)
{
	_uint check = Safe_AddRef(m_pGraphic_Device);
}

HRESULT CScene::Ready_Scene()
{
	return NOERROR;
}

_int CScene::Update_Scene(_double TimeDelta)
{
	return _int();
}

HRESULT CScene::Render_Scene()
{
	return NOERROR;
}

void CScene::Free()
{
	_uint check = Safe_Release(m_pGraphic_Device);	
}
