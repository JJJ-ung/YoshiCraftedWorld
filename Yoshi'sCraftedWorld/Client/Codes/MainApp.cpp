#include "stdafx.h"
#include "..\Headers\MainApp.h"
#include "Management.h"
#include "Scene_Logo.h"
#include "FontManager.h"
#include "Collider.h"
#include "MeshCollision.h"
#include "Mouse_Manager.h"
#include "Frustum.h"

USING(Client)

CMainApp::CMainApp()
{
}

HRESULT CMainApp::Ready_MainApp()
{
	//if (FAILED(Ready_DefaultSetting(CGraphic_Device::MODE_WIN, g_nWinCX, g_nWinCY)))
	//	return E_FAIL;
	if (FAILED(Ready_DefaultSetting(CGraphic_Device::MODE_FULL, g_nWinCX, g_nWinCY)))
		return E_FAIL;

	if (FAILED(Ready_Component_Prototype()))
		return E_FAIL;

	if (FAILED(Ready_Start_Scene(SCENE_LOGO)))
		return E_FAIL;

	ShowCursor(false);

	return NOERROR;
}

_int CMainApp::Update_MainApp(_double TimeDelta)
{
	if (m_pManagement == nullptr ||
		m_pInput_Instance == nullptr )
		return -1;

	m_TimerAcc += TimeDelta;

	m_pInput_Instance->Set_InputDev();
	m_pMouse_Manager->Update_Ray();

	if (m_pInput_Instance->Key_Down(KEY_F3))
	{
		if (!g_bRenderRay)
			g_bRenderRay = true;
		else
			g_bRenderRay = false;
	}

	return m_pManagement->Update_Management(TimeDelta);
}

HRESULT CMainApp::Render_MainApp()
{
	if (nullptr == m_pGraphic_Device ||
		nullptr == m_pManagement 	 ||
		nullptr == m_pRenderer)
		return E_FAIL;

	m_pGraphic_Device->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DXCOLOR(0.f, 0.f, 1.f, 1.f), 1.f, 0);
	m_pGraphic_Device->BeginScene();

	if (FAILED(m_pRenderer->Render_ObjectList()))
		return E_FAIL;

	m_pManagement->Render_Management();

	m_pGraphic_Device->EndScene();
	m_pGraphic_Device->Present(nullptr, nullptr, 0, nullptr);

	++m_iRenderCall;

	if (1.0 <= m_TimerAcc)
	{
		wsprintf(m_szFPS, L"Yoshi's Crafted World | FPS:%d", m_iRenderCall);
		m_iRenderCall = 0;
		m_TimerAcc = 0.0;
	}

	SetWindowText(g_hWnd, m_szFPS);

	return NOERROR;
}

HRESULT CMainApp::Ready_DefaultSetting(CGraphic_Device::WINMODE eMode, const _uint & iSizeX, const _uint & iSizeY)
{
	// For.Graphic_Device
	if (FAILED(CGraphic_Device::GetInstance()->Ready_GraphicDev(g_hWnd, eMode, iSizeX, iSizeY, &m_pGraphic_Device)))
		return E_FAIL;

	// For.Input_Device
	m_pInput_Instance = CInput_Device::GetInstance();
	if (nullptr == m_pInput_Instance)
		return E_FAIL;
	Safe_AddRef(m_pInput_Instance);

	if (FAILED(m_pInput_Instance->Ready_InputDev(g_hInst, g_hWnd)))
		return E_FAIL;

	// For.Management
	m_pManagement = CManagement::GetInstance();
	if (nullptr == m_pManagement)
		return E_FAIL;

	m_pMouse_Manager = CMouse_Manager::GetInstance();
	if (m_pMouse_Manager == nullptr)
		return E_FAIL;

	if (FAILED(m_pMouse_Manager->Ready_MouseManager(m_pGraphic_Device)))
		return E_FAIL;

	if(FAILED((CFontManager::GetInstance()->Ready_Font(m_pGraphic_Device, L"Font_CoinUI", L"THE외계인설명서", 40, 60, FW_HEAVY))))
		return E_FAIL;

	Safe_AddRef(m_pManagement);

	if (FAILED(m_pManagement->Ready_Engine(SCENE_END)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CMainApp::Ready_Start_Scene(SCENEID eSceneID)
{
	if (m_pManagement == nullptr)
	{
		MSG_BOX("Management is NULL !");
		return E_FAIL;
	}

	CScene* pScene = nullptr;

	switch (eSceneID)
	{
	case SCENE_LOGO:
		pScene = CScene_Logo::Create(m_pGraphic_Device);
		break;
	case SCENE_TITLE:
		break;
	case SCENE_WORLDMAP:
		break;
	case SCENE_TRAIN:
		break;
	case SCENE_END:
		break;
	}

	if (pScene == nullptr)
	{
		MSG_BOX("Failed To Create Scene");
		return E_FAIL;
	}

	if(FAILED(m_pManagement->SetUp_CurrentScene(pScene)))
	{
		MSG_BOX("Failed To Create Scene");
		return E_FAIL;
	}

	Safe_Release(pScene);

	return NOERROR;
}

HRESULT CMainApp::Ready_Component_Prototype()
{
	if (nullptr == m_pManagement)
		return E_FAIL;

	// For.Component_Shader_Default
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Shader_Default", CShader::Create(m_pGraphic_Device, L"../Bin/Shader/Shader_Default.fx"))))
		return E_FAIL;

	// For.Component_Shader_Default
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Shader_UI", CShader::Create(m_pGraphic_Device, L"../Bin/Shader/Shader_UI.fx"))))
		return E_FAIL;

	// For.Component_Transform
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Transform", CTransform::Create(m_pGraphic_Device))))
		return E_FAIL;

	// For.Component_Renderer
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Renderer", m_pRenderer = CRenderer::Create(m_pGraphic_Device))))
		return E_FAIL;

	// For.Component_Buffer_RcTex
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Buffer_RcTex", CBuffer_RcTex::Create(m_pGraphic_Device))))
		return E_FAIL;

	// For.Component_Texture_Loading
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Short_Loading", CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Short_Loading/ShortLoading%d.png", CTexture::TYPE_GENERAL, 5))))
		return E_FAIL;

	// For.Component_Collider_Sphere
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Collider_Sphere", CCollider::Create(m_pGraphic_Device, CCollider::TYPE_SPHERE))))
		return E_FAIL;

	// For.Component_Collider_AABB
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Collider_AABB", CCollider::Create(m_pGraphic_Device, CCollider::TYPE_BOX, CCollider::BOX_AABB))))
		return E_FAIL;

	// For.Component_Collider_OBB
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Collider_OBB", CCollider::Create(m_pGraphic_Device, CCollider::TYPE_BOX, CCollider::BOX_OBB))))
		return E_FAIL;

	// For.Component_Collider_Mesh
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Mesh_Collision", CMeshCollision::Create(m_pGraphic_Device))))
		return E_FAIL;

	// Component_Frustum
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Frustum", CFrustum::Create(m_pGraphic_Device))))
		return E_FAIL;

	Safe_AddRef(m_pRenderer);

	return NOERROR;
}

CMainApp * CMainApp::Create()
{
	CMainApp*		pMainApp = new CMainApp;

	if (FAILED(pMainApp->Ready_MainApp()))
	{
		MSG_BOX("Failed To Create MainApp Instance");
		Safe_Release(pMainApp);
	}

	return pMainApp;
}

void CMainApp::Free()
{
	CFontManager::GetInstance()->DestroyInstance();
	CSoundMgr::GetInstance()->DestroyInstance();
	Safe_Release(m_pRenderer);
	Safe_Release(m_pInput_Instance);
	Safe_Release(m_pMouse_Manager);
	Safe_Release(m_pGraphic_Device);
	Safe_Release(m_pManagement);

	CManagement::Release_Engine();
}