#include "stdafx.h"
#include "..\Headers\Trigger.h"
#include "Management.h"
#include "Light_Manager.h"
#include "Collider.h"
#include "Yoshi_Stage.h"
#include "Yoshi_Stage_State.h"
#include "Camera_SideScroll.h"
#include "TrainPlatform.h"
#include "TrainA.h"
#include "TrainB.h"

USING(Client)

CTrigger::CTrigger(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CTrigger::CTrigger(const CTrigger & rhs)
	:CGameObject(rhs)
{
}

HRESULT CTrigger::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CTrigger::Ready_GameObject(void * pArg)
{
	m_pTag = (_tchar*)pArg;

	if (FAILED(Add_Component(m_pTag)))
		return E_FAIL;

	return NOERROR;
}

_int CTrigger::Update_GameObject(_double TimeDelta)
{
	m_pColliderCom->Update_Collider();

	if (m_bDead)
		return OBJDEAD;

	return _int();
}

_int CTrigger::LateUpdate_GameObject(_double TimeDelta)
{ 
	if (nullptr == m_pRendererCom)
		return -1;

	CManagement* pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return -1;

	Safe_AddRef(pManagement);

	CYoshi_Stage* pPlayer = (CYoshi_Stage*)pManagement->Get_ObjectFromLayer(SCENE_TRAIN, L"Layer_Object", 0);

	if (m_eType == Z_TRUE)
	{
		if (m_pColliderCom->Collision_Sphere((CCollider*)pManagement->Get_Component(SCENE_TRAIN, L"Layer_Object", L"Com_Collider")))
		{
			pPlayer->Get_ZTrigger() = true;
			_tchar* pTemp = m_pTag;
		}
	}

	if (m_eType == CLOUD_APPEAR)
	{
		if (!m_bDead)
		{
			if (m_pColliderCom->Collision_Sphere((CCollider*)pManagement->Get_Component(SCENE_TRAIN, L"Layer_Object", L"Com_Collider")))
			{
				if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, L"Layer_Gimmick", L"GameObject_CCloud", L"../Bin/Data/Gimmick/TownTrain/Inline/Cloud4.ini")))
					return E_FAIL;

				m_bDead = true;
			}
		}
	}

	if (m_eType == CAMERA_ON)
	{
		if (m_pColliderCom->Collision_Sphere((CCollider*)pManagement->Get_Component(SCENE_TRAIN, L"Layer_Object", L"Com_Collider")))
		{
			CCamera_SideScroll* pCamera = (CCamera_SideScroll*)pManagement->Get_ObjectFromLayer(SCENE_TRAIN, L"Layer_Camera");
			pCamera->Get_Trigger() = true;
		}
		else
		{
			CCamera_SideScroll* pCamera = (CCamera_SideScroll*)pManagement->Get_ObjectFromLayer(SCENE_TRAIN, L"Layer_Camera");
			pCamera->Get_Trigger() = false;
		}
	}

	if (m_eType == CUTSCENE)
	{
		if (m_pColliderCom->Collision_Sphere((CCollider*)pManagement->Get_Component(SCENE_TRAIN, L"Layer_Object", L"Com_Collider")))
		{
			if (!m_bCutScene)
			{
				CCamera_SideScroll* pCamera = (CCamera_SideScroll*)pManagement->Get_ObjectFromLayer(SCENE_TRAIN, L"Layer_Camera");
				pCamera->Get_CutScene((milliseconds)3000) = true;
				pCamera->Get_EnterTime() = system_clock::now();
				pPlayer->Get_CutScene() = true;

				CTrainA* pTrain = (CTrainA*)pManagement->Get_ObjectFromLayer(SCENE_TRAIN, L"Layer_TrainA");
				pTrain->Get_OnTrain() = true;

				CTrainB* pTrainB = (CTrainB*)pManagement->Get_ObjectFromLayer(SCENE_TRAIN, L"Layer_TrainB");
				pTrainB->Get_OnTrain() = true;

				CTrainPlatform* pPlatform = (CTrainPlatform*)pManagement->Get_ObjectFromLayer(SCENE_TRAIN, L"Layer_TrainPlatform");
				pPlatform->Change_State(CTrainPlatform::CUTSCENE);

				m_bCutScene = true;
			}
		}
	}

	if (m_eType == ENDCAM)
	{
		if (m_pColliderCom->Collision_Sphere((CCollider*)pManagement->Get_Component(SCENE_TRAIN, L"Layer_Object", L"Com_Collider")))
		{
			CCamera_SideScroll* pCamera = (CCamera_SideScroll*)pManagement->Get_ObjectFromLayer(SCENE_TRAIN, L"Layer_Camera");
			pCamera->Get_EndCam() = true;
		}
	}

	if (m_eType == TRAIN)
	{
		if (m_pColliderCom->Collision_Sphere((CCollider*)pManagement->Get_Component(SCENE_TRAIN, L"Layer_Object", L"Com_Collider")))
		{
			CYoshi_Stage* pPlayer = (CYoshi_Stage*)pManagement->Get_ObjectFromLayer(SCENE_TRAIN, L"Layer_Object");
			if (pPlayer->GetFSM()->GetCurrentState() != CStage_JumpToTrain::Instance())
				pPlayer->GetFSM()->ChangeState(CStage_JumpToTrain::Instance());
		}
	}

	Safe_Release(pManagement);

	m_pRendererCom->Add_RenderList(CRenderer::RENDER_ALPHA, this);

	return _int();
}

HRESULT CTrigger::Render_GameObject()
{
	if (m_pColliderCom == nullptr)
		return E_FAIL;
	
	m_pColliderCom->Render_Collider();

	return NOERROR;
}

HRESULT CTrigger::Add_Component(_tchar* pPath)
{
	// For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, &CTransform::STATE(5.f, D3DXToRadian(360)))))
		return E_FAIL;

	// For.Com_Renderer	
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// For.Com_Collider
	_matrix matTemp;
	CCollider::COLLIDERDESC		ColliderDesc;

	_tchar m_szData01[255];

	GetPrivateProfileString(pPath, _T("Radius"), NULL, m_szData01, 255, _T("../Bin/Data/Trigger/TownTrain_Trigger.ini"));
	_float fRadius = (_float)_tstof(m_szData01);
	ColliderDesc.vScale = _vec3(fRadius, fRadius, fRadius);
	ColliderDesc.vRotation_Angle = _vec3(0.f, 0.f, 0.f);

	_vec3 vPos;
	GetPrivateProfileString(pPath, _T("PosX"), NULL, m_szData01, 255, _T("../Bin/Data/Trigger/TownTrain_Trigger.ini"));
	vPos.x = (_float)_tstof(m_szData01);
	GetPrivateProfileString(pPath, _T("PosY"), NULL, m_szData01, 255, _T("../Bin/Data/Trigger/TownTrain_Trigger.ini"));
	vPos.y = (_float)_tstof(m_szData01);
	GetPrivateProfileString(pPath, _T("PosZ"), NULL, m_szData01, 255, _T("../Bin/Data/Trigger/TownTrain_Trigger.ini"));
	vPos.z = (_float)_tstof(m_szData01);
	ColliderDesc.vPosition = vPos;

	GetPrivateProfileString(m_pTag, _T("Option"), NULL, m_szData01, 255, _T("../Bin/Data/Trigger/TownTrain_Trigger.ini"));
	m_eType = (TRIGGER_TYPE)_tstoi(m_szData01);

	ColliderDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrixPointer();
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Collider_Sphere", L"Com_Collider", (CComponent**)&m_pColliderCom, &ColliderDesc)))
		return E_FAIL;

	return NOERROR;
}

CGameObject * CTrigger::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CGameObject*		pInstance = new CTrigger(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		MSG_BOX("Failed To Create CGameObject Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTrigger::Clone_GameObject(void * pArg)
{
	CGameObject*		pInstance = new CTrigger(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		MSG_BOX("Failed To Create CGameObject Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTrigger::Free()
{
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTransformCom);

	CGameObject::Free();
}
