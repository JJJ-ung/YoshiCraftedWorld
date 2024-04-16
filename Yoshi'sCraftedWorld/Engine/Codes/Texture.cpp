#include "Texture.h"

USING(Engine)

Engine::CTexture::CTexture(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
{

}

Engine::CTexture::CTexture(const CTexture& rhs)
	: CComponent(rhs)
	, m_vecTexture(rhs.m_vecTexture)
{	
	for (_uint i = 0; i < m_vecTexture.size(); ++i)
		Safe_AddRef(m_vecTexture[i]);
}

HRESULT Engine::CTexture::Ready_Component_Prototype(const _tchar* pPath, TEXTURETYPE eType, const _uint& iCnt)
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

HRESULT CTexture::Ready_Component(void * pArg)
{
	return NOERROR;
}

HRESULT CTexture::SetUp_OnShader(LPD3DXEFFECT pEffect, const char * pConstantName, _uint iIndex)
{
	if (m_vecTexture.size() <= iIndex || 
		nullptr == m_vecTexture[iIndex])
		return E_FAIL;

	return pEffect->SetTexture(pConstantName, m_vecTexture[iIndex]);	
}

CComponent* Engine::CTexture::Clone_Component(void* pArg)
{
	CTexture*		pInstance = new CTexture(*this);

	if (FAILED(pInstance->Ready_Component(pArg)))
	{
		MSG_BOX("Failed To Create CTexture Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CTexture* Engine::CTexture::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pPath, TEXTURETYPE eType, const _uint& iCnt /*= 1*/)
{
	CTexture*		pInstance = new CTexture(pGraphicDev);

	if (FAILED(pInstance->Ready_Component_Prototype(pPath, eType, iCnt)))
	{
		MSG_BOX("Failed To Create CTexture Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void Engine::CTexture::Free(void)
{
	_uint iContainerSize = m_vecTexture.size();

	for (_uint i = 0; i < iContainerSize; ++i)
		Safe_Release(m_vecTexture[i]);

	m_vecTexture.clear();

	CComponent::Free();
}

