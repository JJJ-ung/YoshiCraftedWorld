#pragma once
#include "GameObject.h"
#include "Transform.h"

class CShader;
class CEffectMenu;
class CDynamicMesh;
class CUnit : public CGameObject
{
public:
	explicit CUnit(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CUnit() = default;

public:
	virtual HRESULT Ready_GameObject(const _tchar* pFilePath, const _tchar* pFileName, _vec3 vPos);

public:
	virtual _int Update_GameObject(_double TimeDelta);
	virtual _int LateUpdate_GameObject(_double TimeDelta);
	virtual HRESULT Render_GameObject();

public:
	virtual HRESULT Add_Component(const _tchar* pFilePath, const _tchar* pFileName);
	HRESULT SetUp_ConstantTable();

public:
	CEffectMenu*				m_pEffectMenu = nullptr;

public:
	CDynamicMesh*			m_pMeshCom = nullptr;
	CShader*					m_pShaderCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;

public:
	CString						m_strFilePath;
	CString						m_strFileName;

public:
	list<CString>			m_BoneList;

public:
	static CUnit*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath, const _tchar* pFileName, _vec3 vPos);
};

