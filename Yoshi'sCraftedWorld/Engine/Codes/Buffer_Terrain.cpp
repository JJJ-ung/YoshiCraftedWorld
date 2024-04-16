#include "Buffer_Terrain.h"

USING(Engine)

Engine::CBuffer_Terrain::CBuffer_Terrain(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
	, m_pPosition(nullptr)	
{

}

Engine::CBuffer_Terrain::CBuffer_Terrain(const CBuffer_Terrain& rhs)
	:CVIBuffer(rhs)
	, m_pPosition(rhs.m_pPosition)	
	,m_fh(rhs.m_fh)
	,m_ih(rhs.m_ih)
{

}

HRESULT Engine::CBuffer_Terrain::Ready_Component_Prototype(const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv)
{	
	m_dwTriCnt = (dwCntX - 1) * (dwCntZ - 1) * 2;
	m_dwVtxCnt = dwCntX * dwCntZ;

	m_pPosition = new _vec3[m_dwVtxCnt];

	m_dwVtxFVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
	m_dwVtxSize = sizeof(VTXNORTEX);
	
	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;		

	if (FAILED(CVIBuffer::Ready_Component_Prototype()))
		return E_FAIL;

	VTXNORTEX*		pVtxTex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVtxTex, 0);

	_ulong		dwIndex = 0;

	for (_ulong i = 0; i < dwCntZ; ++i)
	{
		for (_ulong j = 0; j < dwCntX; ++j)
		{
			dwIndex = i * dwCntX + j;

			pVtxTex[dwIndex].vPosition   = _vec3(_float(j) * dwVtxItv, 0.0f, _float(i) * dwVtxItv);
			m_pPosition[dwIndex] = pVtxTex[dwIndex].vPosition;
			
			pVtxTex[dwIndex].vNormal = _vec3(0.f, 0.f, 0.f);

			pVtxTex[dwIndex].vTexUV = _vec2(_float(j) / (dwCntX - 1), _float(i) / (dwCntZ - 1));
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

			pVtxTex[pIndex[dwTriIdx]._0].vNormal += vNormal;
			pVtxTex[pIndex[dwTriIdx]._1].vNormal += vNormal;
			pVtxTex[pIndex[dwTriIdx]._2].vNormal += vNormal;
			++dwTriIdx;

			// ¿ÞÂÊ ¾Æ·¡
			pIndex[dwTriIdx]._0 = dwIndex + dwCntX;
			pIndex[dwTriIdx]._1 = dwIndex + 1;
			pIndex[dwTriIdx]._2 = dwIndex;

			vDest = pVtxTex[pIndex[dwTriIdx]._1].vPosition - pVtxTex[pIndex[dwTriIdx]._0].vPosition;
			vSour = pVtxTex[pIndex[dwTriIdx]._2].vPosition - pVtxTex[pIndex[dwTriIdx]._1].vPosition;
			D3DXVec3Cross(&vNormal, &vDest, &vSour);

			pVtxTex[pIndex[dwTriIdx]._0].vNormal += vNormal;
			pVtxTex[pIndex[dwTriIdx]._1].vNormal += vNormal;
			pVtxTex[pIndex[dwTriIdx]._2].vNormal += vNormal;

			++dwTriIdx;
		}
	}	

	for (_ulong i = 0; i < m_dwVtxCnt; ++i)
		D3DXVec3Normalize(&pVtxTex[i].vNormal, &pVtxTex[i].vNormal);	
	
	m_pVB->Unlock();
	m_pIB->Unlock();

	return NOERROR;
}

HRESULT CBuffer_Terrain::Ready_Component_Prototype(const _tchar * pHeightMap, const _ulong & dwVtxItv)
{
	HANDLE		hFile;
	_ulong		dwByte = 0;

	hFile = CreateFile(pHeightMap, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	ReadFile(hFile, &m_fh, sizeof(BITMAPFILEHEADER), &dwByte, nullptr);
	ReadFile(hFile, &m_ih, sizeof(BITMAPINFOHEADER), &dwByte, nullptr);

	_ulong*	pPixel = new _ulong[m_ih.biWidth * m_ih.biHeight];

	ReadFile(hFile, pPixel, sizeof(_ulong) * m_ih.biWidth * m_ih.biHeight, &dwByte, nullptr);

	_ulong		dwCntX = m_ih.biWidth;
	_ulong		dwCntZ = m_ih.biHeight;

	m_dwTriCnt = (dwCntX - 1) * (dwCntZ - 1) * 2;
	m_dwVtxCnt = dwCntX * dwCntZ;

	m_pPosition = new _vec3[m_dwVtxCnt];

	m_dwVtxFVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
	m_dwVtxSize = sizeof(VTXNORTEX);

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	if (FAILED(CVIBuffer::Ready_Component_Prototype()))
		return E_FAIL;

	VTXNORTEX*		pVtxTex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVtxTex, 0);

	_ulong		dwIndex = 0;

	for (_ulong i = 0; i < dwCntZ; ++i)
	{
		for (_ulong j = 0; j < dwCntX; ++j)
		{
			dwIndex = i * dwCntX + j;

			pVtxTex[dwIndex].vPosition = _vec3(_float(j) * dwVtxItv, (pPixel[dwIndex] & 0x000000ff) / 10.0f, _float(i) * dwVtxItv);
			m_pPosition[dwIndex] = pVtxTex[dwIndex].vPosition;

			pVtxTex[dwIndex].vNormal = _vec3(0.f, 0.f, 0.f);

			pVtxTex[dwIndex].vTexUV = _vec2(_float(j) / (dwCntX - 1), _float(i) / (dwCntZ - 1));
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

			pVtxTex[pIndex[dwTriIdx]._0].vNormal += vNormal;
			pVtxTex[pIndex[dwTriIdx]._1].vNormal += vNormal;
			pVtxTex[pIndex[dwTriIdx]._2].vNormal += vNormal;
			++dwTriIdx;

			// ¿ÞÂÊ ¾Æ·¡
			pIndex[dwTriIdx]._0 = dwIndex + dwCntX;
			pIndex[dwTriIdx]._1 = dwIndex + 1;
			pIndex[dwTriIdx]._2 = dwIndex;

			vDest = pVtxTex[pIndex[dwTriIdx]._1].vPosition - pVtxTex[pIndex[dwTriIdx]._0].vPosition;
			vSour = pVtxTex[pIndex[dwTriIdx]._2].vPosition - pVtxTex[pIndex[dwTriIdx]._1].vPosition;
			D3DXVec3Cross(&vNormal, &vDest, &vSour);

			pVtxTex[pIndex[dwTriIdx]._0].vNormal += vNormal;
			pVtxTex[pIndex[dwTriIdx]._1].vNormal += vNormal;
			pVtxTex[pIndex[dwTriIdx]._2].vNormal += vNormal;

			++dwTriIdx;
		}
	}

	for (_ulong i = 0; i < m_dwVtxCnt; ++i)
		D3DXVec3Normalize(&pVtxTex[i].vNormal, &pVtxTex[i].vNormal);

	m_pVB->Unlock();
	m_pIB->Unlock();

	Safe_Delete_Array(pPixel);
	CloseHandle(hFile);

	return NOERROR;
}



HRESULT CBuffer_Terrain::Ready_Component(void * pArg)
{
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


CBuffer_Terrain* Engine::CBuffer_Terrain::Create(LPDIRECT3DDEVICE9 pGraphicDev, 
										const _ulong& dwCntX,
										const _ulong& dwCntZ,
										const _ulong& dwVtxItv)
{
	CBuffer_Terrain*		pInstance = new CBuffer_Terrain(pGraphicDev);

	if (FAILED(pInstance->Ready_Component_Prototype(dwCntX, dwCntZ, dwVtxItv)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

CBuffer_Terrain * CBuffer_Terrain::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar * pHeightMap, const _ulong & dwVtxItv)
{
	CBuffer_Terrain*		pInstance = new CBuffer_Terrain(pGraphicDev);

	if (FAILED(pInstance->Ready_Component_Prototype(pHeightMap, dwVtxItv)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

CComponent* Engine::CBuffer_Terrain::Clone_Component(void* pArg)
{
	CBuffer_Terrain*	pInstance = new CBuffer_Terrain(*this);

	if (FAILED(pInstance->Ready_Component(pArg)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}
void Engine::CBuffer_Terrain::Free(void)
{
	if(false == m_isClone)
		Safe_Delete_Array(m_pPosition);

	CVIBuffer::Free();
}



