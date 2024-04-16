#pragma once
#include "GameObject.h"

class CShader;
class CTransform;
class CDynamicMesh;
class CGimmick : public CGameObject
{
public:
	explicit CGimmick(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CGimmick() = default;

public:
	virtual HRESULT Ready_GameObject(const _tchar* pFilePath, const _tchar* pFileName, _vec3 vPos, _uint iType, _uint iOption);

public:
	virtual _int Update_GameObject(_double TimeDelta);
	virtual _int LateUpdate_GameObject(_double TimeDelta);
	virtual HRESULT Render_GameObject();

public:
	virtual HRESULT Add_Component(const _tchar* pFilePath, const _tchar* pFileName);
	HRESULT SetUp_ConstantTable();

public:
	void Set_Animation(_uint iIndex);

public:
	CDynamicMesh*			m_pMeshCom = nullptr;
	CShader*					m_pShaderCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;

public:
	CString						m_strFilePath;
	CString						m_strFileName;
	_uint							m_iType;
	_uint							m_iAnimation;
	_uint							m_iOption;

public:
	static CGimmick*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath, const _tchar* pFileName, _vec3 vPos, _uint iType, _uint iOption);
};
