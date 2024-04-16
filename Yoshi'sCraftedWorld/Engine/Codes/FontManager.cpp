#include "FontManager.h"

USING(Engine)
IMPLEMENT_SINGLETON(CFontManager)

Engine::CFontManager::CFontManager(void)
{

}

Engine::CFontManager::~CFontManager(void)
{
	Free();
}

HRESULT Engine::CFontManager::Ready_Font(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFontTag, const _tchar* pFontType, const _uint& iWidth, const _uint& iHeight, const _uint& iWeight)
{
	CFont*		pFont = Find_Font(pFontTag);
	
	if (nullptr != pFont)
		return E_FAIL;

	pFont = CFont::Create(pGraphicDev, pFontType, iWidth, iHeight, iWeight);

	if (nullptr == pFont)
		return E_FAIL;

	m_mapFont.emplace(pFontTag, pFont);

	return S_OK;
}

void Engine::CFontManager::Render_Font(const _tchar* pFontTag, const _tchar* pFontType, const _vec2* pPos, D3DXCOLOR Color)
{
	CFont*	pFont = Find_Font(pFontTag);

	if (nullptr == pFont)
		return;

	pFont->Render_Font(pFontType, pPos, Color);
}

CFont* Engine::CFontManager::Find_Font(const _tchar* pFontTag)
{
	auto	iter = find_if(m_mapFont.begin(), m_mapFont.end(), CTag_Finder(pFontTag));

	if (iter == m_mapFont.end())
		return nullptr;

	return iter->second;
}

void Engine::CFontManager::Free(void)
{
	for (auto& Pair : m_mapFont)
		Safe_Release(Pair.second);
	m_mapFont.clear();

	m_mapFont.clear();
}

