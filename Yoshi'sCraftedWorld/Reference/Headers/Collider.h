#pragma once
#include "Component.h"

BEGIN(Engine)

class CShader;
class CTransform;
class ENGINE_DLL CCollider final : public CComponent
{
public:
	typedef struct tagCollider_Desc
	{
		_vec3		vScale;
		_vec3		vRotation_Angle;
		_vec3		vPosition;
		const _matrix*	pParentMatrix;
		_float		fRadius;
	}COLLIDERDESC;

public:
	typedef struct tagOBB
	{
		_vec3		vPoint[8];
		_vec3		vCenterPoint;
		_vec3		vAlignAxis[3];
		_vec3		vProjAxis[3];
	}OBBDESC;

public:
	enum COLLIDERTYPE {TYPE_BOX, TYPE_SPHERE, TYPE_END};
	enum BOXTYPE {BOX_AABB, BOX_OBB, BOX_NONE};

private:
	explicit CCollider(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CCollider(const CCollider& rhs);
	virtual ~CCollider() = default;

public:
	CTransform* Get_ColliderTransform() { return m_pTransform; }
	_vec3 Get_ColliderPos();
	_bool& Get_IsColl() { return m_isColl; }

public:
	virtual HRESULT Ready_Component_Prototype(COLLIDERTYPE eType, BOXTYPE eBoxType);
	virtual HRESULT Ready_Component(void* pArg);

public:
	HRESULT Compute_BoundingSphere(LPD3DXMESH pMesh);

public:
	_int Update_Collider();
	HRESULT Render_Collider();
	_bool Collision_AABB(CCollider* pTarget);
	_bool Collision_OBB(CCollider* pTarget);
	_bool Collision_Sphere(CCollider* pTarget);
	_bool Collision_Ray(_vec3 vRayPos, _vec3 vRayDir, _float& fDistance /*_vec3& vIntersetPos*/);

private:
	LPD3DXMESH			m_pMesh = nullptr;
	LPD3DXBUFFER		m_pAdjacency = nullptr;

private:
	CShader*			m_pShader = nullptr;
	CTransform*			m_pTransform = nullptr;
	const _matrix*		m_pParentMatrix = nullptr;
	_matrix				m_RenderingMatrix;

private:
	_bool						m_isColl = false;

private:
	BOXTYPE				m_eBoxType = BOX_NONE;
	_vec3						m_vMin, m_vMax;
	OBBDESC				m_OBBDesc;
	_float						m_fRadius;

private:
	HRESULT Ready_SphereMesh();
	HRESULT Ready_BoundingBoxMesh();
	HRESULT SetUp_ConstantTable();
	HRESULT Remove_Rotation(_matrix* pMatrix);
	HRESULT Compute_OBB();

public:
	static CCollider* Create(LPDIRECT3DDEVICE9 pGraphic_Device, COLLIDERTYPE eType, BOXTYPE eBoxType = BOX_NONE);
	virtual CComponent* Clone_Component(void* pArg);
	virtual void Free();
};

END