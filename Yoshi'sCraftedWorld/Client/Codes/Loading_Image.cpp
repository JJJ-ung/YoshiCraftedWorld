#include "stdafx.h"
#include "..\Headers\Loading_Image.h"
#include "Management.h"

USING(Client)

CLoading_Image::CLoading_Image(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CLoading_Image::CLoading_Image(const CLoading_Image & rhs)
	:CGameObject(rhs)
{
}

HRESULT CLoading_Image::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CLoading_Image::Ready_GameObject(void * pArg)
{
	LOADING_IMAGE* p = (LOADING_IMAGE*)pArg;

	m_tInfo.iType = p->iType;
	m_tInfo.strKey = p->strKey;

	if (m_tInfo.iType == 0 )
	{
		m_tInfo.iFrame = 0;
		m_fSpeed = 1.3f;
	}
	else if (m_tInfo.iType == 1)
	{
		m_tInfo.iFrame = 0;
		m_fSpeed = 2.5f;
	}
	else
	{
		m_tInfo.iFrame = 1;
		m_fSpeed = 1.2f;
	}

	if (FAILED(Add_Component()))
		return E_FAIL;

	switch (m_tInfo.iType)
	{
	case 0:
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, _vec3(0.f, 2.f, 0.f));
		m_pTransformCom->SetUp_Scale(_vec3(2.f, 2.f, 2.f));
		break;
	case 1:
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, _vec3(0.f, 0.f, 0.f));
		m_pTransformCom->SetUp_Scale(_vec3(2.f, 2.f, 2.f));
		break;
	case 2:
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, _vec3(0.f, 0.f, 0.f));
		m_pTransformCom->SetUp_Scale(_vec3(2.f, 2.f, 2.f));
		break;
	default:
		break;
	}

	return NOERROR;
}

_int CLoading_Image::Update_GameObject(_double TimeDelta)
{
	switch (m_tInfo.iType)
	{
	case 0:
		Update_LoadIn(TimeDelta);
		break;
	case 1:
		Update_LoadOut_Up(TimeDelta);
		break;
	case 2:
		Update_LoadOut_Down(TimeDelta);
		break;
	default:
		break;
	}

	return _int();
}

_int CLoading_Image::LateUpdate_GameObject(_double TimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	if(m_tInfo.bTop)
		m_pRendererCom->Add_RenderList(CRenderer::RENDER_UI2, this);
	else
		m_pRendererCom->Add_RenderList(CRenderer::RENDER_UI, this);

	return _int();
}

HRESULT CLoading_Image::Render_GameObject()
{
	if (nullptr == m_pBufferCom ||
		nullptr == m_pShaderCom ||
		nullptr == m_pTextureCom)
		return E_FAIL;


	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	if (nullptr == pEffect)
		return E_FAIL;

	Safe_AddRef(pEffect);

	_matrix		matTmp;

	pEffect->SetMatrix("g_matWorld", &m_pTransformCom->Get_WorldMatrix());
	pEffect->SetMatrix("g_matView", D3DXMatrixIdentity(&matTmp));
	pEffect->SetMatrix("g_matProj", D3DXMatrixIdentity(&matTmp));

	if (FAILED(m_pTextureCom->SetUp_OnShader(pEffect, "g_BaseTexture", m_tInfo.iFrame)))
		return E_FAIL;

	pEffect->Begin(nullptr, 0);

	pEffect->BeginPass(0);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);

	return NOERROR;
}

_int CLoading_Image::Update_LoadIn(_double TimeDelta)
{
	m_pTransformCom->Go_Down(TimeDelta);

	if (m_pTransformCom->Get_State(CTransform::STATE_POSITION).y <= 0.f)
	{
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, _vec3(0.f, 0.f, 0.f));
		m_tInfo.bAnimation = true;
	}

	return _int();
}

_int CLoading_Image::Update_LoadOut_Up(_double TimeDelta)
{
	if (m_bStartAnimation)
	{
		m_pTransformCom->Go_Up(TimeDelta);

		if (m_pTransformCom->Get_State(CTransform::STATE_POSITION).y >= 2.f)
		{
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, _vec3(0.f, 2.f, 0.f));
			m_tInfo.bAnimation = true;
		}
	}

	return _int();
}

_int CLoading_Image::Update_LoadOut_Down(_double TimeDelta)
{
	if(m_bStartAnimation)
		m_pTransformCom->Go_Down(TimeDelta);

	return _int();
}


HRESULT CLoading_Image::Add_Component()
{
	// For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, &CTransform::STATE(m_fSpeed, D3DXToRadian(90)))))
		return E_FAIL;

	// For.Com_Renderer	
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// For.Com_Buffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Buffer_RcTex", L"Com_Buffer", (CComponent**)&m_pBufferCom)))
		return E_FAIL;

	// For.Com_Shader
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Shader_Default", L"Com_Shader", (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	// For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, m_tInfo.strKey.c_str(), L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return NOERROR;
}

CGameObject * CLoading_Image::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CGameObject*		pInstance = new CLoading_Image(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		MSG_BOX("Failed To Create CGameObject Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CLoading_Image::Clone_GameObject(void * pArg)
{
	CGameObject*		pInstance = new CLoading_Image(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		MSG_BOX("Failed To Create CGameObject Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoading_Image::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pBufferCom);
	Safe_Release(m_pRendererCom);

	CGameObject::Free();
}
