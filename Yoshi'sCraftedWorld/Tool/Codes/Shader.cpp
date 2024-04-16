#include "stdafx.h"
#include "..\Headers\Shader.h"

CShader::CShader(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{
}

HRESULT CShader::Set_Value(D3DXHANDLE hHandle, void * pData, _uint iSize)
{
	if (nullptr == m_pEffect)
		return E_FAIL;

	m_pEffect->SetValue(hHandle, pData, iSize);

	return NOERROR;
}

HRESULT CShader::Ready_Component(const _tchar* pShaderPath)
{
	if (FAILED(D3DXCreateEffectFromFile(m_pGraphic_Device, pShaderPath, nullptr, nullptr, 0, nullptr, &m_pEffect, &m_pErrBuffer)))
		return E_FAIL;
	
	return NOERROR;
}

CShader * CShader::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pShaderPath)
{
	CShader*		pInstance = new CShader(pGraphic_Device);

	if (FAILED(pInstance->Ready_Component(pShaderPath)))
	{
		MSG_BOX("Failed To Create CShader Instance");
		Safe_Delete(pInstance);
	}

	return pInstance;
}

void CShader::Free()
{
	Safe_Release(m_pErrBuffer);
	Safe_Release(m_pEffect);

	CComponent::Free();
}
