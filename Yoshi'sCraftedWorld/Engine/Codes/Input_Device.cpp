#include "Input_Device.h"

USING(Engine)
IMPLEMENT_SINGLETON(CInput_Device)

Engine::CInput_Device::CInput_Device(void)
{

}

bool CInput_Device::Key_Down(DWORD dwCurKey)
{
	if (!(m_dwKeyDown & dwCurKey) && (m_dwCurKey & dwCurKey))
	{
		m_dwKeyDown |= dwCurKey;
		return true;
	}

	else if ((m_dwKeyDown & dwCurKey) && !(m_dwCurKey & dwCurKey))
	{
		m_dwKeyDown ^= dwCurKey;
		return false;
	}

	return false;
}

bool CInput_Device::Key_Up(DWORD dwCurKey)
{
	if ((m_dwKeyUp & dwCurKey) && !(m_dwCurKey & dwCurKey))
	{
		m_dwKeyUp ^= dwCurKey;
		return true;
	}

	else if (!(m_dwKeyUp & dwCurKey) && (m_dwCurKey & dwCurKey))
	{
		m_dwKeyUp |= dwCurKey;
		return false;
	}

	return false;
}

bool CInput_Device::Key_Pressing(DWORD dwCurKey)
{
	if (m_dwCurKey & dwCurKey)
		return true;

	return false;
}

bool CInput_Device::Key_Combined(DWORD dwFirstKey, DWORD dwSecondKey)
{
	if (Key_Down(dwSecondKey) && Key_Pressing(dwFirstKey))
		return true;

	return false;
}

HRESULT Engine::CInput_Device::Ready_InputDev(HINSTANCE hInst, HWND hWnd)
{
	if (FAILED(DirectInput8Create(hInst,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&m_pInputSDK,
		NULL)))
		return E_FAIL;

	if (FAILED(m_pInputSDK->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, nullptr)))
		return E_FAIL;

	m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard);

	m_pKeyBoard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	m_pKeyBoard->Acquire();

	if (FAILED(m_pInputSDK->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr)))
		return E_FAIL;

	m_pMouse->SetDataFormat(&c_dfDIMouse);

	m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	m_pMouse->Acquire();

	return NOERROR;
}

void Engine::CInput_Device::Set_InputDev(void)
{
	m_dwCurKey = 0;

	m_pKeyBoard->GetDeviceState(256, m_byKeyState);
	m_pMouse->GetDeviceState(sizeof(m_tMouseState), &m_tMouseState);

	if (Get_DIKeyState(DIK_W) & 0x80)
		m_dwCurKey |= KEY_W;
	if (Get_DIKeyState(DIK_S) & 0x80)
		m_dwCurKey |= KEY_S;
	if (Get_DIKeyState(DIK_A) & 0x80)
		m_dwCurKey |= KEY_A;
	if (Get_DIKeyState(DIK_D) & 0x80)
		m_dwCurKey |= KEY_D;
	if (Get_DIKeyState(DIK_LSHIFT))
		m_dwCurKey |= KEY_SHIFT;
	if (Get_DIKeyState(DIK_LCONTROL))
		m_dwCurKey |= KEY_CTRL;
	if (Get_DIKeyState(DIK_SPACE))
		m_dwCurKey |= KEY_SPACE;

	if (Get_DIMouseState(DIM_LB) & 0x80)
		m_dwCurKey |= KEY_LBUTTON;
	if (Get_DIMouseState(DIM_RB) & 0x80)
		m_dwCurKey |= KEY_RBUTTON;

	if (Get_DIKeyState(DIK_RETURN) & 0x80)
		m_dwCurKey |= KEY_ENTER;

	if (Get_DIKeyState(DIK_F1) & 0x80)
		m_dwCurKey |= KEY_F1;
	if (Get_DIKeyState(DIK_F2) & 0x80)
		m_dwCurKey |= KEY_F2;
	if (Get_DIKeyState(DIK_F3) & 0x80)
		m_dwCurKey |= KEY_F3;

	if (Key_Down(KEY_F1))
	{
		if (g_bRenderCollider)
			g_bRenderCollider = false;
		else
			g_bRenderCollider = true;
	}

	if (Key_Down(KEY_F2))
	{
		if (g_bRenderTarget)
			g_bRenderTarget = false;
		else
			g_bRenderTarget = true;
	}
}

void Engine::CInput_Device::Free(void)
{
	Engine::Safe_Release(m_pKeyBoard);
	Engine::Safe_Release(m_pMouse);
	Engine::Safe_Release(m_pInputSDK);
}

