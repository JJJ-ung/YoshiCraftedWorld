#pragma once

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CCollider;
class CFrustum;
class CRenderer;
class CTransform;
class CMesh_Dynamic;
END

BEGIN(Client)
class CCloud final : public CGameObject
{
public:
	enum CLOUD_OPTION { STAIRS, APPEAR, HEART, COIN };

private:
	explicit CCloud(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CCloud(const CCloud& rhs);
	virtual ~CCloud() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int Update_GameObject(_double TimeDelta);
	virtual _int LateUpdate_GameObject(_double TimeDelta);
	virtual HRESULT Render_GameObject();

public:
	_float GetRandomFloat(float lowBound, float highBound);
	void GetRandomVector(D3DXVECTOR3 * out, D3DXVECTOR3 * min, D3DXVECTOR3 * max);

private:
	CRenderer*					m_pRendererCom = nullptr;
	CMesh_Dynamic*		m_pMeshCom = nullptr;
	CShader*						m_pShaderCom = nullptr;
	CTransform*					m_pTransformCom = nullptr;
	CFrustum*					m_pFrustumCom = nullptr;
	CCollider*						m_pColliderCom = nullptr;

private:
	CLOUD_OPTION	m_eOption;
	_double					m_TimeDelta;
	_bool						m_bHit = false;
	_tchar*					m_pTag;
	_float						m_fAngle = 180.f;
	_uint						m_iIndex = -1;
	_bool						m_bSound = false;

private:
	system_clock::time_point m_tHitTime;

private:
	HRESULT Add_Component();
	HRESULT SetUp_ConstantTable();

public:
	static CGameObject* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject(void* pArg);
	virtual void Free();
};
END