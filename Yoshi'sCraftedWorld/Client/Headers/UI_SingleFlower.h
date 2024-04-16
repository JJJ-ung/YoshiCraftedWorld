#pragma once

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CTransform;
class CBuffer_RcTex;
END

BEGIN(Client)
class CUI_SingleFlower final : public CGameObject
{
public:
	typedef struct tagFlowerInfo
	{
		_uint m_iIndex;
	}FLOWER_INFO;

private:
	explicit CUI_SingleFlower(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CUI_SingleFlower(const CUI_SingleFlower& rhs);
	virtual ~CUI_SingleFlower() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int Update_GameObject(_double TimeDelta);
	virtual _int LateUpdate_GameObject(_double TimeDelta);
	virtual HRESULT Render_GameObject();

public:
	HRESULT Add_Flower(_uint iIndex);
	_bool& Get_Show() { return m_bShow; }

private:
	CRenderer*		m_pRendererCom = nullptr;
	CBuffer_RcTex*	m_pBufferCom = nullptr;
	CShader*		m_pShaderCom = nullptr;
	CTexture*		m_pTextureCom = nullptr;
	CTransform*		m_pTransformCom = nullptr;

private:
	_float			m_fX, m_fY, m_fSizeX, m_fSizeY;
	_vec2			m_vTargetPos;
	_bool			m_bMove = true;
	_bool			m_bShow = false;
	_bool			m_bNew = true;

private:
	FLOWER_INFO m_tInfo;

private:
	system_clock::time_point m_tEnterTime;

private:
	HRESULT Add_Component();
	HRESULT SetUp_ConstantTable();

public:
	static CUI_SingleFlower* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject(void* pArg);
	virtual void Free();
};
END