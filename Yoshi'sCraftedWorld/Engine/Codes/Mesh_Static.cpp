#include "..\Headers\Mesh_Static.h"
#include "Texture.h"

CMesh_Static::CMesh_Static(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)	
{
}

CMesh_Static::CMesh_Static(const CMesh_Static & rhs)
	: CComponent(rhs)
	, m_pMesh(rhs.m_pMesh)
	, m_dwNumMaterials(rhs.m_dwNumMaterials)
	, m_pAdjacencyBuff(rhs.m_pAdjacencyBuff)
	, m_pMaterialsBuff(rhs.m_pMaterialsBuff)
	, m_vecMaterials(rhs.m_vecMaterials)
	, m_vecTextures(rhs.m_vecTextures)
{
	Safe_AddRef(m_pMesh);
	Safe_AddRef(m_pAdjacencyBuff); 
	Safe_AddRef(m_pMaterialsBuff);

	for (size_t i = 0; i < m_dwNumMaterials; ++i)
	{
		for (size_t j = 0; j < MESHTEXTURE::TYPE_END; ++j)
			Safe_AddRef(m_vecTextures[i].pTextures[j]);
	}
}

HRESULT CMesh_Static::Ready_Component_Prototype(const _tchar * pFilePath, const _tchar * pFileName)
{
	_tchar		szFullPath[128] = L"";

	lstrcpy(szFullPath, pFilePath);
	lstrcat(szFullPath, pFileName);
	
	if (FAILED(D3DXLoadMeshFromX(szFullPath, D3DXMESH_MANAGED, m_pGraphic_Device, &m_pAdjacencyBuff, &m_pMaterialsBuff, nullptr, &m_dwNumMaterials, &m_pMesh)))
		return E_FAIL;

	if (m_dwNumMaterials != 0 && m_pMaterialsBuff != 0)
	{
		D3DXMATERIAL* pMtrl = (D3DXMATERIAL*)m_pMaterialsBuff->GetBufferPointer();
		MESHTEXTURE tMeshTexture;
		ZeroMemory(&tMeshTexture, sizeof(MESHTEXTURE));

		for (DWORD i = 0; i < m_dwNumMaterials; ++i)
		{
			m_vecMaterials.push_back(pMtrl[i]);

			if (pMtrl[i].pTextureFilename)
			{
				//LPDIRECT3DTEXTURE9 pTex = nullptr;

				lstrcpy(szFullPath, pFilePath);
				_tchar	szTextureFileName[MAX_PATH] = L"";
				MultiByteToWideChar(CP_ACP, 0, pMtrl[i].pTextureFilename, strlen(pMtrl[i].pTextureFilename), szTextureFileName, MAX_PATH);
				lstrcat(szFullPath, szTextureFileName);

				// For.DiffuseMap
				if (FAILED(D3DXCreateTextureFromFile(m_pGraphic_Device, szFullPath, &tMeshTexture.pTextures[MESHTEXTURE::TYPE_DIFFUSE])))
					return E_FAIL;

				// For.NormalMap
				Change_TextureFileName(szFullPath, L"D", L"N");
				if (FAILED(D3DXCreateTextureFromFile(m_pGraphic_Device, szFullPath, &tMeshTexture.pTextures[MESHTEXTURE::TYPE_NORMAL])))
					tMeshTexture.pTextures[MESHTEXTURE::TYPE_NORMAL] = nullptr;

				// For.SpecularMap
				Change_TextureFileName(szFullPath, L"N", L"S");
				if (FAILED(D3DXCreateTextureFromFile(m_pGraphic_Device, szFullPath, &tMeshTexture.pTextures[MESHTEXTURE::TYPE_SPECULAR])))
					tMeshTexture.pTextures[MESHTEXTURE::TYPE_SPECULAR] = nullptr;

				// For.EmissiveMap
				Change_TextureFileName(szFullPath, L"S", L"E");
				if (FAILED(D3DXCreateTextureFromFile(m_pGraphic_Device, szFullPath, &tMeshTexture.pTextures[MESHTEXTURE::TYPE_EMISSIVE])))
					tMeshTexture.pTextures[MESHTEXTURE::TYPE_EMISSIVE] = nullptr;

				m_vecTextures.push_back(tMeshTexture);
			}
			else
				m_vecTextures.push_back(tMeshTexture);
		}
	}

	return NOERROR;
}

HRESULT CMesh_Static::Ready_Component(void * pArg)
{
	return NOERROR;
}

HRESULT CMesh_Static::SetTexture_OnShader(LPD3DXEFFECT pEffect, D3DXHANDLE ConstantName, _uint iAttributeID, MESHTEXTURE::TYPE eType)
{
	if (m_vecTextures.empty() ||		
		m_dwNumMaterials <= iAttributeID)
		return E_FAIL;

	pEffect->SetTexture(ConstantName, m_vecTextures[iAttributeID].pTextures[eType]);
	
	return NOERROR;
}

HRESULT CMesh_Static::SetMaterial_OnShader(LPD3DXEFFECT pEffect, D3DXHANDLE pConstantName, _uint iAttributeID)
{
	if (m_dwNumMaterials <= iAttributeID)
		return E_FAIL;

	pEffect->SetValue(pConstantName, &m_vecMaterials[iAttributeID].MatD3D, sizeof(D3DMATERIAL9));

	return NOERROR;
}

HRESULT CMesh_Static::Render_Mesh(_uint iAttributeID)
{
	if (nullptr == m_pMesh)
		return E_FAIL;

	m_pMesh->DrawSubset(iAttributeID);

	return NOERROR;
}

HRESULT CMesh_Static::Change_TextureFileName(_tchar* pFilePath, _tchar * pSourMark, _tchar * pDestMark)
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

CMesh_Static * CMesh_Static::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar * pFilePath, const _tchar * pFileName)
{
	CMesh_Static*		pInstance = new CMesh_Static(pGraphic_Device);

	if (FAILED(pInstance->Ready_Component_Prototype(pFilePath, pFileName)))
	{
		MSG_BOX("Failed To Create CMesh_Static Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CMesh_Static::Clone_Component(void * pArg)
{
	CMesh_Static*		pInstance = new CMesh_Static(*this);

	if (FAILED(pInstance->Ready_Component(pArg)))
	{
		MSG_BOX("Failed To Create CMesh_Static Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMesh_Static::Free()
{
	for (size_t i = 0; i < m_dwNumMaterials; ++i)
	{
		for (size_t j = 0; j < MESHTEXTURE::TYPE_END; j++)
			Safe_Release(m_vecTextures[i].pTextures[j]);
	}

	if (false == m_isClone)
		m_vecTextures.clear();

	Safe_Release(m_pAdjacencyBuff);
	Safe_Release(m_pMaterialsBuff);
	Safe_Release(m_pMesh);

	CComponent::Free();
}
