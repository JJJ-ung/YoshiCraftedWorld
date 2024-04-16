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
class CMesh_Dynamic;
END

BEGIN(Client)
class CYoshi_Stage;
class CGoalTape final : public CGameObject
{
private:
	explicit CGoalTape(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CGoalTape(const CGoalTape& rhs);
	virtual ~CGoalTape() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int Update_GameObject(_double TimeDelta);
	virtual _int LateUpdate_GameObject(_double TimeDelta);
	virtual HRESULT Render_GameObject();

private:
	CRenderer*					m_pRendererCom = nullptr;
	CShader*						m_pShaderCom = nullptr;
	CTransform*					m_pTransformCom = nullptr;
	CFrustum*					m_pFrustumCom = nullptr;
	CMesh_Dynamic*		m_pMeshCom = nullptr;
	CCollider*						m_pColliderCom = nullptr;

private:
	_double							m_TimeDelta;
	_bool								m_bPass = false;
	_bool								m_bEnd = false;
	_bool								m_bSound = false;

private:
	HRESULT Add_Component();
	HRESULT SetUp_ConstantTable();

public:
	static CGameObject* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject(void* pArg);
	virtual void Free();
};
END