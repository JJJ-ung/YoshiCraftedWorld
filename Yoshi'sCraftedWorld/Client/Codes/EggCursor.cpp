#include "stdafx.h"
#include "..\Headers\EggCursor.h"
#include "Management.h"
#include "Input_Device.h"
#include "Yoshi_Stage.h"
#include "Yoshi_Stage_State.h"

USING(Client)

CEggCursor::CEggCursor(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CEggCursor::CEggCursor(const CEggCursor & rhs)
	: CGameObject(rhs)
{
}

HRESULT CEggCursor::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CEggCursor::Ready_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_fX = g_nWinCX - 100.f;
	m_fY = g_nWinCY - 100.f;
	m_fSizeX = 150.f;
	m_fSizeY = 150.f;

	if (FAILED(D3DXCreateLine(m_pGraphic_Device, &m_pLine)))
		return E_FAIL;

	return NOERROR;
}

_int CEggCursor::Update_GameObject(_double TimeDelta)
{
	CManagement* pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return -1;
	Safe_AddRef(pManagement);

	CYoshi_Stage* pPlayer = (CYoshi_Stage*)pManagement->Get_ObjectFromLayer(SCENE_TRAIN, L"Layer_Object");

	if (pPlayer->GetFSM()->GetCurrentState() == CStage_ThrowIdle::Instance() ||
		pPlayer->GetFSM()->GetCurrentState() == CStage_ThrowRotateTarget::Instance())
		m_bEggCursor = true;
	else
		m_bEggCursor = false;

	Safe_Release(pManagement);


	if (m_bEggCursor)
	{
		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);

		m_fX = (_float)pt.x;
		m_fY = (_float)pt.y;

		m_fFrame += (_float)TimeDelta * 10.f;
		if (m_fFrame > 4.f)
			m_fFrame = 0.f;
	}
	else
		m_fFrame = 0.f;

	return _int();
}

_int CEggCursor::LateUpdate_GameObject(_double TimeDelta)
{
	m_bFocus = false;

	if (!m_bEggCursor)
		return 0;

	if (nullptr == m_pRendererCom)
		return -1;

	if (FAILED(m_pRendererCom->Add_RenderList(CRenderer::RENDER_UI, this)))
		return -1;

	return _int();
}

HRESULT CEggCursor::Render_GameObject()
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

void CEggCursor::Set_Focus(_bool bFocus, _vec3 vPos)
{
	if (m_bFocus)
		return;

	m_bFocus = bFocus;

	_matrix matProj = Get_Transform(D3DTS_PROJECTION);
	_matrix matView = Get_Transform(D3DTS_VIEW);
}

HRESULT CEggCursor::Add_Component()
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
	if (FAILED(CGameObject::Add_Component(SCENE_TRAIN, L"Component_Texture_EggCursor", L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CEggCursor::SetUp_ConstantTable()
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

	m_pTextureCom->SetUp_OnShader(m_pShaderCom->Get_EffectHandle(), "g_BaseTexture", (_uint)m_fFrame);

	return NOERROR;
}

CEggCursor * CEggCursor::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CEggCursor*		pInstance = new CEggCursor(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		MSG_BOX("Failed To Create CGameObject Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CEggCursor::Clone_GameObject(void * pArg)
{
	CEggCursor*		pInstance = new CEggCursor(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		MSG_BOX("Failed To Create CGameObject Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEggCursor::Free()
{
	Safe_Release(m_pLine);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);

	CGameObject::Free();
}
