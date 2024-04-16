#include "..\Headers\Target.h"

CTarget::CTarget(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device(pGraphic_Device)
{
	Safe_AddRef(m_pGraphic_Device);
}

HRESULT CTarget::Ready_Target(_uint iSizeX, _uint iSizeY, D3DFORMAT eFormat, D3DXCOLOR Color)
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	// 1. 텍스쳐를 생성하나다.
	if (FAILED(D3DXCreateTexture(m_pGraphic_Device, iSizeX, iSizeY, 1, D3DUSAGE_RENDERTARGET, eFormat, D3DPOOL_DEFAULT, &m_pTexture)))
		return E_FAIL;

	// 2. 렌더타겟을 얻어온다.
	if (FAILED(m_pTexture->GetSurfaceLevel(0, &m_pSurface)))
		return E_FAIL;

	m_ClearColor = Color;

	return NOERROR;
}

HRESULT CTarget::SetUp_OnShader(LPD3DXEFFECT pEffect, D3DXHANDLE hHandle)
{
	if (nullptr == pEffect)
		return E_FAIL;

	pEffect->SetTexture(hHandle, m_pTexture);

	return NOERROR;
}

HRESULT CTarget::SetUp_OnGraphicDev(_uint iIndex)
{
	// 기존에 있던걸 뺀다.
	m_pGraphic_Device->GetRenderTarget(iIndex, &m_pOldSurface);

	// 내껄 셋팅한다.
	m_pGraphic_Device->SetRenderTarget(iIndex, m_pSurface);


	return NOERROR;
}

HRESULT CTarget::Release_OnGraphicDev(_uint iIndex)
{
	// 내껄 셋팅한다.
	m_pGraphic_Device->SetRenderTarget(iIndex, m_pOldSurface);

	Safe_Release(m_pOldSurface);	

	return NOERROR;
}

HRESULT CTarget::Clear_Target()
{
	// 기존에 있던걸 뺀다.
	m_pGraphic_Device->GetRenderTarget(0, &m_pOldSurface);

	// 내껄 셋팅한다.
	m_pGraphic_Device->SetRenderTarget(0, m_pSurface);

	m_pGraphic_Device->Clear(0, nullptr, D3DCLEAR_TARGET, m_ClearColor, 1.f, 0);

	m_pGraphic_Device->SetRenderTarget(0, m_pOldSurface);

	Safe_Release(m_pOldSurface);

	return NOERROR;
}

HRESULT CTarget::Ready_Debug_Buffer(_float fX, _float fY, _float fSizeX, _float fSizeY)
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	if (FAILED(m_pGraphic_Device->CreateVertexBuffer(sizeof(VTXVIEWPORTTEX) * 4, 0, D3DFVF_XYZRHW | D3DFVF_TEX1, D3DPOOL_MANAGED, &m_pVB, nullptr)))
		return E_FAIL;

	VTXVIEWPORTTEX*		pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0].vPosition = _vec4(fX, fY, 0.f, 1.f);
	pVertices[0].vTexUV = _vec2(0.f, 0.f);

	pVertices[1].vPosition = _vec4(fX + fSizeX, fY, 0.f, 1.f);
	pVertices[1].vTexUV = _vec2(1.f, 0.f);

	pVertices[2].vPosition = _vec4(fX + fSizeX, fY + fSizeY, 0.f, 1.f);
	pVertices[2].vTexUV = _vec2(1.f, 1.f);

	pVertices[3].vPosition = _vec4(fX, fY + fSizeY, 0.f, 1.f);
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

HRESULT CTarget::Render_Debug_Buffer()
{
	if (nullptr == m_pGraphic_Device || 
		nullptr == m_pVB || 
		nullptr == m_pIB)
		return E_FAIL;

	m_pGraphic_Device->SetTexture(0, m_pTexture);

	m_pGraphic_Device->SetStreamSource(0, m_pVB, 0, sizeof(VTXVIEWPORTTEX));
	m_pGraphic_Device->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1);
	m_pGraphic_Device->SetIndices(m_pIB);
	m_pGraphic_Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	return NOERROR;
}

CTarget * CTarget::Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iSizeX, _uint iSizeY, D3DFORMAT eFormat, D3DXCOLOR Color)
{
	CTarget*		pInstance = new CTarget(pGraphic_Device);

	if (FAILED(pInstance->Ready_Target(iSizeX, iSizeY, eFormat, Color)))
	{
		MSG_BOX("Failed To Create CLight Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CTarget::Free()
{
#ifdef _DEBUG
	Safe_Release(m_pVB);
	Safe_Release(m_pIB);
#endif

	Safe_Release(m_pSurface);
	Safe_Release(m_pTexture);
	Safe_Release(m_pGraphic_Device);
}
