#include "stdafx.h"
#include "..\Headers\MapObject.h"
#include "Transform.h"
#include "Shader.h"
#include "StaticMesh.h"

CMapObject::CMapObject(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

HRESULT CMapObject::Ready_GameObject(const _tchar* pFilePath, _vec3 vPos, _uint eType)
{
	if (FAILED(Add_Component(pFilePath)))
		return E_FAIL;

	m_strFilePath = pFilePath;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	m_iShaderPass = 1;

	m_eCollisionType = eType;

	return NOERROR;
}

_int CMapObject::Update_GameObject(_double TimeDelta)
{
	return _int();
}

_int CMapObject::LateUpdate_GameObject(_double TimeDelta)
{
	return _int();
}

HRESULT CMapObject::Render_GameObject()
{
	if (nullptr == m_pStaticMeshCom ||
		nullptr == m_pShaderCom)
		return E_FAIL;

	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if (nullptr == pEffect)
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	if (FAILED(m_pStaticMeshCom->Render_Mesh(pEffect, "g_BaseTexture", m_iShaderPass)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CMapObject::Add_Component(const _tchar* pFilePath)
{
	// For.Com_Transform
	m_pTransformCom = CTransform::Create(m_pGraphic_Device, CTransform::STATE(1.f, D3DXToRadian(90)));
	if (FAILED(CGameObject::Add_Component(L"Com_Transform", m_pTransformCom)))
		return E_FAIL;
	m_pTransformCom->SetUp_Scale(_vec3(0.005f, 0.005f, 0.005f));

	// For.Com_Buffer
	m_pStaticMeshCom = CStaticMesh::Create(m_pGraphic_Device, pFilePath);
	if (FAILED(CGameObject::Add_Component(L"Com_StaticMesh", m_pStaticMeshCom)))
		return E_FAIL;

	// For.Com_Shader
	m_pShaderCom = CShader::Create(m_pGraphic_Device, L"../Bin/Shader/Shader_Default.fx");
	if (FAILED(CGameObject::Add_Component(L"Com_Shader", m_pShaderCom)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CMapObject::SetUp_ConstantTable()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	m_pShaderCom->Set_Value("g_matWorld", m_pTransformCom->Get_WorldMatrix(), sizeof(_matrix));
	m_pShaderCom->Set_Value("g_matView", Get_Transform(D3DTS_VIEW), sizeof(_matrix));
	m_pShaderCom->Set_Value("g_matProj", Get_Transform(D3DTS_PROJECTION), sizeof(_matrix));

	return NOERROR;
}

CMapObject * CMapObject::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath, _vec3 vPos, _uint eType)
{
	CMapObject*		pInstance = new CMapObject(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(pFilePath, vPos, eType)))
	{
		MSG_BOX("Failed To Create CGameObject Instance");
		Safe_Delete(pInstance);
	}

	return pInstance;
}
