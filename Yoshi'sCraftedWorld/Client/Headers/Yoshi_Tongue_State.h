#pragma once
#include "State.h"
#include "Yoshi_Tongue.h"

BEGIN(Engine)
class CInput_Device;
END

BEGIN(Client)
class CYoshi_Tongue;

class CTongue_Idle : public CState<CYoshi_Tongue>
{
	DECLARE_STATE(CTongue_Idle, CYoshi_Tongue)
};

class CTongue_AttackIn : public CState<CYoshi_Tongue>
{
	DECLARE_STATE(CTongue_AttackIn, CYoshi_Tongue)
};

class CTongue_AttackIdle : public CState<CYoshi_Tongue>
{
	DECLARE_STATE(CTongue_AttackIdle, CYoshi_Tongue)
};

class CTongue_AttackOut : public CState<CYoshi_Tongue>
{
	DECLARE_STATE(CTongue_AttackOut, CYoshi_Tongue)
};

END