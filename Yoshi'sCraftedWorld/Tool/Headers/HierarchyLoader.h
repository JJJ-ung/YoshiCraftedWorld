#pragma once

class CHierarchyLoader final : public ID3DXAllocateHierarchy
{
public:
	explicit CHierarchyLoader(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CHierarchyLoader() = default;

public:
	HRESULT Ready_HierarchyLoader(const _tchar* pFilePath);

public:
	STDMETHOD(CreateFrame)(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame);
	STDMETHOD(CreateMeshContainer)(THIS_ LPCSTR Name, CONST D3DXMESHDATA *pMeshData, CONST D3DXMATERIAL *pMaterials, CONST D3DXEFFECTINSTANCE *pEffectInstances, DWORD NumMaterials, CONST DWORD *pAdjacency, LPD3DXSKININFO pSkinInfo, LPD3DXMESHCONTAINER *ppNewMeshContainer);

	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree);
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree);

public:
	LPDIRECT3DDEVICE9	m_pGraphic_Device = nullptr;

public:
	const _tchar*				m_pFilePath = nullptr;

public:
	HRESULT SetUp_Name(char** ppSourName, const char* pDestName);
	HRESULT Change_TextureFileName(_tchar* pFilePath, _tchar * pSourMark, _tchar * pDestMark);

public:
	static CHierarchyLoader* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pFilePath);
	virtual void Free();
};
