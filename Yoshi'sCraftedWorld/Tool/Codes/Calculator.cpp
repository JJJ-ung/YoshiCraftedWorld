#include "stdafx.h"
#include "..\Headers\Calculator.h"

CCalculator::CCalculator(LPDIRECT3DDEVICE9 pGraphicDev)
	:CComponent(pGraphicDev)
{
}

HRESULT CCalculator::Ready_Component()
{
	return NOERROR;
}

_float CCalculator::Compute_HeightOnTerrain(const _vec3 * pPos, const _vec3 * pTerrainVtxPos, const _ulong & dwCntX, const _ulong & dwCntZ)
{
	_ulong	dwIndex = _ulong(pPos->z / 1.f) * dwCntX + _ulong(pPos->x / 1.f);

	_float	fRatioX = pPos->x - pTerrainVtxPos[dwIndex + dwCntX].x;
	_float	fRatioZ = pTerrainVtxPos[dwIndex + dwCntX].z - pPos->z;

	_float	fHeight[4] = {
		pTerrainVtxPos[dwIndex + dwCntX].y,
		pTerrainVtxPos[dwIndex + dwCntX + 1].y,
		pTerrainVtxPos[dwIndex + 1].y,
		pTerrainVtxPos[dwIndex].y
	};

	// 오른쪽 위
	if (fRatioX > fRatioZ)
		return fHeight[0] + (fHeight[1] - fHeight[0]) * fRatioX + (fHeight[2] - fHeight[1]) * fRatioZ;

	// 왼쪽 아래
	else
		return fHeight[0] + (fHeight[2] - fHeight[3]) * fRatioX + (fHeight[3] - fHeight[0]) * fRatioZ;
}

_vec3 CCalculator::Picking_OnTerrain(HWND hWnd, POINT ptMouse, const CBuffer_Terrain * pTerrainBufferCom, const CTransform * pTerrainTransform)
{
	_vec3	vMouse;

	D3DVIEWPORT9		tViewPort;
	ZeroMemory(&tViewPort, sizeof(D3DVIEWPORT9));
	m_pGraphic_Device->GetViewport(&tViewPort);

	vMouse.x = (ptMouse.x / (tViewPort.Width * 0.5f)) - 1.f;
	vMouse.y = (ptMouse.y / -(tViewPort.Height * 0.5f)) + 1.f;
	vMouse.z = 0.f;

	// 투영 -> 뷰스페이스
	_matrix		matProj;
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, 0, &matProj);
	D3DXVec3TransformCoord(&vMouse, &vMouse, &matProj);

	_vec3	vRayDir, vRayPos;

	vRayPos = _vec3(0.f, 0.f, 0.f);
	vRayDir = vMouse - vRayPos;

	// 뷰스페이스 -> 월드
	_matrix		matView;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, 0, &matView);
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);

	// 월드 -> 로컬
	_matrix		matInvWorld;

	matInvWorld = pTerrainTransform->Get_WorldMatrixInv();
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matInvWorld);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matInvWorld);

	_ulong	dwVtxCntX = pTerrainBufferCom->Get_VtxCntX();
	_ulong	dwVtxCntZ = pTerrainBufferCom->Get_VtxCntZ();
	const _vec3*	pTerrainVtx = pTerrainBufferCom->Get_VtxPos();

	_ulong	dwVtxIdx[3] = { 0 };
	_float  fU, fV, fDist;

	for (_ulong i = 0; i < dwVtxCntZ - 1; ++i)
	{
		for (_ulong j = 0; j < dwVtxCntX - 1; ++j)
		{
			_ulong dwIndex = i * dwVtxCntX + j;

			// 오른쪽 위
			dwVtxIdx[0] = dwIndex + dwVtxCntX;
			dwVtxIdx[1] = dwIndex + dwVtxCntX + 1;
			dwVtxIdx[2] = dwIndex + 1;

			if (D3DXIntersectTri(&pTerrainVtx[dwVtxIdx[1]],
				&pTerrainVtx[dwVtxIdx[2]],
				&pTerrainVtx[dwVtxIdx[0]],
				&vRayPos, &vRayDir, &fU, &fV, &fDist))
			{
				return vRayPos + (fDist * vRayDir);
			}

			// 왼쪽 아래
			dwVtxIdx[0] = dwIndex + dwVtxCntX;
			dwVtxIdx[1] = dwIndex + 1;
			dwVtxIdx[2] = dwIndex;

			if (D3DXIntersectTri(&pTerrainVtx[dwVtxIdx[2]],
				&pTerrainVtx[dwVtxIdx[0]],
				&pTerrainVtx[dwVtxIdx[1]],
				&vRayPos, &vRayDir, &fU, &fV, &fDist))
			{
				return vRayPos + (fDist * vRayDir);
			}

		}
	}

	return _vec3(0.f, 0.f, 0.f);
}

CCalculator * CCalculator::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCalculator*		pInstance = new CCalculator(pGraphicDev);

	if (FAILED(pInstance->Ready_Component()))
	{
		MSG_BOX("Failed To Create CCalculator Instance");
		Safe_Delete(pInstance);
	}

	return pInstance;
}

void CCalculator::Free(void)
{
}
