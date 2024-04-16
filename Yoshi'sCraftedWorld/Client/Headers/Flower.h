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
class CMesh_Dynamic;
END

BEGIN(Client)

class CFlower final : public CGameObject
{
public:
	typedef struct tagFlowerInfo
	{
		tagFlowerInfo() {}
		tagFlowerInfo(_vec3 _vPos, _vec3 _vScale, _vec3 _vAngle, _uint _iIndex)
			:vPos(_vPos), vScale(_vScale), vAngle(_vAngle), iFlowerIndex(_iIndex) {}
	public:
		_vec3					vPos, vScale, vAngle;
		_uint					iFlowerIndex;
	}FLOWER_INFO;

public:
	explicit CFlower(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CFlower(const CFlower& rhs);
	virtual ~CFlower() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int Update_GameObject(_double TimeDelta);
	virtual _int LateUpdate_GameObject(_double TimeDelta);
	virtual HRESULT Render_GameObject();

private:
	CRenderer*					m_pRendererCom = nullptr;
	CMesh_Dynamic*		m_pMeshCom = nullptr;
	CShader*						m_pShaderCom = nullptr;
	CTransform*					m_pTransformCom = nullptr;
	CCollider*						m_pColliderCom = nullptr;
	CFrustum*					m_pFrustumCom = nullptr;

private:
	_double					m_TimeDelta;
	_matrix					m_matCollider;
	FLOWER_INFO		m_tInfo;
	_bool						m_bDead = false;

private:
	HRESULT Add_Component();
	HRESULT SetUp_ConstantTable();

public:
	static CGameObject* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject(void* pArg);
	virtual void Free();
};

END