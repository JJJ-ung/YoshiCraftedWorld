#pragma once

#include "Component.h"
#include "Target_Manager.h"

BEGIN(Engine)

class CShader;
class CGameObject;
class CLight_Manager;
class CTarget_Manager;
class ENGINE_DLL CRenderer final : public CComponent
{
public:
	enum RENDERGROUP { RENDER_PRIORITY, RENDER_NONALPHA, RENDER_ALPHA, RENDER_UI, RENDER_UI2, /*RENDER_UI3, */RENDER_END };

private:
	explicit CRenderer(LPDIRECT3DDEVICE9 pGraphic_Device);	
	virtual ~CRenderer() = default;

public:
	HRESULT Ready_Component_Prototype();
	HRESULT Ready_Component(void* pArg);
	HRESULT Add_RenderList(RENDERGROUP eGroup, CGameObject* pGameObject);
	HRESULT Render_ObjectList();

private:
	list<CGameObject*>			m_RenderList[RENDER_END];
	typedef list<CGameObject*>	RENDERLIST;

private:
	CTarget_Manager*			m_pTarget_Manager = nullptr;
	CLight_Manager*				m_pLight_Manager = nullptr;
	CShader*					m_pShader_LightAcc = nullptr;
	CShader*					m_pShader_Blend = nullptr;
	CShader*					m_pShader_Blur = nullptr;

private:
	LPDIRECT3DVERTEXBUFFER9		m_pVB = nullptr;
	LPDIRECT3DINDEXBUFFER9		m_pIB = nullptr;

private:
	HRESULT Render_Priority();
	HRESULT Render_NonAlpha();
	HRESULT Render_Alpha();
	HRESULT Render_UI();

private:
	HRESULT Render_LightAcc();
	HRESULT Render_Blend(); // 백버퍼에 디퓨즈 * 셰이드의 결과를 그린다.
	HRESULT Render_Blur(); // 백버퍼에 디퓨즈 * 셰이드의 결과를 그린다.

public:
	static CRenderer* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone_Component(void* pArg); // 객체에게 컴포넌트로서 부여될때. 
	virtual void Free();
};

END