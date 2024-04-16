#include "stdafx.h"
#include "..\Headers\Yoshi_World.h"
#include "Management.h"
#include "Light_Manager.h"
#include "Mesh_Dynamic.h"
#include "Input_Device.h"

USING(Client)

CYoshi_World::CYoshi_World(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CYoshi_World::CYoshi_World(const CYoshi_World & rhs)
	:CGameObject(rhs)
{
}

HRESULT CYoshi_World::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CYoshi_World::Ready_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransformCom->SetUp_Scale(_vec3(0.5f, 0.5f, 0.5f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _vec3(8.9f, 1.f, 1.43f));
	m_pTransformCom->SetUp_Rotation(_vec3(0.f, 1.f, 0.f), D3DXToRadian(180.f));

	m_pMeshCom->SetUp_Animation(1);

	return NOERROR;
}

_int CYoshi_World::Update_GameObject(_double TimeDelta)
{
	CInput_Device*		pInput_Instance = CInput_Device::GetInstance();
	if (nullptr == pInput_Instance)
		return -1;

	Safe_AddRef(pInput_Instance);

	if (pInput_Instance->Key_Pressing(KEY_A))
		m_pTransformCom->Go_Left(TimeDelta);

	if (pInput_Instance->Key_Pressing(KEY_D))
		m_pTransformCom->Go_Right(TimeDelta);

	if (pInput_Instance->Key_Pressing(KEY_W))
		m_pTransformCom->Go_Straight(TimeDelta);

	if (pInput_Instance->Key_Pressing(KEY_S))
		m_pTransformCom->Back_Straight(TimeDelta);

	Safe_Release(pInput_Instance);

	m_pMeshCom->Play_Animation(TimeDelta);

	return _int();
}

_int CYoshi_World::LateUpdate_GameObject(_double TimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	m_pRendererCom->Add_RenderList(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

HRESULT CYoshi_World::Render_GameObject()
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

	return NOERROR;
}

HRESULT CYoshi_World::Add_Component()
{
	// For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, &CTransform::STATE(5.f, D3DXToRadian(90)))))
		return E_FAIL;

	// For.Com_Renderer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// For.Com_Shader
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Shader_Mesh", L"Com_Shader", (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	// For.Com_Mesh
	if (FAILED(CGameObject::Add_Component(SCENE_WORLDMAP, L"Component_Mesh_Yoshi_World", L"Com_Mesh", (CComponent**)&m_pMeshCom)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CYoshi_World::SetUp_ConstantTable()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	_matrix			matView;

	m_pShaderCom->Set_Value("g_matWorld", &m_pTransformCom->Get_WorldMatrix(), sizeof(_matrix));
	m_pShaderCom->Set_Value("g_matView", matView = Get_Transform(D3DTS_VIEW), sizeof(_matrix));
	m_pShaderCom->Set_Value("g_matProj", Get_Transform(D3DTS_PROJECTION), sizeof(_matrix));

	return NOERROR;
}

CGameObject * CYoshi_World::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CGameObject*		pInstance = new CYoshi_World(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		MSG_BOX("Failed To Create CGameObject Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CYoshi_World::Clone_GameObject(void * pArg)
{
	CGameObject*		pInstance = new CYoshi_World(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		MSG_BOX("Failed To Create CGameObject Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CYoshi_World::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pMeshCom);
	Safe_Release(m_pRendererCom);

	CGameObject::Free();
}
