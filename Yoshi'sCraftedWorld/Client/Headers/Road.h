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
END

BEGIN(Client)

class CYoshi_Stage;
class CRoad : public CGameObject
{
public:
	typedef struct tagRoadInfo
	{
		tagRoadInfo() {}
		tagRoadInfo(wstring _pTag, SCENEID _eID, _vec3 _vPos, _vec3 _vScale, _vec3 _vAngle)
			:pComponentTag(_pTag), eSceneID(_eID), vPos(_vPos), vScale(_vScale), vAngle(_vAngle) {}
	public:
		wstring				pComponentTag;
		SCENEID			eSceneID;
		_vec3					vPos, vScale, vAngle;
	}ROAD_INFO;

public:
	explicit CRoad(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CRoad(const CRoad& rhs);
	virtual ~CRoad() = default;

public:
	CMeshCollision* Get_Collision() { return m_pCollisionCom; }

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
	CMeshCollision*	m_pCollisionCom = nullptr;
	CFrustum*			m_pFrustumCom = nullptr;

private:
	ROAD_INFO m_tRoadInfo;
	CYoshi_Stage* m_pPlayer = nullptr;

private:
	HRESULT Add_Component();
	HRESULT SetUp_ConstantTable();

public:
	static CGameObject* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject(void* pArg);
	virtual void Free();
};

END