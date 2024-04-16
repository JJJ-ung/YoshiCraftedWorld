#include "stdafx.h"
#include "Input_Device.h"

IMPLEMENT_SINGLETON(CInput_Device)

CInput_Device::CInput_Device()
	: m_dwCurKey(0), m_dwKeyUp(0), m_dwKeyDown(0)
{
}

CInput_Device::~CInput_Device()
{
	Free();
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

HRESULT CInput_Device::Ready_InputDev(HINSTANCE hInst, HWND hWnd)
{
	if (FAILED(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInputSDK, NULL)))
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

void CInput_Device::Set_InputDev(void)
{
	m_dwCurKey = 0;

	m_pKeyBoard->GetDeviceState(256, m_byKeyState);
	m_pMouse->GetDeviceState(sizeof(m_tMouseState), &m_tMouseState);

	if (!m_bLock_KeyBoard)
	{
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
		if (Get_DIKeyState(DIK_Q) & 0x80)
			m_dwCurKey |= KEY_Q;
		if (Get_DIKeyState(DIK_E) & 0x80)
			m_dwCurKey |= KEY_E;
	}

	if (Get_DIMouseState(DIM_LB) & 0x80)
		m_dwCurKey |= KEY_LBUTTON;
	if (Get_DIMouseState(DIM_RB) & 0x80)
		m_dwCurKey |= KEY_RBUTTON;
	if (Get_DIMouseState(DIM_MB) & 0x80)
		m_dwCurKey |= KEY_MBUTTON;
}

void CInput_Device::Free(void)
{
	Safe_Release(m_pKeyBoard);
	Safe_Release(m_pMouse);
	Safe_Release(m_pInputSDK);
}
