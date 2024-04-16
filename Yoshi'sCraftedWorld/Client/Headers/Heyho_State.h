#pragma once
#include "State.h"
#include "Heyho.h"

BEGIN(Engine)
class CInput_Device;
END

BEGIN(Client)
class CHeyho;

// Movement
class CHeyho_Idle : public CState<CHeyho>
{
	DECLARE_STATE(CHeyho_Idle, CHeyho)
};

class CHeyho_Walk : public CState<CHeyho>
{
	DECLARE_STATE(CHeyho_Walk, CHeyho)
private:
	system_clock::time_point m_tEnterTime;
};

class CHeyho_RotateLeft : public CState<CHeyho>
{
	DECLARE_STATE(CHeyho_RotateLeft, CHeyho)
};

class CHeyho_RotateRight : public CState<CHeyho>
{
	DECLARE_STATE(CHeyho_RotateRight, CHeyho)
};

class CHeyho_Jump : public CState<CHeyho>
{
	DECLARE_STATE(CHeyho_Jump, CHeyho)
};

class CHeyho_Sleep : public CState<CHeyho>
{
	DECLARE_STATE(CHeyho_Sleep, CHeyho)
};

class CHeyho_Look : public CState<CHeyho>
{
	DECLARE_STATE(CHeyho_Look, CHeyho)
};

// Reaction
class CHeyho_Eaten : public CState<CHeyho>
{
	DECLARE_STATE(CHeyho_Eaten, CHeyho)
};

class CHeyho_HeadHit : public CState<CHeyho>
{
	DECLARE_STATE(CHeyho_HeadHit, CHeyho)
private:
	_vec3 vScale;
	_bool bUp = false;
};

class CHeyho_Hit : public CState<CHeyho>
{
	DECLARE_STATE(CHeyho_Hit, CHeyho)
};

class CHeyho_KnockBack : public CState<CHeyho>
{
	DECLARE_STATE(CHeyho_KnockBack, CHeyho)
private:
	CTransform* pPlayerTransform = nullptr;
};

END