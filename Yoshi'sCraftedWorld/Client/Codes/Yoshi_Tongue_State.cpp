#include "stdafx.h"
#include "..\Headers\Yoshi_Tongue_State.h"
#include "Mesh_Dynamic.h"
#include "Transform.h"

USING(Client)

IMPLEMENT_STATE(CTongue_Idle)
ENTER_STATE(CTongue_Idle, CYoshi_Tongue)
{
	owner->Get_Mesh()->SetUp_Animation(1);
	owner->Get_Render() = false;
}
UPDATE_STATE(CTongue_Idle, CYoshi_Tongue)
{
}
EXIT_STATE(CTongue_Idle, CYoshi_Tongue)
{

}

IMPLEMENT_STATE(CTongue_AttackIn)
ENTER_STATE(CTongue_AttackIn, CYoshi_Tongue)
{
	owner->Get_Mesh()->SetUp_Animation(0);
	owner->Get_Render() = true;
}
UPDATE_STATE(CTongue_AttackIn, CYoshi_Tongue)
{
	if (owner->Get_Mesh()->Finish_Animation())
		CHANGE_STATE(CTongue_AttackIdle);
}
EXIT_STATE(CTongue_AttackIn, CYoshi_Tongue)
{

}

IMPLEMENT_STATE(CTongue_AttackIdle)
ENTER_STATE(CTongue_AttackIdle, CYoshi_Tongue)
{
	owner->Get_Mesh()->SetUp_Animation(1);
	owner->Get_Render() = true;
}
UPDATE_STATE(CTongue_AttackIdle, CYoshi_Tongue)
{
}
EXIT_STATE(CTongue_AttackIdle, CYoshi_Tongue)
{

}

IMPLEMENT_STATE(CTongue_AttackOut)
ENTER_STATE(CTongue_AttackOut, CYoshi_Tongue)
{
	owner->Get_Mesh()->SetUp_Animation(2);
	owner->Get_Render() = true;
}
UPDATE_STATE(CTongue_AttackOut, CYoshi_Tongue)
{
}
EXIT_STATE(CTongue_AttackOut, CYoshi_Tongue)
{

}
