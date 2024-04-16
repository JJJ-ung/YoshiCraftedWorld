#pragma once
#include "GameObject.h"

class CShader;
class CTransform;
class CStaticMesh;
class CMapObject : public CGameObject
{
public:
	explicit CMapObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CMapObject() = default;

public:
	virtual HRESULT Ready_GameObject(const _tchar* pFilePath, _vec3 vPos, _uint eType);

public:
	virtual _int Update_GameObject(_double TimeDelta);
	virtual _int LateUpdate_GameObject(_double TimeDelta);
	virtual HRESULT Render_GameObject();

public:
	virtual HRESULT Add_Component(const _tchar* pFilePath);
	HRESULT SetUp_ConstantTable();

public:
	CStaticMesh*			m_pStaticMeshCom = nullptr;
	CShader*					m_pShaderCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;

public:
	CString						m_strFilePath;
	_uint							m_eCollisionType;

public:
	static CMapObject*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath, _vec3 vPos, _uint eType);
};
