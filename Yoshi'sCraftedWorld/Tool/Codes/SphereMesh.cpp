#include "stdafx.h"
#include "..\Headers\SphereMesh.h"
#include "Transform.h"
#include "Shader.h"
#include "Camera.h"

CSphereMesh::CSphereMesh(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CComponent(pGraphicDevice)
{
}

HRESULT CSphereMesh::Ready_Component(float fRadius)
{
	m_fRadius = fRadius;
	m_pTransform = CTransform::Create(m_pGraphic_Device, CTransform::STATE(5.f, D3DXToRadian(90.f)));

	m_pShader = CShader::Create(m_pGraphic_Device, L"../Bin/Shader/Shader_Collider.fx");
	if (nullptr == m_pShader)
		return E_FAIL;

	if (FAILED(D3DXCreateSphere(m_pGraphic_Device, m_fRadius, 10, 10, &m_pMesh, NULL)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CSphereMesh::Render_Mesh()
{
	if (nullptr == m_pMesh ||
		nullptr == m_pShader ||
		nullptr == m_pTransform)
		return E_FAIL;

	LPD3DXEFFECT pEffect = m_pShader->Get_EffectHandle();
	if (nullptr == pEffect)
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	m_pMesh->DrawSubset(0);

	pEffect->EndPass();
	pEffect->End();

	return NOERROR;
}

_int CSphereMesh::Update_Collider()
{
	m_RenderingMatrix = nullptr == m_pParentMatrix
		? m_pTransform->Get_WorldMatrix() : m_pTransform->Get_WorldMatrix() * *m_pParentMatrix;

	return _int();
}

HRESULT CSphereMesh::SetUp_ConstantTable()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pShader->Set_Value("g_matWorld", &m_RenderingMatrix, sizeof(_matrix));
	m_pShader->Set_Value("g_matView", &CCamera::GetInstance()->m_matView, sizeof(_matrix));
	m_pShader->Set_Value("g_matProj", &CCamera::GetInstance()->m_matProj, sizeof(_matrix));
	m_pShader->Get_EffectHandle()->SetBool("g_isColl", true);

	return NOERROR;
}

CSphereMesh * CSphereMesh::Create(LPDIRECT3DDEVICE9 pGraphicDev, float fRadius)
{
	CSphereMesh* pInstance = new CSphereMesh(pGraphicDev);

	if (FAILED(pInstance->Ready_Component(fRadius)))
		Safe_Delete(pInstance);

	return pInstance;
}

void CSphereMesh::Free()
{
	Safe_Release(m_pMesh);
}
