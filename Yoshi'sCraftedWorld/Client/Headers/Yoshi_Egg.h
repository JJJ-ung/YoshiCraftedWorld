#pragma once
#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CCollider;
class CRenderer;
class CTransform;
class CMesh_Static;
class CMesh_Dynamic;
END

BEGIN(Client)

class CYoshi_Egg : public CGameObject
{
public:
	enum EGG_MODE { IDLE, MOVE, THROW, RELEASE, FOCUS, RELEASE_DEAD, MODE_END };

public:
	typedef struct tagEggDesc
	{
		tagEggDesc() {}
		tagEggDesc(CTransform* _pPlayerTransform, CTransform* _pTargetTransform)
			:pPlayerTransform(_pPlayerTransform), pTargetTransform(_pTargetTransform) {}
	public:
		CTransform* pPlayerTransform;
		CTransform* pTargetTransform;
	}EGGINFO;

public:
	explicit CYoshi_Egg(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CYoshi_Egg(const CYoshi_Egg& rhs);
	virtual ~CYoshi_Egg() = default;

public:
	CTransform* Get_EggTransform() { return m_pTransformCom; }
	EGGINFO& Get_EggInfo() { return m_tEggInfo; }
	EGG_MODE& Get_CurrMode() { return m_eCurrMode; }
	_double& Get_TimeDelta() { return m_TimeDelta; }
	_float& Get_Progress() { return m_fProgress; }
	CCollider* Get_Collider() { return m_pColliderCom; }
	_vec3& Get_ThrowDirection() { return m_vThrowDirection; }

public:
	HRESULT Change_Mode(EGG_MODE eMode, _vec3 vTarget = _vec3(0.f, 0.f, 0.f));

public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(EGGINFO tEggInfo);
	virtual _int Update_GameObject(_double TimeDelta);
	virtual _int LateUpdate_GameObject(_double TimeDelta);
	virtual HRESULT Render_GameObject();

public:
	_int Update_IdleMode(_double TimeDelta);
	_int Update_MoveMode(_double TimeDelta);
	_int Update_ThrowMode(_double TimeDelta);
	_int Update_ReleaseMode(_double TimeDelta);

public:
	HRESULT Convert_World();

private:
	CRenderer*			m_pRendererCom = nullptr;
	CMesh_Static*		m_pMeshCom = nullptr;
	CShader*				m_pShaderCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CCollider*				m_pColliderCom = nullptr;

private:
	CTransform*					m_pParentTransformCom = nullptr;
	CMesh_Dynamic*		m_pParentMeshCom = nullptr;

private:
	EGGINFO		m_tEggInfo;
	EGG_MODE	m_eCurrMode = IDLE;

private:
	_double m_TimeDelta = 0.0;
	_float		m_fProgress = 0.f;
	_matrix	m_matCollider;
	_vec3		m_vThrowDirection;
	_vec3		m_vTarget;

private:
	HRESULT Add_Component();
	HRESULT SetUp_ConstantTable();
	HRESULT Compute_ParentMatrix();

public:
	static CYoshi_Egg* Create(LPDIRECT3DDEVICE9 pGraphic_Device, EGGINFO tEggInfo);
	virtual CGameObject* Clone_GameObject(void* pArg);
	virtual void Free();
};

END