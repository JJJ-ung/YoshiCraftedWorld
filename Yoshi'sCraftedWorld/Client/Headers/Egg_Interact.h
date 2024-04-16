#pragma once

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CFrustum;
class CCollider;
class CRenderer;
class CTransform;
class CMesh_Static;
END

BEGIN(Client)

class CYoshi_Stage;
class CEgg_Interact : public CGameObject
{
public:
	typedef struct tagEggInteractInfo
	{
		tagEggInteractInfo() {}
		tagEggInteractInfo(wstring _pTag, SCENEID _eID, _vec3 _vPos, _vec3 _vScale, _vec3 _vAngle)
			:pComponentTag(_pTag), eSceneID(_eID), vPos(_vPos), vScale(_vScale), vAngle(_vAngle) {}
	public:
		wstring				pComponentTag;
		SCENEID			eSceneID;
		_vec3					vPos, vScale, vAngle;
	}EGGINTERACT_INFO;

private:
	explicit CEgg_Interact(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CEgg_Interact(const CEgg_Interact& rhs);
	virtual ~CEgg_Interact() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int Update_GameObject(_double TimeDelta);
	virtual _int LateUpdate_GameObject(_double TimeDelta);
	virtual HRESULT Render_GameObject();

private:
	CRenderer*			m_pRendererCom = nullptr;
	CMesh_Static*		m_pMeshCom = nullptr;
	CShader*				m_pShaderCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CCollider*				m_pColliderCom = nullptr;
	CFrustum*			m_pFrustumCom = nullptr;

private:
	EGGINTERACT_INFO	m_tEnvironmentInfo;
	CYoshi_Stage* m_pPlayer = nullptr;

private:
	_bool m_bObjectHit = false;
	_float	m_fJumpPower = 16.f;
	_float	m_fJumpAccel = 40.f;
	_vec3 m_vDirection;
	_matrix m_matCollider;
	_bool	m_bSound = false;

private:
	HRESULT Add_Component();
	HRESULT SetUp_ConstantTable();

public:
	static CGameObject* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject(void* pArg);
	virtual void Free();
};

END