#include "stdafx.h"
#include "..\Headers\Yoshi_Stage_State.h"
#include "Yoshi_Tongue_State.h"
#include "Mesh_Dynamic.h"
#include "Input_Device.h"
#include "Transform.h"
#include "Yoshi_Egg.h"
#include "Trigger.h"
#include "Camera_SideScroll.h"
#include "Management.h"

USING(Client)

// Game Start //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_STATE(CStage_IdleBase)
ENTER_STATE(CStage_IdleBase, CYoshi_Stage)
{
	owner->Get_Mesh()->Set_AnimationSpeed(1.f);
	owner->Get_Mesh()->SetUp_Animation(41);
}
UPDATE_STATE(CStage_IdleBase, CYoshi_Stage)
{
	if (owner->Check_OnGround())
		owner->Set_OnGround();

	CInput_Device* pInputDevice = CInput_Device::GetInstance();
	Safe_AddRef(pInputDevice);

	if (pInputDevice->Key_Pressing(KEY_ENTER))
		CHANGE_STATE(CStage_GameStart);

	Safe_Release(pInputDevice);
}
EXIT_STATE(CStage_IdleBase, CYoshi_Stage)
{
}

//

IMPLEMENT_STATE(CStage_GameStart)
ENTER_STATE(CStage_GameStart, CYoshi_Stage)
{
	owner->Get_Mesh()->Set_AnimationSpeed(1.5f);
	owner->Get_Mesh()->SetUp_Animation(3);
	CSoundMgr::GetInstance()->PlaySound(L"Start.wav", CSoundMgr::EFFECT);
}
UPDATE_STATE(CStage_GameStart, CYoshi_Stage)
{
	if (owner->Check_OnGround())
		owner->Set_OnGround();

	_tchar m_szData01[255];
	GetPrivateProfileString(_T("Stage"), _T("GameStart"), NULL, m_szData01, 255, _T("../Bin/Data/Information/Yoshi_Animation.ini"));
	_double GameStart = _tstof(m_szData01);

	if (owner->Get_Mesh()->Finish_Animation(GameStart))
		CHANGE_STATE(CStage_Idle);
}
EXIT_STATE(CStage_GameStart, CYoshi_Stage)
{
	owner->Set_StartCamera();
	owner->Set_TrainStart();
	CSoundMgr::GetInstance()->PlayBGM(L"BGM_MAIN_0215.wav");
}

// Idle //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_STATE(CStage_Idle)
ENTER_STATE(CStage_Idle, CYoshi_Stage)
{
	owner->Get_Mesh()->Set_AnimationSpeed(1.f);
	owner->Get_Mesh()->SetUp_Animation(40);

	m_EnterTime = system_clock::now();
}
UPDATE_STATE(CStage_Idle, CYoshi_Stage)
{
	if (owner->Check_OnGround())
		owner->Set_OnGround();

	CInput_Device* pInputDevice = CInput_Device::GetInstance();
	Safe_AddRef(pInputDevice);

	system_clock::duration tIdle = system_clock::now() - m_EnterTime;
	if (duration_cast<seconds>(tIdle) > (seconds)3)
		CHANGE_STATE(CStage_Idle_Play);

	if (!owner->Get_ZTrigger())
	{
		if (owner->Get_Look().x >= 0.f)
			CHANGE_STATE(CStage_RotateRight);
		else
			CHANGE_STATE(CStage_RotateLeft);
	}

	// ¾Æ·¡
	if (pInputDevice->Key_Pressing(KEY_S))
	{
		if (owner->Get_ZTrigger())
		{
			if (fabs(owner->Get_Look().x) <= 0.001)
			{
				if (owner->Get_Look().z >= 0.f)
					CHANGE_STATE(CStage_RotateDown);
				else
					CHANGE_STATE(CStage_WalkIn);
			}
			else
				CHANGE_STATE(CStage_RotateDown);
		}
		else
			CHANGE_STATE(CStage_Sit);
	}
	// ¿À¸¥ÂÊ
	else if (pInputDevice->Key_Pressing(KEY_D))
	{
		if (fabs(owner->Get_Look().z) <= 0.001)
		{
			if (owner->Get_Look().x >= 0.f)
				CHANGE_STATE(CStage_WalkIn);
			else
				CHANGE_STATE(CStage_RotateRight);
		}
		else
			CHANGE_STATE(CStage_RotateRight);
	}
	// ¿ÞÂÊ
	else if (pInputDevice->Key_Pressing(KEY_A))
	{
		if (fabs(owner->Get_Look().z) <= 0.001)
		{
			if (owner->Get_Look().x < 0.f)
				CHANGE_STATE(CStage_WalkIn);
			else
				CHANGE_STATE(CStage_RotateLeft);
		}
		else
			CHANGE_STATE(CStage_RotateLeft);
	}
	// À§
	else if (pInputDevice->Key_Pressing(KEY_W))
	{
		if (owner->Get_ZTrigger())
		{
			if (fabs(owner->Get_Look().x) <= 0.001)
			{
				if (owner->Get_Look().z <= 0.f)
					CHANGE_STATE(CStage_RotateUp);
				else
					CHANGE_STATE(CStage_WalkIn);
			}
			else
				CHANGE_STATE(CStage_RotateUp);
		}
	}

	if (pInputDevice->Key_Down(KEY_LBUTTON))
		CHANGE_STATE(CStage_TongueAttackIn);
	if (pInputDevice->Key_Down(KEY_RBUTTON))
	{
		if (owner->Get_EggQueue().size() > 0)
			CHANGE_STATE(CStage_ThrowIdle);
		else
			CHANGE_STATE(CStage_Throw);
	}
	if (pInputDevice->Key_Pressing(KEY_SPACE))
		CHANGE_STATE(CStage_JumpStart);
	if (owner->Get_KnockBack())
		CHANGE_STATE(CStage_KnockBack);

	Safe_Release(pInputDevice);
}
EXIT_STATE(CStage_Idle, CYoshi_Stage)
{
}

//

IMPLEMENT_STATE(CStage_Idle_Play)
ENTER_STATE(CStage_Idle_Play, CYoshi_Stage)
{
	owner->Get_Mesh()->Set_AnimationSpeed(1.f);
	owner->Get_Mesh()->SetUp_Animation(8 + (rand() % 2));
}
UPDATE_STATE(CStage_Idle_Play, CYoshi_Stage)
{
	if (owner->Check_OnGround())
		owner->Set_OnGround();

	CInput_Device* pInputDevice = CInput_Device::GetInstance();
	Safe_AddRef(pInputDevice);

	_tchar m_szData01[255];
	GetPrivateProfileString(_T("Stage"), _T("IdlePlay"), NULL, m_szData01, 255, _T("../Bin/Data/Information/Yoshi_Animation.ini"));
	_double IdlePlay = _tstof(m_szData01);

	CSoundMgr::GetInstance()->PlaySound(L"Yoshi_Utau01.wav", CSoundMgr::PLAYER2);

	if (owner->Get_Mesh()->Finish_Animation(IdlePlay))
		CHANGE_STATE(CStage_Idle);

	// ¾Æ·¡
	if (pInputDevice->Key_Pressing(KEY_S))
	{
		if (owner->Get_TriggerType() == CTrigger::Z_TRUE)
		{
			if (fabs(owner->Get_Look().x) <= 0.001)
			{
				if (owner->Get_Look().z >= 0.f)
					CHANGE_STATE(CStage_RotateDown);
				else
					CHANGE_STATE(CStage_WalkIn);
			}
			else
				CHANGE_STATE(CStage_RotateDown);
		}
		else
			CHANGE_STATE(CStage_Sit);
	}
	// ¿À¸¥ÂÊ
	else if (pInputDevice->Key_Pressing(KEY_D))
	{
		if (fabs(owner->Get_Look().z) <= 0.001)
		{
			if (owner->Get_Look().x >= 0.f)
				CHANGE_STATE(CStage_WalkIn);
			else
				CHANGE_STATE(CStage_RotateRight);
		}
		else
			CHANGE_STATE(CStage_RotateRight);
	}
	// ¿ÞÂÊ
	else if (pInputDevice->Key_Pressing(KEY_A))
	{
		if (fabs(owner->Get_Look().z) <= 0.01)
		{
			if (owner->Get_Look().x < 0.f)
				CHANGE_STATE(CStage_WalkIn);
			else
				CHANGE_STATE(CStage_RotateLeft);
		}
		else
			CHANGE_STATE(CStage_RotateLeft);
	}
	// À§
	else if (pInputDevice->Key_Pressing(KEY_W))
	{
		if (owner->Get_TriggerType() == CTrigger::Z_TRUE)
		{
			if (fabs(owner->Get_Look().x) <= 0.001)
			{
				if (owner->Get_Look().z <= 0.f)
					CHANGE_STATE(CStage_RotateUp);
				else
					CHANGE_STATE(CStage_WalkIn);
			}
			else
				CHANGE_STATE(CStage_RotateUp);
		}
	}

	if (pInputDevice->Key_Down(KEY_LBUTTON))
		CHANGE_STATE(CStage_TongueAttackIn);
	if (pInputDevice->Key_Down(KEY_RBUTTON))
	{
		if (owner->Get_EggQueue().size() > 0)
			CHANGE_STATE(CStage_ThrowIdle);
		else
			CHANGE_STATE(CStage_Throw);
	}
	if (pInputDevice->Key_Pressing(KEY_SPACE))
		CHANGE_STATE(CStage_JumpStart);
	if (owner->Get_KnockBack())
		CHANGE_STATE(CStage_KnockBack);

	Safe_Release(pInputDevice);
}
EXIT_STATE(CStage_Idle_Play, CYoshi_Stage)
{
}

// Rotation //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_STATE(CStage_RotateRight)
ENTER_STATE(CStage_RotateRight, CYoshi_Stage)
{
}
UPDATE_STATE(CStage_RotateRight, CYoshi_Stage)
{
	_vec3 vLook = owner->Get_Look();
	_float fStart = 0.f, fEnd = 90.f;

	// ¾ç¿·º½
	if (fabs(vLook.z) <= 0.001)
	{
		// ¿À¸¥ÂÊ º½
		if (vLook.x >= 0.f)
			fStart = 90.f;
		// ¿ÞÂÊ º½
		else
			fStart = -90.f;
	}
	// À§¾Æ·¡º½
	else
	{
		// À§ÂÊ º½
		if (vLook.z >= 0.f)
			fStart = 0.f;
		else
			fStart = 180.f;
	}

	if (owner->Get_PlayerTransform()->Rotation_Degree(owner->Get_TimeDelta(), 0.1, D3DXToRadian(fStart), D3DXToRadian(fEnd)))
		CHANGE_STATE(CStage_Idle);
}
EXIT_STATE(CStage_RotateRight, CYoshi_Stage)
{
}

IMPLEMENT_STATE(CStage_RotateLeft)
ENTER_STATE(CStage_RotateLeft, CYoshi_Stage)
{
}
UPDATE_STATE(CStage_RotateLeft, CYoshi_Stage)
{
	_vec3 vLook = owner->Get_Look();
	_float fStart = 0.f, fEnd = -90.f;

	// ¾ç¿·º½
	if (fabs(vLook.z) <= 0.001)
	{
		// ¿À¸¥ÂÊ º½
		if (vLook.x >= 0.f)
			fStart = 90.f;
		// ¿ÞÂÊ º½
		else
			fStart = -90.f;
	}
	// À§¾Æ·¡º½
	else
	{
		// À§ÂÊ º½
		if (vLook.z >= 0.f)
			fStart = 0.f;
		else
			fStart = 180.f;
	}

	if (owner->Get_PlayerTransform()->Rotation_Degree(owner->Get_TimeDelta(), 0.1, D3DXToRadian(fStart), D3DXToRadian(fEnd)))
		CHANGE_STATE(CStage_Idle);
}
EXIT_STATE(CStage_RotateLeft, CYoshi_Stage)
{
}

IMPLEMENT_STATE(CStage_RotateUp)
ENTER_STATE(CStage_RotateUp, CYoshi_Stage)
{
}
UPDATE_STATE(CStage_RotateUp, CYoshi_Stage)
{
	_vec3 vLook = owner->Get_Look();
	_float fStart = 0.f, fEnd = 0.f;

	// ¾ç¿·º½
	if (fabs(vLook.z) <= 0.001)
	{
		// ¿À¸¥ÂÊ º½
		if (vLook.x >= 0.f)
			fStart = 90.f;
		// ¿ÞÂÊ º½
		else
			fStart = -90.f;
	}
	// À§¾Æ·¡º½
	else
	{
		// À§ÂÊ º½
		if (vLook.z >= 0.f)
			fStart = 0.f;
		else
			fStart = 180.f;
	}

	if (owner->Get_PlayerTransform()->Rotation_Degree(owner->Get_TimeDelta(), 0.1, D3DXToRadian(fStart), D3DXToRadian(fEnd)))
		CHANGE_STATE(CStage_Idle);
}
EXIT_STATE(CStage_RotateUp, CYoshi_Stage)
{
}

IMPLEMENT_STATE(CStage_RotateDown)
ENTER_STATE(CStage_RotateDown, CYoshi_Stage)
{
}
UPDATE_STATE(CStage_RotateDown, CYoshi_Stage)
{
	_vec3 vLook = owner->Get_Look();
	_float fStart = 0.f, fEnd = 180.f;

	// ¾ç¿·º½
	if (fabs(vLook.z) <= 0.001)
	{
		// ¿À¸¥ÂÊ º½
		if (vLook.x >= 0.f)
			fStart = 90.f;
		// ¿ÞÂÊ º½
		else
			fStart = -90.f;
	}
	// À§¾Æ·¡º½
	else
	{
		// À§ÂÊ º½
		if (vLook.z >= 0.f)
			fStart = 0.f;
		else
			fStart = 180.f;
	}

	if (owner->Get_PlayerTransform()->Rotation_Degree(owner->Get_TimeDelta(), 0.1, D3DXToRadian(fStart), D3DXToRadian(fEnd)))
		CHANGE_STATE(CStage_Idle);
}
EXIT_STATE(CStage_RotateDown, CYoshi_Stage)
{
}

// Walk / Run //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_STATE(CStage_WalkIn)
ENTER_STATE(CStage_WalkIn, CYoshi_Stage)
{
	owner->Get_PlayerTransform()->Set_SpeedPerSec(4.5f);
	owner->Get_Mesh()->Set_AnimationSpeed(1.5f);
	owner->Get_Mesh()->SetUp_Animation(39);
}
UPDATE_STATE(CStage_WalkIn, CYoshi_Stage)
{
	CSoundMgr::GetInstance()->PlaySound(L"SD_PAPER2_HI_1031.wav", CSoundMgr::PLAYERMOVE);
	owner->Add_WalkEffect();

	if (owner->Collision_Look(false) && !owner->Collision_Front(false))
		owner->Get_PlayerTransform()->Go_Straight(owner->Get_TimeDelta());

	if (owner->Check_OnGround())
		owner->Set_OnGround();
	else
	{
		if (owner->Collision_Fall())
		{
			owner->Get_JumpPower() = 0.f;
			CHANGE_STATE(CStage_JumpFall);
		}
	}

	_tchar m_szData01[255];
	GetPrivateProfileString(_T("Stage"), _T("WalkIn"), NULL, m_szData01, 255, _T("../Bin/Data/Information/Yoshi_Animation.ini"));
	_double WalkIn = _tstof(m_szData01);

	if (owner->Get_Mesh()->Finish_Animation(WalkIn))
		CHANGE_STATE(CStage_Run);

	CInput_Device* pInputDevice = CInput_Device::GetInstance();
	Safe_AddRef(pInputDevice);

	if (pInputDevice->Key_Up(KEY_D) ||
		pInputDevice->Key_Up(KEY_A) ||
		pInputDevice->Key_Up(KEY_W) ||
		pInputDevice->Key_Up(KEY_S))
		CHANGE_STATE(CStage_Idle);

	// ¾Æ·¡
	if (pInputDevice->Key_Pressing(KEY_S))
	{
		if (owner->Get_ZTrigger())
		{
			if (fabs(owner->Get_Look().x) <= 0.001)
			{
				if (owner->Get_Look().z >= 0.f)
					CHANGE_STATE(CStage_RotateDown);
			}
			else
				CHANGE_STATE(CStage_RotateDown);
		}
		else
			CHANGE_STATE(CStage_Sit);
	}
	// ¿À¸¥ÂÊ
	else if (pInputDevice->Key_Pressing(KEY_D))
	{
		if (fabs(owner->Get_Look().z) > 0.001)
		{
			if (owner->Get_Look().x < 0.f)
				CHANGE_STATE(CStage_RotateRight);
		}
	}
	// ¿ÞÂÊ
	else if (pInputDevice->Key_Pressing(KEY_A))
	{
		if (fabs(owner->Get_Look().z) > 0.001)
		{
			if (owner->Get_Look().x >= 0.f)
				CHANGE_STATE(CStage_RotateLeft);
		}
	}
	// À§
	else if (pInputDevice->Key_Pressing(KEY_W))
	{
		if (owner->Get_ZTrigger())
		{
			if (fabs(owner->Get_Look().x) <= 0.001)
			{
				if (owner->Get_Look().z <= 0.f)
					CHANGE_STATE(CStage_RotateUp);
			}
			else
				CHANGE_STATE(CStage_RotateUp);
		}
	}

	if (pInputDevice->Key_Down(KEY_LBUTTON))
		CHANGE_STATE(CStage_TongueAttackIn);
	if (pInputDevice->Key_Down(KEY_RBUTTON))
	{
		if (owner->Get_EggQueue().size() > 0)
			CHANGE_STATE(CStage_ThrowIdle);
		else
			CHANGE_STATE(CStage_Throw);
	}
	if (pInputDevice->Key_Pressing(KEY_SPACE))
		CHANGE_STATE(CStage_Jump);
	if (owner->Get_KnockBack())
		CHANGE_STATE(CStage_KnockBack);

	Safe_Release(pInputDevice);
}
EXIT_STATE(CStage_WalkIn, CYoshi_Stage)
{
}

//

IMPLEMENT_STATE(CStage_WalkOut)
ENTER_STATE(CStage_WalkOut, CYoshi_Stage)
{
	owner->Get_PlayerTransform()->Set_SpeedPerSec(1.f);
	owner->Get_Mesh()->Set_AnimationSpeed(1.5f);
	owner->Get_Mesh()->SetUp_Animation(39);
}
UPDATE_STATE(CStage_WalkOut, CYoshi_Stage)
{
	CSoundMgr::GetInstance()->PlaySound(L"SD_PAPER2_HI_1031.wav", CSoundMgr::PLAYERMOVE);
	owner->Add_WalkEffect();

	if (owner->Collision_Look(false) && !owner->Collision_Front(false))
		owner->Get_PlayerTransform()->Go_Straight(owner->Get_TimeDelta());

	if (owner->Check_OnGround())
		owner->Set_OnGround();
	else
	{
		if (owner->Collision_Fall())
		{
			owner->Get_JumpPower() = 0.f;
			CHANGE_STATE(CStage_JumpFall);
		}
	}

	_tchar m_szData01[255];
	GetPrivateProfileString(_T("Stage"), _T("WalkOut"), NULL, m_szData01, 255, _T("../Bin/Data/Information/Yoshi_Animation.ini"));
	_double WalkOut = _tstof(m_szData01);

	if (owner->Get_Mesh()->Finish_Animation(WalkOut))
		CHANGE_STATE(CStage_Idle);

	CInput_Device* pInputDevice = CInput_Device::GetInstance();
	Safe_AddRef(pInputDevice);

	// ¾Æ·¡
	if (pInputDevice->Key_Pressing(KEY_S))
	{
		if (owner->Get_ZTrigger())
		{
			if (fabs(owner->Get_Look().x) <= 0.001)
			{
				if (owner->Get_Look().z >= 0.f)
					CHANGE_STATE(CStage_RotateDown);
			}
			else
				CHANGE_STATE(CStage_RotateDown);
		}
		else
			CHANGE_STATE(CStage_Sit);
	}
	// ¿À¸¥ÂÊ
	else if (pInputDevice->Key_Pressing(KEY_D))
	{
		if (fabs(owner->Get_Look().z) > 0.001)
		{
			if (owner->Get_Look().x < 0.f)
				CHANGE_STATE(CStage_RotateRight);
		}
	}
	// ¿ÞÂÊ
	else if (pInputDevice->Key_Pressing(KEY_A))
	{
		if (fabs(owner->Get_Look().z) > 0.001)
		{
			if (owner->Get_Look().x >= 0.f)
				CHANGE_STATE(CStage_RotateLeft);
		}
	}
	// À§
	else if (pInputDevice->Key_Pressing(KEY_W))
	{
		if (owner->Get_ZTrigger())
		{
			if (fabs(owner->Get_Look().x) <= 0.001)
			{
				if (owner->Get_Look().z <= 0.f)
					CHANGE_STATE(CStage_RotateUp);
			}
			else
				CHANGE_STATE(CStage_RotateUp);
		}
	}

	if (pInputDevice->Key_Down(KEY_LBUTTON))
		CHANGE_STATE(CStage_TongueAttackIn);
	if (pInputDevice->Key_Down(KEY_RBUTTON))
	{
		if (owner->Get_EggQueue().size() > 0)
			CHANGE_STATE(CStage_ThrowIdle);
		else
			CHANGE_STATE(CStage_Throw);
	}
	if (pInputDevice->Key_Pressing(KEY_SPACE))
		CHANGE_STATE(CStage_Jump);
	if (owner->Get_KnockBack())
		CHANGE_STATE(CStage_KnockBack);

	Safe_Release(pInputDevice);
}
EXIT_STATE(CStage_WalkOut, CYoshi_Stage)
{
}

//

IMPLEMENT_STATE(CStage_Run)
ENTER_STATE(CStage_Run, CYoshi_Stage)
{
	owner->Get_PlayerTransform()->Set_SpeedPerSec(5.f);
	owner->Get_Mesh()->Set_AnimationSpeed(1.5f);
	owner->Get_Mesh()->SetUp_Animation(38);
}
UPDATE_STATE(CStage_Run, CYoshi_Stage)
{
	CSoundMgr::GetInstance()->PlaySound(L"SD_PAPER2_HI_1031.wav", CSoundMgr::PLAYERMOVE);

	owner->Add_WalkEffect();

	if (owner->Collision_Look(false) && !owner->Collision_Front(false))
		owner->Get_PlayerTransform()->Go_Straight(owner->Get_TimeDelta());

	if (owner->Check_OnGround())
		owner->Set_OnGround();
	else
	{
		if (owner->Collision_Fall())
		{
			owner->Get_JumpPower() = 0.f;
			CHANGE_STATE(CStage_JumpFall);
		}
	}

	CInput_Device* pInputDevice = CInput_Device::GetInstance();
	Safe_AddRef(pInputDevice);

	// ¾Æ·¡
	if (pInputDevice->Key_Pressing(KEY_S))
	{
		if (owner->Get_ZTrigger())
		{
			if (fabs(owner->Get_Look().x) <= 0.001)
			{
				if (owner->Get_Look().z >= 0.f)
					CHANGE_STATE(CStage_RotateDown);
			}
			else
				CHANGE_STATE(CStage_RotateDown);
		}
		else
			CHANGE_STATE(CStage_Sit);
	}
	// ¿À¸¥ÂÊ
	if (pInputDevice->Key_Pressing(KEY_D))
	{
		if (fabs(owner->Get_Look().z) <= 0.001)
		{
			if (owner->Get_Look().x < 0.f)
				CHANGE_STATE(CStage_RotateRight);
		}
		else
			CHANGE_STATE(CStage_RotateRight);
	}
	// ¿ÞÂÊ
	if (pInputDevice->Key_Pressing(KEY_A))
	{
		if (fabs(owner->Get_Look().z) <= 0.001)
		{
			if (owner->Get_Look().x >= 0.f)
				CHANGE_STATE(CStage_RotateLeft);
		}
		else
			CHANGE_STATE(CStage_RotateLeft);
	}
	// À§
	if (pInputDevice->Key_Pressing(KEY_W))
	{
		if (owner->Get_ZTrigger())
		{
			if (fabs(owner->Get_Look().x) <= 0.001)
			{
				if (owner->Get_Look().z <= 0.f)
					CHANGE_STATE(CStage_RotateUp);
			}
			else
				CHANGE_STATE(CStage_RotateUp);
		}
	}

	if (pInputDevice->Key_Up(KEY_D) ||
		pInputDevice->Key_Up(KEY_A) ||
		pInputDevice->Key_Up(KEY_W) ||
		pInputDevice->Key_Up(KEY_S))
		CHANGE_STATE(CStage_WalkOut);
	if (pInputDevice->Key_Down(KEY_LBUTTON))
		CHANGE_STATE(CStage_TongueAttackIn);
	if (pInputDevice->Key_Down(KEY_RBUTTON))
	{
		if (owner->Get_EggQueue().size() > 0)
			CHANGE_STATE(CStage_ThrowIdle);
		else
			CHANGE_STATE(CStage_Throw);
	}
	if (pInputDevice->Key_Pressing(KEY_SPACE))
		CHANGE_STATE(CStage_Jump);
	if (owner->Get_KnockBack())
		CHANGE_STATE(CStage_KnockBack);

	Safe_Release(pInputDevice);
}
EXIT_STATE(CStage_Run, CYoshi_Stage)
{
}

// Sit //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_STATE(CStage_Sit)
ENTER_STATE(CStage_Sit, CYoshi_Stage)
{
	owner->Get_Mesh()->Set_Weight(0.2f);
	owner->Get_PlayerTransform()->Set_SpeedPerSec(1.f);
	owner->Get_Mesh()->Set_AnimationSpeed(1.f);
	owner->Get_Mesh()->SetUp_Animation(36);
	CSoundMgr::GetInstance()->PlaySound(L"SDV_HA5_P1.wav", CSoundMgr::PLAYER);
}
UPDATE_STATE(CStage_Sit, CYoshi_Stage)
{
	if (owner->Check_OnGround())
		owner->Set_OnGround();

	CInput_Device* pInputDevice = CInput_Device::GetInstance();
	Safe_AddRef(pInputDevice);

	if (pInputDevice->Key_Up(KEY_S))
		CHANGE_STATE(CStage_Idle);
	if (pInputDevice->Key_Pressing(KEY_S))
	{
		if (pInputDevice->Key_Pressing(KEY_D))
		{
			if (owner->Get_Look().x >= 0.f)
				CHANGE_STATE(CStage_SitWalk);
			else
				CHANGE_STATE(CStage_SitRotationRight);
		}
		else if (pInputDevice->Key_Pressing(KEY_A))
		{
			if (owner->Get_Look().x >= 0.f)
				CHANGE_STATE(CStage_SitRotationLeft);
			else
				CHANGE_STATE(CStage_SitWalk);
		}
	}
	if (pInputDevice->Key_Down(KEY_LBUTTON))
		CHANGE_STATE(CStage_TongueAttackIn);
	if (pInputDevice->Key_Down(KEY_RBUTTON))
	{
		if (owner->Get_EggQueue().size() > 0)
			CHANGE_STATE(CStage_ThrowIdle);
		else
			CHANGE_STATE(CStage_Throw);
	}
	if (pInputDevice->Key_Pressing(KEY_SPACE))
		CHANGE_STATE(CStage_JumpStart);
	if (owner->Get_KnockBack())
		CHANGE_STATE(CStage_KnockBack);

	Safe_Release(pInputDevice);
}
EXIT_STATE(CStage_Sit, CYoshi_Stage)
{
	owner->Get_Mesh()->Set_Weight(0.1f);
}

//

IMPLEMENT_STATE(CStage_SitWalk)
ENTER_STATE(CStage_SitWalk, CYoshi_Stage)
{
	owner->Get_PlayerTransform()->Set_SpeedPerSec(1.f);
	owner->Get_Mesh()->Set_AnimationSpeed(1.f);
	owner->Get_Mesh()->SetUp_Animation(35);
}
UPDATE_STATE(CStage_SitWalk, CYoshi_Stage)
{
	CSoundMgr::GetInstance()->PlaySound(L"SD_PAPER4_HI.wav", CSoundMgr::PLAYERMOVE);

	CInput_Device* pInputDevice = CInput_Device::GetInstance();
	Safe_AddRef(pInputDevice);

	if (owner->Collision_Look(false) && !owner->Collision_Front(false))
		owner->Get_PlayerTransform()->Go_Straight(owner->Get_TimeDelta());

	if (owner->Check_OnGround())
		owner->Set_OnGround();
	else
	{
		if (owner->Collision_Fall())
		{
			owner->Get_JumpPower() = 0.f;
			CHANGE_STATE(CStage_JumpFall);
		}
	}

	if (pInputDevice->Key_Up(KEY_S))
		CHANGE_STATE(CStage_WalkIn);
	if (pInputDevice->Key_Up(KEY_D) ||
		pInputDevice->Key_Up(KEY_A))
		CHANGE_STATE(CStage_Sit);
	if (pInputDevice->Key_Pressing(KEY_S))
	{
		if (pInputDevice->Key_Pressing(KEY_D))
		{
			if (owner->Get_Look().x < 0.f)
				CHANGE_STATE(CStage_SitRotationRight);
		}
		else if (pInputDevice->Key_Pressing(KEY_A))
		{
			if (owner->Get_Look().x >= 0.f)
				CHANGE_STATE(CStage_SitRotationLeft);
		}
	}
	if (pInputDevice->Key_Down(KEY_LBUTTON))
		CHANGE_STATE(CStage_TongueAttackIn);
	if (pInputDevice->Key_Down(KEY_RBUTTON))
	{
		if (owner->Get_EggQueue().size() > 0)
			CHANGE_STATE(CStage_ThrowIdle);
		else
			CHANGE_STATE(CStage_Throw);
	}
	if (pInputDevice->Key_Pressing(KEY_SPACE))
		CHANGE_STATE(CStage_JumpStart);
	if (owner->Get_KnockBack())
		CHANGE_STATE(CStage_KnockBack);

	Safe_Release(pInputDevice);
}
EXIT_STATE(CStage_SitWalk, CYoshi_Stage)
{
}

//

IMPLEMENT_STATE(CStage_SitRotationLeft)
ENTER_STATE(CStage_SitRotationLeft, CYoshi_Stage)
{
}
UPDATE_STATE(CStage_SitRotationLeft, CYoshi_Stage)
{
	if (owner->Get_PlayerTransform()->Rotation_Degree(owner->Get_TimeDelta(), 0.1, D3DXToRadian(90.f), D3DXToRadian(-90.f)))
		CHANGE_STATE(CStage_Sit);
}
EXIT_STATE(CStage_SitRotationLeft, CYoshi_Stage)
{
}

//

IMPLEMENT_STATE(CStage_SitRotationRight)
ENTER_STATE(CStage_SitRotationRight, CYoshi_Stage)
{
}
UPDATE_STATE(CStage_SitRotationRight, CYoshi_Stage)
{
	if (owner->Get_PlayerTransform()->Rotation_Degree(owner->Get_TimeDelta(), 0.1, D3DXToRadian(-90.f), D3DXToRadian(90.f)))
		CHANGE_STATE(CStage_Sit);
}
EXIT_STATE(CStage_SitRotationRight, CYoshi_Stage)
{
}

// Tongue & Egg //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_STATE(CStage_TongueAttackIn)
ENTER_STATE(CStage_TongueAttackIn, CYoshi_Stage)
{
	owner->Get_Mesh()->Set_AnimationSpeed(1.f);
	owner->Get_Mesh()->SetUp_Animation(29);
	owner->Get_Tongue()->GetFSM()->ChangeState(CTongue_AttackIn::Instance());
	if (CSoundMgr::GetInstance()->PlaySound(L"SDV_PERON1.wav", CSoundMgr::PLAYER2))
		CSoundMgr::GetInstance()->PlaySound(L"SDV_PERON1.wav", CSoundMgr::PLAYER);
}
UPDATE_STATE(CStage_TongueAttackIn, CYoshi_Stage)
{
	if (owner->Check_OnGround())
		owner->Set_OnGround();

	_tchar m_szData01[255];
	GetPrivateProfileString(_T("Stage"), _T("TongueIn"), NULL, m_szData01, 255, _T("../Bin/Data/Information/Yoshi_Animation.ini"));
	_double TongueIn = _tstof(m_szData01);

	if (owner->Get_Mesh()->Finish_Animation(TongueIn))
		CHANGE_STATE(CStage_TongueAttackOut);
	if (owner->Get_KnockBack())
		CHANGE_STATE(CStage_KnockBack);
}
EXIT_STATE(CStage_TongueAttackIn, CYoshi_Stage)
{
}

//

IMPLEMENT_STATE(CStage_TongueAttackOut)
ENTER_STATE(CStage_TongueAttackOut, CYoshi_Stage)
{
	owner->Get_Mesh()->Set_AnimationSpeed(1.f);
	owner->Get_Mesh()->SetUp_Animation(28);
	owner->Get_Tongue()->GetFSM()->ChangeState(CTongue_AttackOut::Instance());
}
UPDATE_STATE(CStage_TongueAttackOut, CYoshi_Stage)
{
	if (owner->Check_OnGround())
		owner->Set_OnGround();

	// Ãæµ¹ÇÑ ¸ó½ºÅÍ°¡ ÀÖÀ¸¸é ¾Ë ½Î±â
	if (owner->Get_TongueCollision())
	{
		_tchar m_szData01[255];
		GetPrivateProfileString(_T("Stage"), _T("TongueOutEgg"), NULL, m_szData01, 255, _T("../Bin/Data/Information/Yoshi_Animation.ini"));
		_double TongueOutEgg = _tstof(m_szData01);
		if (owner->Get_Mesh()->Finish_Animation(TongueOutEgg))
		{
			owner->Get_Tongue()->GetFSM()->ChangeState(CTongue_Idle::Instance());
			owner->Get_TongueCollision() = false;
			owner->Reset_Collider();
			CHANGE_STATE(CStage_LayEgg);
		}
	}
	// Ãæµ¹ÇÑ ¸ó½ºÅÍ°¡ ¾øÀ¸¸é Idle
	else
	{
		_tchar m_szData01[255];
		GetPrivateProfileString(_T("Stage"), _T("TongueOut"), NULL, m_szData01, 255, _T("../Bin/Data/Information/Yoshi_Animation.ini"));
		_double TongueOut = _tstof(m_szData01);
		if (owner->Get_Mesh()->Finish_Animation(TongueOut))
		{
			owner->Get_Tongue()->GetFSM()->ChangeState(CTongue_Idle::Instance());
			CInput_Device* pInputDevice = CInput_Device::GetInstance();
			Safe_AddRef(pInputDevice);

			if (pInputDevice->Key_Pressing(KEY_S))
			{
				if (pInputDevice->Key_Pressing(KEY_D) || pInputDevice->Key_Pressing(KEY_A))
					CHANGE_STATE(CStage_SitWalk);
				else
					CHANGE_STATE(CStage_Sit);
			}
			else
			{
				if (pInputDevice->Key_Pressing(KEY_D) || pInputDevice->Key_Pressing(KEY_A))
					CHANGE_STATE(CStage_Run);
				else
					CHANGE_STATE(CStage_Idle);
			}
			if (pInputDevice->Key_Pressing(KEY_SPACE))
				CHANGE_STATE(CStage_JumpStart);

			Safe_Release(pInputDevice);
		}
	}
}
EXIT_STATE(CStage_TongueAttackOut, CYoshi_Stage)
{
}

//

IMPLEMENT_STATE(CStage_LayEgg)
ENTER_STATE(CStage_LayEgg, CYoshi_Stage)
{
	owner->Reset_Collider();
	owner->Get_Mesh()->Set_AnimationSpeed(1.25f);
	owner->Get_Mesh()->SetUp_Animation(18);
}
UPDATE_STATE(CStage_LayEgg, CYoshi_Stage)
{
	if (owner->Check_OnGround())
		owner->Set_OnGround();

	_tchar m_szData01[255];
	GetPrivateProfileString(_T("Stage"), _T("LayEgg"), NULL, m_szData01, 255, _T("../Bin/Data/Information/Yoshi_Animation.ini"));
	_double LayEgg = _tstof(m_szData01);

	if (owner->Get_Mesh()->Finish_Animation(LayEgg))
	{
		if (CSoundMgr::GetInstance()->PlaySound(L"SDV_PON1.wav", CSoundMgr::PLAYER))
			CSoundMgr::GetInstance()->PlaySound(L"SDV_PON1.wav", CSoundMgr::PLAYER2);

		if (FAILED(owner->Add_Egg()))
			return;

		CInput_Device* pInputDevice = CInput_Device::GetInstance();
		Safe_AddRef(pInputDevice);

		if (pInputDevice->Key_Pressing(KEY_S))
		{
			if (pInputDevice->Key_Pressing(KEY_D) || pInputDevice->Key_Pressing(KEY_A))
				CHANGE_STATE(CStage_SitWalk);
			else
				CHANGE_STATE(CStage_Sit);
		}
		else
		{
			if (pInputDevice->Key_Pressing(KEY_D) || pInputDevice->Key_Pressing(KEY_A))
				CHANGE_STATE(CStage_Run);
			else
				CHANGE_STATE(CStage_Idle);
		}

		if (pInputDevice->Key_Down(KEY_RBUTTON))
		{
			if (owner->Get_EggQueue().size() > 0)
				CHANGE_STATE(CStage_ThrowIdle);
			else
				CHANGE_STATE(CStage_Throw);
		}
		if (pInputDevice->Key_Pressing(KEY_SPACE))
			CHANGE_STATE(CStage_JumpStart);

		Safe_Release(pInputDevice);
	}
}
EXIT_STATE(CStage_LayEgg, CYoshi_Stage)
{
	owner->Get_KnockBack() = false;
	owner->Get_Tongue()->GetFSM()->ChangeState(CTongue_Idle::Instance());
}

// Throw //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_STATE(CStage_ThrowIdle)
ENTER_STATE(CStage_ThrowIdle, CYoshi_Stage)
{
	owner->Get_Mesh()->Set_AnimationSpeed(2.f);
	owner->Get_Mesh()->SetUp_Animation(21);
	owner->Hold_Egg();
	CSoundMgr::GetInstance()->PlaySound(L"SDV_NERAU1.wav", CSoundMgr::THROW);
}
UPDATE_STATE(CStage_ThrowIdle, CYoshi_Stage)
{
	if (owner->Check_OnGround())
		owner->Set_OnGround();

	// ¼±ÅÃµÈ ¾Ö°¡ ¾ø´Ù
	if (owner->Get_FocusTarget() == nullptr)
	{
		if (owner->Set_NoFocus())
			return;
	}
	// ¼±ÅÃµÈ ¾Ö°¡ ÀÖ´Ù
	else
	{
		if (owner->Set_LookFocus())
			return;
	}

	CInput_Device* pInputDevice = CInput_Device::GetInstance();
	Safe_AddRef(pInputDevice);

	if (owner->Get_KnockBack())
	{
		owner->PushBack_Egg();
		CHANGE_STATE(CStage_KnockBack);
	}
	if (pInputDevice->Key_Up(KEY_RBUTTON))
		CHANGE_STATE(CStage_Throw);
	if (pInputDevice->Key_Down(KEY_SPACE))
	{
		owner->PushBack_Egg();
		CHANGE_STATE(CStage_JumpStart);
	}
	if (pInputDevice->Key_Down(KEY_LBUTTON))
	{
		owner->PushBack_Egg();
		CHANGE_STATE(CStage_TongueAttackIn);
	}

	Safe_Release(pInputDevice);
}
EXIT_STATE(CStage_ThrowIdle, CYoshi_Stage)
{
	CSoundMgr::GetInstance()->StopSound(CSoundMgr::THROW);
}

//

IMPLEMENT_STATE(CStage_Throw)
ENTER_STATE(CStage_Throw, CYoshi_Stage)
{
	owner->Get_Mesh()->Set_AnimationSpeed(1.f);
	owner->Get_Mesh()->SetUp_Animation(22);
	owner->Release_Egg();
	if (CSoundMgr::GetInstance()->PlaySound(L"SDV_HA1.wav", CSoundMgr::PLAYER))
		CSoundMgr::GetInstance()->PlaySound(L"SDV_HA1.wav", CSoundMgr::PLAYER2);
}
UPDATE_STATE(CStage_Throw, CYoshi_Stage)
{
	if (owner->Check_OnGround())
		owner->Set_OnGround();

	_tchar m_szData01[255];
	GetPrivateProfileString(_T("Stage"), _T("Throw"), NULL, m_szData01, 255, _T("../Bin/Data/Information/Yoshi_Animation.ini"));
	_double Throw = _tstof(m_szData01);

	if (owner->Get_KnockBack())
		CHANGE_STATE(CStage_KnockBack);
	if (owner->Get_Mesh()->Finish_Animation(Throw))
	{
		CInput_Device* pInputDevice = CInput_Device::GetInstance();
		Safe_AddRef(pInputDevice);

		if (pInputDevice->Key_Pressing(KEY_S))
		{
			if (fabs(owner->Get_Look().x) >= 0.001)
			{
				if (owner->Get_Look().x > 0)
					CHANGE_STATE(CStage_SitRotationRight);
				else
					CHANGE_STATE(CStage_SitRotationLeft);
			}
			else
			{
				if (pInputDevice->Key_Pressing(KEY_D) || pInputDevice->Key_Pressing(KEY_A))
					CHANGE_STATE(CStage_SitWalk);
				else
					CHANGE_STATE(CStage_Sit);
			}
		}
		else
		{
			if (fabs(owner->Get_Look().x) >= 0.001)
			{
				if (owner->Get_Look().x > 0)
					CHANGE_STATE(CStage_RotateRight);
				else
					CHANGE_STATE(CStage_RotateLeft);
			}
			else
			{
				if (pInputDevice->Key_Pressing(KEY_D) || pInputDevice->Key_Pressing(KEY_A))
					CHANGE_STATE(CStage_WalkIn);
				else
					CHANGE_STATE(CStage_Idle);
			}
		}

		Safe_Release(pInputDevice);
	}
}
EXIT_STATE(CStage_Throw, CYoshi_Stage)
{
	owner->Get_FocusRotate() = true;
}

//

IMPLEMENT_STATE(CStage_ThrowRotateTarget)
ENTER_STATE(CStage_ThrowRotateTarget, CYoshi_Stage)
{
	owner->Get_FocusRotate() = true;
	vStartLook =/* owner->Get_PlayerTransform()->Get_State(CTransform::STATE_POSITION) + */owner->Get_PlayerTransform()->Get_State(CTransform::STATE_LOOK);
	fProgress = 0.f;
}
UPDATE_STATE(CStage_ThrowRotateTarget, CYoshi_Stage)
{
	_vec3 vLookAt = _vec3(0.f, 0.f, 0.f);
	fProgress += 10.f * (_float)owner->Get_TimeDelta();
	D3DXVec3Lerp(&vLookAt, &vStartLook, &owner->Get_FocusTargetLook(), fProgress);
	if (fProgress >= 1.f)
	{
		//owner->Get_FocusRotate() = true;
		owner->Get_PlayerTransform()->Rotation_LookAt(owner->Get_Position() + owner->Get_FocusTargetLook());
		CHANGE_STATE(CStage_ThrowIdle);
		return;
	}
	owner->Get_PlayerTransform()->Rotation_LookAt(owner->Get_Position() + vLookAt);
}
EXIT_STATE(CStage_ThrowRotateTarget, CYoshi_Stage)
{
	vStartLook = _vec3(0.f, 0.f, 0.f);
	fProgress = 0.f;
}

// Jump //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_STATE(CStage_JumpStart)
ENTER_STATE(CStage_JumpStart, CYoshi_Stage)
{
	owner->Reset_Jump();
	owner->Get_PlayerTransform()->Set_SpeedPerSec(4.f);
	owner->Get_Mesh()->Set_AnimationSpeed(1.f);
	owner->Get_Mesh()->SetUp_Animation(34);
	if (CSoundMgr::GetInstance()->PlaySound(L"SDV_HA1.wav", CSoundMgr::JUMP))
		CSoundMgr::GetInstance()->PlaySound(L"SDV_HA2.wav", CSoundMgr::PLAYERMOVE);
	CSoundMgr::GetInstance()->PlaySound(L"SD_JUMP1.wav", CSoundMgr::JUMP2);
}
UPDATE_STATE(CStage_JumpStart, CYoshi_Stage)
{
	if (owner->Check_OnGround())
		owner->Set_OnGround();

	_tchar m_szData01[255];
	GetPrivateProfileString(_T("Stage"), _T("JumpStart"), NULL, m_szData01, 255, _T("../Bin/Data/Information/Yoshi_Animation.ini"));
	_double JumpStart = _tstof(m_szData01);

	if (owner->Get_KnockBack())
		CHANGE_STATE(CStage_KnockBack);
	if (owner->Get_Mesh()->Finish_Animation(JumpStart))
		CHANGE_STATE(CStage_Jump);
}
EXIT_STATE(CStage_JumpStart, CYoshi_Stage)
{
	owner->Get_HeadCollision() = false;
}

//

IMPLEMENT_STATE(CStage_Jump)
ENTER_STATE(CStage_Jump, CYoshi_Stage)
{
	owner->Get_PlayerTransform()->Set_SpeedPerSec(4.f);
	owner->Get_Mesh()->Set_AnimationSpeed(1.f);
	owner->Get_Mesh()->SetUp_Animation(33);
	owner->Get_Camera()->Get_RenewTarget() = true;
	if (CSoundMgr::GetInstance()->PlaySound(L"SDV_HA1.wav", CSoundMgr::JUMP))
		CSoundMgr::GetInstance()->PlaySound(L"SDV_HA2.wav", CSoundMgr::PLAYERMOVE);
	CSoundMgr::GetInstance()->PlaySound(L"SD_JUMP1.wav", CSoundMgr::JUMP2);
}
UPDATE_STATE(CStage_Jump, CYoshi_Stage)
{
	_vec3 vPos = owner->Get_Position();
	vPos.y += owner->Get_JumpPower() * (_float)owner->Get_TimeDelta();
	owner->Get_JumpPower() -= owner->Get_JumpAccel() * (_float)owner->Get_TimeDelta();
	owner->Get_PlayerTransform()->Set_State(CTransform::STATE_POSITION, vPos);

	CInput_Device* pInputDevice = CInput_Device::GetInstance();
	Safe_AddRef(pInputDevice);

	if (owner->Collision_Up())
	{
		owner->Get_JumpPower() = 0.f;
		CHANGE_STATE(CStage_JumpFall);
	}
	if (owner->Get_JumpPower() <= 0.f)
	{
		if (pInputDevice->Key_Pressing(KEY_SPACE))
			CHANGE_STATE(CStage_Hovering);
		else
			CHANGE_STATE(CStage_JumpFall);
	}
	if (pInputDevice->Key_Down(KEY_S))
		CHANGE_STATE(CStage_HipPushStart);
	if (pInputDevice->Key_Pressing(KEY_D))
	{
		if (/*owner->Collision_Look(false) && */!owner->Collision_Front(true))
		{
			if (owner->Get_Look().x >= 0.f)
				owner->Get_PlayerTransform()->Go_Straight(owner->Get_TimeDelta());
			else
				CHANGE_STATE(CStage_JumpRotationRight);
		}
	}
	else if (pInputDevice->Key_Pressing(KEY_A))
	{
		if (/*owner->Collision_Look(false) &&*/ !owner->Collision_Front(true))
		{
			if (owner->Get_Look().x >= 0.f)
				CHANGE_STATE(CStage_JumpRotationLeft);
			else
				owner->Get_PlayerTransform()->Go_Straight(owner->Get_TimeDelta());
		}
	}

	Safe_Release(pInputDevice);
}
EXIT_STATE(CStage_Jump, CYoshi_Stage)
{
	owner->Get_HeadCollision() = false;
}

//

IMPLEMENT_STATE(CStage_Hovering)
ENTER_STATE(CStage_Hovering, CYoshi_Stage)
{
	owner->Get_PlayerTransform()->Set_SpeedPerSec(2.5f);
	owner->Get_Mesh()->Set_AnimationSpeed(1.5f);
	owner->Get_Mesh()->SetUp_Animation(32);
	owner->Get_Camera()->Get_RenewTarget() = true;
	CSoundMgr::GetInstance()->PlaySound(L"yoshi-hmmph 2.wav", CSoundMgr::HOVERING);
}
UPDATE_STATE(CStage_Hovering, CYoshi_Stage)
{
	_tchar m_szData01[255];
	GetPrivateProfileString(_T("Test"), _T("Hovering"), NULL, m_szData01, 255, _T("../Bin/Data/Information/Yoshi_Animation.ini"));
	_float Hovering = (_float)_tstof(m_szData01);

	m_fRadian += (_float)owner->Get_TimeDelta() * 0.3f;
	if (m_fRadian >= Hovering)
	{
		m_fRadian = 0.f;
		CHANGE_STATE(CStage_JumpFall);
	}

	_vec3 vPos = owner->Get_Position();
	vPos.y += sinf(D3DXToRadian(m_fRadian)) * 5.f;
	owner->Get_PlayerTransform()->Set_State(CTransform::STATE_POSITION, vPos);

	if (owner->Collision_Up())
	{
		owner->Get_JumpPower() = 0.f;
		CHANGE_STATE(CStage_JumpFall);
	}

	CInput_Device* pInputDevice = CInput_Device::GetInstance();
	Safe_AddRef(pInputDevice);

	if (owner->Get_HeadCollision())
	{
		owner->Get_KnockBack() = false;
		owner->Get_HeadCollision() = false;
		CHANGE_STATE(CStage_JumpStart);
	}
	if (owner->Get_KnockBack())
		CHANGE_STATE(CStage_KnockBack);
	if (pInputDevice->Key_Up(KEY_SPACE))
	{
		m_fRadian = 0.f;
		CHANGE_STATE(CStage_JumpFall);
	}
	if (pInputDevice->Key_Down(KEY_S))
		CHANGE_STATE(CStage_HipPushStart);
	if (pInputDevice->Key_Pressing(KEY_D))
	{
		if (/*owner->Collision_Look(false) &&*/ !owner->Collision_Front(true))
		{
			if (owner->Get_Look().x >= 0.f)
			{
				owner->Get_PlayerTransform()->Go_Straight(owner->Get_TimeDelta());
			}
			else
				CHANGE_STATE(CStage_HoveringRotationRight);
		}
	}
	else if (pInputDevice->Key_Pressing(KEY_A))
	{
		if (/*owner->Collision_Look(false) &&*/ !owner->Collision_Front(true))
		{
			if (owner->Get_Look().x >= 0.f)
				CHANGE_STATE(CStage_HoveringRotationLeft);
			else
			{
				owner->Get_PlayerTransform()->Go_Straight(owner->Get_TimeDelta());
			}
		}
	}

	Safe_Release(pInputDevice);
}
EXIT_STATE(CStage_Hovering, CYoshi_Stage)
{
	CSoundMgr::GetInstance()->StopSound(CSoundMgr::HOVERING);
	owner->Get_HeadCollision() = false;
}

//

IMPLEMENT_STATE(CStage_JumpFall)
ENTER_STATE(CStage_JumpFall, CYoshi_Stage)
{
	owner->Get_PlayerTransform()->Set_SpeedPerSec(4.f);
	owner->Get_Mesh()->Set_AnimationSpeed(1.5f);
	owner->Get_Mesh()->SetUp_Animation(31);
	m_EnterTime = system_clock::now();
	owner->Get_Camera()->Get_RenewTarget() = true;
}
UPDATE_STATE(CStage_JumpFall, CYoshi_Stage)
{
	_vec3 vPos = owner->Get_Position();
	vPos.y += owner->Get_JumpPower() * (_float)owner->Get_TimeDelta();
	owner->Get_JumpPower() -= owner->Get_JumpAccel() * (_float)owner->Get_TimeDelta();
	owner->Get_PlayerTransform()->Set_State(CTransform::STATE_POSITION, vPos);

	CInput_Device* pInputDevice = CInput_Device::GetInstance();
	Safe_AddRef(pInputDevice);

	if (owner->Get_HeadCollision())
	{
		owner->Get_KnockBack() = false;
		owner->Get_HeadCollision() = false;
		CHANGE_STATE(CStage_JumpStart);
	}
	if (owner->Get_KnockBack())
		CHANGE_STATE(CStage_KnockBack);
	if (pInputDevice->Key_Down(KEY_SPACE))
	{
		system_clock::duration temp = system_clock::now() - m_EnterTime;
		if (temp > (milliseconds)200)
		{
			owner->Get_JumpPower() = 0.5f;
			CHANGE_STATE(CStage_Hovering);
		}
	}
	if (pInputDevice->Key_Down(KEY_S))
		CHANGE_STATE(CStage_HipPushStart);

	if (owner->Check_Jumping())
	{
		owner->Reset_Jump();
		owner->Get_Camera()->Get_RenewTarget() = false;
		//owner->Get_Camera()->Get_StartPos() = owner->Get_PlayerTransform()->Get_State(CTransform::STATE_POSITION);

		if (pInputDevice->Key_Pressing(KEY_S))
		{
			if (pInputDevice->Key_Pressing(KEY_D) || pInputDevice->Key_Pressing(KEY_A))
				CHANGE_STATE(CStage_SitWalk);
			else
				CHANGE_STATE(CStage_Sit);
		}
		else
		{
			if (pInputDevice->Key_Pressing(KEY_D) || pInputDevice->Key_Pressing(KEY_A))
				CHANGE_STATE(CStage_WalkIn);
			else
				CHANGE_STATE(CStage_JumpEnd);
		}
	}

	if (pInputDevice->Key_Pressing(KEY_D))
	{
		if (owner->Get_Look().x >= 0.f)
		{
			if (/*owner->Collision_Look(false) &&*/ !owner->Collision_Front(false))
				owner->Get_PlayerTransform()->Go_Straight(owner->Get_TimeDelta());
		}
		else
			CHANGE_STATE(CStage_JumpRotationRight);
	}
	else if (pInputDevice->Key_Pressing(KEY_A))
	{
		if (owner->Get_Look().x >= 0.f)
			CHANGE_STATE(CStage_JumpRotationLeft);
		else
		{
			if (/*owner->Collision_Look(false) &&*/ !owner->Collision_Front(false))
				owner->Get_PlayerTransform()->Go_Straight(owner->Get_TimeDelta());
		}
	}

	Safe_Release(pInputDevice);
}
EXIT_STATE(CStage_JumpFall, CYoshi_Stage)
{
	owner->Get_HeadCollision() = false;
}

//

IMPLEMENT_STATE(CStage_JumpEnd)
ENTER_STATE(CStage_JumpEnd, CYoshi_Stage)
{
	_tchar m_szData01[255];
	GetPrivateProfileString(_T("Stage"), _T("JumpEndSpeed"), NULL, m_szData01, 255, _T("../Bin/Data/Information/Yoshi_Animation.ini"));
	_float JumpEndSpeed = (_float)_tstof(m_szData01);
	owner->Get_Mesh()->Set_AnimationSpeed(JumpEndSpeed);
	owner->Get_Mesh()->SetUp_Animation(30);

	CSoundMgr::GetInstance()->PlaySound(L"SD_PAPER4_HI.wav", CSoundMgr::PLAYER);

	if (owner->Get_KnockBack())
		CHANGE_STATE(CStage_KnockBack);
}
UPDATE_STATE(CStage_JumpEnd, CYoshi_Stage)
{
	if (owner->Check_OnGround())
		owner->Set_OnGround();

	_tchar m_szData01[255];
	GetPrivateProfileString(_T("Stage"), _T("JumpEnd"), NULL, m_szData01, 255, _T("../Bin/Data/Information/Yoshi_Animation.ini"));
	_double JumpEnd = _tstof(m_szData01);
	if (owner->Get_Mesh()->Finish_Animation(JumpEnd))
		CHANGE_STATE(CStage_Idle);

	CInput_Device* pInputDevice = CInput_Device::GetInstance();
	Safe_AddRef(pInputDevice);

	if (owner->Get_KnockBack())
		CHANGE_STATE(CStage_KnockBack);
	if (pInputDevice->Key_Pressing(KEY_S))
	{
		if (pInputDevice->Key_Pressing(KEY_D) || pInputDevice->Key_Pressing(KEY_A))
			CHANGE_STATE(CStage_SitWalk);
		else
			CHANGE_STATE(CStage_Sit);
	}
	else
	{
		if (pInputDevice->Key_Pressing(KEY_D) || pInputDevice->Key_Pressing(KEY_A))
			CHANGE_STATE(CStage_WalkIn);
	}

	Safe_Release(pInputDevice);
}
EXIT_STATE(CStage_JumpEnd, CYoshi_Stage)
{
	owner->Get_HeadCollision() = false;
	owner->Reset_Jump();
}

//

IMPLEMENT_STATE(CStage_JumpRotationLeft)
ENTER_STATE(CStage_JumpRotationLeft, CYoshi_Stage)
{
}
UPDATE_STATE(CStage_JumpRotationLeft, CYoshi_Stage)
{
	if (owner->Get_PlayerTransform()->Rotation_Degree(owner->Get_TimeDelta(), 0.1, D3DXToRadian(90.f), D3DXToRadian(-90.f)))
	{
		if (owner->Get_JumpPower() < 0.2f)
			CHANGE_STATE(CStage_JumpFall);
		else
			CHANGE_STATE(CStage_Jump);
	}
}
EXIT_STATE(CStage_JumpRotationLeft, CYoshi_Stage)
{
}

//

IMPLEMENT_STATE(CStage_JumpRotationRight)
ENTER_STATE(CStage_JumpRotationRight, CYoshi_Stage)
{
}
UPDATE_STATE(CStage_JumpRotationRight, CYoshi_Stage)
{
	if (owner->Get_PlayerTransform()->Rotation_Degree(owner->Get_TimeDelta(), 0.1, D3DXToRadian(-90.f), D3DXToRadian(90.f)))
	{
		if (owner->Get_JumpPower() < 0.2f)
			CHANGE_STATE(CStage_JumpFall);
		else
			CHANGE_STATE(CStage_Jump);
	}
}
EXIT_STATE(CStage_JumpRotationRight, CYoshi_Stage)
{
}

//

IMPLEMENT_STATE(CStage_HoveringRotationLeft)
ENTER_STATE(CStage_HoveringRotationLeft, CYoshi_Stage)
{
}
UPDATE_STATE(CStage_HoveringRotationLeft, CYoshi_Stage)
{
	if (owner->Get_PlayerTransform()->Rotation_Degree(owner->Get_TimeDelta(), 0.1, D3DXToRadian(90.f), D3DXToRadian(-90.f)))
		CHANGE_STATE(CStage_Hovering);
}
EXIT_STATE(CStage_HoveringRotationLeft, CYoshi_Stage)
{
}

//

IMPLEMENT_STATE(CStage_HoveringRotationRight)
ENTER_STATE(CStage_HoveringRotationRight, CYoshi_Stage)
{
}
UPDATE_STATE(CStage_HoveringRotationRight, CYoshi_Stage)
{
	if (owner->Get_PlayerTransform()->Rotation_Degree(owner->Get_TimeDelta(), 0.1, D3DXToRadian(-90.f), D3DXToRadian(90.f)))
		CHANGE_STATE(CStage_Hovering);
}
EXIT_STATE(CStage_HoveringRotationRight, CYoshi_Stage)
{
}

// Hip Push //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_STATE(CStage_HipPushStart)
ENTER_STATE(CStage_HipPushStart, CYoshi_Stage)
{
	owner->Get_Mesh()->Set_AnimationSpeed(1.f);
	owner->Get_Mesh()->SetUp_Animation(17);
	if (CSoundMgr::GetInstance()->PlaySound(L"YoshiHipDrop-Kururin01-Sp-0_75.wav", CSoundMgr::PLAYER))
		CSoundMgr::GetInstance()->PlaySound(L"YoshiHipDrop-Kururin01-Sp-0_75.wav", CSoundMgr::PLAYER3);
}
UPDATE_STATE(CStage_HipPushStart, CYoshi_Stage)
{
	_tchar m_szData01[255];
	GetPrivateProfileString(_T("Stage"), _T("HipPushStart"), NULL, m_szData01, 255, _T("../Bin/Data/Information/Yoshi_Animation.ini"));
	_double HipPushStart = _tstof(m_szData01);

	if (owner->Get_Mesh()->Finish_Animation(HipPushStart))
		CHANGE_STATE(CStage_HipPush);
}
EXIT_STATE(CStage_HipPushStart, CYoshi_Stage)
{
}

//

IMPLEMENT_STATE(CStage_HipPush)
ENTER_STATE(CStage_HipPush, CYoshi_Stage)
{
	owner->Get_Mesh()->Set_AnimationSpeed(1.f);
	owner->Get_Mesh()->SetUp_Animation(16);

	_vec3 vPos = owner->Get_Position();
	m_fPower = vPos.y * 0.1f;
	if (CSoundMgr::GetInstance()->PlaySound(L"YoshiHipDrop-Ha01.wav", CSoundMgr::PLAYER3))
		CSoundMgr::GetInstance()->PlaySound(L"YoshiHipDrop-Ha01.wav", CSoundMgr::PLAYER2);
}
UPDATE_STATE(CStage_HipPush, CYoshi_Stage)
{
	_tchar m_szData01[255];
	GetPrivateProfileString(_T("Test"), _T("HipPushPower"), NULL, m_szData01, 255, _T("../Bin/Data/Information/Yoshi_Animation.ini"));
	_float HipPushPower = (_float)_tstof(m_szData01);

	_vec3 vPos = owner->Get_Position();
	vPos.y += owner->Get_JumpPower() * (_float)owner->Get_TimeDelta();
	owner->Get_JumpPower() -= (HipPushPower / m_fPower) * (_float)owner->Get_TimeDelta();
	owner->Get_PlayerTransform()->Set_State(CTransform::STATE_POSITION, vPos);

	//if (owner->Get_Position().y <= 0.1f)
	if (owner->Check_Jumping())
		CHANGE_STATE(CStage_HipPushImpact);
}
EXIT_STATE(CStage_HipPush, CYoshi_Stage)
{
}

//

IMPLEMENT_STATE(CStage_HipPushImpact)
ENTER_STATE(CStage_HipPushImpact, CYoshi_Stage)
{
	owner->Get_Mesh()->Set_AnimationSpeed(2.f);
	owner->Get_Mesh()->SetUp_Animation(15);
	owner->Reset_Jump();
}
UPDATE_STATE(CStage_HipPushImpact, CYoshi_Stage)
{
	if (owner->Check_OnGround())
		owner->Set_OnGround();

	_tchar m_szData01[255];
	GetPrivateProfileString(_T("Stage"), _T("HipPushImpact"), NULL, m_szData01, 255, _T("../Bin/Data/Information/Yoshi_Animation.ini"));
	_double HipPushImpact = _tstof(m_szData01);

	if (owner->Get_Mesh()->Finish_Animation(HipPushImpact))
		CHANGE_STATE(CStage_HipPushEnd);
}
EXIT_STATE(CStage_HipPushImpact, CYoshi_Stage)
{
}

//

IMPLEMENT_STATE(CStage_HipPushEnd)
ENTER_STATE(CStage_HipPushEnd, CYoshi_Stage)
{
	owner->Get_Mesh()->Set_AnimationSpeed(1.2f);
	owner->Get_Mesh()->SetUp_Animation(13);
}
UPDATE_STATE(CStage_HipPushEnd, CYoshi_Stage)
{
	if (owner->Check_OnGround())
		owner->Set_OnGround();

	_tchar m_szData01[255];
	GetPrivateProfileString(_T("Stage"), _T("HipPushEnd"), NULL, m_szData01, 255, _T("../Bin/Data/Information/Yoshi_Animation.ini"));
	_double HipPushEnd = _tstof(m_szData01);

	CInput_Device* pInputDevice = CInput_Device::GetInstance();
	Safe_AddRef(pInputDevice);

	if (owner->Get_KnockBack())
		CHANGE_STATE(CStage_KnockBack);
	if (owner->Get_Mesh()->Finish_Animation(HipPushEnd))
	{
		owner->Reset_Jump();

		if (pInputDevice->Key_Pressing(KEY_S))
		{
			if (pInputDevice->Key_Pressing(KEY_D) || pInputDevice->Key_Pressing(KEY_A))
				CHANGE_STATE(CStage_SitWalk);
			else
				CHANGE_STATE(CStage_Sit);
		}
		else
		{
			if (pInputDevice->Key_Pressing(KEY_D) || pInputDevice->Key_Pressing(KEY_A))
				CHANGE_STATE(CStage_WalkIn);
			else
				CHANGE_STATE(CStage_Idle);
		}
	}

	Safe_Release(pInputDevice);
}
EXIT_STATE(CStage_HipPushEnd, CYoshi_Stage)
{
}

// Reaction //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_STATE(CStage_KnockBack)
ENTER_STATE(CStage_KnockBack, CYoshi_Stage)
{
	owner->Get_PlayerTransform()->Set_SpeedPerSec(1.f);
	owner->Get_Mesh()->Set_AnimationSpeed(3.f);
	owner->Get_Mesh()->SetUp_Animation(11);
	if (CSoundMgr::GetInstance()->PlaySound(L"SDV_DAME1.wav", CSoundMgr::PLAYER3))
		CSoundMgr::GetInstance()->PlaySound(L"SDV_YOSHI_ODOROKI1.wav", CSoundMgr::PLAYER);
}
UPDATE_STATE(CStage_KnockBack, CYoshi_Stage)
{
	owner->Get_PlayerTransform()->Back_Straight(owner->Get_TimeDelta());

	if (owner->Check_OnGround())
		owner->Set_OnGround();

	_tchar m_szData01[255];
	GetPrivateProfileString(_T("Stage"), _T("KnockBack"), NULL, m_szData01, 255, _T("../Bin/Data/Information/Yoshi_Animation.ini"));
	_double KnockBack = _tstof(m_szData01);

	CInput_Device* pInputDevice = CInput_Device::GetInstance();
	Safe_AddRef(pInputDevice);

	if (owner->Get_Mesh()->Finish_Animation(KnockBack))
	{
		owner->Get_KnockBack() = false;

		if (pInputDevice->Key_Pressing(KEY_S))
		{
			if (pInputDevice->Key_Pressing(KEY_D) || pInputDevice->Key_Pressing(KEY_A))
				CHANGE_STATE(CStage_SitWalk);
			else
				CHANGE_STATE(CStage_Sit);
		}
		else
		{
			if (pInputDevice->Key_Pressing(KEY_D) || pInputDevice->Key_Pressing(KEY_A))
				CHANGE_STATE(CStage_WalkIn);
			else
				CHANGE_STATE(CStage_Idle);
		}
	}

	Safe_Release(pInputDevice);
}
EXIT_STATE(CStage_KnockBack, CYoshi_Stage)
{
	owner->Reset_Collider();
}

//

IMPLEMENT_STATE(CStage_JumpToTrain)
ENTER_STATE(CStage_JumpToTrain, CYoshi_Stage)
{
	owner->Get_PlayerTransform()->Set_SpeedPerSec(6.f);
	owner->Get_Mesh()->Set_AnimationSpeed(1.5f);
	owner->Get_Mesh()->SetUp_Animation(33);
	m_EnterTime = system_clock::now();
	if (CSoundMgr::GetInstance()->PlaySound(L"SDV_HA2.wav", CSoundMgr::PLAYER3))
		CSoundMgr::GetInstance()->PlaySound(L"SDV_HA2.wav", CSoundMgr::PLAYER2);
}
UPDATE_STATE(CStage_JumpToTrain, CYoshi_Stage)
{
	//_vec3 vPos = owner->Get_Position();
	//vPos.y += owner->Get_JumpPower() * (_float)owner->Get_TimeDelta();
	//owner->Get_JumpPower() -= owner->Get_JumpAccel() * (_float)owner->Get_TimeDelta();
	//owner->Get_PlayerTransform()->Set_State(CTransform::STATE_POSITION, vPos);

	owner->Get_OnTrain(true) = false;
	owner->Get_OnTrain(false) = false;

	_vec3 vTarget = _vec3(184.2f, 2.07f, 36.74f);

	owner->Get_PlayerTransform()->Goto_Target(vTarget, owner->Get_TimeDelta());
	
	_vec3 vDistance = vTarget - owner->Get_PlayerTransform()->Get_State(CTransform::STATE_POSITION);

	if (D3DXVec3Length(&vDistance) < 0.1f)
	{
		owner->Get_PlayerTransform()->Set_State(CTransform::STATE_POSITION, vTarget);
		owner->Get_PlayerTransform()->Rotation_LookAt(vTarget + _vec3(1.f, 0.f, 0.f));
		CHANGE_STATE(CStage_WaitTrain);
	}
}
EXIT_STATE(CStage_JumpToTrain, CYoshi_Stage)
{
}

//

IMPLEMENT_STATE(CStage_WaitTrain)
ENTER_STATE(CStage_WaitTrain, CYoshi_Stage)
{
	owner->Get_PlayerTransform()->Set_SpeedPerSec(4.f);
	owner->Get_Mesh()->Set_AnimationSpeed(1.5f);
	owner->Get_Mesh()->SetUp_Animation(40);
	m_EnterTime = system_clock::now();
	CSoundMgr::GetInstance()->PlaySound(L"SD_TRAIN_KITEKI2.wav", CSoundMgr::EFFECT2);
}
UPDATE_STATE(CStage_WaitTrain, CYoshi_Stage)
{
	system_clock::duration temp = system_clock::now() - m_EnterTime;

	if(duration_cast<milliseconds>(temp) > (milliseconds)2000)
		CHANGE_STATE(CStage_OnTrain);
}
EXIT_STATE(CStage_WaitTrain, CYoshi_Stage)
{
}

//

IMPLEMENT_STATE(CStage_OnTrain)
ENTER_STATE(CStage_OnTrain, CYoshi_Stage)
{
	owner->Get_PlayerTransform()->Set_SpeedPerSec(4.f);
	owner->Get_Mesh()->Set_AnimationSpeed(1.5f);
	owner->Get_Mesh()->SetUp_Animation(40);
	owner->StartTrain();
	m_EnterTime = system_clock::now();
}
UPDATE_STATE(CStage_OnTrain, CYoshi_Stage)
{
	if (owner->Set_OnTrain())
		CHANGE_STATE(CStage_JumpOffTrain);
}
EXIT_STATE(CStage_OnTrain, CYoshi_Stage)
{
}


//

IMPLEMENT_STATE(CStage_JumpOffTrain)
ENTER_STATE(CStage_JumpOffTrain, CYoshi_Stage)
{
	owner->Get_PlayerTransform()->Set_SpeedPerSec(5.f);
	owner->Get_Mesh()->Set_AnimationSpeed(1.5f);
	owner->Get_Mesh()->SetUp_Animation(33);
	owner->StartTrain();
	if (CSoundMgr::GetInstance()->PlaySound(L"SDV_HA1.wav", CSoundMgr::PLAYER3))
		CSoundMgr::GetInstance()->PlaySound(L"SDV_HA1.wav", CSoundMgr::PLAYER2);
}
UPDATE_STATE(CStage_JumpOffTrain, CYoshi_Stage)
{
	owner->Get_OnTrain(true) = false;
	owner->Get_OnTrain(false) = false;

	_vec3 vTarget = _vec3(186.87f, 1.55f, 68.f);

	owner->Get_PlayerTransform()->Goto_Target(vTarget, owner->Get_TimeDelta());

	_vec3 vDistance = vTarget - owner->Get_PlayerTransform()->Get_State(CTransform::STATE_POSITION);

	if (D3DXVec3Length(&vDistance) < 0.1f)
	{
		owner->Get_PlayerTransform()->Set_State(CTransform::STATE_POSITION, vTarget);
		CHANGE_STATE(CStage_RotateDown);
	}
}
EXIT_STATE(CStage_JumpOffTrain, CYoshi_Stage)
{
	owner->Get_OnTrain(true) = false;
	owner->Get_OnTrain(false) = false;
}

//

IMPLEMENT_STATE(CStage_GoToGoalPoint)
ENTER_STATE(CStage_GoToGoalPoint, CYoshi_Stage)
{
	owner->Get_PlayerTransform()->Set_SpeedPerSec(10.f);
	owner->Get_Mesh()->Set_AnimationSpeed(1.5f);
	owner->Get_Mesh()->SetUp_Animation(33);
	owner->Reset_Jump();
}
UPDATE_STATE(CStage_GoToGoalPoint, CYoshi_Stage)
{
	_vec3 vTarget = _vec3(202.5f, 0.29f, 61.25f);
	owner->Get_PlayerTransform()->Goto_Target(vTarget, owner->Get_TimeDelta());

	_vec3 vPos = owner->Get_Position();
	vPos.y += owner->Get_JumpPower() * (_float)owner->Get_TimeDelta();
	owner->Get_JumpPower() -= owner->Get_JumpAccel() * (_float)owner->Get_TimeDelta();

	if (owner->Get_JumpPower() < 0.f)
		owner->Get_Mesh()->SetUp_Animation(31);

	if (vPos.y < 0.29f)
	{
		vPos.y = 0.29f;
		owner->Get_Mesh()->SetUp_Animation(40);
		owner->Get_PlayerTransform()->SetUp_Rotation(_vec3(0.f, 1.f, 0.f), D3DXToRadian(90.f));
	}
	
	_vec3 vDistance = vTarget - vPos;
	if (D3DXVec3Length(&vDistance) < 0.1f)
		CHANGE_STATE(CStage_RotateGoalPoint);
	else
		owner->Get_PlayerTransform()->Set_State(CTransform::STATE_POSITION, vPos);
}
EXIT_STATE(CStage_GoToGoalPoint, CYoshi_Stage)
{
	owner->Get_PlayerTransform()->Set_State(CTransform::STATE_POSITION, _vec3(202.5f, 0.29f, 61.25f));
	owner->Get_PlayerTransform()->Rotation_LookAt(_vec3(202.5f, 0.29f, 60.25f));
	owner->Set_Camera(false); 
}

//

IMPLEMENT_STATE(CStage_RotateGoalPoint)
ENTER_STATE(CStage_RotateGoalPoint, CYoshi_Stage)
{
	owner->Get_PlayerTransform()->Set_SpeedPerSec(1.f);
	owner->Get_Mesh()->Set_AnimationSpeed(1.5f);
	owner->Get_Mesh()->SetUp_Animation(40);
	m_tEnterTime = system_clock::now();
	CSoundMgr::GetInstance()->PlaySound(L"Clear.wav", CSoundMgr::BGM);
}
UPDATE_STATE(CStage_RotateGoalPoint, CYoshi_Stage)
{
	system_clock::duration temp = system_clock::now() - m_tEnterTime;
	if (duration_cast<milliseconds>(temp) > (milliseconds)1700)
		CHANGE_STATE(CStage_YoshiDance);
	//owner->Set_Camera(true);

	//if(owner->Get_PlayerTransform()->Rotation_Degree(owner->Get_TimeDelta(), 0.1, D3DXToRadian(90.f), D3DXToRadian(180.f)))
	//	CHANGE_STATE(CStage_YoshiDance);
}
EXIT_STATE(CStage_RotateGoalPoint, CYoshi_Stage)
{
	//
	owner->Set_Camera(false);
}

//

IMPLEMENT_STATE(CStage_YoshiDance)
ENTER_STATE(CStage_YoshiDance, CYoshi_Stage)
{
	owner->Get_PlayerTransform()->Set_SpeedPerSec(4.f);
	owner->Get_Mesh()->Set_AnimationSpeed(1.9f);
	owner->Get_Mesh()->SetUp_Animation(2);
	owner->Get_PlayerTransform()->SetUp_Rotation(_vec3(0.f, 1.f, 0.f), D3DXToRadian(83.f));
	owner->Add_GoalEffect();
}
UPDATE_STATE(CStage_YoshiDance, CYoshi_Stage)
{
	//if (owner->Check_OnGround())
	//	owner->Set_OnGround();

	if (!m_bSound)
	{
		if (owner->Get_Mesh()->Finish_Animation(2.0))
		{
			CSoundMgr::GetInstance()->StopAll();
			CSoundMgr::GetInstance()->PlayBGM(L"BGM_BeadsGetResult_FIX.wav");
			CManagement* pManagement = CManagement::GetInstance();
			pManagement->Add_GameObjectToLayer(SCENE_TRAIN, L"Layer_End", L"GameObject_CEndingCredit");
			m_bSound = true;
		}
	}
}
EXIT_STATE(CStage_YoshiDance, CYoshi_Stage)
{
}
