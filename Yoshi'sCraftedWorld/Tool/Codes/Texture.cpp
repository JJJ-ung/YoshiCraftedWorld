#include "stdafx.h"
#include "Texture.h"

CTexture::CTexture(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
{

}

HRESULT CTexture::Ready_Component(const _tchar* pPath, TEXTURETYPE eType, const _uint& iCnt)
{
	m_vecTexture.reserve(iCnt);

	IDirect3DBaseTexture9*		pTexture = nullptr;

	for (_uint i = 0; i < iCnt; ++i)
	{
		_tchar		szFileName[128] = L"";

		wsprintf(szFileName, pPath, i);

		switch (eType)
		{
		case TYPE_GENERAL:
			if (FAILED(D3DXCreateTextureFromFile(m_pGraphic_Device, szFileName, (LPDIRECT3DTEXTURE9*)&pTexture)))
				return E_FAIL;
			break;

		case TYPE_CUBE:
			if (FAILED(D3DXCreateCubeTextureFromFile(m_pGraphic_Device, szFileName, (LPDIRECT3DCUBETEXTURE9*)&pTexture)))
				return E_FAIL;
			break;
		}

		m_vecTexture.push_back(pTexture);
	}

	return NOERROR;
}

HRESULT CTexture::SetUp_OnShader(LPD3DXEFFECT pEffect, const char * pConstantName, _uint iIndex)
{
	if (m_vecTexture.size() <= iIndex || 
		nullptr == m_vecTexture[iIndex])
		return E_FAIL;

	return pEffect->SetTexture(pConstantName, m_vecTexture[iIndex]);	
}

CTexture* CTexture::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pPath, TEXTURETYPE eType, const _uint& iCnt /*= 1*/)
{
	CTexture*		pInstance = new CTexture(pGraphicDev);

	if (FAILED(pInstance->Ready_Component(pPath, eType, iCnt)))
	{
		MSG_BOX("Failed To Create CTexture Instance");
		Safe_Delete(pInstance);
	}

	return pInstance;
}

void CTexture::Free(void)
{
	_uint iContainerSize = m_vecTexture.size();

	for (_uint i = 0; i < iContainerSize; ++i)
		Safe_Release(m_vecTexture[i]);

	m_vecTexture.clear();

	CComponent::Free();
}

