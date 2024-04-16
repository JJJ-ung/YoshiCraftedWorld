#pragma once

namespace Engine
{
	typedef struct tagMeshTexture
	{
		enum TYPE { TYPE_DIFFUSE, TYPE_NORMAL, TYPE_SPECULAR, TYPE_EMISSIVE, TYPE_END };

		LPDIRECT3DTEXTURE9		pTextures[TYPE_END];
	}MESHTEXTURE;

	typedef struct tagD3DXMeshContainer_Derived : public D3DXMESHCONTAINER
	{
		LPD3DXMESH			pOriginalMesh;
		MESHTEXTURE*		pMeshTexture;
		D3DXMATRIX*			pOffsetMatrices;
		D3DXMATRIX**		ppCombinedTransformationMatrices;
		D3DXMATRIX*			pRenderMatrices;
		unsigned long		dwNumBones;

	}D3DXMESHCONTAINER_DERIVED, *LPD3DXMESHCONTAINER_DERIVED;

	typedef struct tagD3DXFrame_Derived : public D3DXFRAME
	{
		// 나의 TransformationMatrix * 부모뼈의 CombinedTransformationMatrix
		D3DXMATRIX		CombinedTransformationMatrix;
	}D3DXFRAME_DERIVED, *LPD3DXFRAME_DERIVED;

	typedef struct tagVertex_ViewPort_Texture
	{
		D3DXVECTOR4		vPosition;
		D3DXVECTOR2		vTexUV;
	}VTXVIEWPORTTEX;


	typedef struct tagVertex_Cube_Texture
	{
		D3DXVECTOR3		vPosition;
		D3DXVECTOR3		vTexUV;
	}VTXCUBE;

	typedef struct tagVertex_Normal_Texture
	{
		D3DXVECTOR3		vPosition;
		D3DXVECTOR3		vNormal;
		D3DXVECTOR2		vTexUV;
	}VTXNORTEX;

	typedef struct tagVertex_Texture
	{
		D3DXVECTOR3		vPosition;
		D3DXVECTOR2		vTexUV;
	}VTXTEX;

	typedef struct tagIndex16
	{
		unsigned short	_0, _1, _2;
	}INDEX16;

	typedef struct tagIndex32
	{
		unsigned long	_0, _1, _2;
	}INDEX32;
}