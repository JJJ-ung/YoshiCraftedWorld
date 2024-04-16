#include "..\Headers\Shader.h"

CShader::CShader(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{
}

CShader::CShader(const CShader & rhs)
	: CComponent(rhs)
	, m_pEffect(rhs.m_pEffect)
	, m_pErrBuffer(rhs.m_pErrBuffer)
{
	Safe_AddRef(m_pErrBuffer);
	Safe_AddRef(m_pEffect);
}

HRESULT CShader::Set_Value(D3DXHANDLE hHandle, void * pData, _uint iSize)
{
	if (nullptr == m_pEffect)
		return E_FAIL;

	m_pEffect->SetValue(hHandle, pData, iSize);

	return NOERROR;
}

HRESULT CShader::Ready_Component_Prototype(const _tchar* pShaderFilesPath)
{
	if (FAILED(D3DXCreateEffectFromFile(m_pGraphic_Device, pShaderFilesPath, nullptr, nullptr, 0, nullptr, &m_pEffect, &m_pErrBuffer)))
		return E_FAIL;
	/*{
		MessageBoxA(0, (char*)m_pErrBuffer->GetBufferPointer(), nullptr, MB_OK);
		return E_FAIL;
	}*/
	
	return NOERROR;
}

HRESULT CShader::Ready_Component(void * pArg)
{
	return NOERROR;
}

CShader * CShader::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pShaderFilesPath)
{
	CShader*		pInstance = new CShader(pGraphic_Device);

	if (FAILED(pInstance->Ready_Component_Prototype(pShaderFilesPath)))
	{
		MSG_BOX("Failed To Create CShader Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CShader::Clone_Component(void * pArg)
{
	CShader*		pInstance = new CShader(*this);

	if (FAILED(pInstance->Ready_Component(pArg)))
	{
		MSG_BOX("Failed To Create CShader Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShader::Free()
{
	Safe_Release(m_pErrBuffer);
	Safe_Release(m_pEffect);

	CComponent::Free();
}
