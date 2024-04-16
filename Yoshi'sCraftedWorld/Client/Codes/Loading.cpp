#include "stdafx.h"
#include "Loading.h"
#include "Management.h"
#include "Environment.h"
#include "Camera_Free.h"
#include "Camera_Follow.h"
#include "Camera_Title.h"
#include "Camera_SideScroll.h"
#include "FontManager.h"
#include "Texture.h"
#include "Loading_Image.h"
#include "Loading_Screen.h"
#include "Mesh_Dynamic.h"
#include "Yoshi_World.h"
#include "Yoshi_Stage.h"
#include "Yoshi_Tongue.h"
#include "Yoshi_Egg.h"
#include "Heyho.h"
#include "Packun.h"
#include "Road.h"
#include "OneWayFloor.h"
#include "EggCursor.h"
#include "Coin.h"
#include "UI_Coin.h"
#include "Flower.h"
#include "Egg_Interact.h"
#include "RollTape.h"
#include "Trigger.h"
#include "Sky.h"
#include "Hill.h"
#include "Cloud.h"
#include "TransformBridge.h"
#include "TransformFountain.h"
#include "TransformHouse.h"
#include "Stairs.h"
#include "Open_FlowerUI.h"
#include "UI_Flower.h"
#include "UI_SingleFlower.h"
#include "TrainPlatform.h"
#include "SteamTrain.h"
#include "TrainA.h"
#include "TrainB.h"
#include "GoalTape.h"
#include "GoalText.h"
#include "GoalEffect.h"
#include "Effect_Smoke.h"
#include "Effect_Star.h"
#include "EndingCredit.h"

USING(Client)

CLoading::CLoading(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_bFinish(false)
{
	Safe_AddRef(m_pGraphicDev);
}

HRESULT CLoading::Ready_Loading(SCENEID eLoadingID)
{
	InitializeCriticalSection(&m_Crt);

	m_hThread = (HANDLE)_beginthreadex(NULL, 0, Thread_Main, this, 0, NULL);

	m_eLoadingID = eLoadingID;

	return S_OK;
}

_uint CLoading::Loading_ForTitle()
{
	CManagement*	pManagement = CManagement::GetInstance();

	if (nullptr == pManagement)
		return -1;

	Safe_AddRef(pManagement);

	if (FAILED(pManagement->Add_GameObject_Prototype(L"GameObject_CCamera_Title", CCamera_Title::Create(m_pGraphicDev))))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_Prototype(L"GameObject_CCamera_Follow", CCamera_Follow::Create(m_pGraphicDev))))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_Prototype(L"GameObject_CCamera_Free", CCamera_Free::Create(m_pGraphicDev))))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_Prototype(L"GameObject_Environment", CEnvironment::Create(m_pGraphicDev))))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_Prototype(L"GameObject_Road", CRoad::Create(m_pGraphicDev))))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_Prototype(L"GameObject_Hill", CHill::Create(m_pGraphicDev))))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_Prototype(L"GameObject_OneWayFloor", COneWayFloor::Create(m_pGraphicDev))))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_Prototype(L"GameObject_LoadImage", CLoading_Image::Create(m_pGraphicDev))))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_Prototype(L"GameObject_LoadScreen", CLoading_Screen::Create(m_pGraphicDev))))
		return E_FAIL;

	if (FAILED(Load_MeshFromFile(SCENE_TITLE, L"../Bin/Data/PathExtract/Title.dat")))
		return E_FAIL;

	if (FAILED(pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Logo", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Textures/Logo/Logo.png", CTexture::TYPE_GENERAL))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Start", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Textures/Logo/Start.png", CTexture::TYPE_GENERAL))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Loading_In", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Textures/Loading_In/0.png", CTexture::TYPE_GENERAL))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Loading_Progress", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Textures/Loading_Progress/%d.png", CTexture::TYPE_GENERAL, 11))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Loading_Out", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Textures/Loading_Out/%d.png", CTexture::TYPE_GENERAL, 10))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Loading_Base", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Textures/Loading_Out/Base%d.png", CTexture::TYPE_GENERAL, 2))))
		return E_FAIL;

	// Component_Shader_Mesh
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Shader_Mesh", CShader::Create(m_pGraphicDev, L"../Bin/Shader/Shader_Mesh.fx"))))
		return E_FAIL;
	// Component_Shader_Effect
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Shader_Effect", CShader::Create(m_pGraphicDev, L"../Bin/Shader/Shader_Effect.fx"))))
		return E_FAIL;

	m_bFinish = true;

	Safe_Release(pManagement);

	return 0;
}

_uint CLoading::Loading_ForWorldMap()
{
	CManagement*	pManagement = CManagement::GetInstance();

	if (nullptr == pManagement)
		return -1;

	Safe_AddRef(pManagement);

	if (FAILED(pManagement->Add_GameObject_Prototype(L"GameObject_CYoshi_World", CYoshi_World::Create(m_pGraphicDev))))
		return E_FAIL;

	if (FAILED(Load_MeshFromFile(SCENE_WORLDMAP, L"../Bin/Data/PathExtract/WorldMap.dat")))
		return E_FAIL;

	if (FAILED(pManagement->Add_Component_Prototype(SCENE_WORLDMAP, L"Component_Mesh_Yoshi_World", CMesh_Dynamic::Create(m_pGraphicDev, L"../Bin/Resources/Characters/Yoshi/WorldMap/", L"Yoshi_WorldMap.X"))))
		return E_FAIL;

	m_bFinish = true;

	Safe_Release(pManagement);

	return 0;
}

_uint CLoading::Loading_ForTownTrain()
{
	CManagement*	pManagement = CManagement::GetInstance();

	if (nullptr == pManagement)
		return -1;

	Safe_AddRef(pManagement);

	if (FAILED(pManagement->Add_GameObject_Prototype(L"GameObject_CYoshi_Stage", CYoshi_Stage::Create(m_pGraphicDev))))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_Prototype(L"GameObject_CYoshi_Tongue", CYoshi_Tongue::Create(m_pGraphicDev))))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_Prototype(L"GameObject_CHeyho", CHeyho::Create(m_pGraphicDev))))
		return E_FAIL;
	
	if (FAILED(pManagement->Add_GameObject_Prototype(L"GameObject_CPackun", CPackun::Create(m_pGraphicDev))))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_Prototype(L"GameObject_CCamera_SideScroll", CCamera_SideScroll::Create(m_pGraphicDev))))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_Prototype(L"GameObject_CEggCursor", CEggCursor::Create(m_pGraphicDev))))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_Prototype(L"GameObject_CCoin", CCoin::Create(m_pGraphicDev))))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_Prototype(L"GameObject_CUI_Coin", CUI_Coin::Create(m_pGraphicDev))))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_Prototype(L"GameObject_CFlower", CFlower::Create(m_pGraphicDev))))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_Prototype(L"GameObject_CEgg_Interact", CEgg_Interact::Create(m_pGraphicDev))))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_Prototype(L"GameObject_CRollTape", CRollTape::Create(m_pGraphicDev))))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_Prototype(L"GameObject_CTrigger", CTrigger::Create(m_pGraphicDev))))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_Prototype(L"GameObject_CSky", CSky::Create(m_pGraphicDev))))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_Prototype(L"GameObject_CCloud", CCloud::Create(m_pGraphicDev))))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_Prototype(L"GameObject_CTransformBridge", CTransformBridge::Create(m_pGraphicDev))))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_Prototype(L"GameObject_CTransformFountain", CTransformFountain::Create(m_pGraphicDev))))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_Prototype(L"GameObject_CTransformHouse", CTransformHouse::Create(m_pGraphicDev))))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_Prototype(L"GameObject_CStairs", CStairs::Create(m_pGraphicDev))))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_Prototype(L"GameObject_COpen_FlowerUI", COpen_FlowerUI::Create(m_pGraphicDev))))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_Prototype(L"GameObject_CUI_Flower", CUI_Flower::Create(m_pGraphicDev))))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_Prototype(L"GameObject_CUI_SingleFlower", CUI_SingleFlower::Create(m_pGraphicDev))))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_Prototype(L"GameObject_CTrainPlatform", CTrainPlatform::Create(m_pGraphicDev))))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_Prototype(L"GameObject_CSteamTrain", CSteamTrain::Create(m_pGraphicDev))))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_Prototype(L"GameObject_CTrainA", CTrainA::Create(m_pGraphicDev))))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_Prototype(L"GameObject_CTrainB", CTrainB::Create(m_pGraphicDev))))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_Prototype(L"GameObject_CGoalTape", CGoalTape::Create(m_pGraphicDev))))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_Prototype(L"GameObject_CGoalText", CGoalText::Create(m_pGraphicDev))))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_Prototype(L"GameObject_CGoalEffect", CGoalEffect::Create(m_pGraphicDev))))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_Prototype(L"GameObject_CEffect_Smoke", CEffect_Smoke::Create(m_pGraphicDev))))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_Prototype(L"GameObject_CEffect_Star", CEffect_Star::Create(m_pGraphicDev))))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_Prototype(L"GameObject_CEndingCredit", CEndingCredit::Create(m_pGraphicDev))))
		return E_FAIL;

	if (FAILED(Load_MeshFromFile(SCENE_TRAIN, L"../Bin/Data/PathExtract/TownTrain.dat")))
		return E_FAIL;

	if (FAILED(Load_MeshFromFile(SCENE_TRAIN, L"../Bin/Data/PathExtract/TownTrain_Collision.dat")))
		return E_FAIL;

	if (FAILED(Load_DynamicMeshFromFile(SCENE_TRAIN)))
		return E_FAIL;

	if (FAILED(pManagement->Add_Component_Prototype(SCENE_TRAIN, L"Component_Mesh_Yoshi_Stage", CMesh_Dynamic::Create(m_pGraphicDev, L"../Bin/Resources/Characters/Yoshi/Main/", L"Yoshi.X"))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Component_Prototype(SCENE_TRAIN, L"Component_Mesh_Yoshi_Tongue", CMesh_Dynamic::Create(m_pGraphicDev, L"../Bin/Resources/Characters/Yoshi/Tongue/", L"YoshiTongue.X"))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Component_Prototype(SCENE_TRAIN, L"Component_Mesh_Egg", CMesh_Static::Create(m_pGraphicDev, L"../Bin/Resources/Characters/Egg/", L"Egg.X"))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Component_Prototype(SCENE_TRAIN, L"Component_Mesh_Heyho", CMesh_Dynamic::Create(m_pGraphicDev, L"../Bin/Resources/Characters/Heyho/", L"Heyho.X"))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Component_Prototype(SCENE_TRAIN, L"Component_Mesh_Flower", CMesh_Dynamic::Create(m_pGraphicDev, L"../Bin/Resources/Items/Flower/", L"Flower.X"))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Component_Prototype(SCENE_TRAIN, L"Component_Texture_EggCursor", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Effect/EggCursor/%d.png", CTexture::TYPE_GENERAL, 5))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Component_Prototype(SCENE_TRAIN, L"Component_Texture_CoinUI", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Textures/UI/TU_CoinIcon00.tga", CTexture::TYPE_GENERAL))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Component_Prototype(SCENE_TRAIN, L"Component_Texture_FlowerUI", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Textures/UI/TU_FlowerIcon00.tga", CTexture::TYPE_GENERAL))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Component_Prototype(SCENE_TRAIN, L"Component_Texture_FlowerBack", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Textures/UI/Flower_Back.png", CTexture::TYPE_GENERAL))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Component_Prototype(SCENE_TRAIN, L"Component_Flower_OpenUI", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Textures/UI/OpenUI_%d.png", CTexture::TYPE_GENERAL, 2))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Component_Prototype(SCENE_TRAIN, L"Component_Heyho_Blue", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Characters/Heyho/T_HeyhoBlue_D.tga", CTexture::TYPE_GENERAL))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Component_Prototype(SCENE_TRAIN, L"Component_Heyho_Green", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Characters/Heyho/T_HeyhoGreen_D.tga", CTexture::TYPE_GENERAL))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Component_Prototype(SCENE_TRAIN, L"Component_Heyho_LightBlue", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Characters/Heyho/T_HeyhoLightBlue_D.tga", CTexture::TYPE_GENERAL))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Component_Prototype(SCENE_TRAIN, L"Component_Heyho_Orange", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Characters/Heyho/T_HeyhoOrange_D.tga", CTexture::TYPE_GENERAL))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Component_Prototype(SCENE_TRAIN, L"Component_Heyho_Pink", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Characters/Heyho/T_HeyhoPink_D.tga", CTexture::TYPE_GENERAL))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Component_Prototype(SCENE_TRAIN, L"Component_Heyho_Yellow", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Characters/Heyho/T_HeyhoYellow_D.tga", CTexture::TYPE_GENERAL))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Component_Prototype(SCENE_TRAIN, L"Component_Effect_Hit", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Effect/Textures/TP_Hit00_D.tga", CTexture::TYPE_GENERAL))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Component_Prototype(SCENE_TRAIN, L"Component_Effect_Smoke", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Effect/Textures/TP_Smoke00_D.png", CTexture::TYPE_GENERAL))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Component_Prototype(SCENE_TRAIN, L"Component_Effect_Star0", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Effect/Textures/TP_Star03_D.png", CTexture::TYPE_GENERAL))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Component_Prototype(SCENE_TRAIN, L"Component_Effect_Star1", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Effect/Textures/TP_Star04_D.png", CTexture::TYPE_GENERAL))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Component_Prototype(SCENE_TRAIN, L"Component_Effect_Star2", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Effect/Textures/TP_Star05_D.png", CTexture::TYPE_GENERAL))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Component_Prototype(SCENE_TRAIN, L"Component_Effect_Star3", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Effect/Textures/TP_Star07_D.tga", CTexture::TYPE_GENERAL))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Component_Prototype(SCENE_TRAIN, L"Component_EndingCredit", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Textures/UI/EndingCredit.png", CTexture::TYPE_GENERAL))))
		return E_FAIL;

	m_bFinish = true;

	Safe_Release(pManagement);

	return 0;
}

HRESULT CLoading::Load_MeshFromFile(SCENEID eLoadingID, const _tchar * pFilePath)
{
	CManagement*	pManagement = CManagement::GetInstance();

	if (nullptr == pManagement)
		return -1;

	Safe_AddRef(pManagement);

	//lstrcpy(m_szString, L"메시 로딩 중....");

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

	while (true)
	{
		fin >> strFilePath >> strFileName;

		if(fin.eof())
			break;

		if (FAILED(pManagement->Add_Component_Prototype(eLoadingID, strFileName, CMesh_Static::Create(m_pGraphicDev, strFilePath.c_str(), strFileName.c_str()))))
			return E_FAIL;
	}

	Safe_Release(pManagement);

	return NOERROR;
}

HRESULT CLoading::Load_DynamicMeshFromFile(SCENEID eLoadingID)
{
	CManagement*	pManagement = CManagement::GetInstance();

	if (nullptr == pManagement)
		return -1;

	Safe_AddRef(pManagement);

	if (FAILED(pManagement->Add_Component_Prototype(eLoadingID, L"Component_Mesh_GoalTape", CMesh_Dynamic::Create(m_pGraphicDev, L"../Bin/Resources/Gimmicks/Common/GoalTape/", L"GoalTapeA.X"))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Component_Prototype(eLoadingID, L"Component_Mesh_HatenaCloud", CMesh_Dynamic::Create(m_pGraphicDev, L"../Bin/Resources/Gimmicks/Common/HatenaCloud/", L"HatenaCloudA.X"))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Component_Prototype(eLoadingID, L"Component_Mesh_Stairs", CMesh_Dynamic::Create(m_pGraphicDev, L"../Bin/Resources/Gimmicks/Common/Stairs/", L"StairsA.X"))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Component_Prototype(eLoadingID, L"Component_Mesh_RollTape", CMesh_Dynamic::Create(m_pGraphicDev, L"../Bin/Resources/Gimmicks/TownTrain/RollTape/", L"RollTapeA.X"))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Component_Prototype(eLoadingID, L"Component_Mesh_TrainB", CMesh_Dynamic::Create(m_pGraphicDev, L"../Bin/Resources/Gimmicks/TownTrain/SteamTrain/", L"TrainEndA.X"))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Component_Prototype(eLoadingID, L"Component_Mesh_TrainA", CMesh_Dynamic::Create(m_pGraphicDev, L"../Bin/Resources/Gimmicks/TownTrain/SteamTrain/", L"TrainTopA.X"))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Component_Prototype(eLoadingID, L"Component_Mesh_TransformBridge", CMesh_Dynamic::Create(m_pGraphicDev, L"../Bin/Resources/Gimmicks/TownTrain/TransformBridge/", L"TransformBridgeC.X"))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Component_Prototype(eLoadingID, L"Component_Mesh_TransformFountain", CMesh_Dynamic::Create(m_pGraphicDev, L"../Bin/Resources/Gimmicks/TownTrain/TransformFountain/", L"TransformFountainA.X"))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Component_Prototype(eLoadingID, L"Component_Mesh_TransformHouse", CMesh_Dynamic::Create(m_pGraphicDev, L"../Bin/Resources/Gimmicks/TownTrain/TransformHouse/", L"TransformHouseC.X"))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Component_Prototype(eLoadingID, L"Component_Mesh_SteamTrain", CMesh_Dynamic::Create(m_pGraphicDev, L"../Bin/Resources/Gimmicks/TownTrain/SteamTrain/", L"SteamTrainA.X"))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Component_Prototype(eLoadingID, L"Component_Mesh_TrainPlatform", CMesh_Dynamic::Create(m_pGraphicDev, L"../Bin/Resources/Gimmicks/TownTrain/TrainPlatform/", L"TrainPlatform.X"))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Component_Prototype(SCENE_TRAIN, L"Component_Mesh_GoalText", CMesh_Dynamic::Create(m_pGraphicDev, L"../Bin/Resources/Effect/GoalText/", L"GoalText.X"))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Component_Prototype(SCENE_TRAIN, L"Component_Mesh_GoalEffect0", CMesh_Dynamic::Create(m_pGraphicDev, L"../Bin/Resources/Effect/GoalText/", L"GoalEffect_0.X"))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Component_Prototype(SCENE_TRAIN, L"Component_Mesh_GoalEffect1", CMesh_Dynamic::Create(m_pGraphicDev, L"../Bin/Resources/Effect/GoalText/", L"GoalEffect_1.X"))))
		return E_FAIL;

	Safe_Release(pManagement);

	return NOERROR;
}

unsigned int CALLBACK CLoading::Thread_Main(void* pArg)
{
	CLoading*	pLoading = (CLoading*)pArg;

	_uint		iFlag = 0;

	EnterCriticalSection(pLoading->Get_Crt());

	switch (pLoading->Get_LoadingID())
	{
	case SCENE_TITLE:
		iFlag = pLoading->Loading_ForTitle();
		break;
	case SCENE_WORLDMAP:
		iFlag = pLoading->Loading_ForWorldMap();
		break;
	case SCENE_TRAIN:
		iFlag = pLoading->Loading_ForTownTrain();
		break;
	}

	LeaveCriticalSection(pLoading->Get_Crt());

	return iFlag;
}

CLoading* CLoading::Create(LPDIRECT3DDEVICE9 pGraphicDev, SCENEID eLoadingID)
{
	CLoading*	pInstance = new CLoading(pGraphicDev);

	if (FAILED(pInstance->Ready_Loading(eLoadingID)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CLoading::Free(void)
{
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
	DeleteCriticalSection(&m_Crt);

	Safe_Release(m_pGraphicDev);
}

