#pragma once

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CFrustum;
class CRenderer;
class CTransform;
class CMesh_Static;
class CMeshCollision;
class CMesh_Dynamic;
END

BEGIN(Client)
class CYoshi_Stage;
class CStairs final : public CGameObject
{
private:
	explicit CStairs(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CStairs(const CStairs& rhs);
	virtual ~CStairs() = default;

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
	CMesh_Static*				m_pCollisionMesh = nullptr;
	CTransform*					m_pCollisionTransform = nullptr;
	CMeshCollision*			m_pCollisionCom = nullptr;

private:
	CYoshi_Stage*				m_pPlayer = nullptr;

private:
	_double					m_TimeDelta;
	_bool						m_bDead = false;
	_bool						m_bPlayAnimation = true;
	_tchar*					m_pTag;

private:
	HRESULT Add_Component();
	HRESULT SetUp_ConstantTable();

public:
	static CGameObject* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject(void* pArg);
	virtual void Free();
};
END