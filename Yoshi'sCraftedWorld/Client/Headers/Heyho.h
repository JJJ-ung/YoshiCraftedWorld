#pragma once

#include "Defines.h"
#include "GameObject.h"
#include "StateMachine.h"
#include "MonsterInfo.h"

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

class CYoshi_Tongue;
class CHeyho final : public CGameObject
{
public:
	typedef CStateMachine<CHeyho>*	STATE;

private:
	explicit CHeyho(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CHeyho(const CHeyho& rhs);
	virtual ~CHeyho() = default;

public:
	STATE GetFSM() { return m_pState; }
	CMesh_Dynamic* Get_Mesh() { return m_pMeshCom; }
	CTransform* Get_HeyhoTransform() { return m_pTransformCom; }
	CCollider* Get_Collider() { return m_pColliderCom; }

public:
	_double& Get_TimeDelta() { return m_TimeDelta; }
	_bool& Get_TongueCollision() { return m_bTongueCollision; }
	_bool& Get_MouthCollision() { return m_bMouthCollision; }
	_bool& Get_HeadCollision() { return m_bHeadCollision; }
	_bool& Get_BodyCollision() { return m_bBodyCollision; }
	_bool& Get_ObjectDead() { return m_bDead; }
	_bool& Get_EggCollision() { return m_bEggCollision; }
	_float& Get_JumpPower() { return m_fJumpPower; }
	_float& Get_JumpAccel() { return m_fJumpAccel; }
	_vec3& Get_Direction() { return m_vDirection; }
	_vec3& Get_StartPos() { return m_vStartPos; }

public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int Update_GameObject(_double TimeDelta);
	virtual _int LateUpdate_GameObject(_double TimeDelta);
	virtual HRESULT Render_GameObject();

private:
	void Set_HeadCollider();

private:
	CRenderer*					m_pRendererCom = nullptr;
	CMesh_Dynamic*		m_pMeshCom = nullptr;
	CShader*						m_pShaderCom = nullptr;
	CTransform*					m_pTransformCom = nullptr;
	CCollider*						m_pColliderCom = nullptr;
	CCollider*						m_pHeadColliderCom = nullptr;
	CCollider*						m_pEggColliderCom = nullptr;
	CFrustum*					m_pFrustumCom = nullptr;
	CTexture*						m_pTextureCom = nullptr;

private:
	CYoshi_Tongue*			m_pYoshiTongue = nullptr;
	MONSTER_INFO			m_tInfo;

private:
	STATE								m_pState = nullptr;
	_uint								m_iRandom = 0;

private:
	_bool							m_bBodyCollision = false;
	_bool							m_bHeadCollision = false;
	_bool							m_bTongueCollision = false;
	_bool							m_bMouthCollision = false;
	_bool							m_bEggCollision = false;

private:
	_bool							m_bDead = false;

private:
	_matrix						m_matHeadCollider;

private:
	_double						m_TimeDelta;

private:
	_float	m_fJumpPower = 10.f;
	_float	m_fJumpAccel = 20.f;
	_vec3 m_vDirection;
	_vec3 m_vStartPos;

private:
	HRESULT Add_Component();
	HRESULT SetUp_ConstantTable();

public:
	static CGameObject* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject(void* pArg);
	virtual void Free();
};

END