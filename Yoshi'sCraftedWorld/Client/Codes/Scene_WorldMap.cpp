#include "stdafx.h"
#include "..\Headers\Scene_WorldMap.h"
#include "Management.h"
#include "Loading.h"
#include "Light_Manager.h"
#include "Environment.h"
#include "Camera_Free.h"
#include "Camera_Follow.h"
#include "Input_Device.h"
#include "Loading_Screen.h"
#include "Scene_Loading.h"

USING(Client)

CScene_WorldMap::CScene_WorldMap(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CScene(pGraphic_Device)
{
}

HRESULT CScene_WorldMap::Ready_Scene()
{
	if (FAILED(Ready_LightInfo()))
		return E_FAIL;

	if (FAILED(Ready_GameObject_Prototype()))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(L"Layer_BackGround")))
		return E_FAIL;

	if (FAILED(Ready_Layer_Object(L"Layer_Object")))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(L"Layer_Camera")))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI(L"Layer_UI")))
		return E_FAIL;

	return NOERROR;
}

_int CScene_WorldMap::Update_Scene(_double TimeDelta)
{
	return _int();
}

HRESULT CScene_WorldMap::Render_Scene()
{
	CManagement*	pManagement = CManagement::GetInstance();
	CInput_Device* pInputDevice = CInput_Device::GetInstance();

	Safe_AddRef(pManagement);
	Safe_AddRef(pInputDevice);

	if (m_bLoadScreen)
	{
		if (m_pLoadingScreen->Get_IsAnimationEnd())
		{
			if (FAILED(pManagement->Delete_ObjectFromLayer(SCENE_WORLDMAP, L"Layer_UI", 0)))
				return E_FAIL;

			m_bLoadScreen = false;
		}
	}

	if (pInputDevice->Key_Down(KEY_ENTER))
	{
		if (!m_bTransition)
		{
			if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_WORLDMAP, L"Layer_Transition", L"GameObject_LoadScreen", (void*)&CLoading_Screen::LOADING_SCREEN(SCENE_WORLDMAP, SCENE_WORLDMAP, CLoading_Screen::LOAD_IN))))
				return -1;
			m_bTransition = true;
		}
	}

	if (m_bTransition)
	{
		CLoading_Screen* pScreen = (CLoading_Screen*)pManagement->Get_ObjectFromLayer(SCENE_WORLDMAP, L"Layer_Transition", 0);
		if (pScreen->Get_IsAnimationEnd())
		{
			CScene_Loading* pScene = CScene_Loading::Create(m_pGraphic_Device, SCENE_WORLDMAP, SCENE_TRAIN);
			if (pScene == nullptr)
				goto except;

			if (FAILED(pManagement->SetUp_CurrentScene(pScene)))
			{
				Safe_Release(pScene);
				goto except;
			}

			Safe_Release(pScene);
		}
	}

	Safe_Release(pInputDevice);
	Safe_Release(pManagement);

	return NOERROR;

except:
	Safe_Release(pInputDevice);
	Safe_Release(pManagement);
	return E_FAIL;
}

HRESULT CScene_WorldMap::Ready_LightInfo()
{
	CLight_Manager*	pLight_Manager = CLight_Manager::GetInstance();
	if (nullptr == pLight_Manager)
		return E_FAIL;

	Safe_AddRef(pLight_Manager);

	// For.Directional
	D3DLIGHT9		LightInfo;
	ZeroMemory(&LightInfo, sizeof(D3DLIGHT9));

	LightInfo.Type = D3DLIGHT_DIRECTIONAL;
	LightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Direction = _vec3(1.f, -1.f, 1.f);

	if (FAILED(pLight_Manager->Add_Light(m_pGraphic_Device, LightInfo)))
		return E_FAIL;

	Safe_Release(pLight_Manager);

	return NOERROR;
}

HRESULT CScene_WorldMap::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CScene_WorldMap::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CManagement*		pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return E_FAIL;

	Safe_AddRef(pManagement);

	// GameObject_Camera_Free
	CCamera_Free::CAMERADESC	CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera_Free::CAMERADESC));

	CameraDesc.vEye = _vec3(0.0f, 15.0f, -10.0f);
	CameraDesc.vAt = _vec3(0.0f, 0.0f, 0.0f);
	CameraDesc.vUp = _vec3(0.f, 1.f, 0.f);

	CameraDesc.fFovY = D3DXToRadian(60.0f);
	CameraDesc.fAspect = _float(g_nWinCX) / g_nWinCY;
	CameraDesc.fNear = 0.2f;
	CameraDesc.fFar = 500.f;

	if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_WORLDMAP, pLayerTag, L"GameObject_CCamera_Free", &CameraDesc)))
		return E_FAIL;

	Safe_Release(pManagement);

	return NOERROR;
}

HRESULT CScene_WorldMap::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	if (FAILED(Add_StaticObjectFromFile(L"../Bin/Data/Object/WorldMap.dat", pLayerTag)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CScene_WorldMap::Ready_Layer_Object(const _tchar * pLayerTag)
{
	CManagement*		pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return E_FAIL;

	Safe_AddRef(pManagement);

	if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_WORLDMAP, pLayerTag, L"GameObject_CYoshi_World")))
		return E_FAIL;

	Safe_Release(pManagement);

	return NOERROR;
}

HRESULT CScene_WorldMap::Ready_Layer_UI(const _tchar * pLayerTag)
{
	CManagement*		pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return E_FAIL;

	Safe_AddRef(pManagement);

	if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_WORLDMAP, pLayerTag, L"GameObject_LoadScreen", (void*)&CLoading_Screen::LOADING_SCREEN(SCENE_WORLDMAP, SCENE_WORLDMAP, CLoading_Screen::LOAD_OUT))))
		return E_FAIL;

	m_pLoadingScreen = (CLoading_Screen*)pManagement->Get_ObjectFromLayer(SCENE_WORLDMAP, pLayerTag, 0);

	Safe_Release(pManagement);

	return NOERROR;
}

HRESULT CScene_WorldMap::Add_StaticObjectFromFile(const _tchar * pFilePath, const _tchar * pLayerTag)
{
	CManagement*	pManagement = CManagement::GetInstance();

	if (nullptr == pManagement)
		return -1;

	Safe_AddRef(pManagement);

	std::locale::global(std::locale("Korean"));
#ifdef _UNICODE
	wifstream fin;
#else
	wifstream fin;
#endif // _UNICODE

	fin.open(pFilePath);

	if (fin.fail())
		return E_FAIL;

	wstring strFileName = L"";
	_vec3 vPos, vScale, vAngle;
	_uint iCollType;

	while (true)
	{
		fin >> strFileName >> vPos.x >> vPos.y >> vPos.z >> vScale.x >> vScale.y >> vScale.z >> vAngle.x >> vAngle.y >> vAngle.z >> iCollType;

		if (fin.eof())
			break;

		if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_WORLDMAP, pLayerTag, L"GameObject_Environment", (void*)&CEnvironment::ENVIRONMENT_INFO(strFileName, SCENE_WORLDMAP, vPos, vScale, vAngle))))
			return E_FAIL;
	}

	Safe_Release(pManagement);

	return NOERROR;
}

CScene_WorldMap * CScene_WorldMap::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CScene_WorldMap*	pInstance = new CScene_WorldMap(pGraphic_Device);

	if (FAILED(pInstance->Ready_Scene()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CScene_WorldMap::Free()
{
	CManagement* pManagement = CManagement::GetInstance();

	Safe_AddRef(pManagement);
	if (nullptr == pManagement)
		return;

	if (FAILED(pManagement->Clear_Scene(SCENE_WORLDMAP)))
		MSG_BOX("Fail To Clear Scene");

	Safe_Release(pManagement);

	CScene::Free();
}

