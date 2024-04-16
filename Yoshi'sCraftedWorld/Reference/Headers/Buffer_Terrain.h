#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CBuffer_Terrain final : public CVIBuffer
{
private:
	explicit CBuffer_Terrain(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBuffer_Terrain(const CBuffer_Terrain& rhs);
	virtual ~CBuffer_Terrain(void) = default;

public:
	const _vec3* Get_VtxPos(void) const { return m_pPosition;	}

public:
	virtual HRESULT Ready_Component_Prototype(const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv);
	virtual HRESULT Ready_Component_Prototype(const _tchar* pHeightMap, const _ulong& dwVtxItv);
	virtual HRESULT Ready_Component(void* pArg);
	virtual HRESULT Render_Buffer(void);

private:
	HANDLE				m_hFile;
	BITMAPFILEHEADER	m_fh;
	BITMAPINFOHEADER	m_ih;
	_vec3*				m_pPosition = nullptr;	

public:
	static CBuffer_Terrain* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv);
	static CBuffer_Terrain* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pHeightMap, const _ulong& dwVtxItv);
	virtual CComponent*	Clone_Component(void* pArg);
	virtual void Free(void);
};

END
