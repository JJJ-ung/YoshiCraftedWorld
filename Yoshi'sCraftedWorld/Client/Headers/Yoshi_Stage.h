#pragma once

#include "Defines.h"
#include "GameObject.h"
#include "StateMachine.h"
#include "Trigger.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CCollider;
class CRenderer;
class CTransform;
class CMeshCollision;
class CMesh_Dynamic;
END

BEGIN(Client)

class CRoad;
class CYoshi_Egg;
class CYoshi_Tongue;
class CCamera_SideScroll;
class CYoshi_Stage final : public CGameObject
{
public:
	typedef CStateMachine<CYoshi_Stage>*	STATE;
	typedef list<CYoshi_Egg*> EGGS;
	typedef list<CMeshCollision*> COLLISION;

private:
	explicit CYoshi_Stage(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CYoshi_Stage(const CYoshi_Stage& rhs);
	virtual ~CYoshi_Stage() = default;

public:
	_vec3 Get_Look();
	_vec3 Get_Position();
	STATE GetFSM() { return m_pState; }
	EGGS& Get_EggQueue() { return m_Eggs; }
	_double Get_TimeDelta() { return m_TimeDelta; }
	CYoshi_Egg* Get_CurrEgg() { return m_pCurrEgg; }
	list<CYoshi_Egg*>* Get_ReleasedEgg() { return &m_ReleasedEggs; }
	CMesh_Dynamic* Get_Mesh() { return m_pMeshCom; }
	CTransform* Get_PlayerTransform() { return m_pTransformCom; }

public:
	CYoshi_Tongue* Get_Tongue() { return m_pTongue; }
	_bool& Get_TongueCollision() { return m_bTongueColl; }
	void Set_Tongue(CYoshi_Tongue* pTongue) { m_pTongue = pTongue; }

// Monster & Egg
public:
	_float& Get_JumpAccel() { return m_fJumpAccel; }
	_float& Get_JumpPower() { return m_fJumpPower; }
	_bool& Get_KnockBack() { return m_bKnockBack; };
	_bool& Get_HeadCollision() { return m_bHeadCollision; };

// Collision
public:
	_float& Get_CollisionDistance() { return m_fCollisionDistance; }
	_bool& Get_GroundCollision() { return m_bGroundCollision; }
	_bool& Get_UpCollision() { return m_bUpCollision; }

// Trigger
public:
	CTrigger::TRIGGER_TYPE& Get_TriggerType() { return m_eTriggerColl; }
	_bool& Get_ZTrigger() { return m_bZTrigger; }
	_vec3& Get_TriggerPos() { return m_vTriggerPos; }
	_bool& Get_CutScene() { return m_bCutScene; }

public:
	CCamera_SideScroll* Get_Camera();

public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int Update_GameObject(_double TimeDelta);
	virtual _int LateUpdate_GameObject(_double TimeDelta);
	virtual HRESULT Render_GameObject();

public:
	void Reset_Jump();
	void Reset_Collider();
	void Set_MouthCollider();

public:
	HRESULT	Add_Egg();
	HRESULT	Hold_Egg();
	HRESULT	PushBack_Egg();
	HRESULT	Release_Egg();

public:
	HRESULT	Add_CollisionList(CMeshCollision* pCollision);
	HRESULT	Add_HeadCollisionList(CMeshCollision* pCollision);
	_bool			Collision_Ground();
	_bool			Collision_Up();
	_bool			Collision_Look(_bool Jumping);
	_bool			Collision_Front(_bool Jumping);
	_bool			Collision_Fall();

public:
	void				Set_OnGround();
	_bool			Check_OnGround();
	_bool			Check_Jumping();

public:
	_bool&				Get_EggTargetOn() { return m_bEggTarget; }
	CTransform*		Get_FocusTarget() { return m_pFocusTransform; }
	void						Set_Focus(CTransform* pTargetTransform, _float fDistance = -1.f, _bool bInteract = false);
	void						Reset_Focus();
	_bool					Set_NoFocus();
	_bool					Set_LookFocus();
	_bool&				Get_FocusRotate() { return m_bTargetRotate; }
	_vec3&				Get_FocusTargetPos() { return m_vFocusTarget; }
	_vec3&				Get_FocusTargetLook() { return m_vFocusLook; }
	void						Set_StartCamera();

public:
	void						Set_TrainStart();
	_bool&				Get_OnTrain(_bool bA);
	//_matrix				Get_TrainMatrix();
	void						StartTrain();
	_bool					Set_OnTrain();
	void						Set_Camera(_bool bStart);
	void						Add_GoalEffect();

public:
	void						Add_WalkEffect();

private:
	CRenderer*					m_pRendererCom = nullptr;
	CMesh_Dynamic*		m_pMeshCom = nullptr;
	CShader*						m_pShaderCom = nullptr;
	CTransform*					m_pTransformCom = nullptr;
	CCollider*						m_pColliderCom = nullptr;
	CCollider*						m_pMouthColliderCom = nullptr;

private:
	EGGS								m_Eggs;
	EGGS								m_ReleasedEggs;
	CYoshi_Egg*					m_pCurrEgg = nullptr;
	STATE								m_pState = nullptr;
	CYoshi_Tongue*			m_pTongue = nullptr;

private:
	_double							m_TimeDelta = 0.0;

private:
	_bool								m_bTongueColl = true;

private:
	_float								m_fJumpPower = 16.f;
	_float								m_fJumpAccel = 40.f;

private:
	_bool								m_bKnockBack = false;
	_bool								m_bHeadCollision = false;

private:
	_matrix							m_matMouthCollision;

private:
	COLLISION						m_CollisionList;
	COLLISION						m_HeadCollisionList;
	_vec3								m_vCollisionPoint;
	_vec3								m_vLookCollisionPoint;
	_float								m_fCollisionDistance = 0.f;
	_bool								m_bLookCollision;
	_bool								m_bGroundCollision;
	_bool								m_bUpCollision;

private:
	CTransform*					m_pFocusTransform = nullptr;
	_bool								m_bEggTarget = false;
	_bool								m_bTargetRotate = false;
	_bool								m_bFocusInteract = false;
	_float								m_fFocusDistance = -1.f;
	_vec3								m_vFocusTarget;
	_vec3								m_vFocusLook;

private:
	CTrigger::TRIGGER_TYPE		m_eTriggerColl = CTrigger::NONE;
	_bool								m_bZTrigger = false;
	_vec3								m_vTriggerPos;

private:
	_bool								m_bCutScene = false;
	_bool								m_bOnTrainA = false;
	_bool								m_bOnTrainB = false;

private:
	system_clock::time_point m_EnterTime;

private:
	LPD3DXLINE			m_pLine = nullptr;
	_vec3						m_vTest[2];
	LPD3DXLINE			m_pLine2 = nullptr;
	_vec3						m_vTest2[2];
	LPD3DXLINE			m_pLine3 = nullptr;
	_vec3						m_vTest3[2];

private:
	HRESULT Add_Component();
	HRESULT SetUp_ConstantTable();

public:
	static CGameObject* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject(void* pArg);
	virtual void Free();
};

END

