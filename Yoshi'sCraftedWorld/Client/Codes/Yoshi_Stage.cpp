#include "stdafx.h"
#include "Yoshi_Stage.h"

#include "Yoshi_Stage_State.h"
#include "Light_Manager.h"
#include "Mesh_Dynamic.h"
#include "Yoshi_Tongue.h"
#include "Management.h"
#include "Input_Device.h"
#include "Yoshi_Egg.h"
#include "Collider.h"
#include "MeshCollision.h"
#include "Mouse_Manager.h"
#include "Camera_SideScroll.h"
#include "SteamTrain.h"
#include "TrainA.h"
#include "Effect_Smoke.h"

_bool g_bRenderRay = false;

USING(Client)

CYoshi_Stage::CYoshi_Stage(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CYoshi_Stage::CYoshi_Stage(const CYoshi_Stage & rhs)
	: CGameObject(rhs)
{
}

_vec3 CYoshi_Stage::Get_Look()
{
	return m_pTransformCom->Get_State(CTransform::STATE_LOOK);
}

_vec3 CYoshi_Stage::Get_Position()
{
	return m_pTransformCom->Get_State(CTransform::STATE_POSITION);
}

void CYoshi_Stage::Reset_Jump()
{
	_tchar m_szData01[255];
	GetPrivateProfileString(_T("Test"), _T("JumpPower"), NULL, m_szData01, 255, _T("../Bin/Data/Information/Yoshi_Animation.ini"));
	_float JumpPower = (_float)_tstof(m_szData01);
	m_fJumpPower = JumpPower;

	_tchar m_szData02[255];
	GetPrivateProfileString(_T("Test"), _T("JumpAccel"), NULL, m_szData02, 255, _T("../Bin/Data/Information/Yoshi_Animation.ini"));
	_float JumpAccel = (_float)_tstof(m_szData02);
	m_fJumpAccel = JumpAccel;
}

void CYoshi_Stage::Reset_Collider()
{
	m_pColliderCom->Get_IsColl() = false;
	m_pMouthColliderCom->Get_IsColl() = false;
	m_pTongue->Reset_Collider();
}

CCamera_SideScroll * CYoshi_Stage::Get_Camera()
{
	CManagement* pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return nullptr;

	Safe_AddRef(pManagement);
	CCamera_SideScroll* pCamera = (CCamera_SideScroll*)pManagement->Get_ObjectFromLayer(SCENE_TRAIN, L"Layer_Camera");
	Safe_Release(pManagement);

	if (pCamera == nullptr)
		return nullptr;
	 
	return pCamera;
}

HRESULT CYoshi_Stage::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CYoshi_Stage::Ready_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransformCom->SetUp_Rotation(_vec3(0.f, 1.f, 0.f), D3DXToRadian(90.f));

	_vec3 vPos;	_tchar m_szData01[255];

	GetPrivateProfileString(_T("Position"), _T("X"), NULL, m_szData01, 255, _T("../Bin/Data/Information/TownTrain_StartInfo.ini"));
	vPos.x = (_float)_tstof(m_szData01);

	GetPrivateProfileString(_T("Position"), _T("Y"), NULL, m_szData01, 255, _T("../Bin/Data/Information/TownTrain_StartInfo.ini"));
	vPos.y = (_float)_tstof(m_szData01);

	GetPrivateProfileString(_T("Position"), _T("Z"), NULL, m_szData01, 255, _T("../Bin/Data/Information/TownTrain_StartInfo.ini"));
	vPos.z = (_float)_tstof(m_szData01);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);

	m_pMeshCom->SetUp_Animation(38);

	m_pState = new CStateMachine<CYoshi_Stage>(this);
	m_pState->SetCurrentState(CStage_IdleBase::Instance());
	m_pState->GetCurrentState()->Enter(this);

	m_bTongueColl = false;
	Reset_Jump();

	if (FAILED(D3DXCreateLine(m_pGraphic_Device, &m_pLine)))
		return E_FAIL;
	if (FAILED(D3DXCreateLine(m_pGraphic_Device, &m_pLine2)))
		return E_FAIL;
	if (FAILED(D3DXCreateLine(m_pGraphic_Device, &m_pLine3)))
		return E_FAIL;

	m_EnterTime = system_clock::now();

	return NOERROR;
}

_int CYoshi_Stage::Update_GameObject(_double TimeDelta)
{
	m_TimeDelta = TimeDelta;

	Set_MouthCollider();

	m_pColliderCom->Update_Collider();
	m_pMouthColliderCom->Update_Collider();

	for (auto& pEgg : m_Eggs)
		pEgg->Update_GameObject(TimeDelta);
	if (m_pCurrEgg != nullptr)
		m_pCurrEgg->Update_GameObject(TimeDelta);

	for (auto& iter = m_ReleasedEggs.begin(); iter != m_ReleasedEggs.end();)
	{
		if ((*iter)->Update_GameObject(TimeDelta) == OBJDEAD)
		{
			Safe_Release(*iter);
			iter = m_ReleasedEggs.erase(iter);
		}
		else
			++iter;
	}

	return _int();
}

_int CYoshi_Stage::LateUpdate_GameObject(_double TimeDelta)
{
	if (m_bCutScene)
	{
		if(m_pState->GetCurrentState() == CStage_Run::Instance() || m_pState->GetCurrentState() == CStage_WalkIn::Instance())
			m_pState->ChangeState(CStage_WalkOut::Instance());
		else
			m_pState->ChangeState(CStage_Idle::Instance());
	}

	m_pState->Update();
	m_bZTrigger = false;

	if (nullptr == m_pRendererCom)
		return -1;

	m_pRendererCom->Add_RenderList(CRenderer::RENDER_NONALPHA, this);

	for (auto& pEgg : m_Eggs)
		pEgg->LateUpdate_GameObject(TimeDelta);
	for (auto& pEgg : m_ReleasedEggs)
		pEgg->LateUpdate_GameObject(TimeDelta);
	if (m_pCurrEgg != nullptr)
		m_pCurrEgg->LateUpdate_GameObject(TimeDelta);

	m_HeadCollisionList.clear();
	m_CollisionList.clear();

	return _int();
}

HRESULT CYoshi_Stage::Render_GameObject()
{
	if (nullptr == m_pMeshCom ||
		nullptr == m_pShaderCom)
		return E_FAIL;

	m_pMeshCom->Play_Animation(m_TimeDelta);

	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if (nullptr == pEffect)
		return E_FAIL;

	Safe_AddRef(pEffect);

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	for (size_t i = 0; i < m_pMeshCom->Get_NumMeshContainer(); ++i)
	{
		m_pMeshCom->Update_SkinnedMesh(i);

		_ulong	dwNumMaterials = m_pMeshCom->Get_NumMaterials(i);

		for (size_t j = 0; j < dwNumMaterials; ++j)
		{
			m_pMeshCom->SetTexture_OnShader(pEffect, "g_BaseTexture", i, j, MESHTEXTURE::TYPE_DIFFUSE);
			m_pMeshCom->SetMaterial_OnShader(pEffect, "g_Material", i, j);

			pEffect->CommitChanges();

			m_pMeshCom->Render_Mesh(i, j);
		}
	}

	pEffect->EndPass();
	pEffect->End();

	for (auto& pEgg : m_Eggs)
		pEgg->Render_GameObject();
	for (auto& pEgg : m_ReleasedEggs)
		pEgg->Render_GameObject();
	if (m_pCurrEgg != nullptr)
		m_pCurrEgg->Render_GameObject();

	m_pColliderCom->Render_Collider();
	m_pMouthColliderCom->Render_Collider();

	Safe_Release(pEffect);

	if (g_bRenderRay)
	{
		_matrix			matView, matProj;
		m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matView);
		m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &matProj);

		m_pLine->SetWidth(2.f);
		m_pLine->Begin();
		m_pLine->DrawTransform(m_vTest, 2, &(matView * matProj), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
		m_pLine->End();

		m_pLine2->SetWidth(2.f);
		m_pLine2->Begin();
		m_pLine2->DrawTransform(m_vTest2, 2, &(matView * matProj), D3DXCOLOR(0.f, 0.f, 1.f, 1.f));
		m_pLine2->End();

		m_pLine3->SetWidth(5.f);
		m_pLine3->Begin();
		m_pLine3->DrawTransform(m_vTest3, 2, &(matView * matProj), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
		m_pLine3->End();
	}

	return NOERROR;
}

HRESULT CYoshi_Stage::Add_Component()
{
	// For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, &CTransform::STATE(5.f, D3DXToRadian(360)))))
		return E_FAIL;

	// For.Com_Mesh
	list<const char*>	FrameList;
	FrameList.push_back("J_egg");
	FrameList.push_back("J_attackTongue");
	FrameList.push_back("J_attackTongue_end");
	FrameList.push_back("J_mouth_end");

	if (FAILED(CGameObject::Add_Component(SCENE_TRAIN, L"Component_Mesh_Yoshi_Stage", L"Com_Mesh", (CComponent**)&m_pMeshCom, &FrameList)))
		return E_FAIL;

	// For.Com_Renderer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// For.Com_Shader
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Shader_Mesh", L"Com_Shader", (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	// For.Com_Collider
	CCollider::COLLIDERDESC		ColliderDesc;
	ColliderDesc.vScale = _vec3(0.7f, 0.7f, 0.7f);
	ColliderDesc.vRotation_Angle = _vec3(0.f, 0.f, 0.f);
	ColliderDesc.vPosition = _vec3(0.0f, 0.25f, 0.01f);
	ColliderDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrixPointer();
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Collider_Sphere", L"Com_Collider", (CComponent**)&m_pColliderCom, &ColliderDesc)))
		return E_FAIL;

	// For.Com_MouthCollider
	CCollider::COLLIDERDESC		MouthColliderDesc;
	MouthColliderDesc.vScale = _vec3(0.3f, 0.3f, 0.3f);
	MouthColliderDesc.vRotation_Angle = _vec3(0.f, 0.f, 0.f);
	MouthColliderDesc.vPosition = _vec3(0.0f, 0.0f, 0.0f);
	MouthColliderDesc.pParentMatrix = &m_matMouthCollision;
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Collider_Sphere", L"Com_MouthCollider", (CComponent**)&m_pMouthColliderCom, &MouthColliderDesc)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CYoshi_Stage::SetUp_ConstantTable()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	_matrix			matView;

	m_pShaderCom->Set_Value("g_matWorld", &m_pTransformCom->Get_WorldMatrix(), sizeof(_matrix));
	m_pShaderCom->Set_Value("g_matView", matView = Get_Transform(D3DTS_VIEW), sizeof(_matrix));
	m_pShaderCom->Set_Value("g_matProj", Get_Transform(D3DTS_PROJECTION), sizeof(_matrix));

	return NOERROR;
}

void CYoshi_Stage::Set_MouthCollider()
{
	_matrix matWorld = m_pTransformCom->Get_WorldMatrix();
	_matrix matFrame = m_pMeshCom->Get_FrameMatrix("J_mouth_end");

	m_matMouthCollision = matFrame * matWorld;

	_vec3 vRight, vUp, vLook;

	memcpy(&vRight, &m_matMouthCollision.m[0][0], sizeof(_vec3));
	memcpy(&vUp, &m_matMouthCollision.m[1][0], sizeof(_vec3));
	memcpy(&vLook, &m_matMouthCollision.m[2][0], sizeof(_vec3));

	D3DXVec3Normalize(&vRight, &vRight);
	D3DXVec3Normalize(&vUp, &vUp);
	D3DXVec3Normalize(&vLook, &vLook);

	memcpy(&m_matMouthCollision.m[0][0], &vRight, sizeof(_vec3));
	memcpy(&m_matMouthCollision.m[1][0], &vUp, sizeof(_vec3));
	memcpy(&m_matMouthCollision.m[2][0], &vLook, sizeof(_vec3));
}

HRESULT CYoshi_Stage::Add_Egg()
{
	CTransform* pTarget = nullptr;

	if (m_Eggs.size() == 0)
		pTarget = m_pTransformCom;
	else
		pTarget = m_Eggs.back()->Get_EggTransform();

	CYoshi_Egg* pEgg = CYoshi_Egg::Create(m_pGraphic_Device, CYoshi_Egg::EGGINFO(m_pTransformCom, pTarget));

	if (pEgg == nullptr)
		return E_FAIL;

	m_Eggs.push_back(pEgg);
	return NOERROR;
}

HRESULT CYoshi_Stage::Hold_Egg()
{
	if (m_pCurrEgg != nullptr)
		return E_FAIL;

	m_pCurrEgg = m_Eggs.front();
	if (m_pCurrEgg == nullptr)
		return E_FAIL;

	if (FAILED(m_pCurrEgg->Change_Mode(CYoshi_Egg::MOVE)))
		return E_FAIL;
	m_Eggs.erase(m_Eggs.begin());

	if (!m_Eggs.empty())
	{
		CYoshi_Egg* pNextFront = m_Eggs.front();
		pNextFront->Get_EggInfo().pTargetTransform = m_pTransformCom;
	}

	return NOERROR;
}

HRESULT CYoshi_Stage::PushBack_Egg()
{
	if (m_pCurrEgg == nullptr)
		return E_FAIL;

	m_pCurrEgg->Change_Mode(CYoshi_Egg::IDLE);
	if (!m_Eggs.empty())
		m_pCurrEgg->Get_EggInfo().pTargetTransform = m_Eggs.back()->Get_EggTransform();
	m_Eggs.push_back(m_pCurrEgg);
	m_pCurrEgg = nullptr;

	return NOERROR;
}

HRESULT CYoshi_Stage::Release_Egg()
{
	if (m_pCurrEgg == nullptr)
		return NOERROR;

	if (m_pFocusTransform == nullptr)
		m_pCurrEgg->Change_Mode(CYoshi_Egg::RELEASE);
	else
		m_pCurrEgg->Change_Mode(CYoshi_Egg::FOCUS, m_vFocusTarget);

	m_ReleasedEggs.emplace_back(m_pCurrEgg);
	m_pCurrEgg = nullptr;
	Reset_Focus();
	m_bTargetRotate = false;

	return NOERROR;
}

HRESULT CYoshi_Stage::Add_CollisionList(CMeshCollision * pCollision)
{
	if (pCollision == nullptr)
		return E_FAIL;

	m_CollisionList.emplace_back(pCollision);

	return NOERROR;
}

HRESULT CYoshi_Stage::Add_HeadCollisionList(CMeshCollision * pCollision)
{
	if (pCollision == nullptr)
		return E_FAIL;

	m_HeadCollisionList.emplace_back(pCollision);

	return NOERROR;
}

_bool CYoshi_Stage::Collision_Ground()
{
	_vec3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	_vec3 vOriginPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION) + _vec3(0.f, 0.5f, 0.f);
	_vec3 vOriginDir = m_pTransformCom->Get_State(CTransform::STATE_UP) * -1.f;

	for (auto pCollision : m_CollisionList)
	{
		_vec3 vPos = vOriginPos, vDir = vOriginDir;
		
		if (FAILED(pCollision->Convert_Local(vPos, vDir)))
			return false;

		if(pCollision->Collision_Mesh(vPos, vDir, m_fCollisionDistance) == TRUE)
		{
			m_vCollisionPoint = pCollision->Get_CollisionPoint();
			m_vCollisionPoint = pCollision->Convert_World(m_vCollisionPoint);
			if (m_fCollisionDistance < 1.f)
				return true;
		}
	}

	return false;
}

_bool CYoshi_Stage::Collision_Up()
{
	_vec3 vOriginPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION) + _vec3(0.f, 1.f, 0.f);
	_vec3 vOriginDir = m_pTransformCom->Get_State(CTransform::STATE_UP);
	_float fDistance;
	_vec3 vPoint;

	for (auto pCollision : m_HeadCollisionList)
	{
		_vec3 vPos = vOriginPos, vDir = vOriginDir;

		if (FAILED(pCollision->Convert_Local(vPos, vDir)))
			return false;

		if (pCollision->Collision_Mesh(vPos, vDir, fDistance) == TRUE)
		{
			vPoint = pCollision->Get_CollisionPoint();
			vPoint = pCollision->Convert_World(vPoint);

			if (fDistance <= 0.3f)
				return true;
		}
	}

	return false;
}

_bool CYoshi_Stage::Collision_Look(_bool Jumping)
{
	//if (m_bOnTrainA || m_bOnTrainB)
	//	return true;

	_vec3 vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	_vec3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_vec3 vOriginPos, vOriginDir, vOriginDirFar;
	if (Jumping)
	{
		vOriginDir = vLook;
		vOriginDirFar = vLook * 1.5f;
		vOriginPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION) + _vec3(0.f, -0.1f, 0.f);
	}
	else
	{
		vOriginDir = _vec3(vLook.x * 0.5f, -1.f, vLook.z * 0.5f);
		vOriginDirFar = _vec3(vLook.x * 1.5f, -1.f, vLook.z * 1.5f);
		vOriginPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION) + _vec3(0.f, 1.f, 0.f);
	}

	_float fTemp = 0.f;

	for (auto pCollision : m_CollisionList)
	{
		_bool bOut = false, bOut2 = false;
		_vec3 vPos = vOriginPos, vDir = vOriginDir;
		_vec3 vPos2 = vOriginPos, vDir2 = vOriginDirFar;

		if (FAILED(pCollision->Convert_Local(vPos, vDir)))
			return false;
		if (FAILED(pCollision->Convert_Local(vPos2, vDir2)))
			return false;

		if (pCollision->Collision_Mesh(vPos2, vDir2, fTemp) == TRUE)
		{
			_vec3 v = pCollision->Convert_World(pCollision->Get_CollisionPoint());
			m_vTest2[0] = vOriginPos;
			m_vTest2[1] = v;
			bOut = true;
		}

		if (pCollision->Collision_Mesh(vPos, vDir, m_fCollisionDistance) == TRUE)
		{
			m_vLookCollisionPoint = pCollision->Convert_World(pCollision->Get_CollisionPoint());
			m_vTest3[0] = vOriginPos;
			m_vTest3[1] = m_vLookCollisionPoint;

			bOut2 = true;
		}
		else
		{
			if (Jumping)
				return false;
		}

		if (bOut || bOut2)
			return true;
	}

	return false;
}

_bool CYoshi_Stage::Collision_Front(_bool Jumping)
{
	//if (m_bOnTrainA || m_bOnTrainB)
	//	return true;

	_vec3 vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	_vec3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_vec3 vOriginPos;
	if(!Jumping)
		vOriginPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION) + _vec3(0.f, 1.f, 0.f);
	else
		vOriginPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION) + _vec3(0.f, -0.1f, 0.f);

	_vec3 vOriginDir = vLook * 0.7f;

	for (auto pCollision : m_CollisionList)
	{
		_vec3 vPos = vOriginPos, vDir = vOriginDir;

		m_vTest[0] = vOriginPos;
		m_vTest[1] = vOriginPos + vOriginDir;

		if (FAILED(pCollision->Convert_Local(vPos, vDir)))
			return false;

		if (pCollision->Collision_Mesh(vPos, vDir, m_fCollisionDistance) == TRUE)
		{
			if(m_fCollisionDistance <= 1.f)
				return true;
		}
	}

	return false;
}

_bool CYoshi_Stage::Collision_Fall()
{
	_vec3 vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	_vec3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_vec3 vOriginPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION) + _vec3(vLook.x, 1.3f, 0.f);
	_vec3 vOriginDir = _vec3(0.f, -1.f, 0.f);

	for (auto pCollision : m_CollisionList)
	{
		_vec3 vPos = vOriginPos, vDir = vOriginDir;

		if (FAILED(pCollision->Convert_Local(vPos, vDir)))
			return false;

		if (pCollision->Collision_Mesh(vPos, vDir, m_fCollisionDistance) == TRUE)
		{
			return true;
		}
	}

	return false;
}

void CYoshi_Stage::Set_OnGround()
{
	Get_Camera()->Get_RenewTarget() = false;
	//Get_Camera()->Get_Height() = m_pTransformCom->Get_State(CTransform::STATE_POSITION).y;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vCollisionPoint);
}

_bool CYoshi_Stage::Check_OnGround()
{
	if (Collision_Ground())
	{
		if (m_fCollisionDistance <= 0.9f)
			return true;
	}
	return false;
}

_bool CYoshi_Stage::Check_Jumping()
{
	if (Collision_Ground())
	{
		if (m_fCollisionDistance <= 0.65f)
			return true;
	}
	return false;
}

void CYoshi_Stage::Set_Focus(CTransform * pTargetTransform, _float fDistance, _bool bInteract)
{
	if (m_pFocusTransform != nullptr)
	{
		// 지금꺼가 더 가까우면 나가야함
		if (m_fFocusDistance < fDistance)
			return;
	}

	if (m_pFocusTransform == pTargetTransform)
		return;

	m_bTargetRotate = false;
	m_pFocusTransform = pTargetTransform;
	m_fFocusDistance = fDistance;
	m_bFocusInteract = bInteract;
}

void CYoshi_Stage::Reset_Focus()
{
	m_pFocusTransform = nullptr;
	m_fFocusDistance = -1.f;
	m_bTargetRotate = true;
}

_bool CYoshi_Stage::Set_NoFocus()
{
	_vec3 vRayPos = CMouse_Manager::GetInstance()->Get_RayPos();
	_vec3 vRayDir = CMouse_Manager::GetInstance()->Get_RayDir();

	_vec3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_vec3 vNormal = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	vNormal.z *= 100.f;

	_vec3 vRayTarget = vRayPos + (vRayDir * 100.f);
	_vec3 vTarget;

	D3DXPLANE Plane;
	D3DXPlaneFromPointNormal(&Plane, &vPos, &vNormal);
	D3DXPlaneIntersectLine(&vTarget, &Plane, &vRayPos, &vRayTarget);

	// 오른쪽
	if (vTarget.x > vPos.x)
	{
		if (m_pTransformCom->Get_State(CTransform::STATE_LOOK).x >= 0.f)
			return false;
	}
	// 왼쪽
	else
	{
		if (m_pTransformCom->Get_State(CTransform::STATE_LOOK).x < 0.f)
			return false;
	}

	m_vFocusLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK) * -1.f;
	m_vFocusTarget = m_pTransformCom->Get_State(CTransform::STATE_POSITION) + m_vFocusLook;
	m_pState->ChangeState(CStage_ThrowRotateTarget::Instance());
	return true;
}

_bool CYoshi_Stage::Set_LookFocus()
{
	if (m_bTargetRotate)
		return false;

	if(!m_bFocusInteract)
		m_vFocusTarget = m_pFocusTransform->Get_State(CTransform::STATE_POSITION);
	else
		m_vFocusTarget = m_pFocusTransform->Get_State(CTransform::STATE_POSITION) + _vec3(0.f, 0.5f, 0.f);

	m_vFocusLook = m_vFocusTarget - m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	D3DXVec3Normalize(&m_vFocusLook, &m_vFocusLook);

	_vec3 vCheck = Get_Look() - m_vFocusLook;
	if (D3DXVec3Length(&vCheck) > 0.1)
	{
		m_pState->ChangeState(CStage_ThrowRotateTarget::Instance());
		return true;
	}

	return false;
}

void CYoshi_Stage::Set_StartCamera()
{
	CManagement* pManagement = CManagement::GetInstance();
	Safe_AddRef(pManagement);

	CCamera_SideScroll* pCamera = (CCamera_SideScroll*)pManagement->Get_ObjectFromLayer(SCENE_TRAIN, L"Layer_Camera");
	pCamera->Get_Start() = false;

	Safe_Release(pManagement);
}

void CYoshi_Stage::Set_TrainStart()
{
	CManagement* pManagement = CManagement::GetInstance();
	Safe_AddRef(pManagement);

	CSteamTrain* pCamera = (CSteamTrain*)pManagement->Get_ObjectFromLayer(SCENE_TRAIN, L"Layer_SteamTrain");
	pCamera->Get_Stop() = false;

	Safe_Release(pManagement);
}

_bool & CYoshi_Stage::Get_OnTrain(_bool bA)
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	if (bA)
		return m_bOnTrainA;
	else
		return m_bOnTrainB;
}

void CYoshi_Stage::StartTrain()
{
	CManagement* pManagement = CManagement::GetInstance();
	Safe_AddRef(pManagement);

	CSteamTrain* pTrain = (CSteamTrain*)pManagement->Get_ObjectFromLayer(SCENE_TRAIN, L"Layer_SteamTrain");
	//pTrain->Change_CurrVec();
	pTrain->Get_Stop() = false;
	pTrain->Get_Animation() = true;

	Safe_Release(pManagement);
}

_bool CYoshi_Stage::Set_OnTrain()
{
	CManagement* pManagement = CManagement::GetInstance();
	Safe_AddRef(pManagement);

	CSteamTrain* pSteamTrain = (CSteamTrain*)pManagement->Get_ObjectFromLayer(SCENE_TRAIN, L"Layer_SteamTrain");

	if (pSteamTrain->Get_IsEnd())
	{
		Safe_Release(pManagement);
		//if (m_pTransformCom->Rotation_Degree(m_TimeDelta, 0.1, D3DXToRadian(90.f), D3DXToRadian(180.f)))
		//	return true;
		//return false;
		return true;
	}

	CTrainA* pTrain = (CTrainA*)pManagement->Get_ObjectFromLayer(SCENE_TRAIN, L"Layer_TrainA");

	_vec3 vPos = pTrain->Get_TrainTransform()->Get_State(CTransform::STATE_POSITION);
	vPos.y = 2.07f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	_vec3 vTarget = vPos + pTrain->Get_Look();

	m_pTransformCom->Rotation_LookAt(vTarget);

	Safe_Release(pManagement);

	return false;
}

void CYoshi_Stage::Set_Camera(_bool bStart)
{
		CManagement* pManagement = CManagement::GetInstance();
		Safe_AddRef(pManagement);
	
		CCamera_SideScroll* pCam = (CCamera_SideScroll*)pManagement->Get_ObjectFromLayer(SCENE_TRAIN, L"Layer_Camera");
	
		if (bStart)
			pCam->Get_End() = true;
		else
			pCam->Get_End() = true;

		Safe_Release(pManagement);
}

void CYoshi_Stage::Add_GoalEffect()
{
		CManagement* pManagement = CManagement::GetInstance();
		Safe_AddRef(pManagement);
	
		pManagement->Add_GameObjectToLayer(SCENE_TRAIN, L"Layer_GoalText", L"GameObject_CGoalText");
		int a = 0;
		pManagement->Add_GameObjectToLayer(SCENE_TRAIN, L"Layer_GoalText", L"GameObject_CGoalEffect", &a);
		int b = 1;
		pManagement->Add_GameObjectToLayer(SCENE_TRAIN, L"Layer_GoalText", L"GameObject_CGoalEffect", &b);

		Safe_Release(pManagement);
}

void CYoshi_Stage::Add_WalkEffect()
{
	CManagement* pManagement = CManagement::GetInstance();
	_vec3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	system_clock::duration temp = system_clock::now() - m_EnterTime;
	if (duration_cast<milliseconds>(temp) > (milliseconds)150)
	{
		m_EnterTime = system_clock::now();
		pManagement->Add_GameObjectToLayer(SCENE_TRAIN, L"Layer_Effect", L"GameObject_CEffect_Smoke", (void*)&CEffect_Smoke::EFFECT_INFO(m_pTransformCom->Get_State(CTransform::STATE_POSITION), _vec3(-0.1f, 0.f, 0.f), _vec3(0.1f, 0.f, 0.f)));
	}
}

//_matrix CYoshi_Stage::Get_TrainMatrix()
//{
//	CManagement* pManagement = CManagement::GetInstance();
//	Safe_AddRef(pManagement);
//
//	CTransform* pTrain = (CTransform*)pManagement->Get_Component(SCENE_TRAIN, L"Layer_TrainA", L"Com_Transform");
//
//	Safe_Release(pManagement);
//
//	return pTrain->Get_WorldMatrix();
//}

CGameObject * CYoshi_Stage::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CGameObject*		pInstance = new CYoshi_Stage(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		MSG_BOX("Failed To Create CGameObject Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CYoshi_Stage::Clone_GameObject(void * pArg)
{
	CGameObject*		pInstance = new CYoshi_Stage(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		MSG_BOX("Failed To Create CGameObject Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CYoshi_Stage::Free()
{
	for (auto& pEgg : m_Eggs)
		Safe_Release(pEgg);
	m_Eggs.clear();

	for (auto& pEgg : m_ReleasedEggs)
		Safe_Release(pEgg);
	m_ReleasedEggs.clear();

	if (m_pCurrEgg != nullptr)
		Safe_Release(m_pCurrEgg);

	Safe_Release(m_pLine);
	Safe_Release(m_pLine2);
	Safe_Release(m_pLine3);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pMouthColliderCom);
	Safe_Release(m_pState);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pMeshCom);
	Safe_Release(m_pRendererCom);

	CGameObject::Free();
}
