#include "..\Headers\Renderer.h"
#include "GameObject.h"
#include "Shader.h"
#include "Target_Manager.h"
#include "Light_Manager.h"

CRenderer::CRenderer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
	, m_pTarget_Manager(CTarget_Manager::GetInstance())
	, m_pLight_Manager(CLight_Manager::GetInstance())
{
	Safe_AddRef(m_pTarget_Manager);
	Safe_AddRef(m_pLight_Manager);
}


HRESULT CRenderer::Ready_Component_Prototype()
{
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	D3DVIEWPORT9		ViewPort;
	m_pGraphic_Device->GetViewport(&ViewPort);

	// For.Target_Diffuse : 한픽셀당 색(rgba정수)을 담는다. D3DFMT_A8R8G8B8
	if (FAILED(m_pTarget_Manager->Add_Target(m_pGraphic_Device, L"Target_Diffuse", ViewPort.Width, ViewPort.Height, D3DFMT_A8R8G8B8, D3DXCOLOR(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	// For.Target_Normal : 한픽셀당 벡터(xyzw실수)를 담는다. D3DFMT_A16B16G16R16F or D3DFMT_A32B32G32R32F (0.0f ~ 1.0f까지 담는다)
	if (FAILED(m_pTarget_Manager->Add_Target(m_pGraphic_Device, L"Target_Normal", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;

	// For.Target_Depth : 한픽셀당 벡터(xyzw실수)를 담는다. D3DFMT_A16B16G16R16F or D3DFMT_A32B32G32R32F (0.0f ~ 1.0f까지 담는다)
	if (FAILED(m_pTarget_Manager->Add_Target(m_pGraphic_Device, L"Target_Depth", ViewPort.Width, ViewPort.Height, D3DFMT_A32B32G32R32F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;

	// For.Target_Depth : 한픽셀당 벡터(xyzw실수)를 담는다. D3DFMT_A16B16G16R16F or D3DFMT_A32B32G32R32F (0.0f ~ 1.0f까지 담는다)
	if (FAILED(m_pTarget_Manager->Add_Target(m_pGraphic_Device, L"Target_LinearDepth", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;

	// For.Target_Shade :  : 한픽셀당 벡터(xyzw실수)를 담는다. D3DFMT_A16B16G16R16F or D3DFMT_A32B32G32R32F (0.0f ~ 1.0f까지 담는다)
	if (FAILED(m_pTarget_Manager->Add_Target(m_pGraphic_Device, L"Target_Shade", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;

	// For.Target_Specular :  : 한픽셀당 벡터(xyzw실수)를 담는다. D3DFMT_A16B16G16R16F or D3DFMT_A32B32G32R32F (0.0f ~ 1.0f까지 담는다)
	if (FAILED(m_pTarget_Manager->Add_Target(m_pGraphic_Device, L"Target_Specular", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;

	// For.Target_Specular :  : 한픽셀당 벡터(xyzw실수)를 담는다. D3DFMT_A16B16G16R16F or D3DFMT_A32B32G32R32F (0.0f ~ 1.0f까지 담는다)
	if (FAILED(m_pTarget_Manager->Add_Target(m_pGraphic_Device, L"Target_Blur", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;

	// 내가 원하는 기준(동시에 장치에 셋팅되는) 으로 타겟들을 묶어놓는다.

	// For.MRT_Deferred
	if (FAILED(m_pTarget_Manager->Add_MRT(L"MRT_Deferred", L"Target_Diffuse")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(L"MRT_Deferred", L"Target_Normal")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(L"MRT_Deferred", L"Target_Depth")))
		return E_FAIL;

	// For.MRT_LightAcc
	if (FAILED(m_pTarget_Manager->Add_MRT(L"MRT_LightAcc", L"Target_Shade")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(L"MRT_LightAcc", L"Target_Specular")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(L"MRT_LightAcc", L"Target_LinearDepth")))
		return E_FAIL;

	// For.MRT_Blur
	if (FAILED(m_pTarget_Manager->Add_MRT(L"MRT_Blur", L"Target_Blur")))
		return E_FAIL;

	m_pShader_LightAcc = CShader::Create(m_pGraphic_Device, L"../Bin/Shader/Shader_LightAcc.fx");
	if (nullptr == m_pShader_LightAcc)
		return E_FAIL;

	m_pShader_Blend = CShader::Create(m_pGraphic_Device, L"../Bin/Shader/Shader_Blend.fx");
	if (nullptr == m_pShader_Blend)
		return E_FAIL;

	m_pShader_Blur = CShader::Create(m_pGraphic_Device, L"../Bin/Shader/Shader_Blur.fx");
	if (nullptr == m_pShader_Blur)
		return E_FAIL;

	if (FAILED(m_pGraphic_Device->CreateVertexBuffer(sizeof(VTXVIEWPORTTEX) * 4, 0, D3DFVF_XYZRHW | D3DFVF_TEX1, D3DPOOL_MANAGED, &m_pVB, nullptr)))
		return E_FAIL;

	VTXVIEWPORTTEX*		pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0].vPosition = _vec4(0.0f - 0.5f, 0.0f - 0.5f, 0.f, 1.f);
	pVertices[0].vTexUV = _vec2(0.f, 0.f);

	pVertices[1].vPosition = _vec4(ViewPort.Width - 0.5f, 0.0f - 0.5f, 0.f, 1.f);
	pVertices[1].vTexUV = _vec2(1.f, 0.f);

	pVertices[2].vPosition = _vec4(ViewPort.Width - 0.5f, ViewPort.Height - 0.5f, 0.f, 1.f);
	pVertices[2].vTexUV = _vec2(1.f, 1.f);

	pVertices[3].vPosition = _vec4(0.0f - 0.5f, ViewPort.Height - 0.5f, 0.f, 1.f);
	pVertices[3].vTexUV = _vec2(0.f, 1.f);

	m_pVB->Unlock();


	if (FAILED(m_pGraphic_Device->CreateIndexBuffer(sizeof(INDEX16) * 2, 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB, nullptr)))
		return E_FAIL;

	INDEX16*		pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	pIndices[0]._0 = 0;
	pIndices[0]._1 = 1;
	pIndices[0]._2 = 2;

	pIndices[1]._0 = 0;
	pIndices[1]._1 = 2;
	pIndices[1]._2 = 3;

	m_pIB->Unlock();


#ifdef _DEBUG
	if (FAILED(m_pTarget_Manager->Ready_Debug_Buffer(L"Target_Diffuse", 0.f, 0.f, 200.f, 200.f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug_Buffer(L"Target_Normal", 0.f, 200.f, 200.f, 200.f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug_Buffer(L"Target_Depth", 0.f, 400.f, 200.f, 200.f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug_Buffer(L"Target_Shade", 200.f, 0.f, 200.f, 200.f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug_Buffer(L"Target_Specular", 200.f, 200.f, 200.f, 200.f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug_Buffer(L"Target_Blur", 200.f, 400.f, 200.f, 200.f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug_Buffer(L"Target_LinearDepth", 400.f, 0.f, 200.f, 200.f)))
		return E_FAIL;
#endif

	return NOERROR;
}


HRESULT CRenderer::Ready_Component(void* pArg)
{	
	return NOERROR;
}


HRESULT CRenderer::Add_RenderList(RENDERGROUP eGroup, CGameObject * pGameObject)
{
	if (nullptr == pGameObject || 
		RENDER_END <= eGroup)
		return E_FAIL;

	m_RenderList[eGroup].push_back(pGameObject);

	Safe_AddRef(pGameObject);

	return NOERROR;
}

HRESULT CRenderer::Render_ObjectList()
{
	if (FAILED(Render_Priority()))
		return E_FAIL;

	if (FAILED(Render_NonAlpha()))
		return E_FAIL;

	if (FAILED(Render_LightAcc()))
		return E_FAIL;

	if (FAILED(Render_Blend()))
		return E_FAIL;

	if (FAILED(Render_Blur()))
		return E_FAIL;

	if (FAILED(Render_Alpha()))
		return E_FAIL;

	if (FAILED(Render_UI()))
		return E_FAIL;	

#ifdef _DEBUG
	m_pTarget_Manager->Render_Debug_Buffer(L"MRT_Deferred");
	m_pTarget_Manager->Render_Debug_Buffer(L"MRT_LightAcc");
	m_pTarget_Manager->Render_Debug_Buffer(L"MRT_Blur");
#endif

	return NOERROR;
}

HRESULT CRenderer::Render_Priority()
{
	for (auto& pGameObject : m_RenderList[RENDER_PRIORITY])
	{
		if (nullptr != pGameObject)
			pGameObject->Render_GameObject();
		Safe_Release(pGameObject);
	}
	m_RenderList[RENDER_PRIORITY].clear();

	return NOERROR;
}

HRESULT CRenderer::Render_NonAlpha()
{
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Begin_MRT(L"MRT_Deferred")))
		return E_FAIL;

	// Begin();
	// 백버퍼를 빼자.
	// 디퓨즈, 노멀타겟을 셋팅하자.

	// 그린다.
	for (auto& pGameObject : m_RenderList[RENDER_NONALPHA])
	{
		if (nullptr != pGameObject)
			pGameObject->Render_GameObject();
		Safe_Release(pGameObject);
	}
	m_RenderList[RENDER_NONALPHA].clear();

	if (FAILED(m_pTarget_Manager->End_MRT(L"MRT_Deferred")))
		return E_FAIL;
	// End();
	// 디퓨즈와 노멀타겟을 회수.
	// 백버퍼를 셋팅하자.



	return NOERROR;
}

_bool Compare(CGameObject* pSour, CGameObject* pDest)
{
	return pSour->Get_ViewZ() > pDest->Get_ViewZ();
}

HRESULT CRenderer::Render_Alpha()
{
	m_RenderList[RENDER_ALPHA].sort(Compare);

	for (auto& pGameObject : m_RenderList[RENDER_ALPHA])
	{
		if (nullptr != pGameObject)
			pGameObject->Render_GameObject();
		Safe_Release(pGameObject);
	}
	m_RenderList[RENDER_ALPHA].clear();

	return NOERROR;
}

HRESULT CRenderer::Render_UI()
{
	for (auto& pGameObject : m_RenderList[RENDER_UI])
	{
		if (nullptr != pGameObject)
			pGameObject->Render_GameObject();
		Safe_Release(pGameObject);
	}
	m_RenderList[RENDER_UI].clear();

	for (auto& pGameObject : m_RenderList[RENDER_UI2])
	{
		if (nullptr != pGameObject)
			pGameObject->Render_GameObject();
		Safe_Release(pGameObject);
	}
	m_RenderList[RENDER_UI2].clear();

	return NOERROR;
}

HRESULT CRenderer::Render_LightAcc()
{
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Begin_MRT(L"MRT_LightAcc")))
		return E_FAIL;



	LPD3DXEFFECT pEffect = m_pShader_LightAcc->Get_EffectHandle();
	if (nullptr == pEffect)
		return E_FAIL;

	m_pTarget_Manager->SetUp_OnShader(pEffect, L"Target_Normal", "g_NormalTexture");
	m_pTarget_Manager->SetUp_OnShader(pEffect, L"Target_Depth", "g_DepthTexture");

	pEffect->Begin(nullptr, 0);


	m_pLight_Manager->Render_Light(pEffect);

	pEffect->End();


	if (FAILED(m_pTarget_Manager->End_MRT(L"MRT_LightAcc")))
		return E_FAIL;

	return NOERROR;
}

HRESULT CRenderer::Render_Blend()
{
	if (nullptr == m_pGraphic_Device || 
		nullptr == m_pVB ||
		nullptr == m_pIB)
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Begin_MRT(L"MRT_Blur")))
		return E_FAIL;

	LPD3DXEFFECT pEffect = m_pShader_Blend->Get_EffectHandle();
	if (nullptr == pEffect)
		return E_FAIL;
	
	// 셰이더에 디퓨즈 텍스쳐, 셰이드 텍스쳐 셋팅해놓는다.
	if(FAILED(m_pTarget_Manager->SetUp_OnShader(pEffect, L"Target_Diffuse", "g_DiffuseTexture")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->SetUp_OnShader(pEffect, L"Target_Shade", "g_ShadeTexture")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->SetUp_OnShader(pEffect, L"Target_Specular", "g_SpecularTexture")))
		return E_FAIL;
	
	// 셰이더를 비긴.
	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	// 백버퍼를 채우기위한 사각형 버퍼를 렌더링하낟.
	m_pGraphic_Device->SetStreamSource(0, m_pVB, 0, sizeof(VTXVIEWPORTTEX));
	m_pGraphic_Device->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1);
	m_pGraphic_Device->SetIndices(m_pIB);
	m_pGraphic_Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
	
	// 셰이더를 엔드.
	pEffect->EndPass();
	pEffect->End();

	if (FAILED(m_pTarget_Manager->End_MRT(L"MRT_Blur")))
		return E_FAIL;

	return NOERROR;
}

HRESULT CRenderer::Render_Blur()
{
	if (nullptr == m_pGraphic_Device ||
		nullptr == m_pVB ||
		nullptr == m_pIB)
		return E_FAIL;

	LPD3DXEFFECT pEffect = m_pShader_Blur->Get_EffectHandle();
	if (nullptr == pEffect)
		return E_FAIL;

	// 셰이더에 디퓨즈 텍스쳐, 셰이드 텍스쳐 셋팅해놓는다.
	if (FAILED(m_pTarget_Manager->SetUp_OnShader(pEffect, L"Target_Blur", "Texture0")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->SetUp_OnShader(pEffect, L"Target_LinearDepth", "Depth")))
		return E_FAIL;

	pEffect->CommitChanges();

	// 셰이더를 비긴.
	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	// 백버퍼를 채우기위한 사각형 버퍼를 렌더링하낟.
	m_pGraphic_Device->SetStreamSource(0, m_pVB, 0, sizeof(VTXVIEWPORTTEX));
	m_pGraphic_Device->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1);
	m_pGraphic_Device->SetIndices(m_pIB);
	m_pGraphic_Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	// 셰이더를 엔드.
	pEffect->EndPass();
	pEffect->End();

	return NOERROR;
}

CRenderer * CRenderer::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CRenderer*		pInstance = new CRenderer(pGraphic_Device);

	if (FAILED(pInstance->Ready_Component_Prototype()))
	{
		MSG_BOX("Failed To Create CRenderer Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CRenderer::Clone_Component(void * pArg)
{
	AddRef();

	return this;
}

void CRenderer::Free()
{
	for (size_t i = 0; i < RENDER_END; ++i)
	{
		for (auto& pGameObject : m_RenderList[i])
			Safe_Release(pGameObject);
		m_RenderList[i].clear();
	}

	Safe_Release(m_pVB);
	Safe_Release(m_pIB);

	Safe_Release(m_pShader_Blend);
	Safe_Release(m_pShader_Blur);
	Safe_Release(m_pShader_LightAcc);
	Safe_Release(m_pLight_Manager);
	Safe_Release(m_pTarget_Manager);

	CComponent::Free();
}
