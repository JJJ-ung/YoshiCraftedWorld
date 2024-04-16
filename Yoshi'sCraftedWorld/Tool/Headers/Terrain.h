#pragma once
#include "GameObject.h"
#include "Buffer_Terrain.h"
#include "Texture.h"
#include "Shader.h"
#include "Transform.h"

class CTerrain :	public CGameObject
{
public:
	explicit CTerrain(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CTerrain() = default;

public:
	virtual HRESULT Ready_GameObject();

public:
	virtual _int Update_GameObject(_double TimeDelta);
	virtual _int LateUpdate_GameObject(_double TimeDelta);
	virtual HRESULT Render_GameObject();

public:
	virtual HRESULT Add_Component();

public:
	HRESULT Change_TerrainSize(const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv);
	HRESULT Change_TerrainColor(float fA, float fR, float fG, float fB);
	HRESULT SetUp_ConstantTable();

public:
	CBuffer_Terrain*		m_pBufferCom = nullptr;
	CShader*						m_pShaderCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;

public:
	_vec4								m_vTerrainColor;

public:
	static CTerrain*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
};