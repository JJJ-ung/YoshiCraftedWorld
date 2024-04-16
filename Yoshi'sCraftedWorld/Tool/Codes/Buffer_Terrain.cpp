#include "stdafx.h"
#include "Buffer_Terrain.h"

CBuffer_Terrain::CBuffer_Terrain(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
	, m_pPosition(nullptr)	
{

}

HRESULT CBuffer_Terrain::Ready_Component(const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv)
{	
	m_dwTriCnt = (dwCntX - 1) * (dwCntZ - 1) * 2;
	m_dwVtxCnt = dwCntX * dwCntZ;

	m_iH.biWidth = dwCntX;
	m_iH.biHeight = dwCntZ;

	m_pPosition = new _vec3[m_dwVtxCnt];

	m_dwVtxFVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;
	m_dwVtxSize = sizeof(VTXCOL);
	
	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;		

	if (FAILED(CVIBuffer::Ready_Component()))
		return E_FAIL;

	VTXCOL*		pVtxTex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVtxTex, 0);

	_ulong		dwIndex = 0;

	for (_ulong i = 0; i < dwCntZ; ++i)
	{
		for (_ulong j = 0; j < dwCntX; ++j)
		{
			dwIndex = i * dwCntX + j;

			pVtxTex[dwIndex].vPosition   = _vec3(_float(j) * dwVtxItv, 0.0f, _float(i) * dwVtxItv);
			m_pPosition[dwIndex] = pVtxTex[dwIndex].vPosition;
		}
	}

	INDEX32*		pIndex = NULL;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	_ulong	dwTriIdx = 0;

	for (_ulong i = 0; i < dwCntZ - 1; ++i)
	{
		for (_ulong j = 0; j < dwCntX - 1; ++j)
		{
			dwIndex = i * dwCntX + j;

			pIndex[dwTriIdx]._0 = dwIndex + dwCntX;
			pIndex[dwTriIdx]._1 = dwIndex + dwCntX + 1;
			pIndex[dwTriIdx]._2 = dwIndex + 1;

			_vec3	vDest, vSour, vNormal;

			vDest = pVtxTex[pIndex[dwTriIdx]._1].vPosition - pVtxTex[pIndex[dwTriIdx]._0].vPosition;
			vSour = pVtxTex[pIndex[dwTriIdx]._2].vPosition - pVtxTex[pIndex[dwTriIdx]._1].vPosition;
			D3DXVec3Cross(&vNormal, &vDest, &vSour);

			++dwTriIdx;

			// ¿ÞÂÊ ¾Æ·¡
			pIndex[dwTriIdx]._0 = dwIndex + dwCntX;
			pIndex[dwTriIdx]._1 = dwIndex + 1;
			pIndex[dwTriIdx]._2 = dwIndex;

			vDest = pVtxTex[pIndex[dwTriIdx]._1].vPosition - pVtxTex[pIndex[dwTriIdx]._0].vPosition;
			vSour = pVtxTex[pIndex[dwTriIdx]._2].vPosition - pVtxTex[pIndex[dwTriIdx]._1].vPosition;
			D3DXVec3Cross(&vNormal, &vDest, &vSour);

			++dwTriIdx;
		}
	}	
	
	m_pVB->Unlock();
	m_pIB->Unlock();

	return NOERROR;
}

HRESULT CBuffer_Terrain::Render_Buffer(void)
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


CBuffer_Terrain* CBuffer_Terrain::Create(LPDIRECT3DDEVICE9 pGraphicDev, 
										const _ulong& dwCntX,
										const _ulong& dwCntZ,
										const _ulong& dwVtxItv)
{
	CBuffer_Terrain*		pInstance = new CBuffer_Terrain(pGraphicDev);

	if (FAILED(pInstance->Ready_Component(dwCntX, dwCntZ, dwVtxItv)))
		Safe_Delete(pInstance);

	return pInstance;
}

void CBuffer_Terrain::Free(void)
{
	Safe_Delete_Array(m_pPosition);

	CVIBuffer::Free();
}



