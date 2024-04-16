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
class CMesh_Static;
END

BEGIN(Client)

class CCoin final : public CGameObject
{
public:
	enum COIN_TYPE { NORMAL_COIN = 0, LARGE_COIN = 1, FALL_COIN };

public:
	typedef struct tagCoinInfo
	{
		tagCoinInfo() {}
		tagCoinInfo(wstring _pTag, SCENEID _eID, _vec3 _vPos, _vec3 _vScale, _vec3 _vAngle, COIN_TYPE _eType, _vec3 _Direction = _vec3(0.f, 0.f, 0.f))
			:pComponentTag(_pTag), eSceneID(_eID), vPos(_vPos), vScale(_vScale), vAngle(_vAngle), eCoinType(_eType), vDirection(_Direction) {}
	public:
		wstring				pComponentTag;
		SCENEID			eSceneID;
		_vec3					vPos, vScale, vAngle;
		_vec3					vDirection;
		COIN_TYPE		eCoinType;
	}COIN_INFO;

public:
	explicit CCoin(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CCoin(const CCoin& rhs);
	virtual ~CCoin() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int Update_GameObject(_double TimeDelta);
	virtual _int LateUpdate_GameObject(_double TimeDelta);
	virtual HRESULT Render_GameObject();

private:
	CRenderer*			m_pRendererCom = nullptr;
	CMesh_Static*		m_pMeshCom = nullptr;
	CShader*				m_pShaderCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CCollider*				m_pColliderCom = nullptr;
	CFrustum*			m_pFrustumCom = nullptr;

private:
	_matrix			m_matCollider;
	COIN_INFO	m_tInfo;
	_bool				m_bDead = false;
	_float				m_fAngle = 0.f;
	_float				m_fJumpPower = 5.f;
	_float				m_fJumpAccel = 10.f;

private:
	HRESULT Add_Component();
	HRESULT SetUp_ConstantTable();

public:
	static CGameObject* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject(void* pArg);
	virtual void Free();
};

END