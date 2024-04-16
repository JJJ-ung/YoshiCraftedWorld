#include "stdafx.h"
#include "..\Headers\Heyho_State.h"
#include "Mesh_Dynamic.h"
#include "Management.h"
#include "Transform.h"
#include "Collider.h"
#include "Yoshi_Stage.h"

USING(Client)

// Movement //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_STATE(CHeyho_Idle)
ENTER_STATE(CHeyho_Idle, CHeyho)
{
	owner->Get_HeyhoTransform()->Set_SpeedPerSec(1.f);
	owner->Get_Mesh()->Set_AnimationSpeed(1.f);
	owner->Get_Mesh()->SetUp_Animation(0);
}
UPDATE_STATE(CHeyho_Idle, CHeyho)
{
	if (owner->Get_TongueCollision())
		CHANGE_STATE(CHeyho_Eaten);
	if (owner->Get_HeadCollision())
		CHANGE_STATE(CHeyho_HeadHit);
	if (owner->Get_BodyCollision())
		CHANGE_STATE(CHeyho_KnockBack);
	if (owner->Get_EggCollision())
		CHANGE_STATE(CHeyho_Hit);
}
EXIT_STATE(CHeyho_Idle, CHeyho)
{
}

//

IMPLEMENT_STATE(CHeyho_Walk)
ENTER_STATE(CHeyho_Walk, CHeyho)
{
	owner->Get_HeyhoTransform()->Set_SpeedPerSec(0.5f);
	owner->Get_Mesh()->Set_AnimationSpeed(1.f);
	owner->Get_Mesh()->SetUp_Animation(0);
	m_tEnterTime = system_clock::now();
}
UPDATE_STATE(CHeyho_Walk, CHeyho)
{
	system_clock::duration tDuration = system_clock::now() - m_tEnterTime;
	if (duration_cast<seconds>(tDuration) > (seconds)2)
	{
		owner->Get_Mesh()->SetUp_Animation(1);
		owner->Get_HeyhoTransform()->Go_Straight(owner->Get_TimeDelta());
	}

	_vec3 vDistance = owner->Get_StartPos() - owner->Get_HeyhoTransform()->Get_State(CTransform::STATE_POSITION);
	if (D3DXVec3Length(&vDistance) >= 1.3f)
	{
		_vec3 vLook = owner->Get_HeyhoTransform()->Get_State(CTransform::STATE_LOOK);
		owner->Get_StartPos() = owner->Get_HeyhoTransform()->Get_State(CTransform::STATE_POSITION);

		// ¿À¸¥ÂÊ º½
		if (vLook.x >= 0.f)
			CHANGE_STATE(CHeyho_RotateLeft);
		// ¿ÞÂÊ º½
		else
			CHANGE_STATE(CHeyho_RotateRight);
	}

	if (owner->Get_TongueCollision())
		CHANGE_STATE(CHeyho_Eaten);
	if (owner->Get_HeadCollision())
		CHANGE_STATE(CHeyho_HeadHit);
	if (owner->Get_BodyCollision())
		CHANGE_STATE(CHeyho_KnockBack);
	if (owner->Get_EggCollision())
		CHANGE_STATE(CHeyho_Hit);
}
EXIT_STATE(CHeyho_Walk, CHeyho)
{
}

//

IMPLEMENT_STATE(CHeyho_RotateLeft)
ENTER_STATE(CHeyho_RotateLeft, CHeyho)
{
	owner->Get_HeyhoTransform()->Set_SpeedPerSec(1.f);
	owner->Get_Mesh()->Set_AnimationSpeed(1.f);
	owner->Get_Mesh()->SetUp_Animation(0);
}
UPDATE_STATE(CHeyho_RotateLeft, CHeyho)
{
	if (owner->Get_HeyhoTransform()->Rotation_Degree(owner->Get_TimeDelta(), 0.5, D3DXToRadian(90.f), D3DXToRadian(-90.f)))
			CHANGE_STATE(CHeyho_Walk);

	if (owner->Get_TongueCollision())
		CHANGE_STATE(CHeyho_Eaten);
	if (owner->Get_HeadCollision())
		CHANGE_STATE(CHeyho_HeadHit);
	if (owner->Get_BodyCollision())
		CHANGE_STATE(CHeyho_KnockBack);
	if (owner->Get_EggCollision())
		CHANGE_STATE(CHeyho_Hit);
}
EXIT_STATE(CHeyho_RotateLeft, CHeyho)
{
}

//

IMPLEMENT_STATE(CHeyho_RotateRight)
ENTER_STATE(CHeyho_RotateRight, CHeyho)
{
	owner->Get_HeyhoTransform()->Set_SpeedPerSec(1.f);
	owner->Get_Mesh()->Set_AnimationSpeed(1.f);
	owner->Get_Mesh()->SetUp_Animation(0);
}
UPDATE_STATE(CHeyho_RotateRight, CHeyho)
{
	if (owner->Get_HeyhoTransform()->Rotation_Degree(owner->Get_TimeDelta(), 0.5, D3DXToRadian(-90.f), D3DXToRadian(90.f)))
		CHANGE_STATE(CHeyho_Walk);

	if (owner->Get_TongueCollision())
		CHANGE_STATE(CHeyho_Eaten);
	if (owner->Get_HeadCollision())
		CHANGE_STATE(CHeyho_HeadHit);
	if (owner->Get_BodyCollision())
		CHANGE_STATE(CHeyho_KnockBack);
	if (owner->Get_EggCollision())
		CHANGE_STATE(CHeyho_Hit);
}
EXIT_STATE(CHeyho_RotateRight, CHeyho)
{
}

//

IMPLEMENT_STATE(CHeyho_Sleep)
ENTER_STATE(CHeyho_Sleep, CHeyho)
{
	owner->Get_HeyhoTransform()->Set_SpeedPerSec(5.f);
	owner->Get_Mesh()->Set_AnimationSpeed(1.f);
	owner->Get_Mesh()->SetUp_Animation(15);
}
UPDATE_STATE(CHeyho_Sleep, CHeyho)
{
	if (owner->Get_TongueCollision())
		CHANGE_STATE(CHeyho_Eaten);
	if (owner->Get_HeadCollision())
		CHANGE_STATE(CHeyho_HeadHit);
	if (owner->Get_BodyCollision())
		CHANGE_STATE(CHeyho_KnockBack);
	if (owner->Get_EggCollision())
		CHANGE_STATE(CHeyho_Hit);
}
EXIT_STATE(CHeyho_Sleep, CHeyho)
{
}

//

IMPLEMENT_STATE(CHeyho_Look)
ENTER_STATE(CHeyho_Look, CHeyho)
{
	owner->Get_HeyhoTransform()->Set_SpeedPerSec(5.f);
	owner->Get_Mesh()->Set_AnimationSpeed(1.f);
	owner->Get_Mesh()->SetUp_Animation(7);
}
UPDATE_STATE(CHeyho_Look, CHeyho)
{
	if (owner->Get_TongueCollision())
		CHANGE_STATE(CHeyho_Eaten);
	if (owner->Get_HeadCollision())
		CHANGE_STATE(CHeyho_HeadHit);
	if (owner->Get_BodyCollision())
		CHANGE_STATE(CHeyho_KnockBack);
	if (owner->Get_EggCollision())
		CHANGE_STATE(CHeyho_Hit);
}
EXIT_STATE(CHeyho_Look, CHeyho)
{
}

// Jump //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_STATE(CHeyho_Jump)
ENTER_STATE(CHeyho_Jump, CHeyho)
{
	owner->Get_HeyhoTransform()->Set_SpeedPerSec(5.f);
	owner->Get_Mesh()->Set_AnimationSpeed(1.f);
	owner->Get_Mesh()->SetUp_Animation(3);
}
UPDATE_STATE(CHeyho_Jump, CHeyho)
{
	if (owner->Get_TongueCollision())
		CHANGE_STATE(CHeyho_Eaten);
	if (owner->Get_HeadCollision())
		CHANGE_STATE(CHeyho_HeadHit);
	if (owner->Get_BodyCollision())
		CHANGE_STATE(CHeyho_KnockBack);
	if (owner->Get_EggCollision())
		CHANGE_STATE(CHeyho_Hit);
}
EXIT_STATE(CHeyho_Jump, CHeyho)
{
}

// Reaction //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_STATE(CHeyho_Eaten)
ENTER_STATE(CHeyho_Eaten, CHeyho)
{
	owner->Get_HeyhoTransform()->Set_SpeedPerSec(5.f);
	owner->Get_Mesh()->Set_AnimationSpeed(1.f);
	owner->Get_Mesh()->SetUp_Animation(9);
}
UPDATE_STATE(CHeyho_Eaten, CHeyho)
{
	CManagement* pManagement = CManagement::GetInstance();
	Safe_AddRef(pManagement);

	if (owner->Get_MouthCollision())
	{
		CYoshi_Stage* pPlayer = (CYoshi_Stage*)pManagement->Get_ObjectFromLayer(SCENE_TRAIN, L"Layer_Object", 0);
		pPlayer->Get_TongueCollision() = true;
	}

	CCollider* pCollider = (CCollider*)pManagement->Get_Component(SCENE_TRAIN, L"Layer_Tongue", L"Com_Collider");
	_vec3 vPos = pCollider->Get_ColliderPos();
	vPos.y = owner->Get_HeyhoTransform()->Get_State(CTransform::STATE_POSITION).y;
	owner->Get_HeyhoTransform()->Set_State(CTransform::STATE_POSITION, vPos);

	Safe_Release(pManagement);
}
EXIT_STATE(CHeyho_Eaten, CHeyho)
{
}

//

IMPLEMENT_STATE(CHeyho_HeadHit)
ENTER_STATE(CHeyho_HeadHit, CHeyho)
{
	owner->Get_HeyhoTransform()->Set_SpeedPerSec(5.f);
	owner->Get_Mesh()->Set_AnimationSpeed(1.f);
	owner->Get_Mesh()->SetUp_Animation(11);

	_float fScaleX = owner->Get_HeyhoTransform()->Get_Scale(CTransform::STATE_RIGHT);
	_float fScaleY = owner->Get_HeyhoTransform()->Get_Scale(CTransform::STATE_UP);
	_float fScaleZ = owner->Get_HeyhoTransform()->Get_Scale(CTransform::STATE_LOOK);

	vScale = _vec3(fScaleX, fScaleY, fScaleZ);
	CSoundMgr::GetInstance()->PlaySound(L"SD_PON6.wav", CSoundMgr::MONSTER);
}
UPDATE_STATE(CHeyho_HeadHit, CHeyho)
{
	if (!bUp)
	{
		vScale.y -= (_float)owner->Get_TimeDelta() * 5.f;
		if (vScale.y <= 0.3f)
			bUp = true;
	}
	else
	{
		vScale.y += (_float)owner->Get_TimeDelta() * 5.f;
		if (vScale.y >= 0.7f)
		{
			CManagement* pManagement = CManagement::GetInstance();
			Safe_AddRef(pManagement);
			CYoshi_Stage* pPlayer = (CYoshi_Stage*)pManagement->Get_ObjectFromLayer(SCENE_TRAIN, L"Layer_Object", 0);
			pPlayer->Reset_Collider();
			Safe_Release(pManagement);

			bUp = false;
			owner->Get_ObjectDead() = true;
		}
	}

	owner->Get_HeyhoTransform()->SetUp_Scale(vScale);
}
EXIT_STATE(CHeyho_HeadHit, CHeyho)
{
}

//

IMPLEMENT_STATE(CHeyho_Hit)
ENTER_STATE(CHeyho_Hit, CHeyho)
{
	owner->Get_HeyhoTransform()->Set_SpeedPerSec(1.f);
	owner->Get_Mesh()->Set_AnimationSpeed(1.f);
	owner->Get_Mesh()->SetUp_Animation(11);
}
UPDATE_STATE(CHeyho_Hit, CHeyho)
{
	_vec3 vPos = owner->Get_HeyhoTransform()->Get_State(CTransform::STATE_POSITION);
	vPos.y += owner->Get_JumpPower() * (_float)owner->Get_TimeDelta();
	owner->Get_JumpPower() -= owner->Get_JumpAccel() * (_float)owner->Get_TimeDelta();
	owner->Get_HeyhoTransform()->Set_State(CTransform::STATE_POSITION, vPos);
	owner->Get_HeyhoTransform()->Move_Direction(owner->Get_Direction(), owner->Get_TimeDelta());

	if (owner->Get_JumpPower() < -8.f)
		owner->Get_ObjectDead() = true;
}
EXIT_STATE(CHeyho_Hit, CHeyho)
{
}

//

IMPLEMENT_STATE(CHeyho_KnockBack)
ENTER_STATE(CHeyho_KnockBack, CHeyho)
{
	CManagement* pManagetment = CManagement::GetInstance();
	Safe_AddRef(pManagetment);
	pPlayerTransform = (CTransform*)pManagetment->Get_Component(SCENE_TRAIN, L"Layer_Object", L"Com_Transform");
	Safe_Release(pManagetment);

	owner->Get_HeyhoTransform()->Set_SpeedPerSec(0.5f);
	owner->Get_Mesh()->Set_AnimationSpeed(1.f);
	owner->Get_Mesh()->SetUp_Animation(12);
}
UPDATE_STATE(CHeyho_KnockBack, CHeyho)
{
	_vec3 vDir = pPlayerTransform->Get_State(CTransform::STATE_LOOK);
	owner->Get_HeyhoTransform()->Move_Direction(vDir, owner->Get_TimeDelta());

	_tchar m_szData01[255];
	GetPrivateProfileString(_T("Heyho"), _T("KnockBack"), NULL, m_szData01, 255, _T("../Bin/Data/Information/Monster_Animation.ini"));
	_double KnockBack = _tstof(m_szData01);
	if (owner->Get_Mesh()->Finish_Animation(KnockBack))
		CHANGE_STATE(CHeyho_Idle);
}
EXIT_STATE(CHeyho_KnockBack, CHeyho)
{
}

//