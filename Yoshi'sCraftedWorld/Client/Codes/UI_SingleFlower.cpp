#include "stdafx.h"
#include "..\Headers\UI_SingleFlower.h"
#include "Management.h"

USING(Client)

CUI_SingleFlower::CUI_SingleFlower(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CUI_SingleFlower::CUI_SingleFlower(const CUI_SingleFlower & rhs)
	: CGameObject(rhs)
{
}

HRESULT CUI_SingleFlower::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CUI_SingleFlower::Ready_GameObject(void * pArg)
{
	memcpy(&m_tInfo, pArg, sizeof(FLOWER_INFO));

	if (FAILED(Add_Component()))
		return E_FAIL;

	m_fX = 640.f;
	m_fY = 360.f;
	m_fSizeX = 200.f;
	m_fSizeY = 200.f;

	switch (m_tInfo.m_iIndex)
	{
	case 0:
		m_vTargetPos = _vec2(416.f, 86.f);
		break;
	case 1:
		m_vTargetPos = _vec2(506.f, 86.f);
		break;
	case 2:
		m_vTargetPos = _vec2(596.f, 86.f);
		break;
	case 3:
		m_vTargetPos = _vec2(685.f, 86.f);
		break;
	case 4:
		m_vTargetPos = _vec2(775.f, 86.f);
		break;
	case 5:
		m_vTargetPos = _vec2(864.f, 86.f);
		break;
	default:
		return E_FAIL;
	}

	return NOERROR;
}

_int CUI_SingleFlower::Update_GameObject(_double TimeDelta)
{
	if (m_bNew)
	{
		m_fSizeX -= (_float)TimeDelta * 80.f;
		m_fSizeY -= (_float)TimeDelta * 80.f;

		if (m_fSizeX < 87.f)
			m_fSizeX = 87.f;
		if (m_fSizeY < 87.f)
			m_fSizeY = 87.f;

		if (m_bMove)
		{
			_vec2 vOut;
			D3DXVec2Lerp(&vOut, &_vec2(m_fX, m_fY), &m_vTargetPos, (_float)TimeDelta * 3);

			m_fX = vOut.x;
			m_fY = vOut.y;

			_vec2 Distance = m_vTargetPos - vOut;
			if (D3DXVec2Length(&Distance) < 0.1f)
			{
				m_fX = m_vTargetPos.x;
				m_fY = m_vTargetPos.y;
				m_bMove = false;
				m_bNew = false;
			}
		}
	}

	if (!m_bNew)
	{
		if (m_bShow)
		{
			m_fY += 200.f * (_float)TimeDelta;
			if (m_fY > 87.f)
				m_fY = 87.f;
		}
		else
		{
			m_fY -= 200.f *  (_float)TimeDelta;
			if (m_fY < -87.f)
				m_fY = -87.f;
		}
	}

	return _int();
}

_int CUI_SingleFlower::LateUpdate_GameObject(_double TimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	if (FAILED(m_pRendererCom->Add_RenderList(CRenderer::RENDER_UI2, this)))
		return -1;

	return _int();
} 

HRESULT CUI_SingleFlower::Render_GameObject()
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

HRESULT CUI_SingleFlower::Add_Flower(_uint iIndex)
{
	if (iIndex > 6)
		return E_FAIL;

	return NOERROR;
}

HRESULT CUI_SingleFlower::Add_Component()
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
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Shader_UI", L"Com_Shader", (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	// For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_TRAIN, L"Component_Texture_FlowerUI", L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CUI_SingleFlower::SetUp_ConstantTable()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pTextureCom)
		return E_FAIL;

	m_pTransformCom->SetUp_Scale(_vec3(m_fSizeX, m_fSizeY, 1.f));

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _vec3(m_fX - (g_nWinCX >> 1), (g_nWinCY >> 1) - m_fY, 0.f));

	m_pShaderCom->Set_Value("g_matWorld", &m_pTransformCom->Get_WorldMatrix(), sizeof(_matrix));

	_matrix			matView;
	m_pShaderCom->Set_Value("g_matView", D3DXMatrixIdentity(&matView), sizeof(_matrix));

	_matrix			matProj;
	D3DXMatrixOrthoLH(&matProj, g_nWinCX, g_nWinCY, 0.f, 1.f);

	m_pShaderCom->Set_Value("g_matProj", &matProj, sizeof(_matrix));

	m_pTextureCom->SetUp_OnShader(m_pShaderCom->Get_EffectHandle(), "g_BaseTexture", 0);

	return NOERROR;
}

CUI_SingleFlower * CUI_SingleFlower::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUI_SingleFlower*		pInstance = new CUI_SingleFlower(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		MSG_BOX("Failed To Create CGameObject Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CUI_SingleFlower::Clone_GameObject(void * pArg)
{
	CUI_SingleFlower*		pInstance = new CUI_SingleFlower(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		MSG_BOX("Failed To Create CGameObject Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_SingleFlower::Free()
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);

	CGameObject::Free();
}
