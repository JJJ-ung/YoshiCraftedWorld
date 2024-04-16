#include "stdafx.h"
#include "..\Headers\Mouse.h"

#include "MainFrm.h"
#include "ToolMenu.h"

#include "Terrain.h"
#include "Calculator.h"
#include "Input_Device.h"

#include "MapObject.h"
#include "Gimmick.h"
#include "Trigger.h"
#include "Object_Manager.h"

CMouse::CMouse(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

HRESULT CMouse::Ready_GameObject()
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	m_pToolMenu = dynamic_cast<CToolMenu*>(pFrameWnd->m_WndSplit.GetPane(0, 0));

	m_pInput_Device = CInput_Device::GetInstance();

	m_pTerrain = CObject_Manager::GetInstance()->m_pTerrain;

	return NOERROR;
}

_int CMouse::Update_GameObject(_double TimeDelta)
{
	if (!m_bClick)
		return _int();

	if (m_pInput_Device->Key_Down(KEY_LBUTTON))
	{
		POINT	ptMouse = { 0 };
		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);

		if (ptMouse.x < 0 ||
			ptMouse.x > WINCX ||
			ptMouse.y < 0 ||
			ptMouse.y > WINCY)
			return _int();

		_vec3		vPickPos = m_pCalculatorCom->Picking_OnTerrain(g_hWnd, ptMouse, m_pTerrain->m_pBufferCom, m_pTerrain->m_pTransformCom);
		CString strFilePath = L"";
		CString strFileName = L"";

		int i = m_pToolMenu->m_TabCtrl.GetCurSel();
		switch (i)
		{
			// Main
		case 0:
			break;
			// Objects
		case 1:
			strFilePath = m_pToolMenu->m_pObjects->GetObjectPath();
			if (strFilePath == L"")
				break;
			if (m_pToolMenu->m_pObjects->m_PickMode[0].GetCheck())
			{
				if (FAILED(Add_MapObject(strFilePath, vPickPos, (COLLISION_TYPE)m_pToolMenu->m_pObjects->m_CollisionType.GetCurSel())))
					return -1;
			}
			break;
			// Gimmicks
		case 2:
			if (m_pToolMenu->m_pGimmicks->m_PickMode[0].GetCheck())
			{
				strFilePath = m_pToolMenu->m_pGimmicks->GetObjectPath();
				strFileName = m_pToolMenu->m_pGimmicks->GetObjectName();
				if (strFilePath == L"" || strFileName == L"")
					break;

				if (FAILED(Add_Gimmick(strFilePath, strFileName, vPickPos, m_pToolMenu->m_pGimmicks->m_GimmickType.GetCurSel(), m_pToolMenu->m_pGimmicks->m_iOption)))
					return -1;
			}
			else	 if (m_pToolMenu->m_pGimmicks->m_PickMode[1].GetCheck())
			{
				if (FAILED(Modify_Trigger(vPickPos)))
					return -1;
			}
			break;
			// Navigation
		case 3:
			break;
			// Animation
		case 4:
			break;
			// Effect
		case 5:
			break;
		}
	}

	if (m_pInput_Device->Key_Pressing(KEY_RBUTTON))
	{
		POINT	ptMouse = { 0 };
		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);

		if (ptMouse.x < 0 ||
			ptMouse.x > WINCX ||
			ptMouse.y < 0 ||
			ptMouse.y > WINCY)
			return _int();

		_vec3		vPickPos = m_pCalculatorCom->Picking_OnTerrain(g_hWnd, ptMouse, m_pTerrain->m_pBufferCom, m_pTerrain->m_pTransformCom);
	
		int i = m_pToolMenu->m_TabCtrl.GetCurSel();
		switch (i)
		{
			// Main
		case 0:
			break;
			// Objects
		case 1:
			if (m_pToolMenu->m_pObjects->m_PickMode[1].GetCheck())
			{
				if (FAILED(Modify_MapObject(vPickPos)))
					return -1;
			}
			break;
			// Gimmicks
		case 2:
			if (m_pToolMenu->m_pGimmicks->m_PickMode[1].GetCheck())
			{
				if (FAILED(Modify_Gimmick(vPickPos)))
					return -1;
			}
			break;
			// Navigation
		case 3:
			break;
			// Animation
		case 4:
			break;
			// Effect
		case 5:
			if (FAILED(Modify_EffectEmitter(vPickPos)))
				return -1;
			break;
		}
	}

	return _int();
}

_int CMouse::LateUpdate_GameObject(_double TimeDelta)
{
	return _int();
}

HRESULT CMouse::Render_GameObject()
{
	return NOERROR;
}

HRESULT CMouse::Add_Component()
{
	m_pCalculatorCom = CCalculator::Create(m_pGraphic_Device);
	if (FAILED(CGameObject::Add_Component(L"Com_Transform", m_pCalculatorCom)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CMouse::Add_MapObject(CString strFilePath, _vec3 vPos, COLLISION_TYPE eType)
{
	CMapObject* pObj = CMapObject::Create(m_pGraphic_Device, strFilePath.GetString(), vPos, eType);
	if (pObj != nullptr)
	{
		CObject_Manager::GetInstance()->Add_GameObject(CObject_Manager::OBJECT, pObj);
		HTREEITEM hItem = m_pToolMenu->m_pObjects->m_MeshList.GetSelectedItem();
		m_pToolMenu->m_pObjects->m_ObjectList.AddString(m_pToolMenu->m_pObjects->m_MeshList.GetItemText(hItem));
	}

	return NOERROR;
}

HRESULT CMouse::Modify_MapObject(_vec3 vPos)
{
	int i = m_pToolMenu->m_pObjects->m_ObjectList.GetCurSel();
	if (i == -1)
	{
		AfxMessageBox(L"Select Object");
		return E_FAIL;
	}

	CGameObject*	pObj = CObject_Manager::GetInstance()->Get_Object(CObject_Manager::OBJECT, i);
	vPos.y = dynamic_cast<CMapObject*>(pObj)->m_pTransformCom->Get_State(CTransform::STATE_POSITION).y;
	dynamic_cast<CMapObject*>(pObj)->m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);

	m_pToolMenu->m_pObjects->UpdateData(TRUE);

	m_pToolMenu->m_pObjects->m_fPosX = vPos.x;
	m_pToolMenu->m_pObjects->m_fPosY = vPos.y;
	m_pToolMenu->m_pObjects->m_fPosZ = vPos.z;

	m_pToolMenu->m_pObjects->UpdateData(FALSE);

	return NOERROR;
}

HRESULT CMouse::Add_Gimmick(CString strFilePath, CString strFileName, _vec3 vPos, _uint eType, _uint iOption)
{
	CGimmick* pObj = CGimmick::Create(m_pGraphic_Device, strFilePath.GetString(), strFileName.GetString(), vPos, eType, iOption);
	if (pObj == nullptr)
		return E_FAIL;

	CObject_Manager::GetInstance()->Add_GameObject(CObject_Manager::GIMMICK, pObj);
	m_pToolMenu->m_pGimmicks->m_GimmickList.AddString(strFileName);

	return NOERROR;
}

HRESULT CMouse::Modify_Gimmick(_vec3 vPos)
{
	int i = m_pToolMenu->m_pGimmicks->m_GimmickList.GetCurSel();
	if (i == -1)
	{
		AfxMessageBox(L"Select Gimmick");
		return E_FAIL;
	}

	CGameObject*	pObj = CObject_Manager::GetInstance()->Get_Object(CObject_Manager::GIMMICK, i);
	if (pObj == nullptr)
		return E_FAIL;
	vPos.y = dynamic_cast<CGimmick*>(pObj)->m_pTransformCom->Get_State(CTransform::STATE_POSITION).y;
	dynamic_cast<CGimmick*>(pObj)->m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);

	m_pToolMenu->m_pGimmicks->UpdateData(TRUE);

	m_pToolMenu->m_pGimmicks->m_fPosX = vPos.x;
	m_pToolMenu->m_pGimmicks->m_fPosY = vPos.y;
	m_pToolMenu->m_pGimmicks->m_fPosZ = vPos.z;

	m_pToolMenu->m_pGimmicks->UpdateData(FALSE);

	return NOERROR;
}

HRESULT CMouse::Modify_Trigger(_vec3 vPos)
{
	int i = m_pToolMenu->m_pGimmicks->m_TriggerList.GetCurSel();
	if (i == -1)
	{
		AfxMessageBox(L"Select Trigger");
		return E_FAIL;
	}

	CGameObject*	pObj = CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TRIGGER, i);
	if (pObj == nullptr)
		return E_FAIL;
	vPos.y = dynamic_cast<CTrigger*>(pObj)->m_pTransformCom->Get_State(CTransform::STATE_POSITION).y;
	dynamic_cast<CTrigger*>(pObj)->m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);

	m_pToolMenu->m_pGimmicks->UpdateData(TRUE);

	m_pToolMenu->m_pGimmicks->m_fTriggerX = vPos.x;
	m_pToolMenu->m_pGimmicks->m_fTriggerY = vPos.y;
	m_pToolMenu->m_pGimmicks->m_fTriggerZ = vPos.z;

	m_pToolMenu->m_pGimmicks->UpdateData(FALSE);

	return NOERROR;
}

HRESULT CMouse::Modify_EffectEmitter(_vec3 vPos)
{
	//int iIndex = m_pToolMenu->m_pEffect->m_ParticleSystemList.GetCurSel();
	//if (iIndex == -1)
	//{
	//	AfxMessageBox(L"Select Object");
	//	return E_FAIL;
	//}

	//CParticleSystem* pParticleSystem = dynamic_cast<CParticleSystem*>(CObject_Manager::GetInstance()->Get_Object(CObject_Manager::PARTICLE, iIndex));
	//if (pParticleSystem == nullptr)
	//	return E_FAIL;

	//CEmitter* pEmitter = pParticleSystem->m_vecEmitters[pParticleSystem->m_iSelectedEmitter];
	//pEmitter->m_tEmitterOption.vEmitterPos = vPos;
	//
	//m_pToolMenu->m_pEffect->UpdateData(TRUE);

	//m_pToolMenu->m_pEffect->m_fPosX = vPos.x;
	//m_pToolMenu->m_pEffect->m_fPosZ = vPos.z;

	//m_pToolMenu->m_pEffect->UpdateData(FALSE);

	return NOERROR;
}

CMouse * CMouse::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMouse*		pInstance = new CMouse(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("Failed To Create CGameObject Instance");
		Safe_Delete(pInstance);
	}

	return pInstance;
}
