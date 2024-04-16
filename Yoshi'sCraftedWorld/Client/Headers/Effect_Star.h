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

class CEffect_Star : public CGameObject
{
public:
	typedef struct Effect
	{
		Effect() {}
		Effect(_vec3 _Pos, _vec3 _Min, _vec3 _Max, _int _index)
			: vPos(_Pos), vMin(_Min), vMax(_Max), iIndex(_index)
		{}
	public:
		_vec3 vPos;
		_vec3 vMin;
		_vec3 vMax;
		_int iIndex;
	}EFFECT_INFO;

public:
	explicit CEffect_Star(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CEffect_Star(const CEffect_Star& rhs);
	virtual ~CEffect_Star() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int Update_GameObject(_double TimeDelta);
	virtual _int LateUpdate_GameObject(_double TimeDelta);
	virtual HRESULT Render_GameObject();

private:
	CRenderer*				m_pRendererCom = nullptr;
	CBuffer_RcTex*		m_pBufferCom = nullptr;
	CShader*					m_pShaderCom = nullptr;
	CTexture*					m_pTextureCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;

private:
	_matrix						m_ParentMatrix;
	_float							m_fAlpha = 1.f;
	_float							m_fU, m_fV;
	_vec4							m_vColorMul;
	_vec3							m_vDir;
	EFFECT_INFO			m_tInfo;
	_bool							m_bEnd = false;
	_vec3							m_vAdd;

private:
	HRESULT Add_Component();
	HRESULT SetUp_ConstantTable();

	_float GetRandomFloat(float lowBound, float highBound);

	void GetRandomVector(D3DXVECTOR3 * out, D3DXVECTOR3 * min, D3DXVECTOR3 * max);

public:
	static CEffect_Star* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject(void* pArg);
	virtual void Free();
};

