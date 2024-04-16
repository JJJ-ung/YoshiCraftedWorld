#include "stdafx.h"
#include "..\Headers\Scene_Test.h"

#include "Management.h"
#include "Loading.h"
#include "Light_Manager.h"
#include "Environment.h"
#include "Camera_Free.h"
#include "Back_Logo.h"

USING(Client)

CScene_Test::CScene_Test(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CScene(pGraphic_Device)
{
}

HRESULT CScene_Test::Ready_Scene()
{
	if (FAILED(Ready_LightInfo()))
		return E_FAIL;

	if (FAILED(Ready_GameObject_Prototype()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(L"Layer_Camera")))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(L"Layer_BackGround")))
		return E_FAIL;

	if (FAILED(Ready_Layer_Unit(L"Layer_Unit")))
		return E_FAIL;

	return NOERROR;
}

_int CScene_Test::Update_Scene(_double TimeDelta)
{
	return _int();
}

HRESULT CScene_Test::Render_Scene()
{
	return NOERROR;
}

HRESULT CScene_Test::Ready_LightInfo()
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

HRESULT CScene_Test::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CScene_Test::Ready_Layer_Camera(const _tchar * pLayerTag)
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

	if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TITLE, pLayerTag, L"GameObject_CCamera_Free", &CameraDesc)))
		return E_FAIL;

	Safe_Release(pManagement);

	return NOERROR;
}

HRESULT CScene_Test::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	if (FAILED(Add_StaticObjectFromFile(L"../Bin/Data/Object/TownTrain.dat", pLayerTag)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CScene_Test::Ready_Layer_Unit(const _tchar * pLayerTag)
{


	return NOERROR;
}

HRESULT CScene_Test::Add_StaticObjectFromFile(const _tchar * pFilePath, const _tchar* pLayerTag)
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

		if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_STATIC, pLayerTag, L"GameObject_Environment", (void*)&CEnvironment::ENVIRONMENT_INFO(strFileName, SCENE_STATIC, vPos, vScale, vAngle))))
			return E_FAIL;
	}

	Safe_Release(pManagement);

	return NOERROR;
}

CScene_Test * CScene_Test::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CScene_Test* pInstance = new CScene_Test(pGraphic_Device);

	if (FAILED(pInstance->Ready_Scene()))
	{
		MSG_BOX("Failed To Create CScene_Test Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CScene_Test::Free()
{
	CScene::Free();
}
