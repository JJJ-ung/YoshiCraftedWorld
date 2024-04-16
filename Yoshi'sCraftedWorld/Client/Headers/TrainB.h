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
class CMeshCollision;
class CMesh_Dynamic;
END

BEGIN(Client)
class CSteamTrain;
class CYoshi_Stage;
class CTrainB final : public CGameObject
{
private:
	explicit CTrainB(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CTrainB(const CTrainB& rhs);
	virtual ~CTrainB() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int Update_GameObject(_double TimeDelta);
	virtual _int LateUpdate_GameObject(_double TimeDelta);
	virtual HRESULT Render_GameObject();

public:
	HRESULT Change_CurrVec(vector<_vec3>& vPath);
	_bool& Get_OnTrain() { return m_bOnTrain; }

private:
	CRenderer*					m_pRendererCom = nullptr;
	CShader*						m_pShaderCom = nullptr;
	CTransform*					m_pTransformCom = nullptr;
	CFrustum*					m_pFrustumCom = nullptr;
	CMesh_Dynamic*		m_pMeshCom = nullptr;
	CCollider*						m_pColliderCom = nullptr;

private:
	_double					m_TimeDelta;
	_vec3						m_vLook;
	_bool						m_bOnTrain = false;
	CSteamTrain*		m_pTrain = nullptr;
	CYoshi_Stage*		m_pPlayer = nullptr;

private:
	_float						m_fSpeed = 0.f;

private:
	vector<_vec3>		m_vecCurrPath;

private:
	vector<_vec3>::iterator m_iterTarget;

private:
	HRESULT Add_Component();
	HRESULT SetUp_ConstantTable();

public:
	static CGameObject* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject(void* pArg);
	virtual void Free();
};
END