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
class CTrainPlatform final : public CGameObject
{
public:
	enum PLATFORM_STATE
	{
		START, CUTSCENE, IDLE
	};

private:
	explicit CTrainPlatform(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CTrainPlatform(const CTrainPlatform& rhs);
	virtual ~CTrainPlatform() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int Update_GameObject(_double TimeDelta);
	virtual _int LateUpdate_GameObject(_double TimeDelta);
	virtual HRESULT Render_GameObject();

public:
	HRESULT Change_State(PLATFORM_STATE eState);

public:
	_int Update_Start(_double TimeDelta);
	_int Update_Idle(_double TimeDelta);
	_int Update_CutScene(_double TimeDelta);

private:
	CRenderer*					m_pRendererCom = nullptr;
	CShader*						m_pShaderCom = nullptr;
	CTransform*					m_pTransformCom = nullptr;
	CFrustum*					m_pFrustumCom = nullptr;
	CMesh_Dynamic*		m_pMeshCom = nullptr;
	LPD3DXMESH				m_pCollisionMesh = nullptr;
	CTransform*					m_pCollisionTransform = nullptr;
	CMeshCollision*			m_pCollisionCom = nullptr;

private:
	CYoshi_Stage*				m_pPlayer = nullptr;
	PLATFORM_STATE		m_eState = START;

private:
	_double					m_TimeDelta;
	_bool						m_bDead = false;
	_bool						m_bPlayAnimation = true;
	_tchar*					m_pTag;
	_uint						m_iAnimationIndex = 3;

private:
	HRESULT Add_Component();
	HRESULT SetUp_ConstantTable();

public:
	static CGameObject* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject(void* pArg);
	virtual void Free();
};
END