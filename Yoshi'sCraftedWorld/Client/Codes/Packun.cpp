#include "stdafx.h"
#include "..\Headers\Packun.h"
#include "Management.h"
#include "Light_Manager.h"
#include "Mesh_Dynamic.h"
#include "Collider.h"
#include "Yoshi_Stage.h"

CPackun::CPackun(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CPackun::CPackun(const CPackun & rhs)
	: CGameObject(rhs)
{
}

HRESULT CPackun::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CPackun::Ready_GameObject(void * pArg)
{
	memcpy(&m_tInfo, pArg, sizeof(MONSTER_INFO));

	if (FAILED(Add_Component()))
		return E_FAIL;

	//	m_pTransformCom->SetUp_Rotation(_vec3(0.f, 1.f, 0.f), D3DXToRadian(180.f));

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_tInfo.vPos);
	m_pTransformCom->SetUp_Scale(m_tInfo.vScale);
	m_pTransformCom->SetUp_Rotation(CTransform::STATE_RIGHT, m_tInfo.vRoation.x);
	m_pTransformCom->SetUp_Rotation(CTransform::STATE_UP, m_tInfo.vRoation.y);
	m_pTransformCom->SetUp_Rotation(CTransform::STATE_LOOK, m_tInfo.vRoation.z);

	m_pMeshCom->SetUp_Animation(2);
	//m_pState = new CStateMachine<CPackun>(this);
	//m_pState->SetCurrentState(CHeyho_Idle::Instance());
	//m_pState->GetCurrentState()->Enter(this);

	m_pTransformCom->SetUp_Rotation(_vec3(0.f, 1.f, 0.f), D3DXToRadian(-90.f));

	return NOERROR;
}

_int CPackun::Update_GameObject(_double TimeDelta)
{
	m_TimeDelta = TimeDelta;

	m_pColliderCom->Update_Collider();
	m_pThreatColliderCom->Update_Collider();
	
	return _int();
}

_int CPackun::LateUpdate_GameObject(_double TimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	m_pRendererCom->Add_RenderList(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

HRESULT CPackun::Render_GameObject()
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

	m_pColliderCom->Render_Collider();
	m_pThreatColliderCom->Render_Collider();

	Safe_Release(pEffect);

	return NOERROR;
}

HRESULT CPackun::Add_Component()
{
	// For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, &CTransform::STATE(5.f, D3DXToRadian(360)))))
		return E_FAIL;

	// For.Com_Mesh
	if (FAILED(CGameObject::Add_Component(SCENE_TRAIN, L"Component_Mesh_Packun", L"Com_Mesh", (CComponent**)&m_pMeshCom)))
		return E_FAIL;

	// For.Com_Renderer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// For.Com_Shader
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Shader_Mesh", L"Com_Shader", (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	// For.Com_Collider
	CCollider::COLLIDERDESC		ColliderDesc;
	ColliderDesc.vScale = _vec3(1.3f, 1.3f, 1.3f);
	ColliderDesc.vRotation_Angle = _vec3(0.f, 0.f, 0.f);
	ColliderDesc.vPosition = _vec3(0.0f, 0.5f, 0.0f);
	ColliderDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrixPointer();
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Collider_Sphere", L"Com_Collider", (CComponent**)&m_pColliderCom, &ColliderDesc)))
		return E_FAIL;

	// For.Com_ThreatCollider
	CCollider::COLLIDERDESC		ThreatColliderDesc;
	ThreatColliderDesc.vScale = _vec3(5.f, 5.f, 5.f);
	ThreatColliderDesc.vRotation_Angle = _vec3(0.f, 0.f, 0.f);
	ThreatColliderDesc.vPosition = _vec3(0.0f, 0.2f, 0.0f);
	ThreatColliderDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrixPointer();
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Collider_Sphere", L"Com_ThreatCollider", (CComponent**)&m_pThreatColliderCom, &ThreatColliderDesc)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CPackun::SetUp_ConstantTable()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	_matrix			matView;

	m_pShaderCom->Set_Value("g_matWorld", &m_pTransformCom->Get_WorldMatrix(), sizeof(_matrix));
	m_pShaderCom->Set_Value("g_matView", matView = Get_Transform(D3DTS_VIEW), sizeof(_matrix));
	m_pShaderCom->Set_Value("g_matProj", Get_Transform(D3DTS_PROJECTION), sizeof(_matrix));

	return NOERROR;
}

CGameObject * CPackun::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CGameObject*		pInstance = new CPackun(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		MSG_BOX("Failed To Create CGameObject Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CPackun::Clone_GameObject(void * pArg)
{
	CGameObject*		pInstance = new CPackun(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		MSG_BOX("Failed To Create CGameObject Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPackun::Free()
{
	Safe_Release(m_pState);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pThreatColliderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pMeshCom);
	Safe_Release(m_pRendererCom);

	CGameObject::Free();
}
