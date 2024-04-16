#include "stdafx.h"
#include "..\Headers\Flower.h"
#include "Management.h"
#include "Light_Manager.h"
#include "Collider.h"
#include "Mesh_Dynamic.h"
#include "Yoshi_Stage.h"
#include "Yoshi_Stage_State.h"
#include "Yoshi_Egg.h"
#include "Mouse_Manager.h"
#include "Frustum.h"
#include "UI_Flower.h"

USING(Client)

CFlower::CFlower(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CFlower::CFlower(const CFlower & rhs)
	:CGameObject(rhs)
{
}

HRESULT CFlower::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CFlower::Ready_GameObject(void * pArg)
{
	memcpy(&m_tInfo, pArg, sizeof(FLOWER_INFO));

	if (FAILED(Add_Component()))
		return E_FAIL;
	
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_tInfo.vPos);
	m_pTransformCom->SetUp_Scale(m_tInfo.vScale);
	m_pTransformCom->SetUp_Rotation(CTransform::STATE_RIGHT, m_tInfo.vAngle.x);
	m_pTransformCom->SetUp_Rotation(CTransform::STATE_UP, m_tInfo.vAngle.y);
	m_pTransformCom->SetUp_Rotation(CTransform::STATE_LOOK, m_tInfo.vAngle.z);

	m_pMeshCom->SetUp_Animation(0);

	return NOERROR;
}

_int CFlower::Update_GameObject(_double TimeDelta)
{
	if (m_bDead)
		return OBJDEAD;

	m_TimeDelta = TimeDelta;

	m_matCollider = m_pTransformCom->Get_WorldMatrix();

	_vec3 vRight, vUp, vLook;

	memcpy(&vRight, &m_matCollider.m[0][0], sizeof(_vec3));
	memcpy(&vUp, &m_matCollider.m[1][0], sizeof(_vec3));
	memcpy(&vLook, &m_matCollider.m[2][0], sizeof(_vec3));

	D3DXVec3Normalize(&vRight, &vRight);
	D3DXVec3Normalize(&vUp, &vUp);
	D3DXVec3Normalize(&vLook, &vLook);

	memcpy(&m_matCollider.m[0][0], &vRight, sizeof(_vec3));
	memcpy(&m_matCollider.m[1][0], &vUp, sizeof(_vec3));
	memcpy(&m_matCollider.m[2][0], &vLook, sizeof(_vec3));

	m_pColliderCom->Update_Collider();
	
	return _int();
}
 
_int CFlower::LateUpdate_GameObject(_double TimeDelta)
{
	if (m_pRendererCom == nullptr)
		return -1;

	CManagement* pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return -1;

	Safe_AddRef(pManagement);

	CYoshi_Stage* pPlayer = (CYoshi_Stage*)pManagement->Get_ObjectFromLayer(SCENE_TRAIN, L"Layer_Object");

	for (auto& pEgg : *pPlayer->Get_ReleasedEgg())
	{
		if (m_pColliderCom->Collision_Sphere(pEgg->Get_Collider()))
		{
			CUI_Flower* pUI = (CUI_Flower*)pManagement->Get_ObjectFromLayer(SCENE_TRAIN, L"Layer_FlowerUI");
			pUI->Add_Flower(m_tInfo.iFlowerIndex);

			m_bDead = true;
		}
	}

	if (m_pColliderCom->Collision_Sphere((CCollider*)pManagement->Get_Component(SCENE_TRAIN, L"Layer_Object", L"Com_Collider")))
	{
		CUI_Flower* pUI = (CUI_Flower*)pManagement->Get_ObjectFromLayer(SCENE_TRAIN, L"Layer_FlowerUI");
		pUI->Add_Flower(m_tInfo.iFlowerIndex);

		m_bDead = true;
	}
	
	if (pPlayer->GetFSM()->GetCurrentState() == CStage_ThrowIdle::Instance())
	{
		_vec3 vRayPos = CMouse_Manager::GetInstance()->Get_RayPos();
		_vec3 vRayDir = CMouse_Manager::GetInstance()->Get_RayDir();

		_float fDistance = 0.f;
		if (m_pColliderCom->Collision_Ray(vRayPos, vRayDir, fDistance))
			pPlayer->Set_Focus(m_pTransformCom, fDistance);
		else
		{
			if (pPlayer->Get_FocusTarget() == m_pTransformCom)
				pPlayer->Reset_Focus();
		}
	}

	Safe_Release(pManagement);

	if (true == m_pFrustumCom->Culling_OnWorldSpace(&m_pTransformCom->Get_State(CTransform::STATE_POSITION), 1.f))
		m_pRendererCom->Add_RenderList(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

HRESULT CFlower::Render_GameObject()
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
	Safe_Release(pEffect);

	m_pColliderCom->Render_Collider();

	return NOERROR;
}

HRESULT CFlower::Add_Component()
{
	// For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, &CTransform::STATE(1.f, D3DXToRadian(90)))))
		return E_FAIL;

	// For.Com_Renderer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// For.Com_Mesh
	if (FAILED(CGameObject::Add_Component(SCENE_TRAIN, L"Component_Mesh_Flower", L"Com_Mesh", (CComponent**)&m_pMeshCom)))
		return E_FAIL;

	// For.Com_Shader
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Shader_Mesh", L"Com_Shader", (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	// For.Com_Collider
	CCollider::COLLIDERDESC		ColliderDesc;
	ColliderDesc.vScale = _vec3(0.8f, 0.8f, 0.8f);
	ColliderDesc.vRotation_Angle = _vec3(0.f, 0.f, 0.f);
	ColliderDesc.vPosition = _vec3(0.0f, 0.0f, 0.0f);
	ColliderDesc.pParentMatrix = &m_matCollider;
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Collider_Sphere", L"Com_Collider", (CComponent**)&m_pColliderCom, &ColliderDesc)))
		return E_FAIL;

	// For.Com_Frustum
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Frustum", L"Com_Frustum", (CComponent**)&m_pFrustumCom)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CFlower::SetUp_ConstantTable()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	_matrix			matView;

	m_pShaderCom->Set_Value("g_matWorld", &m_pTransformCom->Get_WorldMatrix(), sizeof(_matrix));
	m_pShaderCom->Set_Value("g_matView", matView = Get_Transform(D3DTS_VIEW), sizeof(_matrix));
	m_pShaderCom->Set_Value("g_matProj", Get_Transform(D3DTS_PROJECTION), sizeof(_matrix));

	return NOERROR;
}

CGameObject * CFlower::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CGameObject*		pInstance = new CFlower(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		MSG_BOX("Failed To Create CGameObject Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CFlower::Clone_GameObject(void * pArg)
{
	CGameObject*		pInstance = new CFlower(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		MSG_BOX("Failed To Create CGameObject Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFlower::Free()
{
	Safe_Release(m_pFrustumCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pMeshCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pColliderCom);

	CGameObject::Free();
}
