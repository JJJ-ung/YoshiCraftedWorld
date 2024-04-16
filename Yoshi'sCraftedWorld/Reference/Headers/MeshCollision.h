#pragma once
#include "Component.h"

BEGIN(Engine)

class CTransform;
class ENGINE_DLL CMeshCollision final : public CComponent
{
public:
	typedef struct tagCollisonDesc
	{
		tagCollisonDesc() {}
		tagCollisonDesc(CTransform* pTransform, LPD3DXMESH pCollMesh, _bool _bChange = false)
			:pTargetTransform(pTransform), pMesh(pCollMesh), bChange(_bChange) {}
	public:
		CTransform* pTargetTransform;
		LPD3DXMESH pMesh;
		_bool bChange = false;
	}COLLINFO;
public:
	explicit CMeshCollision(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CMeshCollision(const CMeshCollision& rhs);
	virtual ~CMeshCollision() = default;

public:
	_vec3 Get_CollisionPoint() { return m_vPoint; }
	BOOL Get_IsCollision() { return m_isColl; }
public:
	virtual HRESULT Ready_Component_Prototype();
	virtual HRESULT Ready_Component(void* pArg);

public:
	_int Collision_Mesh(_vec3 vRayPos, _vec3 vRayDir, _float& fDistance);
	HRESULT Convert_Local(_vec3 vMousePos, _vec3& vRayPos, _vec3& vRayDir);
	HRESULT Convert_Local(_vec3& vRayPos, _vec3& vRayDir);
	_vec3 Convert_World(_vec3 vPoint);

private:
	LPD3DXMESH m_pCollisionMesh = nullptr;
	CTransform* m_pTargetTransform = nullptr;

private:
	BOOL m_isColl = FALSE;
	DWORD m_iFaceIndex = 0;
	_float fU, fV = 0.f;
	_vec3 m_vPoint;
	_bool m_bCheckChange = false;

public:
	static CMeshCollision* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone_Component(void* pArg);
	virtual void Free();
};

END