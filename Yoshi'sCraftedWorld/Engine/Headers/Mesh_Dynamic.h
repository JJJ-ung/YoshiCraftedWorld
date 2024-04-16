#pragma once

#include "Component.h"

BEGIN(Engine)

class CAnimationCtrl;
class CHierarchyLoader;
class ENGINE_DLL CMesh_Dynamic final : public CComponent
{
private:
	explicit CMesh_Dynamic(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CMesh_Dynamic(const CMesh_Dynamic& rhs);
	virtual  ~CMesh_Dynamic() = default;

public:
	HRESULT Set_DelayTime(_double Time);
	HRESULT Set_Weight(_float Weight);
	HRESULT Set_AnimationSpeed(_float Speed);

public:
	_uint Get_NumMeshContainer() const { return m_MeshContainerList.size(); }
	_uint Get_NumMaterials(_uint iMeshConstainerIdx) { return m_MeshContainerList[iMeshConstainerIdx]->NumMaterials; }
	_matrix Get_FrameMatrix(const char* pFrameName);

public:
	virtual HRESULT Ready_Component_Prototype(const _tchar* pFilePath, const _tchar* pFileName);
	virtual HRESULT Ready_Component(void* pArg);

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

private:
	D3DXFRAME*			m_pRootFrame = nullptr;
	_matrix				m_matPivot;
	CHierarchyLoader*	m_pLoader = nullptr;

private:
	CAnimationCtrl*		m_pAniCtrl = nullptr;

private:
	vector<LPD3DXMESHCONTAINER_DERIVED>			m_MeshContainerList;
	typedef vector<LPD3DXMESHCONTAINER_DERIVED>	MESHCONTAINERLIST;

private:
	unordered_map<const char*, const _matrix*>			m_ReferenceBones;
	typedef unordered_map<const char*, const _matrix*>	REFERENCEBONE;

private:
	const _matrix* Find_CombinedTransformationMatrix(const char* pFrameName);

public:
	static CMesh_Dynamic* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pFilePath, const _tchar* pFileName);
	virtual CComponent* Clone_Component(void* pArg);
	virtual void Free();
};

END