#include "stdafx.h"
#include "..\Headers\Mouse_Manager.h"

USING(Client)
IMPLEMENT_SINGLETON(CMouse_Manager)

CMouse_Manager::CMouse_Manager()
{
}

HRESULT CMouse_Manager::Ready_MouseManager(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	m_pGraphic_Device = pGraphic_Device;

	return NOERROR;
}

void CMouse_Manager::Update_Ray()
{
	POINT	ptMouse = { 0 };
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

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

	m_vRayPos = _vec3(0.f, 0.f, 0.f);
	m_vRayDir = vMouse - m_vRayPos;

	// 뷰스페이스 -> 월드
	_matrix		matView;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, 0, &matView);
	D3DXVec3TransformCoord(&m_vRayPos, &m_vRayPos, &matView);
	D3DXVec3TransformNormal(&m_vRayDir, &m_vRayDir, &matView);
}

void CMouse_Manager::Free(void)
{
}
