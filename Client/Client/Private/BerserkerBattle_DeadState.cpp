#include "stdafx.h"

#include "BerserkerBattle_DeadState.h"
#include "GameInstance.h"
#include "BerserkerBattle_Double_CrowState.h"
#include "BerserkerBattle_Double_ClawState.h"
#include "BerserkerBattle_Shock_WaveState.h"

using namespace Berserker;

CBattle_DeadState::CBattle_DeadState(CBerserker* pBerserker)
{
	m_pOwner = pBerserker;
}

CBerserkerState * CBattle_DeadState::AI_Behaviour(_float fTimeDelta)
{
	
	return nullptr;
}

CBerserkerState * CBattle_DeadState::Tick(_float fTimeDelta)
{


	if(false == m_bDeadAnimFinish)
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta*2.f, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");
	


	return nullptr;
}

CBerserkerState * CBattle_DeadState::LateTick(_float fTimeDelta)
{
	
	m_pOwner->Check_Navigation();

	if (m_bIsAnimationFinished && false == m_bDeadAnimFinish)
	{
		CCollision_Manager* pCollisionMgr = GET_INSTANCE(CCollision_Manager);

		pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_pAtkColliderCom);
		m_pAtkColliderCom = nullptr;

		pCollisionMgr->Out_CollisionGroup(CCollision_Manager::COLLISION_MBULLET, m_pOwner);

		RELEASE_INSTANCE(CCollision_Manager);

		m_bDeadAnimFinish = true;
		m_pOwner->Set_GlowUp();

	}

	return nullptr;

}

void CBattle_DeadState::Enter()
{
	m_eStateId = STATE_ID::STATE_IDLE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CBerserker::ANIM::DEAD);

	
	CGameInstance::Get_Instance()->PlaySounds(TEXT("Berserker_Dead.wav"), SOUND_VOICE, 0.5f);
}

void CBattle_DeadState::Exit()
{
	CGameInstance::Get_Instance()->StopSound(SOUND_VOICE);
}