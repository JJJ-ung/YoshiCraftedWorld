#pragma once

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

const DWORD KEY_W = 0x00000001;	
const DWORD KEY_S = 0x00000002;	
const DWORD KEY_A = 0x00000004;	
const DWORD KEY_D = 0x00000008;	

const DWORD KEY_SPACE = 0x00000010;	
const DWORD KEY_SHIFT = 0x00000020;	
const DWORD KEY_CTRL = 0x00000040;

const DWORD KEY_LBUTTON = 0x00000080;
const DWORD KEY_RBUTTON = 0x00000100;

const DWORD KEY_ENTER = 0x00000200;

const DWORD KEY_F1 = 0x00000400;
const DWORD KEY_F2 = 0x00000800;
const DWORD KEY_F3 = 0x00001000;

class ENGINE_DLL CInput_Device : public CBase
{
	DECLARE_SINGLETON(CInput_Device)

public:
	enum MOUSEKEYSTATE { DIM_LB, DIM_RB, DIM_WHEEL, DIM_END };
	enum MOUSEMOVESTATE { DIMS_X, DIMS_Y, DIMS_WHEEL, DIMS_END };

private:
	explicit CInput_Device(void);
	virtual ~CInput_Device(void) = default;

public:
	_byte	Get_DIKeyState(_ubyte byKeyID) { return m_byKeyState[byKeyID]; }
	_byte	Get_DIMouseState(MOUSEKEYSTATE eMouse) { return m_tMouseState.rgbButtons[eMouse]; }
	_long	Get_DIMouseMove(MOUSEMOVESTATE eMouseState)	{	return *(((_long*)&m_tMouseState) + eMouseState);	}

public:
	bool Key_Down(DWORD dwCurKey);
	bool Key_Up(DWORD dwCurKey);
	bool Key_Pressing(DWORD dwCurKey);
	bool Key_Combined(DWORD dwFirstKey, DWORD dwSecondKey);

public:
	HRESULT Ready_InputDev(HINSTANCE hInst, HWND hWnd);
	void	Set_InputDev(void);

private:
	LPDIRECTINPUT8			m_pInputSDK = nullptr;

private:
	LPDIRECTINPUTDEVICE8	m_pKeyBoard = nullptr;
	LPDIRECTINPUTDEVICE8	m_pMouse = nullptr;	

private:
	_byte					m_byKeyState[256];
	DIMOUSESTATE			m_tMouseState;

public:
	_ulong m_dwCurKey;
	_ulong m_dwKeyUp;
	_ulong m_dwKeyDown;

public:
	virtual void	Free(void);
};

END
