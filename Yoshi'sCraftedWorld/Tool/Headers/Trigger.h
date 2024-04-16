#pragma once
#include "GameObject.h"

class CShader;
class CTransform;
class CSphereMesh;
class CTrigger : public CGameObject
{
public:
	explicit CTrigger(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CTrigger() = default;

public:
	virtual HRESULT Ready_GameObject(_vec3 vPos, _float fRadius);

public:
	virtual _int Update_GameObject(_double TimeDelta);
	virtual _int LateUpdate_GameObject(_double TimeDelta);
	virtual HRESULT Render_GameObject();

public:
	virtual HRESULT Add_Component(_float fRadius);
	HRESULT SetUp_ConstantTable();

public:
	void Change_Radius(_float fRadius);

public:
	CString						m_strTriggerKey = L"";
	LPD3DXMESH			m_pMesh = nullptr;
	CShader*					m_pShaderCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	_float							m_fRadius = 0.f;
	D3DXCOLOR				m_Color;

public:
	static CTrigger*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _float fRadius);
};

