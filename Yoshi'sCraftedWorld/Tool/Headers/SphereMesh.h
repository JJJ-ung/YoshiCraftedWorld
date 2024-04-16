#pragma once
#include "Component.h"

class CShader;
class CTransform;
class CSphereMesh : public CComponent
{
public:
	explicit CSphereMesh(LPDIRECT3DDEVICE9 pGraphicDevice);
	virtual ~CSphereMesh() = default;

public:
	virtual HRESULT Ready_Component(float fRadius = 0.3f);
	HRESULT Render_Mesh();

public:
	_int Update_Collider();
	HRESULT SetUp_ConstantTable();

public:
	LPD3DXMESH					m_pMesh;
	_float									m_fRadius = 0.3f;
	_matrix*								m_pParentMatrix;
	_matrix								m_RenderingMatrix;
	CShader*							m_pShader = nullptr;
	CTransform*						m_pTransform = nullptr;

public:
	static CSphereMesh* Create(LPDIRECT3DDEVICE9 pGraphicDev, float fRadius = 0.3f);
	void Free();
};