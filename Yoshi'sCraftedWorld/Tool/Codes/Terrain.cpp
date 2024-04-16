#include "stdafx.h"
#include "..\Headers\Terrain.h"

CTerrain::CTerrain(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
	m_vTerrainColor = _vec4((float)(100.f / 255.f), (float)(100.f / 255.f), (float)(100.f / 255.f), 1.f);
}

HRESULT CTerrain::Ready_GameObject()
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	return NOERROR;
}

_int CTerrain::Update_GameObject(_double TimeDelta)
{
	return _int();
}

_int CTerrain::LateUpdate_GameObject(_double TimeDelta)
{
	return _int();
}

HRESULT CTerrain::Render_GameObject()
{
	if (nullptr == m_pBufferCom ||
		nullptr == m_pShaderCom)
		return E_FAIL;

	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if (nullptr == pEffect)
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(m_iShaderPass);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	return NOERROR;
}

HRESULT CTerrain::Add_Component()
{
	// For.Com_Transform
	m_pTransformCom = CTransform::Create(m_pGraphic_Device, CTransform::STATE(1.f, D3DXToRadian(90)));
	if (FAILED(CGameObject::Add_Component(L"Com_Transform", m_pTransformCom)))
		return E_FAIL;

	// For.Com_Buffer
	m_pBufferCom = CBuffer_Terrain::Create(m_pGraphic_Device, 10, 10, 1);
	if (FAILED(CGameObject::Add_Component(L"Com_Buffer", m_pBufferCom)))
		return E_FAIL;

	// For.Com_Shader
	m_pShaderCom = CShader::Create(m_pGraphic_Device, L"../Bin/Shader/Shader_Color.fx");
	if (FAILED(CGameObject::Add_Component(L"Com_Shader", m_pShaderCom)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CTerrain::Change_TerrainSize(const _ulong & dwCntX, const _ulong & dwCntZ, const _ulong & dwVtxItv)
{
	if (m_pBufferCom != nullptr)
	{
		if (FAILED(CGameObject::Delete_Component(L"Com_Buffer")))
			return E_FAIL;
		m_pBufferCom = nullptr;
	}

	m_pBufferCom = CBuffer_Terrain::Create(m_pGraphic_Device, dwCntX, dwCntZ, dwVtxItv);
	if (FAILED(CGameObject::Add_Component(L"Com_Buffer", m_pBufferCom)))
		return E_FAIL;

	if (m_pBufferCom == nullptr)
		return E_FAIL;

	return NOERROR;
}

HRESULT CTerrain::Change_TerrainColor(float fA, float fR, float fG, float fB)
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	m_vTerrainColor = _vec4(fR, fG, fB, fA);

	return NOERROR;
}

HRESULT CTerrain::SetUp_ConstantTable()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	m_pShaderCom->Set_Value("g_matWorld", Get_Transform(D3DTS_WORLD), sizeof(_matrix));
	m_pShaderCom->Set_Value("g_matView", Get_Transform(D3DTS_VIEW), sizeof(_matrix));
	m_pShaderCom->Set_Value("g_matProj", Get_Transform(D3DTS_PROJECTION), sizeof(_matrix));
	m_pShaderCom->Set_Value("g_vColor", &m_vTerrainColor, sizeof(_vec4));

	return NOERROR;
}

CTerrain * CTerrain::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTerrain*		pInstance = new CTerrain(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("Failed To Create CGameObject Instance");
		Safe_Delete(pInstance);
	}

	return pInstance;
}
