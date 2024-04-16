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
class CTransformFountain final : public CGameObject
{
private:
	explicit CTransformFountain(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CTransformFountain(const CTransformFountain& rhs);
	virtual ~CTransformFountain() = default;

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
	LPD3DXMESH				m_pCollisionMeshA = nullptr;
	LPD3DXMESH				m_pCollisionMeshB = nullptr;
	CTransform*					m_pMeshATransformCom = nullptr;
	CTransform*					m_pMeshBTransformCom = nullptr;
	CMeshCollision*			m_pMeshA_CollisionCom = nullptr;
	CMeshCollision*			m_pMeshB_CollisionCom = nullptr;

private:
	_double					m_TimeDelta;
	_bool						m_bOpen = false;
	_bool						m_bPass = false;
	_tchar*					m_pTag;
	CYoshi_Stage*		m_pPlayer = nullptr;

private:
	HRESULT Add_Component();
	HRESULT SetUp_ConstantTable();

public:
	static CGameObject* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject(void* pArg);
	virtual void Free();
};
END