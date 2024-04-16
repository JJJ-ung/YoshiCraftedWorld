#include "..\Headers\Management.h"
#include "Graphic_Device.h"
#include "Input_Device.h"
#include "Light_Manager.h"
#include "Timer_Manager.h"
#include "Target_Manager.h"
#include "Scene.h"

_bool		g_bRenderTarget = false;
_bool		g_bRenderCollider = false;

IMPLEMENT_SINGLETON(CManagement)

CManagement::CManagement()
{

}

CComponent * CManagement::Get_Component(_uint iSceneID, const _tchar * pLayerTag, const _tchar * pComponentTag, _uint iIndex)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Get_Component(iSceneID, pLayerTag, pComponentTag, iIndex);
}

HRESULT CManagement::Ready_Engine(_uint iNumScene)
{
	// For.Object_Manager
	m_pObject_Manager = CObject_Manager::GetInstance();
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	Safe_AddRef(m_pObject_Manager);

	if (FAILED(m_pObject_Manager->Reserve_Object_Manager(iNumScene)))
		return E_FAIL;

	// For.Component_Manager
	m_pComponent_Manager = CComponent_Manager::GetInstance();
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	Safe_AddRef(m_pComponent_Manager);

	
	if (FAILED(m_pComponent_Manager->Reserve_Component_Manager(iNumScene)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CManagement::SetUp_CurrentScene(CScene * pScene)
{
	if (nullptr == pScene)
		return E_FAIL;

	if (0 != Safe_Release(m_pCurrentScene))
	{
		MSG_BOX("Failed To SetUp MainApp Instance");

		Safe_Release(pScene);

		return E_FAIL;
	}

	m_pCurrentScene = pScene;

	Safe_AddRef(m_pCurrentScene);

	return NOERROR;
}

_int CManagement::Update_Management(_double TimeDelta)
{
	if (nullptr == m_pCurrentScene || 
		nullptr == m_pObject_Manager)
		return -1;

	_int		iState = 0;

	iState = m_pObject_Manager->Update_Object_Manager(TimeDelta);
	if (0x80000000 & iState)
		return -1;

	iState = m_pObject_Manager->LateUpdate_Object_Manager(TimeDelta);
	if (0x80000000 & iState)
		return -1;

	iState = m_pCurrentScene->Update_Scene(TimeDelta);
	if (0x80000000 & iState)
		return -1;

	return iState;
}

HRESULT CManagement::Render_Management()
{
	if (nullptr == m_pCurrentScene)
		return -1;

	return m_pCurrentScene->Render_Scene();
}

HRESULT CManagement::Clear_Scene(_uint iNumScene)
{
	if (nullptr == m_pObject_Manager ||
		nullptr == m_pComponent_Manager)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Clear_Object_Manager(iNumScene)))
		return E_FAIL;

	if (FAILED(m_pComponent_Manager->Clear_Component_Manager(iNumScene)))
		return E_FAIL;


	return NOERROR;
}

HRESULT CManagement::Add_GameObject_Prototype(const _tchar * pPrototypeTag, CGameObject * pPrototype)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_Prototype(pPrototypeTag, pPrototype);
}

HRESULT CManagement::Add_GameObjectToLayer(_uint iSceneID, const _tchar * pLayerTag, const _tchar * pPrototypeTag, void * pArg)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_GameObjectToLayer(iSceneID, pLayerTag, pPrototypeTag, pArg);
}

HRESULT CManagement::Add_Component_Prototype(_uint iSceneID, const wstring pPrototypeTag, CComponent * pPrototype)
{
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	return m_pComponent_Manager->Add_Prototype(iSceneID, pPrototypeTag, pPrototype);
}

CComponent * CManagement::Clone_Component(_uint iSceneID, const wstring pPrototypeTag, void * pArg)
{
	if (nullptr == m_pComponent_Manager)
		return nullptr;

	return m_pComponent_Manager->Clone_Component(iSceneID, pPrototypeTag, pArg);
}

CGameObject * CManagement::Get_ObjectFromLayer(_uint iSceneID, const _tchar * pLayerTag, _uint iIndex)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Get_ObjectFromLayer(iSceneID, pLayerTag, iIndex);
}

list<CGameObject*>& CManagement::Get_Layer(_uint iSceneID, const _tchar * pLayerTag)
{
	return m_pObject_Manager->Get_Layer(iSceneID, pLayerTag);
}

HRESULT CManagement::Delete_ObjectFromLayer(_uint iSceneID, const _tchar * pLayerTag, _uint iIndex)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Delete_ObjectFromLayer(iSceneID, pLayerTag, iIndex);
}


_ulong CManagement::Release_Engine()
{
	_ulong		dwRefCnt = 0;

	if (dwRefCnt = CManagement::GetInstance()->DestroyInstance())
		MSG_BOX("Failed To Release CManagement Instance");

	if (dwRefCnt = CObject_Manager::GetInstance()->DestroyInstance())
		MSG_BOX("Failed To Release CObject_Manager Instance");

	if (dwRefCnt = CComponent_Manager::GetInstance()->DestroyInstance())
		MSG_BOX("Failed To Release CComponent_Manager Instance");

	if (dwRefCnt = CTimer_Manager::GetInstance()->DestroyInstance())
		MSG_BOX("Failed To Release CTimer_Manager Instance");

	if (dwRefCnt = CLight_Manager::GetInstance()->DestroyInstance())
		MSG_BOX("Failed To Release CLight_Manager Instance");

	if (dwRefCnt = CInput_Device::GetInstance()->DestroyInstance())
		MSG_BOX("Failed To Release CInput_Device Instance");

	if (dwRefCnt = CTarget_Manager::GetInstance()->DestroyInstance())
		MSG_BOX("Failed To Release CTarget_Manager Instance");

	if (dwRefCnt = CGraphic_Device::GetInstance()->DestroyInstance())
		MSG_BOX("Failed To Release MainApp Instance");

	return _ulong(dwRefCnt);
}
 
void CManagement::Free()
{
	Safe_Release(m_pObject_Manager);
	Safe_Release(m_pComponent_Manager);
	Safe_Release(m_pCurrentScene);
}
