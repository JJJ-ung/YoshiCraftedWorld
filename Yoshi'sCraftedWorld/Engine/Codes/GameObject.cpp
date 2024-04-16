#include "..\Headers\GameObject.h"
#include "Management.h"

CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device(pGraphic_Device)
{
	Safe_AddRef(m_pGraphic_Device);
}

CGameObject::CGameObject(const CGameObject & rhs)
	: m_pGraphic_Device(rhs.m_pGraphic_Device)
{
	Safe_AddRef(m_pGraphic_Device);
}

_matrix CGameObject::Get_Transform(_D3DTRANSFORMSTATETYPE eType)
{
	_matrix	matTmp;

	m_pGraphic_Device->GetTransform(eType, &matTmp);

	return matTmp;
}

CComponent * CGameObject::Get_Component(const _tchar * pComponentTag)
{
	return Find_Component(pComponentTag);
}

_float CGameObject::Get_ViewZ()
{
	return m_fViewZ;
}

HRESULT CGameObject::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CGameObject::Ready_GameObject(void* pArg)
{
	return NOERROR;
}

_int CGameObject::Update_GameObject(_double TimeDelta)
{
	return _int();
}

_int CGameObject::LateUpdate_GameObject(_double TimeDelta)
{
	return _int();
}

HRESULT CGameObject::Render_GameObject()
{
	return NOERROR;
}

HRESULT CGameObject::Add_Component(_uint iSceneID, const _tchar * pPrototypeTag, const _tchar * pComponentTag, CComponent ** ppOut, void * pArg)
{
	if (nullptr != Find_Component(pComponentTag))
		return E_FAIL;

	CManagement*	pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return E_FAIL;

	Safe_AddRef(pManagement);

	CComponent* pComponent = pManagement->Clone_Component(iSceneID, pPrototypeTag, pArg);
	if (nullptr == pComponent)
		return E_FAIL;

	*ppOut = pComponent;
	Safe_AddRef(pComponent);

	m_Components.insert(COMPONENTS::value_type(pComponentTag, pComponent));
	Safe_AddRef(pComponent);

	Safe_Release(pManagement);

	Safe_Release(pComponent);

	return NOERROR;
}

HRESULT CGameObject::Compute_ViewZ(const _vec3 * pWorldPos)
{
	_matrix	matView = Get_Transform(D3DTS_VIEW);

	_vec3	vViewPos;

	m_fViewZ = D3DXVec3TransformCoord(&vViewPos, pWorldPos, &matView)->z;

	return NOERROR;
}

CComponent * CGameObject::Find_Component(const _tchar * pComponentTag)
{
	auto	iter = find_if(m_Components.begin(), m_Components.end(), CTag_Finder(pComponentTag));
	if (iter == m_Components.end())
		return nullptr;

	return iter->second;
}

void CGameObject::Free()
{
	for (auto& Pair : m_Components)
		Safe_Release(Pair.second);
	m_Components.clear();


	Safe_Release(m_pGraphic_Device);
}
