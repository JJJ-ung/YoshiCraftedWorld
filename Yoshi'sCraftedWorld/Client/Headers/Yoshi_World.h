#pragma once

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CTransform;
class CMesh_Dynamic;
END

BEGIN(Client)

class CYoshi_World final : public CGameObject
{
private:
	explicit CYoshi_World(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CYoshi_World(const CYoshi_World& rhs);
	virtual ~CYoshi_World() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int Update_GameObject(_double TimeDelta);
	virtual _int LateUpdate_GameObject(_double TimeDelta);
	virtual HRESULT Render_GameObject();

private:
	CRenderer*			m_pRendererCom = nullptr;
	CMesh_Dynamic*		m_pMeshCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;

private:
	HRESULT Add_Component();
	HRESULT SetUp_ConstantTable();

public:
	static CGameObject* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject(void* pArg);
	virtual void Free();
};

END