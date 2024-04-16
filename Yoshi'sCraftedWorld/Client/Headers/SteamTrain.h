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
class CSteamTrain final : public CGameObject
{
private:
	explicit CSteamTrain(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CSteamTrain(const CSteamTrain& rhs);
	virtual ~CSteamTrain() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int Update_GameObject(_double TimeDelta);
	virtual _int LateUpdate_GameObject(_double TimeDelta);
	virtual HRESULT Render_GameObject();

public:
	_vec3 Get_Look();
	vector<_vec3>& Get_Path0() { return m_vecPath0; }
	vector<_vec3>& Get_Path1() { return m_vecPath1; }
	vector<_vec3>& Get_CurrPath() { return m_vecCurrPath; }
	_bool&	Get_Stop() { return m_bStop; }
	_bool&	Get_Animation() { return m_bPlayAnimation; }
	_bool&	Get_IsEnd() { return m_bEnd; }

public:
	HRESULT	Change_CurrVec();

private:
	CRenderer*					m_pRendererCom = nullptr;
	CShader*						m_pShaderCom = nullptr;
	CTransform*					m_pTransformCom = nullptr;
	CFrustum*					m_pFrustumCom = nullptr;
	CMesh_Dynamic*		m_pMeshCom = nullptr;

private:
	_double					m_TimeDelta;
	_bool						m_bStop = true;
	_bool						m_bPlayAnimation = false;
	_bool						m_bEnd = false;
	_vec3						m_vLook;

private:
	vector<_vec3>		m_vecCurrPath;

	vector<_vec3>		m_vecPath0;
	vector<_vec3>		m_vecPath1;
	vector<_vec3>		m_vecPath2;

private:
	vector<_vec3>::iterator m_iterTarget;

private:
	_uint						m_iCurrIndex = 0;
	_float						m_fSpeed = 0.f;

private:
	HRESULT Add_Component();
	HRESULT SetUp_ConstantTable();

private:
	HRESULT	Add_Path0();
	HRESULT	Add_Path1();
	HRESULT	Add_Path2();

public:
	static CGameObject* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject(void* pArg);
	virtual void Free();
};
END