#include "stdafx.h"
#include "..\Headers\DynamicMesh.h"
#include "HierarchyLoader.h"
#include "AnimationCtrl.h"


CDynamicMesh::CDynamicMesh(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{

}

HRESULT CDynamicMesh::Ready_Component_Prototype(const _tchar * pFilePath, const _tchar * pFileName)
{
	_tchar		szFullPath[128] = L"";

	LPD3DXMESH		pMesh = nullptr;

	lstrcpy(szFullPath, pFilePath);
	lstrcat(szFullPath, pFileName);

	m_pLoader = CHierarchyLoader::Create(m_pGraphic_Device, pFilePath);
	if (nullptr == m_pLoader)
		return E_FAIL;

	LPD3DXANIMATIONCONTROLLER		pAniCtrl = nullptr;

	if (FAILED(D3DXLoadMeshHierarchyFromX(szFullPath, D3DXMESH_MANAGED, m_pGraphic_Device, m_pLoader, nullptr, &m_pRootFrame, &pAniCtrl)))
		return E_FAIL;

	m_pAniCtrl = CAnimationCtrl::Create(m_pGraphic_Device, pAniCtrl);
	if (nullptr == m_pAniCtrl)
		return E_FAIL;

	_matrix		matScale, matRot;

	D3DXMatrixScaling(&matScale, 0.005f, 0.005f, 0.005f);
	D3DXMatrixRotationY(&matRot, D3DXToRadian(180.f));

	m_matPivot = matScale * matRot;

	Update_CombinedTransformationMatrices(m_pRootFrame, m_matPivot);

	SetUp_CombinedTransformationMatricesPointer(m_pRootFrame);

	return NOERROR;
}

HRESULT CDynamicMesh::Update_CombinedTransformationMatrices(D3DXFRAME* pFrame, _matrix ParentMatrix)
{
	LPD3DXFRAME_DERIVED	pFrame_Derived = (LPD3DXFRAME_DERIVED)pFrame;

	pFrame_Derived->CombinedTransformationMatrix = pFrame_Derived->TransformationMatrix * ParentMatrix;

	if (nullptr != pFrame_Derived->pFrameFirstChild)
		Update_CombinedTransformationMatrices(pFrame_Derived->pFrameFirstChild, pFrame_Derived->CombinedTransformationMatrix);

	if (nullptr != pFrame_Derived->pFrameSibling)
		Update_CombinedTransformationMatrices(pFrame_Derived->pFrameSibling, ParentMatrix);

	return NOERROR;
}

HRESULT CDynamicMesh::SetUp_CombinedTransformationMatricesPointer(D3DXFRAME* pFrame)
{
	if (nullptr != pFrame->pMeshContainer)
	{
		D3DXMESHCONTAINER_DERIVED* pMeshContainer_Derived = (D3DXMESHCONTAINER_DERIVED*)pFrame->pMeshContainer;

		for (_ulong i = 0; i < pMeshContainer_Derived->dwNumBones; ++i)
		{
			LPD3DXSKININFO pTest = pMeshContainer_Derived->pSkinInfo;
			LPCSTR test = pMeshContainer_Derived->pSkinInfo->GetBoneName(i);
			D3DXFRAME_DERIVED*	pFrame_Derived = (D3DXFRAME_DERIVED*)D3DXFrameFind(m_pRootFrame, pMeshContainer_Derived->pSkinInfo->GetBoneName(i));

			pMeshContainer_Derived->ppCombinedTransformationMatrices[i] = &pFrame_Derived->CombinedTransformationMatrix;
		}

		m_MeshContainerList.push_back(pMeshContainer_Derived);
	}

	if (nullptr != pFrame->pFrameFirstChild)
		SetUp_CombinedTransformationMatricesPointer(pFrame->pFrameFirstChild);

	if (nullptr != pFrame->pFrameSibling)
		SetUp_CombinedTransformationMatricesPointer(pFrame->pFrameSibling);

	return NOERROR;
}

HRESULT CDynamicMesh::SetTexture_OnShader(LPD3DXEFFECT pEffect, D3DXHANDLE pConstantName, _uint iMeshContainerIdx, _uint iAttributeID, MESHTEXTURE::TYPE eType)
{
	if (nullptr == m_MeshContainerList[iMeshContainerIdx]->pMeshTexture ||
		m_MeshContainerList[iMeshContainerIdx]->NumMaterials <= iAttributeID)
		return E_FAIL;

	pEffect->SetTexture(pConstantName, m_MeshContainerList[iMeshContainerIdx]->pMeshTexture[iAttributeID].pTextures[eType]);

	return NOERROR;
}

HRESULT CDynamicMesh::SetMaterial_OnShader(LPD3DXEFFECT pEffect, D3DXHANDLE pConstantName, _uint iMeshContainerIdx, _uint iAttributeID)
{
	if (m_MeshContainerList[iMeshContainerIdx]->NumMaterials <= iAttributeID)
		return E_FAIL;

	m_MeshContainerList[iMeshContainerIdx]->pMaterials[iAttributeID].MatD3D.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

	pEffect->SetValue(pConstantName, &m_MeshContainerList[iMeshContainerIdx]->pMaterials[iAttributeID].MatD3D, sizeof(D3DMATERIAL9));

	return NOERROR;
}

HRESULT CDynamicMesh::Update_SkinnedMesh(_int iMeshContainerIdx)
{
	for (_ulong i = 0; i < m_MeshContainerList[iMeshContainerIdx]->dwNumBones; ++i)
		m_MeshContainerList[iMeshContainerIdx]->pRenderMatrices[i] = m_MeshContainerList[iMeshContainerIdx]->pOffsetMatrices[i] * *m_MeshContainerList[iMeshContainerIdx]->ppCombinedTransformationMatrices[i];

	void	*pSourVtx = nullptr, *pDestVtx = nullptr;

	m_MeshContainerList[iMeshContainerIdx]->pOriginalMesh->LockVertexBuffer(0, &pSourVtx);
	m_MeshContainerList[iMeshContainerIdx]->MeshData.pMesh->LockVertexBuffer(0, &pDestVtx);

	m_MeshContainerList[iMeshContainerIdx]->pSkinInfo->UpdateSkinnedMesh(m_MeshContainerList[iMeshContainerIdx]->pRenderMatrices, nullptr, pSourVtx, pDestVtx);

	m_MeshContainerList[iMeshContainerIdx]->pOriginalMesh->UnlockVertexBuffer();
	m_MeshContainerList[iMeshContainerIdx]->MeshData.pMesh->UnlockVertexBuffer();

	return NOERROR;
}

HRESULT CDynamicMesh::Render_Mesh(_uint iMeshContainerIdx, _uint iAttributeID)
{
	m_MeshContainerList[iMeshContainerIdx]->MeshData.pMesh->DrawSubset(iAttributeID);

	return NOERROR;
}

HRESULT CDynamicMesh::SetUp_Animation(_uint iIndex)
{
	if (nullptr == m_pAniCtrl)
		return E_FAIL;

	m_pAniCtrl->SetUp_Animation(iIndex);

	return NOERROR;
}

HRESULT CDynamicMesh::Play_Animation(_double TimeDelta)
{
	if (nullptr == m_pAniCtrl)
		return E_FAIL;

	m_pAniCtrl->Play_Animation(TimeDelta);

	Update_CombinedTransformationMatrices(m_pRootFrame, m_matPivot);

	return NOERROR;
}

_bool CDynamicMesh::Finish_Animation()
{
	return m_pAniCtrl->Finish_Animation();
}

_bool CDynamicMesh::Finish_Animation(_double TimeLimit)
{
	return m_pAniCtrl->Finish_Animation(TimeLimit);
}

const _matrix* CDynamicMesh::Find_CombinedTransformationMatrix(const char* pFrameName)
{
	D3DXFRAME_DERIVED* pFrame = (D3DXFRAME_DERIVED*)D3DXFrameFind(m_pRootFrame, pFrameName);
	if (nullptr == pFrame)
		return nullptr;

	return &pFrame->CombinedTransformationMatrix;
}

CDynamicMesh * CDynamicMesh::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar * pFilePath, const _tchar * pFileName)
{
	CDynamicMesh*		pInstance = new CDynamicMesh(pGraphic_Device);

	if (FAILED(pInstance->Ready_Component_Prototype(pFilePath, pFileName)))
	{
		MSG_BOX("Failed To Create CDynamicMesh Instance");
		Safe_Delete(pInstance);
	}

	return pInstance;
}

void CDynamicMesh::Free()
{
	if (FAILED(m_pLoader->DestroyFrame(m_pRootFrame)))
		return;

	m_MeshContainerList.clear();

	Safe_Delete(m_pAniCtrl);
	Safe_Delete(m_pLoader);

	CComponent::Free();
}
