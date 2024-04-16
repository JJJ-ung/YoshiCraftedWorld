#pragma once

#include "Component.h"

class CAnimationCtrl;
class CHierarchyLoader;
class CDynamicMesh final : public CComponent
{
public:
	explicit CDynamicMesh(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual  ~CDynamicMesh() = default;

public:
	_uint Get_NumMeshContainer() const { return m_MeshContainerList.size(); }
	_uint Get_NumMaterials(_uint iMeshConstainerIdx) { return m_MeshContainerList[iMeshConstainerIdx]->NumMaterials; }

public:
	virtual HRESULT Ready_Component_Prototype(const _tchar* pFilePath, const _tchar* pFileName);

public:
	HRESULT Update_CombinedTransformationMatrices(D3DXFRAME* pFrame, _matrix ParentMatrix);
	HRESULT SetUp_CombinedTransformationMatricesPointer(D3DXFRAME* pFrame);
	HRESULT SetTexture_OnShader(LPD3DXEFFECT pEffect, D3DXHANDLE pConstantName, _uint iMeshContainerIdx, _uint iAttributeID, MESHTEXTURE::TYPE eType);
	HRESULT SetMaterial_OnShader(LPD3DXEFFECT pEffect, D3DXHANDLE pConstantName, _uint iMeshContainerIdx, _uint iAttributeID);
	HRESULT Update_SkinnedMesh(_int iMeshContainerIdx);
	HRESULT Render_Mesh(_uint iMeshContainerIdx, _uint iAttributeID);

public:
	HRESULT SetUp_Animation(_uint iIndex);
	HRESULT Play_Animation(_double TimeDelta);
	_bool Finish_Animation();
	_bool Finish_Animation(_double TimeLimit);

public:
	D3DXFRAME*			m_pRootFrame = nullptr;
	_matrix				m_matPivot;
	CHierarchyLoader*	m_pLoader = nullptr;

public:
	CAnimationCtrl*		m_pAniCtrl = nullptr;

public:
	vector<LPD3DXMESHCONTAINER_DERIVED>			m_MeshContainerList;
	typedef vector<LPD3DXMESHCONTAINER_DERIVED>	MESHCONTAINERLIST;

public:
	const _matrix* Find_CombinedTransformationMatrix(const char* pFrameName);

public:
	static CDynamicMesh* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pFilePath, const _tchar* pFileName);
	virtual void Free();
};
