#pragma once
#include "State.h"
#include "Yoshi_Stage.h"

BEGIN(Engine)
class CInput_Device;
END

BEGIN(Client)
class CYoshi_Stage;

/* Game Start */

class CStage_IdleBase : public CState<CYoshi_Stage>
{
	DECLARE_STATE(CStage_IdleBase, CYoshi_Stage)
};

class CStage_GameStart : public CState<CYoshi_Stage>
{
	DECLARE_STATE(CStage_GameStart, CYoshi_Stage)
};

/* Idle */

class CStage_Idle : public CState<CYoshi_Stage>
{
	DECLARE_STATE(CStage_Idle, CYoshi_Stage)
private:
	system_clock::time_point m_EnterTime;
};

class CStage_Idle_Play : public CState<CYoshi_Stage>
{
	DECLARE_STATE(CStage_Idle_Play, CYoshi_Stage)
};

/* Rotation */

class CStage_RotateRight : public CState<CYoshi_Stage>
{
	DECLARE_STATE(CStage_RotateRight, CYoshi_Stage)
};

class CStage_RotateLeft : public CState<CYoshi_Stage>
{
	DECLARE_STATE(CStage_RotateLeft, CYoshi_Stage)
};

class CStage_RotateUp : public CState<CYoshi_Stage>
{
	DECLARE_STATE(CStage_RotateUp, CYoshi_Stage)
};

class CStage_RotateDown : public CState<CYoshi_Stage>
{
	DECLARE_STATE(CStage_RotateDown, CYoshi_Stage)
};

/* Walk & Run */

class CStage_WalkIn : public CState<CYoshi_Stage>
{
	DECLARE_STATE(CStage_WalkIn, CYoshi_Stage)
};

class CStage_WalkOut : public CState<CYoshi_Stage>
{
	DECLARE_STATE(CStage_WalkOut, CYoshi_Stage)
};

class CStage_Run : public CState<CYoshi_Stage>
{
	DECLARE_STATE(CStage_Run, CYoshi_Stage)
};

/* Sit */

class CStage_Sit : public CState<CYoshi_Stage>
{
	DECLARE_STATE(CStage_Sit, CYoshi_Stage)
};

class CStage_SitWalk : public CState<CYoshi_Stage>
{
	DECLARE_STATE(CStage_SitWalk, CYoshi_Stage)
};

class CStage_SitRotationLeft : public CState<CYoshi_Stage>
{
	DECLARE_STATE(CStage_SitRotationLeft, CYoshi_Stage)
};

class CStage_SitRotationRight : public CState<CYoshi_Stage>
{
	DECLARE_STATE(CStage_SitRotationRight, CYoshi_Stage)
};

/* Tongue */

class CStage_TongueAttackIn : public CState<CYoshi_Stage>
{
	DECLARE_STATE(CStage_TongueAttackIn, CYoshi_Stage)
};
 
class CStage_TongueAttackOut : public CState<CYoshi_Stage>
{
	DECLARE_STATE(CStage_TongueAttackOut, CYoshi_Stage)
};

/* Release */

class CStage_Release : public CState<CYoshi_Stage>
{
	DECLARE_STATE(CStage_Release, CYoshi_Stage)
};

class CStage_LayEgg : public CState<CYoshi_Stage>
{
	DECLARE_STATE(CStage_LayEgg, CYoshi_Stage)
};

/* Throw */

class CStage_ThrowIdle : public CState<CYoshi_Stage>
{
	DECLARE_STATE(CStage_ThrowIdle, CYoshi_Stage)
};

class CStage_Throw : public CState<CYoshi_Stage>
{
	DECLARE_STATE(CStage_Throw, CYoshi_Stage)
};

class CStage_ThrowRotateTarget : public CState<CYoshi_Stage>
{
	DECLARE_STATE(CStage_ThrowRotateTarget, CYoshi_Stage)
private:
	_vec3 vStartLook;
	_float fProgress;
};

/* Jump */

class CStage_JumpStart : public CState<CYoshi_Stage>
{
	DECLARE_STATE(CStage_JumpStart, CYoshi_Stage)
};

class CStage_Jump : public CState<CYoshi_Stage>
{
	DECLARE_STATE(CStage_Jump, CYoshi_Stage)
};

class CStage_Hovering : public CState<CYoshi_Stage>
{
	DECLARE_STATE(CStage_Hovering, CYoshi_Stage)
private:
	_float m_fRadian = 0.f;
};

class CStage_JumpFall : public CState<CYoshi_Stage>
{
	DECLARE_STATE(CStage_JumpFall, CYoshi_Stage)
private:
	system_clock::time_point m_EnterTime;
};

class CStage_JumpEnd : public CState<CYoshi_Stage>
{
	DECLARE_STATE(CStage_JumpEnd, CYoshi_Stage)
};

/* Jump Rotation */

class CStage_JumpRotationLeft : public CState<CYoshi_Stage>
{
	DECLARE_STATE(CStage_JumpRotationLeft, CYoshi_Stage)
};

class CStage_JumpRotationRight : public CState<CYoshi_Stage>
{
	DECLARE_STATE(CStage_JumpRotationRight, CYoshi_Stage)
};

class CStage_HoveringRotationLeft : public CState<CYoshi_Stage>
{
	DECLARE_STATE(CStage_HoveringRotationLeft, CYoshi_Stage)
};

class CStage_HoveringRotationRight : public CState<CYoshi_Stage>
{
	DECLARE_STATE(CStage_HoveringRotationRight, CYoshi_Stage)
};

/* Hip Push */

class CStage_HipPushStart : public CState<CYoshi_Stage>
{
	DECLARE_STATE(CStage_HipPushStart, CYoshi_Stage)
};

class CStage_HipPush : public CState<CYoshi_Stage>
{
	DECLARE_STATE(CStage_HipPush, CYoshi_Stage)
private:
	_float m_fPower;
};

class CStage_HipPushImpact : public CState<CYoshi_Stage>
{
	DECLARE_STATE(CStage_HipPushImpact, CYoshi_Stage)
};

class CStage_HipPushEnd : public CState<CYoshi_Stage>
{
	DECLARE_STATE(CStage_HipPushEnd, CYoshi_Stage)
};

/* Reactions */

class CStage_KnockBack : public CState<CYoshi_Stage>
{
	DECLARE_STATE(CStage_KnockBack, CYoshi_Stage)
};

class CStage_JumpToTrain : public CState<CYoshi_Stage>
{
	DECLARE_STATE(CStage_JumpToTrain, CYoshi_Stage)
private:
	system_clock::time_point m_EnterTime;
};

class CStage_WaitTrain : public CState<CYoshi_Stage>
{
	DECLARE_STATE(CStage_WaitTrain, CYoshi_Stage)
private:
	system_clock::time_point m_EnterTime;
};

class CStage_OnTrain : public CState<CYoshi_Stage>
{
	DECLARE_STATE(CStage_OnTrain, CYoshi_Stage)
private:
	system_clock::time_point m_EnterTime;
};

class CStage_JumpOffTrain : public CState<CYoshi_Stage>
{
	DECLARE_STATE(CStage_JumpOffTrain, CYoshi_Stage)
};

class CStage_GoToGoalPoint : public CState<CYoshi_Stage>
{
	DECLARE_STATE(CStage_GoToGoalPoint, CYoshi_Stage)
};

class CStage_RotateGoalPoint : public CState<CYoshi_Stage>
{
	DECLARE_STATE(CStage_RotateGoalPoint, CYoshi_Stage)
private:
	system_clock::time_point m_tEnterTime;
};

class CStage_YoshiDance : public CState<CYoshi_Stage>
{
	DECLARE_STATE(CStage_YoshiDance, CYoshi_Stage)
private:
	_bool m_bSound = false;
};

END