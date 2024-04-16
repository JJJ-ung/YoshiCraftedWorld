#pragma once
#include "Component.h"
#include "Buffer_Terrain.h"
#include "Transform.h"

class CCalculator : public CComponent
{
public:
	explicit CCalculator(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCalculator() = default;

public:
	HRESULT Ready_Component();

public:
	_float	Compute_HeightOnTerrain(const _vec3* pPos, const _vec3* pTerrainVtxPos, const _ulong& dwCntX, const _ulong& dwCntZ);
	_vec3	Picking_OnTerrain(HWND hWnd, POINT ptMouse, const CBuffer_Terrain* pTerrainBufferCom, const CTransform* pTerrainTransform);

public:
	static CCalculator* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void Free();
};