#include "Font.h"

USING(Engine)

Engine::CFont::CFont(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_pFont(nullptr)
	, m_pSprite(nullptr)
{
	m_pGraphicDev->AddRef();
}

Engine::CFont::~CFont(void)
{

}

HRESULT Engine::CFont::Ready_Font(const _tchar* pFontType, const _uint& iWidth, const _uint& iHeight, const _uint& iWeight)
{
	D3DXFONT_DESC		tFont;
	ZeroMemory(&tFont, sizeof(D3DXFONT_DESC));

	tFont.CharSet = HANGUL_CHARSET;

	lstrcpy(tFont.FaceName, pFontType);
	tFont.Height = iHeight;
	tFont.Weight = iWeight;
	tFont.Width = iWidth;

	if (FAILED(D3DXCreateFontIndirect(m_pGraphicDev, &tFont, &m_pFont)))
		return E_FAIL;
	if(FAILED(D3DXCreateSprite(m_pGraphicDev, &m_pSprite)))
		return E_FAIL;
	
	return S_OK;
}

void Engine::CFont::Render_Font(const _tchar* pString, const _vec2* pPos, D3DXCOLOR Color)
{
	RECT	rc{ _long(pPos->x), _long(pPos->y) };

	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

	m_pFont->DrawTextW(m_pSprite, pString, lstrlen(pString), &rc, DT_NOCLIP, Color);

	m_pSprite->End();
}

CFont* Engine::CFont::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFontType, const _uint& iWidth, const _uint& iHeight, const _uint& iWeight)
{
	CFont*	pInstance = new CFont(pGraphicDev);

	if (FAILED(pInstance->Ready_Font(pFontType, iWidth, iHeight, iWeight)))
		Safe_Release(pInstance);	

	return pInstance;
}

void Engine::CFont::Free(void)
{
	Safe_Release(m_pFont);
	Safe_Release(m_pSprite);
	Safe_Release(m_pGraphicDev);
}

