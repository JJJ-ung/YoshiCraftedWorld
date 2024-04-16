#pragma once

const DWORD KEY_W = 0x00000001;	// 0000 0001
const DWORD KEY_S = 0x00000002;	// 0000 0010
const DWORD KEY_A = 0x00000004;	// 0000 0100
const DWORD KEY_D = 0x00000008;	// 0000 1000

const DWORD KEY_SPACE = 0x00000010;	// 0001 0000
const DWORD KEY_SHIFT = 0x00000020;	// 0010 0000
const DWORD KEY_CTRL = 0x00000040;

const DWORD KEY_LBUTTON = 0x00000080;	// 0100	0000
const DWORD KEY_RBUTTON = 0x00000100;	// 1000 0000
const DWORD KEY_MBUTTON = 0x00000200;	// 1000 0000

const DWORD KEY_Q = 0x00000400;
const DWORD KEY_E = 0x00000800;

class CInput_Device
{
	DECLARE_SINGLETON(CInput_Device)

public:
	enum MOUSEKEYSTATE { DIM_LB, DIM_RB, DIM_MB, DIM_END };
	enum MOUSEMOVESTATE { DIMS_X, DIMS_Y, DIMS_Z, DIMS_END };

public:
	explicit CInput_Device();
	~CInput_Device();

public:
	_byte	Get_DIKeyState(_ubyte byKeyID) { return m_byKeyState[byKeyID]; }
	_byte	Get_DIMouseState(MOUSEKEYSTATE eMouse) { return m_tMouseState.rgbButtons[eMouse]; }
	_long	Get_DIMouseMove(MOUSEMOVESTATE eMouseState) { return *(((_long*)&m_tMouseState) + eMouseState); }

public:
	bool Key_Down(DWORD dwCurKey);
	bool Key_Up(DWORD dwCurKey);
	bool Key_Pressing(DWORD dwCurKey);
	bool Key_Combined(DWORD dwFirstKey, DWORD dwSecondKey);

public:
	HRESULT Ready_InputDev(HINSTANCE hInst, HWND hWnd);
	void	Set_InputDev(void);

public:
	LPDIRECTINPUT8			m_pInputSDK = nullptr;
public:
	LPDIRECTINPUTDEVICE8	m_pKeyBoard = nullptr;
	LPDIRECTINPUTDEVICE8	m_pMouse = nullptr;

public:
	_byte					m_byKeyState[256];
	DIMOUSESTATE			m_tMouseState;

public:
	_ulong m_dwCurKey;
	_ulong m_dwKeyUp;
	_ulong m_dwKeyDown;

public:
	bool	m_bLock_KeyBoard = false;

public:
	void	Free(void);
};
