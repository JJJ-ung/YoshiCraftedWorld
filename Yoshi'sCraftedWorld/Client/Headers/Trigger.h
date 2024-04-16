#pragma once

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CCollider;
class CTransform;
class CRenderer;
END

BEGIN(Client)

class CTrigger final : public CGameObject
{
public:
	enum TRIGGER_TYPE
	{
		Z_TRUE, CLEAR, CLOUD_APPEAR, TRAIN, CAMERA_ON, CUTSCENE, ENDCAM, NONE
	};

private:
	explicit CTrigger(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CTrigger(const CTrigger& rhs);
	virtual ~CTrigger() = default;

public:
	CCollider* Get_Collider() { return m_pColliderCom; }

public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int Update_GameObject(_double TimeDelta);
	virtual _int LateUpdate_GameObject(_double TimeDelta);
	virtual HRESULT Render_GameObject();

private:
	CTransform*					m_pTransformCom = nullptr;
	CCollider*						m_pColliderCom = nullptr;
	CRenderer*					m_pRendererCom = nullptr;
	TRIGGER_TYPE				m_eType;
	_tchar*							m_pTag;
	_bool								m_bCollision = false;
	_bool								m_bDead = false;
	_bool								m_bCutScene = false;

private:
	HRESULT Add_Component(_tchar* pPath);

public:
	static CGameObject* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject(void* pArg);
	virtual void Free();
};

END