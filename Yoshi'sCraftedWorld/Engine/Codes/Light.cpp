#include "..\Headers\Light.h"

CLight::CLight(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device(pGraphic_Device)
{
	Safe_AddRef(m_pGraphic_Device);
}

HRESULT CLight::Ready_Light(const D3DLIGHT9 & LightInfo)
{
	m_LightInfo = LightInfo;

	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	D3DVIEWPORT9	ViewPort;
	m_pGraphic_Device->GetViewport(&ViewPort);

	if (FAILED(m_pGraphic_Device->CreateVertexBuffer(sizeof(VTXVIEWPORTTEX) * 4, 0, D3DFVF_XYZRHW | D3DFVF_TEX1, D3DPOOL_MANAGED, &m_pVB, nullptr)))
		return E_FAIL;

	VTXVIEWPORTTEX*		pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0].vPosition = _vec4(0.0f - 0.5f, 0.0f - 0.5f, 0.f, 1.f);
	pVertices[0].vTexUV = _vec2(0.f, 0.f);

	pVertices[1].vPosition = _vec4(ViewPort.Width - 0.5f, 0.0f - 0.5f, 0.f, 1.f);
	pVertices[1].vTexUV = _vec2(1.f, 0.f);

	pVertices[2].vPosition = _vec4(ViewPort.Width - 0.5f, ViewPort.Height - 0.5f, 0.f, 1.f);
	pVertices[2].vTexUV = _vec2(1.f, 1.f);

	pVertices[3].vPosition = _vec4(0.0f - 0.5f, ViewPort.Height - 0.5f, 0.f, 1.f);
	pVertices[3].vTexUV = _vec2(0.f, 1.f);

	m_pVB->Unlock();


	if (FAILED(m_pGraphic_Device->CreateIndexBuffer(sizeof(INDEX16) * 2, 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB, nullptr)))
		return E_FAIL;

	INDEX16*		pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	pIndices[0]._0 = 0;
	pIndices[0]._1 = 1;
	pIndices[0]._2 = 2;

	pIndices[1]._0 = 0;
	pIndices[1]._1 = 2;
	pIndices[1]._2 = 3;

	m_pIB->Unlock();

	return NOERROR;
}

HRESULT CLight::Render_Light(LPD3DXEFFECT pEffect)
{
	if (nullptr == m_pGraphic_Device ||
		nullptr == m_pVB ||
		nullptr == m_pIB)
		return E_FAIL;

	_uint		iPassIndex = 0;

	if (D3DLIGHT_DIRECTIONAL == m_LightInfo.Type)
	{
		iPassIndex = 0;
		pEffect->SetVector("g_vLightDir", &_vec4(m_LightInfo.Direction, 0.f));
	}

	else if (D3DLIGHT_POINT == m_LightInfo.Type)
	{
		iPassIndex = 1;
		pEffect->SetVector("g_vLightPos", &_vec4(m_LightInfo.Position, 1.f));
		pEffect->SetFloat("g_fRange", m_LightInfo.Range);
	}	

	pEffect->SetVector("g_vLightDiffuse", (_vec4*)&m_LightInfo.Diffuse);
	pEffect->SetVector("g_vLightAmbient", (_vec4*)&m_LightInfo.Ambient);
	pEffect->SetVector("g_vLightSpecular", (_vec4*)&m_LightInfo.Specular);

	_matrix		matView;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, nullptr, &matView);
	pEffect->SetMatrix("g_matViewInv", &matView);
	pEffect->SetVector("g_vCamPosition", (_vec4*)&matView.m[3][0]);

	_matrix		matProj;
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &matProj);

	_vec4 vProjValue;
	vProjValue.x = matProj.m[0][0];
	vProjValue.y = matProj.m[1][1];
	vProjValue.z = matProj.m[3][2];
	vProjValue.w = matProj.m[2][2];

	pEffect->SetVector("g_ProjValues", &vProjValue);

	D3DXMatrixInverse(&matProj, nullptr, &matProj);
	pEffect->SetMatrix("g_matProjInv", &matProj);

	pEffect->CommitChanges();

	pEffect->BeginPass(iPassIndex);

	m_pGraphic_Device->SetStreamSource(0, m_pVB, 0, sizeof(VTXVIEWPORTTEX));
	m_pGraphic_Device->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1);
	m_pGraphic_Device->SetIndices(m_pIB);
	m_pGraphic_Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	pEffect->EndPass();


	return NOERROR;
}

CLight * CLight::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const D3DLIGHT9 & LightInfo)
{
	CLight*		pInstance = new CLight(pGraphic_Device);

	if (FAILED(pInstance->Ready_Light(LightInfo)))
	{
		MSG_BOX("Failed To Create CLight Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CLight::Free()
{
	Safe_Release(m_pVB);
	Safe_Release(m_pIB);
	Safe_Release(m_pGraphic_Device);
}
