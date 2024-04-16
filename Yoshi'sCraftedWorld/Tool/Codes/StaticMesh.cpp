#include "stdafx.h"
#include "..\Headers\StaticMesh.h"

CStaticMesh::CStaticMesh(LPDIRECT3DDEVICE9 pGraphicDev)
	:CComponent(pGraphicDev)
{
}

HRESULT CStaticMesh::Ready_Component(const _tchar * pPath)
{
	LPD3DXBUFFER pAdj = nullptr;
	LPD3DXBUFFER pMtrls = nullptr;

	if (FAILED(D3DXLoadMeshFromX(pPath, D3DXMESH_MANAGED, m_pGraphic_Device, &pAdj, &pMtrls, NULL, &m_dwMtrlNum, &m_pMesh)))
		return E_FAIL;

	if (m_dwMtrlNum != 0 && pMtrls != 0)
	{
		D3DXMATERIAL* pMtrl = (D3DXMATERIAL*)pMtrls->GetBufferPointer();

		for (DWORD i = 0; i < m_dwMtrlNum; ++i)
		{
			pMtrl[i].MatD3D.Diffuse = pMtrl[i].MatD3D.Diffuse;
			pMtrl[i].MatD3D.Ambient = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.f);
			pMtrl[i].MatD3D.Specular = pMtrl[i].MatD3D.Diffuse;
			m_vecMtrl.push_back(pMtrl[i].MatD3D);

			if (pMtrl[i].pTextureFilename)
			{
				LPDIRECT3DTEXTURE9 pTex = nullptr;

				CString temp = pPath;
				temp = temp.Left(temp.ReverseFind('/')) + L"/";
				temp += pMtrl[i].pTextureFilename;

				if (FAILED(D3DXCreateTextureFromFile(m_pGraphic_Device, temp.GetString(), &pTex)))
					return E_FAIL;

				m_vecTex.push_back(pTex);
			}
			else
				m_vecTex.push_back(NULL);
		}
	}

	return NOERROR;
}

HRESULT CStaticMesh::Render_Mesh(LPD3DXEFFECT pEffect, const char* pConstant, int iPass)
{
	if (nullptr == m_pGraphic_Device ||
		nullptr == m_pMesh ||
		nullptr == pEffect )
		return E_FAIL;

	for (DWORD i = 0; i < m_dwMtrlNum; ++i)
	{
		pEffect->SetTexture(pConstant, m_vecTex[i]);
		pEffect->Begin(nullptr, 0);

		pEffect->BeginPass(iPass);

		m_pMesh->DrawSubset(i);

		pEffect->EndPass();
		pEffect->End();
	}

	return NOERROR;
}

CStaticMesh * CStaticMesh::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar * pPath)
{
	CStaticMesh* pInstance = new CStaticMesh(pGraphicDev);

	if (FAILED(pInstance->Ready_Component(pPath)))
		Safe_Delete(pInstance);

	return pInstance;
}

void CStaticMesh::Free()
{
	for (auto& pTex : m_vecTex)
		Safe_Release(pTex);

	Safe_Release(m_pMesh);
	m_vecMtrl.clear();
}
