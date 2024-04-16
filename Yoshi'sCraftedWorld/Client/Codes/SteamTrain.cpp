#include "stdafx.h"
#include "..\Headers\SteamTrain.h"
#include "Management.h"
#include "Light_Manager.h"
#include "Mesh_Dynamic.h"
#include "Frustum.h"
#include "Input_Device.h"
#include "TrainA.h"
#include "TrainB.h"

USING(Client)

CSteamTrain::CSteamTrain(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CSteamTrain::CSteamTrain(const CSteamTrain & rhs)
	:CGameObject(rhs)
{
	m_vecPath0 = rhs.m_vecPath0;
	m_vecPath1 = rhs.m_vecPath1;
	m_vecPath2 = rhs.m_vecPath2;
}

HRESULT CSteamTrain::Ready_GameObject_Prototype()
{
	if (FAILED(Add_Path0()))
		return E_FAIL;
	if (FAILED(Add_Path1()))
		return E_FAIL;
	if (FAILED(Add_Path2()))
		return E_FAIL;

	return NOERROR;
}

HRESULT CSteamTrain::Ready_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_vecCurrPath = m_vecPath0;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vecCurrPath[0] /*+ _vec3(5.f, 0.f, 0.f)*/);

	m_iterTarget = m_vecCurrPath.begin();
	m_iterTarget += 1;
	_vec3 Test = *m_iterTarget;

	m_vLook = (*m_iterTarget) - m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	D3DXVec3Normalize(&m_vLook, &m_vLook);
	m_pTransformCom->Rotation_LookAt(*m_iterTarget);

	m_pMeshCom->SetUp_Animation(0);

	//Change_CurrVec();

	return NOERROR;
}

_int CSteamTrain::Update_GameObject(_double TimeDelta)
{
	m_TimeDelta = TimeDelta;

	if (m_bStop)
	{
		if (m_iterTarget != m_vecCurrPath.end())
		{
			//CInput_Device* pInputDev = CInput_Device::GetInstance();
			//Safe_AddRef(pInputDev);
			//if (pInputDev->Key_Down(KEY_ENTER))
			//{
			//	m_bPlayAnimation = true;
			//	m_bStop = false;
			//}
			//Safe_Release(pInputDev);
			return _int();
		}
	}
	else
		CSoundMgr::GetInstance()->PlaySound(L"SDL_TRAIN_RUN1.wav", CSoundMgr::TRAIN);

	m_fSpeed += 5.f * (_float)TimeDelta;
	if (m_fSpeed > 5.f)
		m_fSpeed = 5.f;

	_vec3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_vec3 vDistance = (*m_iterTarget) - vPos;

	_float fDistance = D3DXVec3Length(&vDistance);

	if ( fDistance < 0.1f )
	{
		if (m_iterTarget != m_vecCurrPath.end())
			++m_iterTarget;
		if (m_iterTarget == m_vecCurrPath.end())
		{
			Change_CurrVec();
			//m_iterTarget = m_vecCurrPath.begin();
			//m_iterTarget += m_vecCurrPath.size() - 1;
			//m_bStop = true;
			//m_bPlayAnimation = false;
		}
	}

	D3DXVec3Lerp(&m_vLook, &m_vLook, &(*D3DXVec3Normalize(&vDistance, &vDistance)), (_float)TimeDelta * 10.f);
	_vec3 vLook = vPos + m_vLook * 5.f /** 10.f*/;

	m_pTransformCom->Move_Direction(*D3DXVec3Normalize(&vDistance, &vDistance), (_float)TimeDelta * m_fSpeed);
	m_pTransformCom->Rotation_LookAt(vLook);

	return _int();
}

_int CSteamTrain::LateUpdate_GameObject(_double TimeDelta)
{
	if (m_pRendererCom == nullptr)
		return -1;

	if (true == m_pFrustumCom->Culling_OnWorldSpace(&m_pTransformCom->Get_State(CTransform::STATE_POSITION), 8.f))
		m_pRendererCom->Add_RenderList(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

HRESULT CSteamTrain::Render_GameObject()
{
	if (nullptr == m_pMeshCom ||
		nullptr == m_pShaderCom)
		return E_FAIL;

	if(m_bPlayAnimation)
		m_pMeshCom->Play_Animation(m_TimeDelta);

	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if (nullptr == pEffect)
		return E_FAIL;

	Safe_AddRef(pEffect);

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	for (size_t i = 0; i < m_pMeshCom->Get_NumMeshContainer(); ++i)
	{
		m_pMeshCom->Update_SkinnedMesh(i);

		_ulong	dwNumMaterials = m_pMeshCom->Get_NumMaterials(i);

		for (size_t j = 0; j < dwNumMaterials; ++j)
		{
			m_pMeshCom->SetTexture_OnShader(pEffect, "g_BaseTexture", i, j, MESHTEXTURE::TYPE_DIFFUSE);
			m_pMeshCom->SetMaterial_OnShader(pEffect, "g_Material", i, j);

			pEffect->CommitChanges();

			m_pMeshCom->Render_Mesh(i, j);
		}
	}

	pEffect->EndPass();
	pEffect->End();
	Safe_Release(pEffect);

	return NOERROR;
}

_vec3 CSteamTrain::Get_Look()
{
	_vec3 vDistance = (*m_iterTarget) - m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	D3DXVec3Normalize(&vDistance, &vDistance);

	return vDistance;
}

HRESULT CSteamTrain::Change_CurrVec()
{
	if (m_iCurrIndex == 0)
	{
		m_vecCurrPath = m_vecPath1;
		m_iCurrIndex = 1;
		CSoundMgr::GetInstance()->PlaySound(L"SD_TRAIN_KITEKI2.wav", CSoundMgr::EFFECT2);
	}
	else if (m_iCurrIndex == 1)
	{
		m_vecCurrPath = m_vecPath2;
		m_iCurrIndex = 2;
		CSoundMgr::GetInstance()->PlaySound(L"SD_TRAIN_KITEKI1.wav", CSoundMgr::EFFECT2);
	}
	else
	{
		m_iterTarget = m_vecCurrPath.begin();
		m_iterTarget += m_vecCurrPath.size() - 1;
		m_bPlayAnimation = false;
		m_bStop = true;
		m_bEnd = true;
		return NOERROR;
	}

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vecCurrPath[0] /*+ _vec3(5.f, 0.f, 0.f)*/);

	m_iterTarget = m_vecCurrPath.begin();
	m_iterTarget += 1;
	_vec3 Test = *m_iterTarget;

	m_vLook = (*m_iterTarget) - m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	D3DXVec3Normalize(&m_vLook, &m_vLook);
	m_pTransformCom->Rotation_LookAt(*m_iterTarget);

	CManagement* pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return E_FAIL;
	Safe_AddRef(pManagement);

	CTrainA* pTrainA = (CTrainA*)pManagement->Get_ObjectFromLayer(SCENE_TRAIN, L"Layer_TrainA");
	CTrainB* pTrainB = (CTrainB*)pManagement->Get_ObjectFromLayer(SCENE_TRAIN, L"Layer_TrainB");

	pTrainA->Change_CurrVec(m_vecCurrPath);
	pTrainB->Change_CurrVec(m_vecCurrPath);

	Safe_Release(pManagement);

	m_bPlayAnimation = false;
	m_bStop = true;

	m_iterTarget = m_vecCurrPath.begin();
	m_iterTarget += 1;

	return NOERROR;
}

HRESULT CSteamTrain::Add_Component()
{
	// For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, &CTransform::STATE(1.f, D3DXToRadian(90)))))
		return E_FAIL;

	// For.Com_Renderer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// For.Com_Mesh
	if (FAILED(CGameObject::Add_Component(SCENE_TRAIN, L"Component_Mesh_SteamTrain", L"Com_Mesh", (CComponent**)&m_pMeshCom)))
		return E_FAIL;

	// For.Com_Shader
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Shader_Mesh", L"Com_Shader", (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	// For.Com_Frustum
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Frustum", L"Com_Frustum", (CComponent**)&m_pFrustumCom)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CSteamTrain::SetUp_ConstantTable()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	_matrix			matView;

	m_pShaderCom->Set_Value("g_matWorld", &m_pTransformCom->Get_WorldMatrix(), sizeof(_matrix));
	m_pShaderCom->Set_Value("g_matView", matView = Get_Transform(D3DTS_VIEW), sizeof(_matrix));
	m_pShaderCom->Set_Value("g_matProj", Get_Transform(D3DTS_PROJECTION), sizeof(_matrix));

	return NOERROR;
}

HRESULT CSteamTrain::Add_Path0()
{
	std::locale::global(std::locale("Korean"));
#ifdef _UNICODE
	wifstream fin;
#else
	wifstream fin;
#endif // _UNICODE

	fin.open(L"../Bin/Data/Trigger/TrainPath_0.dat");

	if (fin.fail())
		return E_FAIL;

	wstring strFileName = L"";
	_uint iTemp;

	while (true)
	{
		_vec3 vPos;
		fin >> strFileName >> vPos.x >> vPos.y >> vPos.z >> iTemp;

		if (fin.eof())
			break;

		vPos += _vec3(0.f, 0.25f, 0.f);

		m_vecPath0.emplace_back(vPos);
	}

	return NOERROR;
}

HRESULT CSteamTrain::Add_Path1()
{
	std::locale::global(std::locale("Korean"));
#ifdef _UNICODE
	wifstream fin;
#else
	wifstream fin;
#endif // _UNICODE

	fin.open(L"../Bin/Data/Trigger/TrainPath_1.dat");

	if (fin.fail())
		return E_FAIL;

	wstring strFileName = L"";
	_uint iTemp;

	while (true)
	{
		_vec3 vPos;
		fin >> strFileName >> vPos.x >> vPos.y >> vPos.z >> iTemp;

		if (fin.eof())
			break;

		vPos += _vec3(0.f, 0.25f, 0.f);

		m_vecPath1.emplace_back(vPos);
	}

	return NOERROR;
}

HRESULT CSteamTrain::Add_Path2()
{
	std::locale::global(std::locale("Korean"));
#ifdef _UNICODE
	wifstream fin;
#else
	wifstream fin;
#endif // _UNICODE

	fin.open(L"../Bin/Data/Trigger/TrainPath_2.dat");

	if (fin.fail())
		return E_FAIL;

	wstring strFileName = L"";
	_uint iTemp;

	while (true)
	{
		_vec3 vPos;
		fin >> strFileName >> vPos.x >> vPos.y >> vPos.z >> iTemp;

		if (fin.eof())
			break;

		vPos += _vec3(0.f, 0.25f, 0.f);

		m_vecPath2.emplace_back(vPos);
	}

	return NOERROR;
}

CGameObject * CSteamTrain::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CGameObject*		pInstance = new CSteamTrain(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		MSG_BOX("Failed To Create CGameObject Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CSteamTrain::Clone_GameObject(void * pArg)
{
	CGameObject*		pInstance = new CSteamTrain(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		MSG_BOX("Failed To Create CGameObject Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSteamTrain::Free()
{
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pFrustumCom);
	Safe_Release(m_pMeshCom);

	CGameObject::Free();
}
