#include "..\Headers\Target_Manager.h"
#include "Target.h"

IMPLEMENT_SINGLETON(CTarget_Manager)

CTarget_Manager::CTarget_Manager()
{

}

HRESULT CTarget_Manager::Add_Target(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar * pTargetTag, _uint iWidth, _uint iHeight, D3DFORMAT eFormat, D3DXCOLOR Color)
{
	CTarget*	pTarget = Find_Target(pTargetTag);

	if (nullptr != pTarget)
		return E_FAIL;

	pTarget = CTarget::Create(pGraphic_Device, iWidth, iHeight, eFormat, Color);
	if (nullptr == pTarget)
		return E_FAIL;

	m_Targets.insert(TARGETS::value_type(pTargetTag, pTarget));

	return NOERROR;
}

HRESULT CTarget_Manager::Add_MRT(const _tchar * pMRTTag, const _tchar * pTargetTag)
{
	CTarget*	pTarget = Find_Target(pTargetTag);

	if (nullptr == pTarget)
		return E_FAIL;

	list<CTarget*>*	pMRTList = Find_MRT(pMRTTag);

	if (nullptr == pMRTList)
	{
		list<CTarget*>		MRTList;

		MRTList.push_back(pTarget);

		m_MRTs.insert(MRTS::value_type(pMRTTag, MRTList));
	}
	else
		pMRTList->push_back(pTarget);

	Safe_AddRef(pTarget);

	return NOERROR;
}

HRESULT CTarget_Manager::Begin_MRT(const _tchar* pMRTTag)
{
	// 디퓨즈 : 백버퍼를 빼고, 디퓨즈를 셋팅하자.
	// 노말 : 널을 빼고, 노멀을 셋팅하ㅏㅈ.

	list<CTarget*>*	pMRTList = Find_MRT(pMRTTag);
	if (nullptr == pMRTList)
		return E_FAIL;


	for (auto& pTarget : *pMRTList)
	{
		if (nullptr != pTarget)
			pTarget->Clear_Target();
	}

	_uint	iIndex = 0;

	for (auto& pTarget : *pMRTList)
	{
		// 디퓨즈 : 백버퍼를 빼고, 디퓨즈를 셋팅하자.
		// 노말 : 널을 빼고, 노멀을 셋팅하ㅏㅈ.
		if (nullptr != pTarget)
			pTarget->SetUp_OnGraphicDev(iIndex++);
	}

	return NOERROR;
}

HRESULT CTarget_Manager::End_MRT(const _tchar * pMRTTag)
{
	list<CTarget*>*	pMRTList = Find_MRT(pMRTTag);
	if (nullptr == pMRTList)
		return E_FAIL;

	_uint	iIndex = 0;

	for (auto& pTarget : *pMRTList)
	{
		// 디퓨즈 : 백버퍼를 빼고, 디퓨즈를 셋팅하자.
		// 노말 : 널을 빼고, 노멀을 셋팅하ㅏㅈ.
		if (nullptr != pTarget)
			pTarget->Release_OnGraphicDev(iIndex++);
	}

	return NOERROR;
}
HRESULT CTarget_Manager::SetUp_OnShader(LPD3DXEFFECT pEffect, const _tchar * pTargetTag, D3DXHANDLE hHandle)
{
	CTarget*	pTarget = Find_Target(pTargetTag);

	if (nullptr == pTarget)
		return E_FAIL;	

	return pTarget->SetUp_OnShader(pEffect, hHandle);;
}

HRESULT CTarget_Manager::Ready_Debug_Buffer(const _tchar * pTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY)
{
	CTarget*	pTarget = Find_Target(pTargetTag);

	if (nullptr == pTarget)
		return E_FAIL;


	return pTarget->Ready_Debug_Buffer(fX, fY, fSizeX, fSizeY);
}

HRESULT CTarget_Manager::Render_Debug_Buffer(const _tchar * pMRTTag)
{
	if (!g_bRenderTarget)
		return NOERROR;

	list<CTarget*>*	pMRTList = Find_MRT(pMRTTag);
	if (nullptr == pMRTList)
		return E_FAIL;

	for (auto& pTarget : *pMRTList)
		pTarget->Render_Debug_Buffer();	

	return NOERROR;
}

CTarget * CTarget_Manager::Find_Target(const _tchar * pTargetTag)
{
	auto	iter = find_if(m_Targets.begin(), m_Targets.end(), CTag_Finder(pTargetTag));
	if (iter == m_Targets.end())
		return nullptr;

	return iter->second;
}

list<CTarget*>* CTarget_Manager::Find_MRT(const _tchar * pMRTTag)
{
	auto	iter = find_if(m_MRTs.begin(), m_MRTs.end(), CTag_Finder(pMRTTag));
	if (iter == m_MRTs.end())
		return nullptr;

	return &iter->second;
}

void CTarget_Manager::Free()
{
	for (auto& Pair : m_MRTs)
	{
		for (auto& pTarget : Pair.second)
			Safe_Release(pTarget);
		Pair.second.clear();		
	}
	m_MRTs.clear();

	for (auto& Pair : m_Targets)
	{
		Safe_Release(Pair.second);
	}
	m_Targets.clear();
}
