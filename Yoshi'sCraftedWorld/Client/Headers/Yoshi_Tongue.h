#pragma once

#include "Defines.h"
#include "GameObject.h"
#include "StateMachine.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CCollider;
class CRenderer;
class CTransform;
class CMesh_Dynamic;
END

BEGIN(Client)

class CYoshi_Tongue final : public CGameObject
{
public:
	typedef CStateMachine<CYoshi_Tongue>*	STATE;

public:
	explicit CYoshi_Tongue(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CYoshi_Tongue(const CYoshi_Tongue& rhs);
	virtual ~CYoshi_Tongue() = default;

public:
	STATE GetFSM() { return m_pState; }
	CMesh_Dynamic* Get_Mesh() { return m_pMeshCom; }
	CTransform* Get_TongueTransform() { return m_pTransformCom; }
	_bool& Get_Render() { return m_bRender; }
	_double Get_TimeDelta() { return m_TimeDelta; }
	_matrix& Get_ColliderMatrix() { return m_matCollider; }

public:
	void Reset_Collider();

public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int Update_GameObject(_double TimeDelta);
	virtual _int LateUpdate_GameObject(_double TimeDelta);
	virtual HRESULT Render_GameObject();

private:
	CRenderer*					m_pRendererCom = nullptr;
	CMesh_Dynamic*			m_pMeshCom = nullptr;
	CShader*						m_pShaderCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CCollider*						m_pColliderCom = nullptr;

private:
	CTransform*			m_pParentTransformCom = nullptr;
	CMesh_Dynamic*		m_pParentMeshCom = nullptr;

private:
	STATE							m_pState = nullptr;
	_matrix						m_matCollider;

private:
	_double m_TimeDelta;
	_bool m_bRender = false;

private:
	HRESULT Add_Component();
	HRESULT SetUp_ConstantTable();
	HRESULT Compute_ParentMatrix();

public:
	static CGameObject* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject(void* pArg);
	virtual void Free();
};

END