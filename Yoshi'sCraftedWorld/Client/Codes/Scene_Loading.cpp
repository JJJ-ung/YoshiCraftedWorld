#include "stdafx.h"
#include "..\Headers\Scene_Loading.h"
#include "Management.h"
#include "Loading.h"
#include "Loading_Screen.h"

#include "Scene_WorldMap.h"
#include "Scene_Train.h"

USING(Client)

CScene_Loading::CScene_Loading(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CScene(pGraphic_Device)
{
}

HRESULT CScene_Loading::Ready_Scene(SCENEID eBeforeScene, SCENEID eNextScene)
{
	m_eBeforeScene = eBeforeScene;
	m_eNextScene = eNextScene;

	if (FAILED(Ready_Layer_BackGround(L"Layer_BackGround")))
		return E_FAIL;

	m_pLoading = CLoading::Create(m_pGraphic_Device, eNextScene);
	if (nullptr == m_pLoading)
		return E_FAIL;

	return NOERROR;
}

_int CScene_Loading::Update_Scene(_double TimeDelta)
{
	if (nullptr == m_pLoading)
		return -1;

	CManagement*	pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return -1;

	Safe_AddRef(pManagement);

	if (true == m_pLoading->Get_Finish())
	{
		switch (m_eNextScene)
		{
		case SCENE_STATIC:
			break;
		case SCENE_LOGO:
			break;
		case SCENE_TITLE:
			break;
		case SCENE_LOAD:
			break;
		case SCENE_WORLDMAP:
		{
			CScene_WorldMap* pScene = CScene_WorldMap::Create(m_pGraphic_Device);
			if (nullptr == pScene)
				goto except;
			if (FAILED(pManagement->SetUp_CurrentScene(pScene)))
			{
				Safe_Release(pScene);
				goto except;
			}
			Safe_Release(pScene);
			break;
		}
		case SCENE_TRAIN:
		{
			CScene_Train* pScene = CScene_Train::Create(m_pGraphic_Device);
			if (nullptr == pScene)
				goto except;
			if (FAILED(pManagement->SetUp_CurrentScene(pScene)))
			{
				Safe_Release(pScene);
				goto except;
			}
			Safe_Release(pScene);
			break;
		}
		case SCENE_END:
			break;
		}
	}

	Safe_Release(pManagement);
	return _int(0);

except:
	Safe_Release(pManagement);
	return -1;
}

HRESULT CScene_Loading::Render_Scene()
{
	return NOERROR;
}

HRESULT CScene_Loading::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CManagement*		pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return E_FAIL;

	Safe_AddRef(pManagement);

	if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_LOAD, L"Load Transition", L"GameObject_LoadScreen", (void*)&CLoading_Screen::LOADING_SCREEN(SCENE_LOAD, SCENE_LOAD, CLoading_Screen::LOAD_PROGRESS))))
		return E_FAIL;

	Safe_Release(pManagement);

	return NOERROR;
}

CScene_Loading * CScene_Loading::Create(LPDIRECT3DDEVICE9 pGraphic_Device, SCENEID eBeforeScene, SCENEID eNextScene)
{
	CScene_Loading*		pInstance = new CScene_Loading(pGraphic_Device);

	if (FAILED(pInstance->Ready_Scene(eBeforeScene, eNextScene)))
	{
		MSG_BOX("Failed To Create CScene_Loading Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CScene_Loading::Free()
{
	Safe_Release(m_pLoading);

	CManagement* pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return;

	Safe_AddRef(pManagement);

	if (FAILED(pManagement->Clear_Scene(SCENE_LOAD)))
		MSG_BOX("Fail To Clear Scene");

	Safe_Release(pManagement);

	CScene::Free();
}
