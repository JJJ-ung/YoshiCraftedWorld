#include "stdafx.h"
#include "..\Headers\Scene_Title.h"

#include "Management.h"
#include "Loading.h"
#include "Light_Manager.h"
#include "Environment.h"
#include "Camera_Free.h"
#include "Back_Logo.h"
#include "Input_Device.h"
#include "Scene_Loading.h"
#include "FontManager.h"
#include "Loading_Screen.h"

USING(Client)

CScene_Title::CScene_Title(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CScene(pGraphic_Device)
{
}

HRESULT CScene_Title::Ready_Scene()
{
	if (FAILED(Ready_LightInfo()))
		return E_FAIL;

	if (FAILED(Ready_GameObject_Prototype()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(L"Layer_Camera")))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(L"Layer_BackGround")))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI(L"Layer_UI")))
		return E_FAIL;

	CSoundMgr::GetInstance()->PlayBGM(L"BGM_Title_FIX.wav");

	return NOERROR;
}

_int CScene_Title::Update_Scene(_double TimeDelta)
{
	CManagement* pManagement = CManagement::GetInstance();
	CInput_Device* pInputDevice = CInput_Device::GetInstance();

	Safe_AddRef(pManagement);
	Safe_AddRef(pInputDevice);

	if (pInputDevice->Key_Down(KEY_ENTER))
	{
		if (!m_bTransition)
		{
			if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TITLE, L"Layer_Transition", L"GameObject_LoadScreen", (void*)&CLoading_Screen::LOADING_SCREEN(SCENE_TITLE, SCENE_TITLE, CLoading_Screen::LOAD_IN))))
				return -1;
			m_bTransition = true;
			CSoundMgr::GetInstance()->PlaySound(L"SD_DECIDE01.wav", CSoundMgr::EFFECT2);
		}
	}

	if (m_bTransition)
	{
		/* WorldMap */////////////////////////////////////////////////////////////////////////////////////////////////////////
		//CLoading_Screen* pScreen = (CLoading_Screen*)pManagement->Get_ObjectFromLayer(SCENE_TITLE, L"Layer_Transition", 0);
		//if (pScreen->Get_IsAnimationEnd())
		//{
		//	CScene_Loading* pScene = CScene_Loading::Create(m_pGraphic_Device, SCENE_TITLE, SCENE_WORLDMAP);
		//	if (pScene == nullptr)
		//		goto except;

		//	if (FAILED(pManagement->SetUp_CurrentScene(pScene)))
		//	{
		//		Safe_Release(pScene);
		//		goto except;
		//	}

		//	Safe_Release(pScene);
		//}

		/* Stage1 */////////////////////////////////////////////////////////////////////////////////////////////////////////
		CLoading_Screen* pScreen = (CLoading_Screen*)pManagement->Get_ObjectFromLayer(SCENE_TITLE, L"Layer_Transition", 0);
		if (pScreen->Get_IsAnimationEnd())
		{
			CSoundMgr::GetInstance()->StopAll();

			CScene_Loading* pScene = CScene_Loading::Create(m_pGraphic_Device, SCENE_TITLE, SCENE_TRAIN);
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

	return _int(0);

except:
	Safe_Release(pInputDevice);
	Safe_Release(pManagement);
	return _int(-1);
}

HRESULT CScene_Title::Render_Scene()
{
	return NOERROR;
}

HRESULT CScene_Title::Ready_LightInfo()
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
	LightInfo.Ambient = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.f);
	LightInfo.Specular = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.f);
	LightInfo.Direction = _vec3(1.f, -1.f, 1.f);

	if (FAILED(pLight_Manager->Add_Light(m_pGraphic_Device, LightInfo)))
		return E_FAIL;

	//// For.Point
	//ZeroMemory(&LightInfo, sizeof(D3DLIGHT9));

	//LightInfo.Type = D3DLIGHT_POINT;
	//LightInfo.Diffuse = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
	//LightInfo.Ambient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.f);
	//LightInfo.Specular = LightInfo.Diffuse;
	//LightInfo.Position = _vec3(5.f, 5.f, 5.f);
	//LightInfo.Range = 5.f;

	//if (FAILED(pLight_Manager->Add_Light(m_pGraphic_Device, LightInfo)))
	//	return E_FAIL;


	//ZeroMemory(&LightInfo, sizeof(D3DLIGHT9));

	//LightInfo.Type = D3DLIGHT_POINT;
	//LightInfo.Diffuse = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);
	//LightInfo.Ambient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.f);
	//LightInfo.Specular = LightInfo.Diffuse;
	//LightInfo.Position = _vec3(10.f, 5.f, 5.f);
	//LightInfo.Range = 5.f;

	//if (FAILED(pLight_Manager->Add_Light(m_pGraphic_Device, LightInfo)))
	//	return E_FAIL;


	Safe_Release(pLight_Manager);

	return NOERROR;
}

HRESULT CScene_Title::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CScene_Title::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CManagement*		pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return E_FAIL;

	Safe_AddRef(pManagement);

	// GameObject_Camera_Free
	CCamera_Free::CAMERADESC	CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera_Free::CAMERADESC));

	CameraDesc.vEye = _vec3(0.0f, 17.0f, -7.0f);
	CameraDesc.vAt = _vec3(22.f, 2.5f, 20.f);
	CameraDesc.vUp = _vec3(0.f, 1.f, 0.f);

	CameraDesc.fFovY = D3DXToRadian(60.0f);
	CameraDesc.fAspect = _float(g_nWinCX) / g_nWinCY;
	CameraDesc.fNear = 0.2f;
	CameraDesc.fFar = 500.f;

	if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TITLE, pLayerTag, L"GameObject_CCamera_Title", &CameraDesc)))
		return E_FAIL;

	Safe_Release(pManagement);

	return NOERROR;
}

HRESULT CScene_Title::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	if (FAILED(Add_StaticObjectFromFile(L"../Bin/Data/Object/Title.dat", pLayerTag)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CScene_Title::Ready_Layer_UI(const _tchar * pLayerTag)
{
	CManagement*		pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return E_FAIL;

	Safe_AddRef(pManagement);

	if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TITLE, pLayerTag, L"GameObject_BackLogo", (void*)&CBack_Logo::LOGO_INFO(L"Component_Texture_Logo", SCENE_STATIC, _vec3(2.f, 2.f, 2.f), _vec3(0.f, 0.f, 0.f), 1.f, 0.f))))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TITLE, pLayerTag, L"GameObject_BackLogo", (void*)&CBack_Logo::LOGO_INFO(L"Component_Texture_Start", SCENE_STATIC, _vec3(2.f, 2.f, 2.f), _vec3(0.f, 0.f, 0.f), 1.f, 0.f))))
		return E_FAIL;

	Safe_Release(pManagement);

	return NOERROR;
}

HRESULT CScene_Title::Add_StaticObjectFromFile(const _tchar * pFilePath, const _tchar* pLayerTag)
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

	while (true)
	{
		fin >> strFileName >> vPos.x >> vPos.y >> vPos.z >> vScale.x >> vScale.y >> vScale.z >> vAngle.x >> vAngle.y >> vAngle.z;

		if (fin.eof())
			break;

		if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TITLE, pLayerTag, L"GameObject_Environment", (void*)&CEnvironment::ENVIRONMENT_INFO(strFileName, SCENE_TITLE, vPos, vScale, vAngle))))
			return E_FAIL;
	}

	Safe_Release(pManagement);

	return NOERROR;
}

CScene_Title * CScene_Title::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CScene_Title* pInstance = new CScene_Title(pGraphic_Device);

	if (FAILED(pInstance->Ready_Scene()))
	{
		MSG_BOX("Failed To Create CScene_Title Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CScene_Title::Free()
{
	CManagement* pManagement = CManagement::GetInstance();

	Safe_AddRef(pManagement);
	if (nullptr == pManagement)
		return;

	if (FAILED(pManagement->Clear_Scene(SCENE_TITLE)))
		MSG_BOX("Fail To Clear Scene");

	Safe_Release(pManagement);

	CScene::Free();
}
