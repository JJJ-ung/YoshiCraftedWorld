#pragma once

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CTransform;
class CMesh_Static;
END

BEGIN(Client)

class CSky : public CGameObject
{
public:
	typedef struct tagEnvironmentInfo
	{
		tagEnvironmentInfo() {}
		tagEnvironmentInfo(wstring _pTag, SCENEID _eID, _vec3 _vPos, _vec3 _vScale, _vec3 _vAngle, _bool _bCull = true)
			:pComponentTag(_pTag), eSceneID(_eID), vPos(_vPos), vScale(_vScale), vAngle(_vAngle), bCull(_bCull) {}
	public:
		wstring				pComponentTag;
		SCENEID			eSceneID;
		_vec3					vPos, vScale, vAngle;
		_bool					bCull = false;
	}ENVIRONMENT_INFO;

private:
	explicit CSky(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CSky(const CSky& rhs);
	virtual ~CSky() = default;

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

private:
	ENVIRONMENT_INFO	m_tEnvironmentInfo;

private:
	HRESULT Add_Component();
	HRESULT SetUp_ConstantTable();

public:
	static CGameObject* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject(void* pArg);
	virtual void Free();
};

END