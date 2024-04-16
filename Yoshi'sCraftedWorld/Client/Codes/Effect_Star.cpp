#include "stdafx.h"
#include "..\Headers\Effect_Star.h"
#include "Management.h"

CEffect_Star::CEffect_Star(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CEffect_Star::CEffect_Star(const CEffect_Star & rhs)
	: CGameObject(rhs)
{
}

HRESULT CEffect_Star::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CEffect_Star::Ready_GameObject(void * pArg)
{
	memcpy(&m_tInfo, pArg, sizeof(EFFECT_INFO));

	if (FAILED(Add_Component()))
		return E_FAIL;

	m_fAlpha = 0.8f;
	m_fU = 1.f;
	m_fV = 1.f;

	switch (m_tInfo.iIndex)
	{
	case 0:
		m_vColorMul = _vec4(1.f, 0.27f, 0.35f, 0.6f);
		break;
	case 1:
		m_vColorMul = _vec4(0.3f, 0.78f, 0.96f, 0.6f);
		break;
	case 2:
		m_vColorMul = _vec4(0.36f, 0.78f, 0.56f, 0.6f);
		break;
	case 3:
		m_vColorMul = _vec4(1.f, 0.84f, 0.3f, 0.6f);
		break;
	case 4:
		m_vColorMul = _vec4(0.78f, 0.5f, 0.86f, 0.6f);
		break;
	}

	_vec3 vRandom;
	GetRandomVector(&vRandom, &m_tInfo.vMin, &m_tInfo.vMax);
	m_vDir = vRandom;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_tInfo.vPos);
	//m_pTransformCom->SetUp_Scale(_vec3(0.f, 0.f, 0.f));
	m_pTransformCom->SetUp_Scale(_vec3(0.5f, 0.5f, 0.5f));

	//m_vAdd = _vec3(0.7f, 0.7f, 0.7f);
	m_vAdd = _vec3(0.f, 0.f, 0.f);

	return NOERROR;
}

_int CEffect_Star::Update_GameObject(_double TimeDelta)
{
	_vec3 vOut = m_pTransformCom->Get_State(CTransform::STATE_POSITION) + (m_vDir * (_float)TimeDelta * 3.f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vOut);

	m_fAlpha -= (_float)TimeDelta * 2.f;
	if (m_fAlpha < 0.f)
		return OBJDEAD;

	return _int();
}

_int CEffect_Star::LateUpdate_GameObject(_double TimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	m_pRendererCom->Add_RenderList(CRenderer::RENDER_ALPHA, this);

	return _int();
}

HRESULT CEffect_Star::Render_GameObject()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pTextureCom ||
		nullptr == m_pBufferCom)
		return E_FAIL;

	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if (nullptr == pEffect)
		return E_FAIL;

	Safe_AddRef(pEffect);

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);

	return NOERROR;
}

HRESULT CEffect_Star::Add_Component()
{
	// For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, &CTransform::STATE(5.f, D3DXToRadian(360)))))
		return E_FAIL;

	// For.Com_Renderer	
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// For.Com_Buffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Buffer_RcTex", L"Com_Buffer", (CComponent**)&m_pBufferCom)))
		return E_FAIL;

	// For.Com_Shader
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Shader_Effect", L"Com_Shader", (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	// For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_TRAIN, L"Component_Effect_Star0", L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	//Component_Effect_Smoke
	return NOERROR;
}

HRESULT CEffect_Star::SetUp_ConstantTable()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	_matrix			matView;

	m_pShaderCom->Set_Value("g_matWorld", &m_pTransformCom->Get_WorldMatrix(), sizeof(_matrix));
	m_pShaderCom->Set_Value("g_matView", matView = Get_Transform(D3DTS_VIEW), sizeof(_matrix));
	m_pShaderCom->Set_Value("g_matProj", Get_Transform(D3DTS_PROJECTION), sizeof(_matrix));

	m_pTextureCom->SetUp_OnShader(m_pShaderCom->Get_EffectHandle(), "g_BaseTexture");

	m_pShaderCom->Set_Value("g_TexAlpha", &m_fAlpha, sizeof(_float));
	m_pShaderCom->Set_Value("g_vColorMultiply", &m_vColorMul, sizeof(_vec4));

	return NOERROR;
}

_float CEffect_Star::GetRandomFloat(float lowBound, float highBound)
{
	if (lowBound >= highBound) // bad input
		return lowBound;

	// get random float in [0, 1] interval
	float f = (rand() % 10000) * 0.0001f;

	// return float in [lowBound, highBound] interval. 
	return (f * (highBound - lowBound)) + lowBound;
}

void CEffect_Star::GetRandomVector(D3DXVECTOR3 * out, D3DXVECTOR3 * min, D3DXVECTOR3 * max)
{
	out->x = GetRandomFloat(min->x, max->x);
	out->y = GetRandomFloat(min->y, max->y);
	out->z = GetRandomFloat(min->z, max->z);
}

CEffect_Star * CEffect_Star::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CEffect_Star*		pInstance = new CEffect_Star(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		MSG_BOX("Failed To Create CGameObject Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CEffect_Star::Clone_GameObject(void * pArg)
{
	CEffect_Star*		pInstance = new CEffect_Star(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		MSG_BOX("Failed To Create CGameObject Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Star::Free()
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);

	CGameObject::Free();
}
