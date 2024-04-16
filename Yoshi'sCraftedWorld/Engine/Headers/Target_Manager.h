#pragma once

#include "Base.h"

BEGIN(Engine)

class CTarget;
class CTarget_Manager final : public CBase
{
	DECLARE_SINGLETON(CTarget_Manager)
private:
	explicit CTarget_Manager();
	virtual ~CTarget_Manager() = default;
public:
	HRESULT Add_Target(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pTargetTag, _uint iWidth, _uint iHeight, D3DFORMAT eFormat, D3DXCOLOR Color);
	HRESULT Add_MRT(const _tchar* pMRTTag, const _tchar* pTargetTag);
	HRESULT Begin_MRT(const _tchar* pMRTTag);
	HRESULT End_MRT(const _tchar* pMRTTag);
	HRESULT SetUp_OnShader(LPD3DXEFFECT pEffect, const _tchar* pTargetTag, D3DXHANDLE hHandle);
#ifdef _DEBUG
public:
	HRESULT Ready_Debug_Buffer(const _tchar* pTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY);
	HRESULT Render_Debug_Buffer(const _tchar* pMRTTag);
#endif
private:
	unordered_map<const _tchar*, CTarget*>			m_Targets;
	typedef unordered_map<const _tchar*, CTarget*>	TARGETS;

private:
	unordered_map<const _tchar*, list<CTarget*>>			m_MRTs;
	typedef unordered_map<const _tchar*, list<CTarget*>>	MRTS;
private:
	CTarget* Find_Target(const _tchar* pTargetTag);
	list<CTarget*>* Find_MRT(const _tchar* pMRTTag);
public:
	virtual void Free();
};

END