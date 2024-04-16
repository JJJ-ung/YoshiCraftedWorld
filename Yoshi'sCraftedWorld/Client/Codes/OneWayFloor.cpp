#include "stdafx.h"
#include "..\Headers\OneWayFloor.h"

#include "Management.h"
#include "Light_Manager.h"
#include "MeshCollision.h"
#include "Yoshi_Stage.h"
#include "Frustum.h"

USING(Client)

COneWayFloor::COneWayFloor(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

COneWayFloor::COneWayFloor(const COneWayFloor & rhs)
	: CGameObject(rhs)
{
}

HRESULT COneWayFloor::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT COneWayFloor::Ready_GameObject(void * pArg)
{
	ROAD_INFO* p = (ROAD_INFO*)pArg;

	m_tRoadInfo.eSceneID = p->eSceneID;
	m_tRoadInfo.pComponentTag = p->pComponentTag;
	m_tRoadInfo.vPos = p->vPos;
	m_tRoadInfo.vScale = p->vScale;
	m_tRoadInfo.vAngle = p->vAngle;

	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_tRoadInfo.vPos);
	m_pTransformCom->SetUp_Scale(m_tRoadInfo.vScale);
	m_pTransformCom->SetUp_Rotation(CTransform::STATE_RIGHT, m_tRoadInfo.vAngle.x);
	m_pTransformCom->SetUp_Rotation(CTransform::STATE_UP, m_tRoadInfo.vAngle.y);
	m_pTransformCom->SetUp_Rotation(CTransform::STATE_LOOK, m_tRoadInfo.vAngle.z);

	CManagement* pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return E_FAIL;
	Safe_AddRef(pManagement);

	m_pPlayer = (CYoshi_Stage*)pManagement->Get_ObjectFromLayer(SCENE_TRAIN, L"Layer_Object", 0);
	if (m_pPlayer == nullptr)
		return E_FAIL;
	//m_pPlayer->Add_CollisionList(m_pCollisionCom);

	Safe_Release(pManagement);

	return NOERROR;
}

_int COneWayFloor::Update_GameObject(_double TimeDelta)
{
	if (true == m_pFrustumCom->Culling_OnWorldSpace(&m_pTransformCom->Get_State(CTransform::STATE_POSITION), 1.f))
		m_pPlayer->Add_CollisionList(m_pCollisionCom);

	return _int();
}

_int COneWayFloor::LateUpdate_GameObject(_double TimeDelta)
{
	if (m_pRendererCom == nullptr)
		return -1;

	if (true == m_pFrustumCom->Culling_OnWorldSpace(&m_pTransformCom->Get_State(CTransform::STATE_POSITION), 1.f))
		m_pRendererCom->Add_RenderList(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

HRESULT COneWayFloor::Render_GameObject()
{
	if (nullptr == m_pMeshCom ||
		nullptr == m_pShaderCom)
		return E_FAIL;

	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if (nullptr == pEffect)
		return E_FAIL;

	Safe_AddRef(pEffect);

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	_ulong	dwNumMaterials = m_pMeshCom->Get_NumMaterials();

	for (size_t i = 0; i < dwNumMaterials; ++i)
	{
		m_pMeshCom->SetTexture_OnShader(pEffect, "g_BaseTexture", i);
		m_pMeshCom->SetMaterial_OnShader(pEffect, "g_Material", i);

		pEffect->CommitChanges();

		m_pMeshCom->Render_Mesh(i);
	}

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);

	return NOERROR;
}

HRESULT COneWayFloor::Add_Component()
{
	// For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, &CTransform::STATE(1.f, D3DXToRadian(90)))))
		return E_FAIL;

	// For.Com_Renderer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// For.Com_Mesh
	if (FAILED(CGameObject::Add_Component(m_tRoadInfo.eSceneID, m_tRoadInfo.pComponentTag.c_str(), L"Com_Mesh", (CComponent**)&m_pMeshCom)))
		return E_FAIL;

	// For.Com_CollisionMesh
	wstring strTag = m_tRoadInfo.pComponentTag;
	size_t start_pos = 0;
	start_pos = strTag.find(L".X");
	strTag.replace(start_pos, strTag.length(), L"_Collision.X");
	if (FAILED(CGameObject::Add_Component(m_tRoadInfo.eSceneID, strTag.c_str(), L"Com_CollisionMesh", (CComponent**)&m_pCollisionMeshCom)))
		return E_FAIL;

	// For.Com_Shader
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Shader_Mesh", L"Com_Shader", (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	// For.Com_Collision
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Mesh_Collision", L"Com_Collision", (CComponent**)&m_pCollisionCom, &CMeshCollision::COLLINFO(m_pTransformCom, m_pCollisionMeshCom->Get_Mesh()))))
		return E_FAIL;

	// For.Com_Frustum
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Frustum", L"Com_Frustum", (CComponent**)&m_pFrustumCom)))
		return E_FAIL;

	return NOERROR;
}

HRESULT COneWayFloor::SetUp_ConstantTable()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	_matrix			matView;

	m_pShaderCom->Set_Value("g_matWorld", &m_pTransformCom->Get_WorldMatrix(), sizeof(_matrix));
	m_pShaderCom->Set_Value("g_matView", matView = Get_Transform(D3DTS_VIEW), sizeof(_matrix));
	m_pShaderCom->Set_Value("g_matProj", Get_Transform(D3DTS_PROJECTION), sizeof(_matrix));

	return NOERROR;
}

CGameObject * COneWayFloor::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CGameObject*		pInstance = new COneWayFloor(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		MSG_BOX("Failed To Create CGameObject Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * COneWayFloor::Clone_GameObject(void * pArg)
{
	CGameObject*		pInstance = new COneWayFloor(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		MSG_BOX("Failed To Create CGameObject Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void COneWayFloor::Free()
{
	Safe_Release(m_pFrustumCom);
	Safe_Release(m_pCollisionCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pMeshCom);
	Safe_Release(m_pCollisionMeshCom);
	Safe_Release(m_pRendererCom);

	CGameObject::Free();
}
