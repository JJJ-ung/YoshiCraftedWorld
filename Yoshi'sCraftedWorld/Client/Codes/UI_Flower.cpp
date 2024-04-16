#include "stdafx.h"
#include "..\Headers\UI_Flower.h"
#include "Management.h"

#include "UI_SingleFlower.h"
#include "TrainPlatform.h"
#include "Camera_SideScroll.h"
#include "Yoshi_Stage.h"
#include "TrainA.h"
#include "TrainB.h"

USING(Client)

CUI_Flower::CUI_Flower(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CUI_Flower::CUI_Flower(const CUI_Flower & rhs)
	: CGameObject(rhs)
{
}

HRESULT CUI_Flower::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CUI_Flower::Ready_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_fX = 640.f;
	m_fY = -87.f;
	m_fSizeX = 600.f;
	m_fSizeY = 120.f;

	//for (size_t i = 0; i < 6; ++i)
	//	Add_Flower(i);

	return NOERROR;
}

_int CUI_Flower::Update_GameObject(_double TimeDelta)
{
	if (m_bShow)
	{
		system_clock::duration temp = system_clock::now() - m_tEnterTime;
		if (duration_cast<milliseconds>(temp) > (milliseconds)3000)
		{
			if (m_FlowerList.size() == 6)
			{
				CManagement* pManagement = CManagement::GetInstance();
				Safe_AddRef(pManagement);

				CSoundMgr::GetInstance()->PlaySound(L"FlowerComp_Kira01.wav", CSoundMgr::UI);

				CCamera_SideScroll* pCamera = (CCamera_SideScroll*)pManagement->Get_ObjectFromLayer(SCENE_TRAIN, L"Layer_Camera");
				pCamera->Get_CutScene((milliseconds)3000) = true;
				pCamera->Get_EnterTime() = system_clock::now();

				CYoshi_Stage* pPlayer = (CYoshi_Stage*)pManagement->Get_ObjectFromLayer(SCENE_TRAIN, L"Layer_Object", 0);
				pPlayer->Get_CutScene() = true;

				CTrainPlatform* pPlatform = (CTrainPlatform*)pManagement->Get_ObjectFromLayer(SCENE_TRAIN, L"Layer_TrainPlatform");
				pPlatform->Change_State(CTrainPlatform::CUTSCENE);

				CTrainA* pTrain = (CTrainA*)pManagement->Get_ObjectFromLayer(SCENE_TRAIN, L"Layer_TrainA");
				pTrain->Get_OnTrain() = true;

				CTrainB* pTrainB = (CTrainB*)pManagement->Get_ObjectFromLayer(SCENE_TRAIN, L"Layer_TrainB");
				pTrainB->Get_OnTrain() = true;

				Safe_Release(pManagement);
			}

			m_bShow = false;
			for (auto& pFlower : m_FlowerList)
				pFlower->Get_Show() = false;
		}

		m_fY += 200.f * (_float)TimeDelta;
		if (m_fY > 87.f)
			m_fY = 87.f;
	}
	else
	{
		m_fY -= 200.f * (_float)TimeDelta;
		if (m_fY < -87.f)
			m_fY = -87.f;
	}

	return _int();
}

_int CUI_Flower::LateUpdate_GameObject(_double TimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	if (FAILED(m_pRendererCom->Add_RenderList(CRenderer::RENDER_UI, this)))
		return -1;

	return _int();
}

HRESULT CUI_Flower::Render_GameObject()
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

HRESULT CUI_Flower::Add_Flower(_uint iIndex)
{
	if (iIndex > 6)
		return E_FAIL;

	CSoundMgr::GetInstance()->PlaySound(L"Flower_Shoot01-3.wav", CSoundMgr::FLOWER);
	CSoundMgr::GetInstance()->PlaySound(L"Flower_Shoot_Chimes01-1.wav", CSoundMgr::FLOWER2);

	CUI_SingleFlower::FLOWER_INFO tInfo;
	tInfo.m_iIndex = iIndex;

	CManagement* pManagement = CManagement::GetInstance();
	Safe_AddRef(pManagement);

	if (FAILED(pManagement->Add_GameObjectToLayer(SCENE_TRAIN, L"Layer_SingleFlower", L"GameObject_CUI_SingleFlower", (void*)&tInfo)))
		return E_FAIL;

	CUI_SingleFlower* pFlower = (CUI_SingleFlower*)pManagement->Get_ObjectFromLayer(SCENE_TRAIN, L"Layer_SingleFlower", m_FlowerList.size());
	m_FlowerList.emplace_back(pFlower);

	Safe_Release(pManagement);

	m_tEnterTime = system_clock::now();
	m_bShow = true;

	for (auto& pFlower : m_FlowerList)
		pFlower->Get_Show() = true;

	return NOERROR;
}

HRESULT CUI_Flower::Add_Component()
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
	if (FAILED(CGameObject::Add_Component(SCENE_TRAIN, L"Component_Texture_FlowerBack", L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CUI_Flower::SetUp_ConstantTable()
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

CUI_Flower * CUI_Flower::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUI_Flower*		pInstance = new CUI_Flower(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		MSG_BOX("Failed To Create CGameObject Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CUI_Flower::Clone_GameObject(void * pArg)
{
	CUI_Flower*		pInstance = new CUI_Flower(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		MSG_BOX("Failed To Create CGameObject Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Flower::Free()
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);

	CGameObject::Free();
}
