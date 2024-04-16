#include "stdafx.h"
#include "..\Headers\Loading_Screen.h"
#include "Management.h"

#include "Back_Logo.h"
#include "Loading_Image.h"
#include "Transform.h"

USING(Client)

CLoading_Screen::CLoading_Screen(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CLoading_Screen::CLoading_Screen(const CLoading_Screen & rhs)
	:CGameObject(rhs)
{
}

HRESULT CLoading_Screen::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CLoading_Screen::Ready_GameObject(void * pArg)
{
	LOADING_SCREEN* p = (LOADING_SCREEN*)pArg;
	m_tInfo.eBeforeScene = p->eBeforeScene;
	m_tInfo.eScene = p->eScene;
	m_tInfo.eType = p->eType;

	switch (m_tInfo.eType)
	{
	case CLoading_Screen::LOAD_IN:
		if (FAILED(Add_Loading_In()))
			return E_FAIL;
		break;
	case CLoading_Screen::LOAD_PROGRESS:
		if (FAILED(Add_Loading_Progress()))
			return E_FAIL;
		break;
	case CLoading_Screen::LOAD_OUT:
		if (FAILED(Add_Loading_Out()))
			return E_FAIL;
		m_pLoadingAnimation->Get_CurrFrame() = 0.0;
		m_tEnterTime = system_clock::now();
		break;
	}

	return NOERROR;
}

_int CLoading_Screen::Update_GameObject(_double TimeDelta)
{
	// In Animation 활성화
	if (m_bCheckAnimation_In)
	{
		if (m_pLoadingImage->GetAnimationEnd())
			m_bAnimationEnd = true;
	}

	// Out Animation 활성화
	if (m_bCheckAnimation_Out)
	{
		system_clock::duration temp = system_clock::now() - m_tEnterTime;

		if (duration_cast<milliseconds>(temp) < (milliseconds)1500)
			m_pLoadingAnimation->Get_CurrFrame() = 0.0;
		else
		{
			if (!m_bSound)
			{
				CSoundMgr::GetInstance()->PlaySound(L"PageOpen02.wav", CSoundMgr::UI);
				m_bSound = true;
			}
		}

		if (!m_bCheckAnimation_Out_End)
		{
			// 뜯는거 끝남
			if (m_pLoadingAnimation->Get_Info().bAnimationEnd)
			{
				CManagement*		pManagement = CManagement::GetInstance();
				if (nullptr == pManagement)
					return E_FAIL;

				Safe_AddRef(pManagement);

				if (FAILED(pManagement->Delete_ObjectFromLayer(m_tInfo.eScene, L"Layer_Out", 0)))
					return -1;

				Safe_Release(pManagement);

				m_pLoadingBase[0]->SetStartAnimation(true);
				m_pLoadingBase[1]->SetStartAnimation(true);

				m_bCheckAnimation_Out_End = true;
			}
		}
		else
		{
			if (m_pLoadingBase[0]->GetAnimationEnd())
				m_bAnimationEnd = true;
		}
	}

	return _int();
}

_int CLoading_Screen::LateUpdate_GameObject(_double TimeDelta)
{
	return _int();
}

HRESULT CLoading_Screen::Render_GameObject()
{
	return NOERROR;
}

HRESULT CLoading_Screen::Add_Loading_In()
{
	CManagement*		pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return E_FAIL;

	Safe_AddRef(pManagement);

	if (FAILED(pManagement->Add_GameObjectToLayer(m_tInfo.eScene, L"Layer_Load_In", L"GameObject_LoadImage", (void*)&CLoading_Image::LOADING_IMAGE(L"Component_Loading_In", 0, true))))
		return E_FAIL;

	m_pLoadingImage = (CLoading_Image*)pManagement->Get_ObjectFromLayer(m_tInfo.eScene, L"Layer_Load_In", 0);
	m_bCheckAnimation_In = true;

	Safe_Release(pManagement);

	return NOERROR;
}

HRESULT CLoading_Screen::Add_Loading_Progress()
{
	CManagement*		pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return E_FAIL;

	Safe_AddRef(pManagement);

	if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_LOAD, L"Layer_Load_Progress", L"GameObject_BackLogo", (void*)&CBack_Logo::LOGO_INFO(L"Component_Loading_Progress", SCENE_STATIC, _vec3(2.f, 2.f, 2.f), _vec3(0.f, 0.f, 0.f), 11.f, 3.f))))
		return E_FAIL;

	Safe_Release(pManagement);

	return NOERROR;
}

HRESULT CLoading_Screen::Add_Loading_Out()
{
	CManagement*		pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return E_FAIL;

	Safe_AddRef(pManagement);

	if (FAILED(pManagement->Add_GameObjectToLayer(m_tInfo.eScene, L"Layer_Out", L"GameObject_BackLogo", (void*)&CBack_Logo::LOGO_INFO(L"Component_Loading_Out", SCENE_STATIC, _vec3(2.f, 2.f, 2.f), _vec3(0.f, 0.f, 0.f), 10.f, 16.f))))
		return E_FAIL;
	m_pLoadingAnimation = (CBack_Logo*)pManagement->Get_ObjectFromLayer(m_tInfo.eScene, L"Layer_Out", 0);

	if (FAILED(pManagement->Add_GameObjectToLayer(m_tInfo.eScene, L"Layer_Base", L"GameObject_LoadImage", (void*)&CLoading_Image::LOADING_IMAGE(L"Component_Loading_Base", 1, false))))
		return E_FAIL;
	m_pLoadingBase[0] = (CLoading_Image*)pManagement->Get_ObjectFromLayer(m_tInfo.eScene, L"Layer_Base", 0);

	if (FAILED(pManagement->Add_GameObjectToLayer(m_tInfo.eScene, L"Layer_Base", L"GameObject_LoadImage", (void*)&CLoading_Image::LOADING_IMAGE(L"Component_Loading_Base", 2, false))))
		return E_FAIL;
	m_pLoadingBase[1] = (CLoading_Image*)pManagement->Get_ObjectFromLayer(m_tInfo.eScene, L"Layer_Base", 1);

	m_bCheckAnimation_Out = true;

	Safe_Release(pManagement);

	return NOERROR;
}

CGameObject * CLoading_Screen::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CGameObject*		pInstance = new CLoading_Screen(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		MSG_BOX("Failed To Create CGameObject Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CLoading_Screen::Clone_GameObject(void * pArg)
{
	CGameObject*		pInstance = new CLoading_Screen(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		MSG_BOX("Failed To Create CGameObject Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoading_Screen::Free()
{
	CGameObject::Free();
}
