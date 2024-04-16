#include "stdafx.h"
#include "..\Headers\Scene_Train.h"
#include "Management.h"
#include "Loading.h"
#include "Light_Manager.h"
#include "Environment.h"
#include "Road.h"
#include "Input_Device.h"
#include "Loading_Screen.h"
#include "Scene_Loading.h"

#include "Camera_Free.h"
#include "Camera_Follow.h"
#include "Camera_SideScroll.h"

#include "Yoshi_Stage.h"
#include "Coin.h"
#include "Flower.h"
#include "Egg_Interact.h"
#include "MonsterInfo.h"
#include "Sky.h"
#include "OneWayFloor.h"

USING(Client)

CScene_Train::CScene_Train(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CScene(pGraphic_Device)
{
}

HRESULT CScene_Train::Ready_Scene()
{
	//if (FAILED(Ready_LightInfo()))
	//	return E_FAIL;

	if (FAILED(Ready_GameObject_Prototype()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Object(L"Layer_Object")))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(L"Layer_BackGround")))
		return E_FAIL;

	if (FAILED(Ready_Layer_Monster(L"Layer_Monster")))
		return E_FAIL;

	if (FAILED(Ready_Layer_Flower(L"Layer_Flower")))
		return E_FAIL;

	if (FAILED(Ready_Layer_Gimmick(L"Layer_Gimmick")))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(L"Layer_Camera")))
		return E_FAIL;

	if (FAILED(Ready_Layer_Trigger(L"Layer_Trigger")))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI(L"Layer_UI")))
		return E_FAIL;

	return NOERROR;
}

_int CScene_Train::Update_Scene(_double TimeDelta)
{
	CManagement*	pManagement = CManagement::GetInstance();

	Safe_AddRef(pManagement);

	if (m_bLoadScreen)
	{
		if (m_pLoadingScreen->Get_IsAnimationEnd())
		{
			if (FAILED(pManagement->Delete_ObjectFromLayer(SCENE_TRAIN, L"Layer_UI", 0)))
				return E_FAIL;

			m_bLoadScreen = false;
		}
	}

	Safe_Release(pManagement);

	return _int();
}

HRESULT CScene_Train::Render_Scene()
{
	return NOERROR;
}

HRESULT CScene_Train::Ready_LightInfo()
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
	LightInfo.Ambient = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.f);
	LightInfo.Specular = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.f);
	LightInfo.Direction = _vec3(1.f, -1.f, 1.f);

	if (FAILED(pLight_Manager->Add_Light(m_pGraphic_Device, LightInfo)))
		return E_FAIL;

	// For.Point
	ZeroMemory(&LightInfo, sizeof(D3DLIGHT9));

	LightInfo.Type = D3DLIGHT_POINT;
	LightInfo.Diffuse = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
	LightInfo.Ambient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.f);
	LightInfo.Specular = LightInfo.Diffuse;
	LightInfo.Position = _vec3(5.f, 5.f, 5.f);
	LightInfo.Range = 5.f;

	if (FAILED(pLight_Manager->Add_Light(m_pGraphic_Device, LightInfo)))
		return E_FAIL;


	ZeroMemory(&LightInfo, sizeof(D3DLIGHT9));

	LightInfo.Type = D3DLIGHT_POINT;
	LightInfo.Diffuse = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);
	LightInfo.Ambient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.f);
	LightInfo.Specular = LightInfo.Diffuse;
	LightInfo.Position = _vec3(10.f, 5.f, 5.f);
	LightInfo.Range = 5.f;

	if (FAILED(pLight_Manager->Add_Light(m_pGraphic_Device, LightInfo)))
		return E_FAIL;


	Safe_Release(pLight_Manager);

	return NOERROR;
}

HRESULT CScene_Train::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CScene_Train::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CManagement*		pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return E_FAIL;

	Safe_AddRef(pManagement);

	//CCamera_Free::CAMERADESC	CameraDesc;
	//ZeroMemory(&CameraDesc, sizeof(CCamera_Free::CAMERADESC));

	//CameraDesc.vEye = _vec3(0.0f, 15.0f, -10.0f);
	//CameraDesc.vAt = _vec3(0.0f, 0.0f, 0.0f);
	//CameraDesc.vUp = _vec3(0.f, 1.f, 0.f);

	//CameraDesc.fFovY = D3DXToRadian(60.0f);
	//CameraDesc.fAspect = _float(g_nWinCX) / g_nWinCY;
	//CameraDesc.fNear = 0.2f;
	//CameraDesc.fFar = 500.f;

	CCamera_SideScroll::CAMERADESC CameraDesc;

	//////////////////////// Player ////////////////////////
	CameraDesc.pTargetTransform = (CTransform*)pManagement->Get_Component(SCENE_TRAIN, L"Layer_Object", L"Com_Transform");
	CameraDesc.vMove = _vec3(0.f, 0.75f, -5.5f);
	CameraDesc.vPlayerOffset = _vec3(0.0f, 1.27f, 0.f);
	CameraDesc.vCameraOffset = _vec3(0.0f, 1.f, 0.f);


	//////////////////////// Train Check ////////////////////////
	//CameraDesc.pTargetTransform = (CTransform*)pManagement->Get_Component(SCENE_TRAIN, L"Layer_TrainA", L"Com_Transform");
	//CameraDesc.vMove = _vec3(0.f, 1.f, -10.f);
	//CameraDesc.vPlayerOffset = _vec3(0.0f, 1.27f, 0.f);
	//CameraDesc.vCameraOffset = _vec3(0.0f, 4.f, 0.f);

	//if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, pLayerTag, L"GameObject_CCamera_Free", &CameraDesc)))
	//	return E_FAIL;
	if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, pLayerTag, L"GameObject_CCamera_SideScroll", &CameraDesc)))
		return E_FAIL;

	Safe_Release(pManagement);

	return NOERROR;
}

HRESULT CScene_Train::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	if (FAILED(Add_StaticObjectFromFile(L"../Bin/Data/Object/TownTrain.dat", pLayerTag)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CScene_Train::Ready_Layer_Object(const _tchar * pLayerTag)
{
	CManagement*		pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return E_FAIL;

	Safe_AddRef(pManagement);

	// Player
	if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, pLayerTag, L"GameObject_CYoshi_Stage")))
		return E_FAIL;

	// Tongue
	if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, L"Layer_Tongue", L"GameObject_CYoshi_Tongue")))
		return E_FAIL;

	 CYoshi_Stage* pPlayer = (CYoshi_Stage*)pManagement->Get_ObjectFromLayer(SCENE_TRAIN, L"Layer_Object", 0);
	 pPlayer->Set_Tongue((CYoshi_Tongue*)pManagement->Get_ObjectFromLayer(SCENE_TRAIN, L"Layer_Tongue", 0));

	 Safe_Release(pManagement);

	return NOERROR;
}

HRESULT CScene_Train::Ready_Layer_Monster(const _tchar * pLayerTag)
{
	if (FAILED(Add_MonsterFromFile(L"../Bin/Data/Gimmick/TownTrain/Monster.dat", pLayerTag)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CScene_Train::Ready_Layer_Flower(const _tchar * pLayerTag)
{
	if (FAILED(Add_FlowerFromFile(L"../Bin/Data/Gimmick/TownTrain/Flower.dat", pLayerTag)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CScene_Train::Ready_Layer_Gimmick(const _tchar * pLayerTag)
{
	CManagement*		pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return E_FAIL;

	Safe_AddRef(pManagement);

	if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, pLayerTag, L"GameObject_CRollTape")))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, pLayerTag, L"GameObject_CTransformBridge")))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, pLayerTag, L"GameObject_CTransformFountain")))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, pLayerTag, L"GameObject_CTransformHouse")))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, pLayerTag, L"GameObject_CCloud", L"../Bin/Data/Gimmick/TownTrain/Inline/Cloud0.ini")))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, pLayerTag, L"GameObject_CCloud", L"../Bin/Data/Gimmick/TownTrain/Inline/Cloud1.ini")))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, pLayerTag, L"GameObject_CCloud", L"../Bin/Data/Gimmick/TownTrain/Inline/Cloud2.ini")))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, pLayerTag, L"GameObject_CCloud", L"../Bin/Data/Gimmick/TownTrain/Inline/Cloud3.ini")))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, pLayerTag, L"GameObject_CCloud", L"../Bin/Data/Gimmick/TownTrain/Inline/Cloud5.ini")))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, L"Layer_TrainPlatform", L"GameObject_CTrainPlatform")))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, L"Layer_SteamTrain", L"GameObject_CSteamTrain")))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, L"Layer_TrainA", L"GameObject_CTrainA")))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, L"Layer_TrainB", L"GameObject_CTrainB")))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, L"Layer_GoalTape", L"GameObject_CGoalTape")))
		return E_FAIL;

	Safe_Release(pManagement);

	return NOERROR;
}

HRESULT CScene_Train::Ready_Layer_Trigger(const _tchar * pLayerTag)
{
	CManagement*		pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return E_FAIL;

	Safe_AddRef(pManagement);

	if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, pLayerTag, L"GameObject_CTrigger", (_tchar*)L"Clear0")))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, pLayerTag, L"GameObject_CTrigger", (_tchar*)L"Clear1")))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, pLayerTag, L"GameObject_CTrigger", (_tchar*)L"Clear2")))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, pLayerTag, L"GameObject_CTrigger", (_tchar*)L"CloudAppear0")))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, pLayerTag, L"GameObject_CTrigger", (_tchar*)L"ZTrigger0_0")))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, pLayerTag, L"GameObject_CTrigger", (_tchar*)L"ZTrigger0_1")))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, pLayerTag, L"GameObject_CTrigger", (_tchar*)L"ZTrigger0_2")))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, pLayerTag, L"GameObject_CTrigger", (_tchar*)L"ZTrigger1_0")))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, pLayerTag, L"GameObject_CTrigger", (_tchar*)L"ZTrigger1_1")))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, pLayerTag, L"GameObject_CTrigger", (_tchar*)L"ZTrigger1_2")))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, pLayerTag, L"GameObject_CTrigger", (_tchar*)L"ZTrigger2_0")))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, pLayerTag, L"GameObject_CTrigger", (_tchar*)L"ZTrigger2_1")))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, pLayerTag, L"GameObject_CTrigger", (_tchar*)L"ZTrigger3_0")))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, pLayerTag, L"GameObject_CTrigger", (_tchar*)L"ZTrigger3_1")))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, pLayerTag, L"GameObject_CTrigger", (_tchar*)L"ZTrigger3_2")))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, pLayerTag, L"GameObject_CTrigger", (_tchar*)L"ZTrigger3_3")))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, pLayerTag, L"GameObject_CTrigger", (_tchar*)L"ZTrigger3_4")))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, pLayerTag, L"GameObject_CTrigger", (_tchar*)L"ZTrigger4_0")))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, pLayerTag, L"GameObject_CTrigger", (_tchar*)L"ZTrigger4_1")))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, pLayerTag, L"GameObject_CTrigger", (_tchar*)L"ZTrigger5_0")))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, pLayerTag, L"GameObject_CTrigger", (_tchar*)L"ZTrigger5_1")))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, pLayerTag, L"GameObject_CTrigger", (_tchar*)L"ZTrigger5_2")))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, pLayerTag, L"GameObject_CTrigger", (_tchar*)L"ZTrigger5_3")))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, pLayerTag, L"GameObject_CTrigger", (_tchar*)L"ZTrigger6_0")))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, pLayerTag, L"GameObject_CTrigger", (_tchar*)L"ZTrigger6_1")))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, pLayerTag, L"GameObject_CTrigger", (_tchar*)L"ZTrigger6_2")))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, pLayerTag, L"GameObject_CTrigger", (_tchar*)L"ZTrigger6_3")))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, pLayerTag, L"GameObject_CTrigger", (_tchar*)L"ZTrigger6_4")))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, pLayerTag, L"GameObject_CTrigger", (_tchar*)L"ZTrigger7_0")))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, pLayerTag, L"GameObject_CTrigger", (_tchar*)L"ZTrigger7_1")))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, pLayerTag, L"GameObject_CTrigger", (_tchar*)L"ZTrigger10_1")))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, pLayerTag, L"GameObject_CTrigger", (_tchar*)L"Camera")))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, pLayerTag, L"GameObject_CTrigger", (_tchar*)L"CutScene")))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, pLayerTag, L"GameObject_CTrigger", (_tchar*)L"OnTrain")))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, pLayerTag, L"GameObject_CTrigger", (_tchar*)L"EndCam")))
		return E_FAIL;

	Safe_Release(pManagement);

	return NOERROR;
}

HRESULT CScene_Train::Ready_Layer_UI(const _tchar * pLayerTag)
{
	CManagement*		pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return E_FAIL;

	Safe_AddRef(pManagement);

	if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, pLayerTag, L"GameObject_LoadScreen", (void*)&CLoading_Screen::LOADING_SCREEN(SCENE_TRAIN, SCENE_TRAIN, CLoading_Screen::LOAD_OUT))))
		return E_FAIL;

	m_pLoadingScreen = (CLoading_Screen*)pManagement->Get_ObjectFromLayer(SCENE_TRAIN, pLayerTag, 0);

	if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, L"Layer_EggCursor", L"GameObject_CEggCursor")))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, L"Layer_CoinUI", L"GameObject_CUI_Coin")))
		return E_FAIL;
	
	if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, L"Layer_FlowerOpenUI", L"GameObject_COpen_FlowerUI")))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, L"Layer_FlowerUI", L"GameObject_CUI_Flower")))
		return E_FAIL;

	Safe_Release(pManagement);

	return NOERROR;
}

HRESULT CScene_Train::Add_StaticObjectFromFile(const _tchar * pFilePath, const _tchar * pLayerTag)
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

		switch (iCollType)
		{
		case 0:	// No Collision
			if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, pLayerTag, L"GameObject_Environment", (void*)&CEnvironment::ENVIRONMENT_INFO(strFileName, SCENE_TRAIN, vPos, vScale, vAngle))))
				return E_FAIL;
			break;
		case 1:	// Interact Egg
			if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, pLayerTag, L"GameObject_CEgg_Interact", (void*)&CEgg_Interact::EGGINTERACT_INFO(strFileName, SCENE_TRAIN, vPos, vScale, vAngle))))
				return E_FAIL;
			break;
		case 2:	// Break Egg
			if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, pLayerTag, L"GameObject_Environment", (void*)&CEnvironment::ENVIRONMENT_INFO(strFileName, SCENE_TRAIN, vPos, vScale, vAngle))))
				return E_FAIL;
			break;
		case 3:	// Default Collision
			if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, pLayerTag, L"GameObject_Road", (void*)&CRoad::ROAD_INFO(strFileName, SCENE_TRAIN, vPos, vScale, vAngle))))
				return E_FAIL;
			break;
		case 4:	// One Way Floor
			if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, pLayerTag, L"GameObject_OneWayFloor", (void*)&COneWayFloor::ROAD_INFO(strFileName, SCENE_TRAIN, vPos, vScale, vAngle))))
				return E_FAIL;
			break;
		case 5:	// Coin
			if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, pLayerTag, L"GameObject_CCoin", (void*)&CCoin::COIN_INFO(strFileName, SCENE_TRAIN, vPos, vScale, vAngle, CCoin::NORMAL_COIN))))
				return E_FAIL;
			break;
		case 6:	// Coin Large
			if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, pLayerTag, L"GameObject_CCoin", (void*)&CCoin::COIN_INFO(strFileName, SCENE_TRAIN, vPos, vScale, vAngle, CCoin::LARGE_COIN))))
				return E_FAIL;
			break;
		case 7:	// Pipe
			if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, pLayerTag, L"GameObject_Hill", (void*)&CEnvironment::ENVIRONMENT_INFO(strFileName, SCENE_TRAIN, vPos, vScale, vAngle))))
				return E_FAIL;
			break;
		case 8:	// No Cull
			if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, pLayerTag, L"GameObject_Environment", (void*)&CEnvironment::ENVIRONMENT_INFO(strFileName, SCENE_TRAIN, vPos, vScale, vAngle, false))))
				return E_FAIL;
			break;
		default:
			if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, pLayerTag, L"GameObject_Environment", (void*)&CEnvironment::ENVIRONMENT_INFO(strFileName, SCENE_TRAIN, vPos, vScale, vAngle))))
				return E_FAIL;
			break;
		}
	}

	if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, pLayerTag, L"GameObject_CSky", (void*)&CSky::ENVIRONMENT_INFO(L"TownTrainSky.X", SCENE_TRAIN, _vec3(119.611f, 1.0f, 56.3007f), _vec3(0.003f, 0.007f, 0.003f), _vec3(0.f, 3.f, 0.f)))))
		return E_FAIL;

	Safe_Release(pManagement);

	return NOERROR;
}

HRESULT CScene_Train::Add_FlowerFromFile(const _tchar * pFilePath, const _tchar * pLayerTag)
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

	wstring strFilePath = L"";
	wstring strFileName = L"";
	_vec3 vPos, vScale, vAngle;
	_int i;

	_uint iIndex = 0;

	while (true)
	{
		fin >> strFilePath >> strFileName >> vPos.x >> vPos.y >> vPos.z >> vScale.x >> vScale.y >> vScale.z >> vAngle.x >> vAngle.y >> vAngle.z >> i >> i >> i;

		if (fin.eof())
			break;

		if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, pLayerTag, L"GameObject_CFlower", (void*)&CFlower::FLOWER_INFO(vPos, vScale, vAngle, iIndex++))))
			return E_FAIL;
	}

	Safe_Release(pManagement);

	return NOERROR;
}

HRESULT CScene_Train::Add_MonsterFromFile(const _tchar * pFilePath, const _tchar * pLayerTag)
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

	wstring strTag = L"";
	_vec3 vPos, vScale, vAngle;
	_int i1, i2, i3;
	_uint iIndex = 0;

	while (true)
	{
		fin >> strTag >> vPos.x >> vPos.y >> vPos.z >> vScale.x >> vScale.y >> vScale.z >> vAngle.x >> vAngle.y >> vAngle.z >> i1 >> i2 >> i3;

		if (fin.eof())
			break;

		MONSTER_INFO tInfo;
		tInfo.vPos = vPos;
		tInfo.vRoation = vAngle;
		tInfo.vScale = vScale;
		tInfo.iOption1 = i1;
		tInfo.iOption2 = i2;
		tInfo.iOption3 = i3;

		if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, pLayerTag, strTag.c_str(), (void*)&tInfo)))
			return E_FAIL;
	}

	Safe_Release(pManagement);

	return NOERROR;
}

CScene_Train * CScene_Train::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CScene_Train*	pInstance = new CScene_Train(pGraphic_Device);

	if (FAILED(pInstance->Ready_Scene()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CScene_Train::Free()
{
	CManagement* pManagement = CManagement::GetInstance();

	Safe_AddRef(pManagement);
	if (nullptr == pManagement)
		return;

	if (FAILED(pManagement->Clear_Scene(SCENE_TRAIN)))
		MSG_BOX("Fail To Clear Scene");

	Safe_Release(pManagement);

	CScene::Free();
}
