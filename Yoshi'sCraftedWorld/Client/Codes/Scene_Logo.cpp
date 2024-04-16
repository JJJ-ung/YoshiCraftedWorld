#include "stdafx.h"
#include "..\Headers\Scene_Logo.h"
#include "Management.h"
#include "Back_Logo.h"
#include "Loading.h"
#include "Scene_Title.h"

USING(Client)

CScene_Logo::CScene_Logo(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CScene(pGraphic_Device)
{
	
}

HRESULT CScene_Logo::Ready_Scene()
{
	if (FAILED(Ready_GameObject_Prototype()))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(L"Layer_BackGround")))
		return E_FAIL;

	m_pLoading = CLoading::Create(m_pGraphic_Device, SCENE_TITLE);
	if (nullptr == m_pLoading)
		return E_FAIL;

	return NOERROR;
}

_int CScene_Logo::Update_Scene(_double TimeDelta)
{
	if (nullptr == m_pLoading)
		return -1;

	CManagement*	pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return -1;

	Safe_AddRef(pManagement);

	if (true == m_pLoading->Get_Finish())
	{
		CScene_Title*	pScene = CScene_Title::Create(m_pGraphic_Device);
		if (nullptr == pScene)
			goto except;

		if (FAILED(pManagement->SetUp_CurrentScene(pScene)))
		{
			Safe_Release(pScene);
			goto except;
		}

		Safe_Release(pScene);
	}

	Safe_Release(pManagement);

	return _int(0);

except:
	Safe_Release(pManagement);
	return -1;
}

HRESULT CScene_Logo::Render_Scene()
{
	return NOERROR;
}

HRESULT CScene_Logo::Ready_GameObject_Prototype()
{
	CManagement*		pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return E_FAIL;

	Safe_AddRef(pManagement);

	// For.GameObject_BackLogo
	if (FAILED(pManagement->Add_GameObject_Prototype(L"GameObject_BackLogo", CBack_Logo::Create(m_pGraphic_Device))))
		return E_FAIL;

	Safe_Release(pManagement);

	return NOERROR;
}

HRESULT CScene_Logo::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CManagement*		pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return E_FAIL;

	Safe_AddRef(pManagement);

	if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_LOGO, pLayerTag, L"GameObject_BackLogo", (void*)&CBack_Logo::LOGO_INFO(L"Component_Texture_Short_Loading", SCENE_STATIC, _vec3(2.f, 2.f, 2.f), _vec3(0.f, 0.f, -0.f), 5.f, 5.f))))
		return E_FAIL;

	Safe_Release(pManagement);

	return NOERROR;
}

CScene_Logo * CScene_Logo::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CScene_Logo*		pInstance = new CScene_Logo(pGraphic_Device);

	if (FAILED(pInstance->Ready_Scene()))
	{
		MSG_BOX("Failed To Create CScene_Logo Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CScene_Logo::Free()
{
	Safe_Release(m_pLoading);

	CManagement*		pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return;

	Safe_AddRef(pManagement);

	if (FAILED(pManagement->Clear_Scene(SCENE_LOGO)))
		MSG_BOX("Fail To Clear Scene");

	Safe_Release(pManagement);

	CScene::Free();
}
