#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CMesh_Static final : public CComponent
{
private:
	explicit CMesh_Static(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CMesh_Static(const CMesh_Static& rhs);
	virtual ~CMesh_Static() = default;

public:
	_ulong Get_NumMaterials() const { return m_dwNumMaterials; }
	LPD3DXMESH Get_Mesh() { return m_pMesh; }

public:
	virtual HRESULT Ready_Component_Prototype(const _tchar* pFilePath, const _tchar* pFileName);
	virtual HRESULT Ready_Component(void* pArg);

public:
	HRESULT SetTexture_OnShader(LPD3DXEFFECT pEffect, D3DXHANDLE pConstantName, _uint iAttributeID, MESHTEXTURE::TYPE eType = MESHTEXTURE::TYPE_DIFFUSE);
	HRESULT SetMaterial_OnShader(LPD3DXEFFECT pEffect, D3DXHANDLE pConstantName, _uint iAttributeID);
	HRESULT Render_Mesh(_uint iAttributeID);

private:
	LPD3DXMESH			m_pMesh = nullptr;
	LPD3DXBUFFER		m_pAdjacencyBuff = nullptr;
	LPD3DXBUFFER		m_pMaterialsBuff = nullptr;
	vector<D3DXMATERIAL>					m_vecMaterials;
	vector<MESHTEXTURE>					m_vecTextures;
	_ulong				m_dwNumMaterials = 0;
	_matrix				m_matPivot;

private:
	HRESULT Change_TextureFileName(_tchar* pFilePath, _tchar * pSourMark, _tchar * pDestMark);

public:
	static CMesh_Static* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pFilePath, const _tchar* pFileName);
	virtual CComponent* Clone_Component(void* pArg);
	virtual void Free();
};

END