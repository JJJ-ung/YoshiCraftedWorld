#pragma once
#include "VIBuffer.h"

class CBuffer_Terrain final : public CVIBuffer
{
public:
	explicit CBuffer_Terrain(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBuffer_Terrain(void) = default;

public:
	const _vec3* Get_VtxPos(void) const { return m_pPosition;	}
	_ulong Get_VtxCntX(void) const { return m_iH.biWidth; }
	_ulong Get_VtxCntZ(void) const { return m_iH.biHeight; }

public:
	virtual HRESULT Ready_Component(const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv);
	virtual HRESULT Render_Buffer(void);

private:
	HANDLE				m_hFile;
	BITMAPFILEHEADER	m_fH;
	BITMAPINFOHEADER	m_iH;
	_vec3*				m_pPosition = nullptr;	

public:
	static CBuffer_Terrain* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv);
	virtual void Free(void);
};