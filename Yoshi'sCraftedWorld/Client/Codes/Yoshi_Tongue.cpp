#include "stdafx.h"
#include "..\Headers\Yoshi_Tongue.h"
#include "Management.h"
#include "Light_Manager.h"
#include "Mesh_Dynamic.h"
#include "Yoshi_Tongue_State.h"
#include "Collider.h"

USING(Client)

CYoshi_Tongue::CYoshi_Tongue(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CYoshi_Tongue::CYoshi_Tongue(const CYoshi_Tongue & rhs)
	:CGameObject(rhs)
{
}

void CYoshi_Tongue::Reset_Collider()
{
	m_pColliderCom->Get_IsColl() = false;
}

HRESULT CYoshi_Tongue::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CYoshi_Tongue::Ready_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	if (FAILED(Compute_ParentMatrix()))
		return E_FAIL;

	m_pTransformCom->SetUp_Scale(_vec3(200.f, 200.f, 200.f));
	m_pTransformCom->SetUp_Rotation(_vec3(0.f, 1.f, 0.f), D3DXToRadian(90.f));

	m_pMeshCom->SetUp_Animation(0);
	m_bRender = true;

	m_pState = new CStateMachine<CYoshi_Tongue>(this);
	m_pState->SetCurrentState(CTongue_Idle::Instance());
	m_pState->GetCurrentState()->Enter(this);

	return NOERROR;
}

_int CYoshi_Tongue::Update_GameObject(_double TimeDelta)
{
	m_TimeDelta = TimeDelta;
	m_pState->Update();

	if (m_bRender)
		m_pMeshCom->Play_Animation(TimeDelta);

	_matrix			matWorld = m_pTransformCom->Get_WorldMatrix();
	_matrix			matFrame = m_pMeshCom->Get_FrameMatrix("J_tongue_end");
	_matrix			matParent = m_pParentTransformCom->Get_WorldMatrix();

	m_matCollider = matWorld * matFrame * matParent;

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

_int CYoshi_Tongue::LateUpdate_GameObject(_double TimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	if(m_bRender)
		m_pRendererCom->Add_RenderList(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

HRESULT CYoshi_Tongue::Render_GameObject()
{
	if (nullptr == m_pMeshCom ||
		nullptr == m_pShaderCom)
		return E_FAIL;

	if (m_bRender)
	{
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
	}

	m_pColliderCom->Render_Collider();

	return NOERROR;
}

HRESULT CYoshi_Tongue::Add_Component()
{
	// For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, &CTransform::STATE(5.f, D3DXToRadian(360)))))
		return E_FAIL;

	// For.Com_Renderer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// For.Com_Shader
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Shader_Mesh", L"Com_Shader", (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	// For.Com_Mesh
	list<const char*>	FrameList;
	FrameList.push_back("J_tongue_end");
	if (FAILED(CGameObject::Add_Component(SCENE_TRAIN, L"Component_Mesh_Yoshi_Tongue", L"Com_Mesh", (CComponent**)&m_pMeshCom, &FrameList)))
		return E_FAIL;

	// For.Com_Collider
	CCollider::COLLIDERDESC		ColliderDesc;
	ColliderDesc.vScale = _vec3(0.4f, 0.4f, 0.4f);
	ColliderDesc.vRotation_Angle = _vec3(0.f, 0.f, 0.f);
	ColliderDesc.vPosition = _vec3(-0.5f, 0.0f, 0.1f);
	ColliderDesc.pParentMatrix = &m_matCollider;
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Collider_Sphere", L"Com_Collider", (CComponent**)&m_pColliderCom, &ColliderDesc)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CYoshi_Tongue::SetUp_ConstantTable()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	_matrix			matView;
	_matrix			matWorld = m_pTransformCom->Get_WorldMatrix();
	_matrix			matFrame = m_pParentMeshCom->Get_FrameMatrix("J_attackTongue");
	_matrix			matParent = m_pParentTransformCom->Get_WorldMatrix();

	_matrix matOut = matWorld * matFrame * matParent;

	m_pShaderCom->Set_Value("g_matWorld", &matOut, sizeof(_matrix));
	m_pShaderCom->Set_Value("g_matView", matView = Get_Transform(D3DTS_VIEW), sizeof(_matrix));
	m_pShaderCom->Set_Value("g_matProj", Get_Transform(D3DTS_PROJECTION), sizeof(_matrix));

	return NOERROR;
}

HRESULT CYoshi_Tongue::Compute_ParentMatrix()
{
	CManagement* pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return E_FAIL;
	Safe_AddRef(pManagement);

	m_pParentTransformCom = (CTransform*)pManagement->Get_Component(SCENE_TRAIN, L"Layer_Object", L"Com_Transform");
	if (nullptr == m_pParentTransformCom)
		return E_FAIL;

	m_pParentMeshCom = (CMesh_Dynamic*)pManagement->Get_Component(SCENE_TRAIN, L"Layer_Object", L"Com_Mesh");
	if (nullptr == m_pParentMeshCom)
		return E_FAIL;

	Safe_Release(pManagement);

	return NOERROR;
}

CGameObject * CYoshi_Tongue::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CGameObject*		pInstance = new CYoshi_Tongue(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		MSG_BOX("Failed To Create CGameObject Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CYoshi_Tongue::Clone_GameObject(void * pArg)
{
	CGameObject*		pInstance = new CYoshi_Tongue(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		MSG_BOX("Failed To Create CGameObject Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CYoshi_Tongue::Free()
{
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pState);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pMeshCom);
	Safe_Release(m_pRendererCom);

	CGameObject::Free();
}
