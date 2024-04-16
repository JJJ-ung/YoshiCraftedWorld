#include "..\Headers\HierarchyLoader.h"

CHierarchyLoader::CHierarchyLoader(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device(pGraphic_Device)
{
	Safe_AddRef(m_pGraphic_Device);
}

HRESULT CHierarchyLoader::Ready_HierarchyLoader(const _tchar* pFilePath)
{
	m_pFilePath = pFilePath;

	return NOERROR;
}

STDMETHODIMP CHierarchyLoader::CreateFrame(LPCSTR Name, LPD3DXFRAME * ppNewFrame)
{
	LPD3DXFRAME_DERIVED		pFrame = new D3DXFRAME_DERIVED;
	ZeroMemory(pFrame, sizeof(D3DXFRAME_DERIVED));

	if (FAILED(SetUp_Name(&pFrame->Name, Name)))
		return E_FAIL;

	pFrame->CombinedTransformationMatrix = *D3DXMatrixIdentity(&pFrame->TransformationMatrix);

	*ppNewFrame = pFrame;

	return NOERROR;
}

STDMETHODIMP CHierarchyLoader::CreateMeshContainer(LPCSTR Name, CONST D3DXMESHDATA * pMeshData, CONST D3DXMATERIAL * pMaterials, CONST D3DXEFFECTINSTANCE * pEffectInstances, DWORD NumMaterials, CONST DWORD * pAdjacency, LPD3DXSKININFO pSkinInfo, LPD3DXMESHCONTAINER * ppNewMeshContainer)
{
	LPD3DXMESHCONTAINER_DERIVED	pMeshContainer = new D3DXMESHCONTAINER_DERIVED;
	ZeroMemory(pMeshContainer, sizeof(D3DXMESHCONTAINER_DERIVED));

	if (FAILED(SetUp_Name(&pMeshContainer->Name, Name)))
		return E_FAIL;

	pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

	LPD3DXMESH		pMesh = pMeshData->pMesh;
	Safe_AddRef(pMesh);

	pMeshContainer->pAdjacency = new _ulong[pMesh->GetNumFaces() * 3];
	memcpy(pMeshContainer->pAdjacency, pAdjacency, sizeof(_ulong) * pMesh->GetNumFaces() * 3);	

	_ulong dwFVF = pMesh->GetFVF();

	if (false == (dwFVF & D3DFVF_NORMAL))
	{
		pMesh->CloneMeshFVF(pMesh->GetOptions(), dwFVF | D3DFVF_NORMAL, m_pGraphic_Device, &pMeshContainer->MeshData.pMesh);

		D3DXComputeNormals(pMeshContainer->MeshData.pMesh, pMeshContainer->pAdjacency);
	}
	else
	{
		D3DVERTEXELEMENT9		Decl[MAX_FVF_DECL_SIZE] = {
			{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
			{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
			{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
			D3DDECL_END()
		};

		pMesh->CloneMeshFVF(pMesh->GetOptions(), dwFVF, m_pGraphic_Device, &pMeshContainer->MeshData.pMesh);
	}

	Safe_Release(pMesh);

	pMeshContainer->NumMaterials = NumMaterials;

	pMeshContainer->pMaterials = new D3DXMATERIAL[pMeshContainer->NumMaterials];
	memcpy(pMeshContainer->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * pMeshContainer->NumMaterials);
	
	pMeshContainer->pMeshTexture = new MESHTEXTURE[pMeshContainer->NumMaterials];
	ZeroMemory(pMeshContainer->pMeshTexture, sizeof(MESHTEXTURE) * pMeshContainer->NumMaterials);

	_tchar		szFullPath[MAX_PATH] = L"";

	for (size_t i = 0; i < pMeshContainer->NumMaterials; ++i)
	{
		lstrcpy(szFullPath, m_pFilePath);

		_tchar	szTextureFileName[MAX_PATH] = L"";

		MultiByteToWideChar(CP_ACP, 0, pMeshContainer->pMaterials[i].pTextureFilename, strlen(pMeshContainer->pMaterials[i].pTextureFilename)
			, szTextureFileName, MAX_PATH);

		lstrcat(szFullPath, szTextureFileName);

		if (FAILED(D3DXCreateTextureFromFile(m_pGraphic_Device, szFullPath, &pMeshContainer->pMeshTexture[i].pTextures[MESHTEXTURE::TYPE_DIFFUSE])))
			return E_FAIL;

		Change_TextureFileName(szFullPath, L"D", L"N");

		D3DXCreateTextureFromFile(m_pGraphic_Device, szFullPath, &pMeshContainer->pMeshTexture[i].pTextures[MESHTEXTURE::TYPE_NORMAL]);

		Change_TextureFileName(szFullPath, L"N", L"S");

		D3DXCreateTextureFromFile(m_pGraphic_Device, szFullPath, &pMeshContainer->pMeshTexture[i].pTextures[MESHTEXTURE::TYPE_SPECULAR]);

		Change_TextureFileName(szFullPath, L"S", L"E");

		D3DXCreateTextureFromFile(m_pGraphic_Device, szFullPath, &pMeshContainer->pMeshTexture[i].pTextures[MESHTEXTURE::TYPE_EMISSIVE]);
	}

	pMeshContainer->pSkinInfo = pSkinInfo;
	Safe_AddRef(pMeshContainer->pSkinInfo);

	pMeshContainer->dwNumBones = pMeshContainer->pSkinInfo->GetNumBones();

	pMeshContainer->pOffsetMatrices = new D3DXMATRIX[pMeshContainer->dwNumBones];
	pMeshContainer->ppCombinedTransformationMatrices = new D3DXMATRIX*[pMeshContainer->dwNumBones];
	pMeshContainer->pRenderMatrices = new D3DXMATRIX[pMeshContainer->dwNumBones];

	for (_ulong i = 0; i < pMeshContainer->dwNumBones; i++)		
		pMeshContainer->pOffsetMatrices[i] = *pMeshContainer->pSkinInfo->GetBoneOffsetMatrix(i);	

	if (FAILED(pMeshContainer->MeshData.pMesh->CloneMeshFVF(pMeshContainer->MeshData.pMesh->GetOptions(), pMeshContainer->MeshData.pMesh->GetFVF(), m_pGraphic_Device, &pMeshContainer->pOriginalMesh)))
		return E_FAIL;

	*ppNewMeshContainer = pMeshContainer;

	return NOERROR;
}

STDMETHODIMP CHierarchyLoader::DestroyFrame(LPD3DXFRAME pFrameToFree)
{
	Safe_Delete_Array(pFrameToFree->Name);

	if (nullptr != pFrameToFree->pMeshContainer)
		DestroyMeshContainer(pFrameToFree->pMeshContainer);

	if (pFrameToFree->pFrameSibling)
		DestroyFrame(pFrameToFree->pFrameSibling);

	if (pFrameToFree->pFrameFirstChild)
		DestroyFrame(pFrameToFree->pFrameFirstChild);

	Safe_Delete(pFrameToFree);

	return NOERROR;
}

STDMETHODIMP CHierarchyLoader::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerToFree)
{
	D3DXMESHCONTAINER_DERIVED*		pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerToFree;
	
	Safe_Delete_Array(pMeshContainer->Name);
	Safe_Release(pMeshContainer->MeshData.pMesh);
	Safe_Delete_Array(pMeshContainer->pMaterials);
	Safe_Delete_Array(pMeshContainer->pAdjacency);
	Safe_Release(pMeshContainer->pSkinInfo);
	Safe_Release(pMeshContainer->pOriginalMesh);

	for (size_t i = 0; i < pMeshContainer->NumMaterials; ++i)
	{
		for (size_t j = 0; j < MESHTEXTURE::TYPE_END; ++j)
		{
			Safe_Release(pMeshContainer->pMeshTexture[i].pTextures[j]);
		}		
	}

	Safe_Delete_Array(pMeshContainer->pMeshTexture);
	Safe_Delete_Array(pMeshContainer->pOffsetMatrices);
	Safe_Delete_Array(pMeshContainer->ppCombinedTransformationMatrices);
	Safe_Delete_Array(pMeshContainer->pRenderMatrices);

	Safe_Delete_Array(pMeshContainer);

	return NOERROR;
}

HRESULT CHierarchyLoader::SetUp_Name(char ** ppSourName, const char * pDestName)
{
	if (nullptr == pDestName)
		return NOERROR;

	_uint iLength = strlen(pDestName);

	*ppSourName = new char[iLength + 1];

	memcpy(*ppSourName, pDestName, iLength + 1);

	return NOERROR;
}

HRESULT CHierarchyLoader::Change_TextureFileName(_tchar * pFilePath, _tchar * pSourMark, _tchar * pDestMark)
{
	_int iLength = lstrlen(pFilePath);

	for (_int i = iLength; i >= 0; --i)
	{
		if (pFilePath[i] == *pSourMark)
		{
			pFilePath[i] = *pDestMark;
			break;
		}
	}
	return NOERROR;
}

CHierarchyLoader * CHierarchyLoader::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pFilePath)
{
	CHierarchyLoader*		pInstance = new CHierarchyLoader(pGraphic_Device);

	if (FAILED(pInstance->Ready_HierarchyLoader(pFilePath)))
	{
		MSG_BOX("Failed To Create CHierarchyLoader Instance");
		Safe_Release(pInstance);	
	}

	return pInstance;
}

void CHierarchyLoader::Free()
{
	Safe_Release(m_pGraphic_Device);
}
