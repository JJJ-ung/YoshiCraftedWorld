#include "stdafx.h"
#include "..\Headers\Buffer_RcTex.h"

CBuffer_RcTex::CBuffer_RcTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{
}

HRESULT CBuffer_RcTex::Ready_Component(void)
{
	m_dwVtxCnt = 4;
	m_dwVtxFVF = D3DFVF_XYZ | D3DFVF_TEX1;
	m_dwTriCnt = 2;
	m_dwVtxSize = sizeof(VTXTEX);

	m_IdxFmt = D3DFMT_INDEX16;

	m_dwIdxSize = sizeof(INDEX16);

	if (FAILED(CVIBuffer::Ready_Component()))
		return E_FAIL;

	VTXTEX*		pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPosition = _vec3(-0.5f, 0.5f, 0.f);
	pVertex[0].vTexUV = _vec2(0.f, 0.f);

	pVertex[1].vPosition = _vec3(0.5f, 0.5f, 0.f);
	pVertex[1].vTexUV = _vec2(1.f, 0.f);

	pVertex[2].vPosition = _vec3(0.5f, -0.5f, 0.f);
	pVertex[2].vTexUV = _vec2(1.f, 1.f);

	pVertex[3].vPosition = _vec3(-0.5f, -0.5f, 0.f);
	pVertex[3].vTexUV = _vec2(0.f, 1.f);

	m_pVB->Unlock();

	INDEX16*		pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	pIndex[0]._0 = 0;
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 2;

	pIndex[1]._0 = 0;
	pIndex[1]._1 = 2;
	pIndex[1]._2 = 3;

	m_pIB->Unlock();

	return NOERROR;
}

HRESULT CBuffer_RcTex::Render_Buffer(void)
{
	if (nullptr == m_pGraphic_Device ||
		nullptr == m_pVB ||
		nullptr == m_pIB)
		return E_FAIL;

	if (FAILED(m_pGraphic_Device->SetStreamSource(0, m_pVB, 0, m_dwVtxSize)))
		return E_FAIL;

	if (FAILED(m_pGraphic_Device->SetFVF(m_dwVtxFVF)))
		return E_FAIL;

	if (FAILED(m_pGraphic_Device->SetIndices(m_pIB)))
		return E_FAIL;

	if (FAILED(m_pGraphic_Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt)))
		return E_FAIL;

	return NOERROR;
}

CBuffer_RcTex* CBuffer_RcTex::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBuffer_RcTex*	pInstance = new CBuffer_RcTex(pGraphicDev);

	if (FAILED(pInstance->Ready_Component()))
		Safe_Delete(pInstance);

	return pInstance;
}

void CBuffer_RcTex::Free(void)
{
	CVIBuffer::Free();
}

