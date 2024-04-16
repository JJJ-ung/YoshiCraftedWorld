#pragma once

#include "Component.h"

class CShader final : public CComponent
{
public:
	explicit CShader(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CShader() = default;

public:
	LPD3DXEFFECT	Get_EffectHandle() { return m_pEffect; }
	HRESULT Set_Value(D3DXHANDLE hHandle, void* pData, _uint iSize);

public:
	virtual HRESULT Ready_Component(const _tchar* pShaderPath);

private:
	LPD3DXEFFECT	m_pEffect = nullptr;
	LPD3DXBUFFER	m_pErrBuffer = nullptr;		

public:
	static CShader* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pShaderPath);
	virtual void Free();
};