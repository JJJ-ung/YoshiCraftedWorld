#include "stdafx.h"
#include "..\Headers\Stairs.h"
#include "Management.h"
#include "Light_Manager.h"
#include "Mesh_Dynamic.h"
#include "Frustum.h"
#include "Yoshi_Stage.h"
#include "MeshCollision.h"

USING(Client)

CStairs::CStairs(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CStairs::CStairs(const CStairs & rhs)
	:CGameObject(rhs)
{
}

HRESULT CStairs::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CStairs::Ready_GameObject(void * pArg)
{
	m_pTag = (_tchar*)pArg;

	if (FAILED(Add_Component()))
		return E_FAIL;

	_tchar m_szData01[255];
	_vec3 vPos, vScale, vAngle;

	GetPrivateProfileString(_T("Position"), _T("X"), NULL, m_szData01, 255, m_pTag);
	vPos.x = (_float) _tstof(m_szData01);
	GetPrivateProfileString(_T("Position"), _T("Y"), NULL, m_szData01, 255, m_pTag);
	vPos.y = (_float)_tstof(m_szData01);
	GetPrivateProfileString(_T("Position"), _T("Z"), NULL, m_szData01, 255, m_pTag);
	vPos.z = (_float)_tstof(m_szData01);

	GetPrivateProfileString(_T("Scale"), _T("X"), NULL, m_szData01, 255, m_pTag);
	vScale.x = (_float)_tstof(m_szData01);
	GetPrivateProfileString(_T("Scale"), _T("Y"), NULL, m_szData01, 255, m_pTag);
	vScale.y = (_float)_tstof(m_szData01);
	GetPrivateProfileString(_T("Scale"), _T("Z"), NULL, m_szData01, 255, m_pTag);
	vScale.z = (_float)_tstof(m_szData01);

	GetPrivateProfileString(_T("Rotation"), _T("X"), NULL, m_szData01, 255, m_pTag);
	vAngle.x = (_float)_tstof(m_szData01);
	GetPrivateProfileString(_T("Rotation"), _T("Y"), NULL, m_szData01, 255, m_pTag);
	vAngle.y = (_float)_tstof(m_szData01);
	GetPrivateProfileString(_T("Rotation"), _T("Z"), NULL, m_szData01, 255, m_pTag);
	vAngle.z = (_float)_tstof(m_szData01);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	m_pTransformCom->SetUp_Scale(vScale);
	m_pTransformCom->SetUp_Rotation(CTransform::STATE_RIGHT, vAngle.x);
	m_pTransformCom->SetUp_Rotation(CTransform::STATE_UP, vAngle.y);
	m_pTransformCom->SetUp_Rotation(CTransform::STATE_LOOK, vAngle.z);

	m_pCollisionTransform->Set_State(CTransform::STATE_POSITION, vPos);
	m_pCollisionTransform->SetUp_Scale(_vec3(0.005f, 0.005f, 0.005f));
	m_pCollisionTransform->SetUp_Rotation(CTransform::STATE_RIGHT, 0.f);
	m_pCollisionTransform->SetUp_Rotation(CTransform::STATE_UP, 0.f);
	m_pCollisionTransform->SetUp_Rotation(CTransform::STATE_LOOK, 0.f);

	m_pMeshCom->SetUp_Animation(0);

	CManagement* pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return E_FAIL;
	Safe_AddRef(pManagement);

	m_pPlayer = (CYoshi_Stage*)pManagement->Get_ObjectFromLayer(SCENE_TRAIN, L"Layer_Object", 0);
	if (m_pPlayer == nullptr)
		return E_FAIL;

	Safe_Release(pManagement);

	CSoundMgr::GetInstance()->PlaySound(L"SDV_YOSHI_KAPO1_Echo.wav", CSoundMgr::GIMMICKS);
	CSoundMgr::GetInstance()->PlaySound(L"Patapata.wav", CSoundMgr::EFFECT);

	return NOERROR;
}

_int CStairs::Update_GameObject(_double TimeDelta)
{
	m_TimeDelta = TimeDelta;

	if (m_bPlayAnimation)
	{
		if (m_pMeshCom->Finish_Animation(0.2))
			m_bPlayAnimation = false;
	}

	if (true == m_pFrustumCom->Culling_OnWorldSpace(&m_pTransformCom->Get_State(CTransform::STATE_POSITION), 8.f))
	{
		m_pPlayer->Add_CollisionList(m_pCollisionCom);
		m_pPlayer->Add_HeadCollisionList(m_pCollisionCom);
	}

	return _int();
}

_int CStairs::LateUpdate_GameObject(_double TimeDelta)
{
	if (m_pRendererCom == nullptr)
		return -1;

	if (true == m_pFrustumCom->Culling_OnWorldSpace(&m_pTransformCom->Get_State(CTransform::STATE_POSITION), 8.f))
		m_pRendererCom->Add_RenderList(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

HRESULT CStairs::Render_GameObject()
{
	if (nullptr == m_pMeshCom ||
		nullptr == m_pShaderCom)
		return E_FAIL;

	if(m_bPlayAnimation)
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

	return NOERROR;
}

HRESULT CStairs::Add_Component()
{
	// For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, &CTransform::STATE(1.f, D3DXToRadian(90)))))
		return E_FAIL;

	// For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_CoillisionTransform", (CComponent**)&m_pCollisionTransform, &CTransform::STATE(1.f, D3DXToRadian(90)))))
		return E_FAIL;

	// For.Com_Renderer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// For.Com_Mesh
	if (FAILED(CGameObject::Add_Component(SCENE_TRAIN, L"Component_Mesh_Stairs", L"Com_Mesh", (CComponent**)&m_pMeshCom)))
		return E_FAIL;

	// For.Com_Shader
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Shader_Mesh", L"Com_Shader", (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	// For.Com_Frustum
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Frustum", L"Com_Frustum", (CComponent**)&m_pFrustumCom)))
		return E_FAIL;

	// For.Com_CollMeshA
	if (FAILED(CGameObject::Add_Component(SCENE_TRAIN, L"StairsA_Collision.X", L"Com_CollMesh", (CComponent**)&m_pCollisionMesh)))
		return E_FAIL;

	// For.Com_Collision
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Mesh_Collision", L"Com_Collision", (CComponent**)&m_pCollisionCom, &CMeshCollision::COLLINFO(m_pCollisionTransform, m_pCollisionMesh->Get_Mesh()))))
		return E_FAIL;

	return NOERROR;
}

HRESULT CStairs::SetUp_ConstantTable()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	_matrix			matView;

	m_pShaderCom->Set_Value("g_matWorld", &m_pTransformCom->Get_WorldMatrix(), sizeof(_matrix));
	m_pShaderCom->Set_Value("g_matView", matView = Get_Transform(D3DTS_VIEW), sizeof(_matrix));
	m_pShaderCom->Set_Value("g_matProj", Get_Transform(D3DTS_PROJECTION), sizeof(_matrix));

	return NOERROR;
}

CGameObject * CStairs::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CGameObject*		pInstance = new CStairs(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		MSG_BOX("Failed To Create CGameObject Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CStairs::Clone_GameObject(void * pArg)
{
	CGameObject*		pInstance = new CStairs(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		MSG_BOX("Failed To Create CGameObject Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStairs::Free()
{
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pFrustumCom);
	Safe_Release(m_pMeshCom);
	Safe_Release(m_pCollisionMesh);
	Safe_Release(m_pCollisionTransform);
	Safe_Release(m_pCollisionCom);

	CGameObject::Free();
}
