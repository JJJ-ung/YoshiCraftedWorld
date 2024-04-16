#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CShader final : public CComponent
{
private:
	explicit CShader(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CShader(const CShader& rhs);
	virtual ~CShader() = default;

public:
	LPD3DXEFFECT	Get_EffectHandle() { return m_pEffect; }
	HRESULT Set_Value(D3DXHANDLE hHandle, void* pData, _uint iSize);

public:
	virtual HRESULT Ready_Component_Prototype(const _tchar* pShaderFilesPath);
	virtual HRESULT Ready_Component(void* pArg);

private:
	LPD3DXEFFECT	m_pEffect = nullptr;
	LPD3DXBUFFER	m_pErrBuffer = nullptr;		

public:
	static CShader* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pShaderFilesPath);
	virtual CComponent* Clone_Component(void* pArg);
	virtual void Free();
};

END