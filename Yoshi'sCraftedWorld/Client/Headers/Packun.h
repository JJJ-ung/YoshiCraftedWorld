#pragma once

#include "Defines.h"
#include "GameObject.h"
#include "StateMachine.h"
#include "MonsterInfo.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CCollider;
class CRenderer;
class CTransform;
class CMesh_Dynamic;
END

class CPackun : public CGameObject
{
public:
	typedef CStateMachine<CPackun>*	STATE;

public:
	explicit CPackun(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CPackun(const CPackun& rhs);
	virtual ~CPackun() = default;

public:
	STATE GetFSM() { return m_pState; }
	CMesh_Dynamic* Get_Mesh() { return m_pMeshCom; }
	CTransform* Get_PackunTransform() { return m_pTransformCom; }

public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int Update_GameObject(_double TimeDelta);
	virtual _int LateUpdate_GameObject(_double TimeDelta);
	virtual HRESULT Render_GameObject();

private:
	CRenderer*					m_pRendererCom = nullptr;
	CMesh_Dynamic*		m_pMeshCom = nullptr;
	CShader*						m_pShaderCom = nullptr;
	CTransform*					m_pTransformCom = nullptr;
	CCollider*						m_pColliderCom = nullptr;
	CCollider*						m_pThreatColliderCom = nullptr;

private:
	STATE								m_pState = nullptr;
	MONSTER_INFO			m_tInfo;

private:
	_double							m_TimeDelta;

private:
	HRESULT Add_Component();
	HRESULT SetUp_ConstantTable();

public:
	static CGameObject* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject(void* pArg);
	virtual void Free();
};

