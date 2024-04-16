#include "stdafx.h"
#include "..\Headers\Trigger.h"
#include "Transform.h"
#include "Shader.h"
#include "SphereMesh.h"

CTrigger::CTrigger(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

HRESULT CTrigger::Ready_GameObject(_vec3 vPos, _float fRadius)
{
	m_fRadius = fRadius;
	if (FAILED(Add_Component(fRadius)))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);

	m_Color = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);

	return NOERROR;
}

_int CTrigger::Update_GameObject(_double TimeDelta)
{
	return _int();
}

_int CTrigger::LateUpdate_GameObject(_double TimeDelta)
{
	return _int();
}

HRESULT CTrigger::Render_GameObject()
{
	if (nullptr == m_pMesh ||
		nullptr == m_pShaderCom ||
		nullptr == m_pTransformCom)
		return E_FAIL;

	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
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

HRESULT CTrigger::Add_Component(_float fRadius)
{
	// For.Com_Transform
	m_pTransformCom = CTransform::Create(m_pGraphic_Device, CTransform::STATE(1.f, D3DXToRadian(90)));
	if (FAILED(CGameObject::Add_Component(L"Com_Transform", m_pTransformCom)))
		return E_FAIL;

	// For.Com_Buffer
	if (FAILED(D3DXCreateSphere(m_pGraphic_Device, m_fRadius, 20, 20, &m_pMesh, NULL)))
		return E_FAIL;

	// For.Com_Shader
	m_pShaderCom = CShader::Create(m_pGraphic_Device, L"../Bin/Shader/Shader_Color.fx");
	if (FAILED(CGameObject::Add_Component(L"Com_Shader", m_pShaderCom)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CTrigger::SetUp_ConstantTable()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	m_pShaderCom->Set_Value("g_matWorld", m_pTransformCom->Get_WorldMatrix(), sizeof(_matrix));
	m_pShaderCom->Set_Value("g_matView", Get_Transform(D3DTS_VIEW), sizeof(_matrix));
	m_pShaderCom->Set_Value("g_matProj", Get_Transform(D3DTS_PROJECTION), sizeof(_matrix));
	m_pShaderCom->Set_Value("g_vColor", &m_Color, sizeof(_vec4));

	return NOERROR;
}

void CTrigger::Change_Radius(_float fRadius)
{
	m_pMesh->Release();
	m_pMesh = nullptr;

	m_fRadius = fRadius;

	if (FAILED(D3DXCreateSphere(m_pGraphic_Device, fRadius, 20, 20, &m_pMesh, NULL)))
		return;
}

CTrigger * CTrigger::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _float fRadius)
{
	CTrigger*		pInstance = new CTrigger(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos, fRadius)))
	{
		MSG_BOX("Failed To Create CGameObject Instance");
		Safe_Delete(pInstance);
	}

	return pInstance;
}
