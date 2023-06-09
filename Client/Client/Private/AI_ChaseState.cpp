#include "stdafx.h"
#include "..\Public\AI_ChaseState.h"
#include "GameInstance.h"
#include "Alphen.h"
#include "Sion.h"
#include "Rinwell.h"
#include "Law.h"
#include "AIAttackNormalState.h"
#include "AI_Alphen_NormalAttackState.h"
#include "AI_Alphen_SkillAttackState.h"
#include "AICheckState.h"
#include "AI_Sion_SkillState.h"
#include "AI_DodgeState.h"
#include "AI_LAW_NomalAttack_State.h"
#include "AI_Rinwell_SkillState.h"
#include "AI_LAW_SkillAttack_State.h"
#include "AI_JumpState.h"


using namespace AIPlayer;

CAI_ChaseState::CAI_ChaseState(CPlayer* pPlayer, STATE_ID eStateType, _uint playerid , CBaseObj* pTarget)
{
	//m_ePreStateID = eStateType;
	m_pOwner = pPlayer;
	m_eCurrentPlayerID = playerid;
	if (nullptr == pTarget)
	{
		m_pTarget = CBattleManager::Get_Instance()->Get_MinDistance_Monster(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	}
	m_pTarget = pTarget;
}

CAIState * CAI_ChaseState::Tick(_float fTimeDelta)
{
	m_fCuttimer += fTimeDelta;
	m_fbreaktimer += fTimeDelta;
	if (m_eCurrentPlayerID == CPlayer::ALPHEN || m_eCurrentPlayerID == CPlayer::LAW)
	{
		if (CheckTarget() == false)
			return nullptr;
	}
	else
	{
		if (CBattleManager::Get_Instance()->IsAllMonsterDead())
			return nullptr;

		CBattleManager* pBattleManager = GET_INSTANCE(CBattleManager);
		CBaseObj* pLockOn = pBattleManager->Get_LackonMonster();
		if (nullptr != pLockOn)
			m_pTarget = pLockOn;
		else
			m_pTarget = pBattleManager->Get_MinDistance_Monster(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

		RELEASE_INSTANCE(CBattleManager);

		if (m_pTarget == nullptr)
			return nullptr;
	}
		
	

	
	if (m_bStopRunning)
	{
		m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN");

		if (!m_bIsAnimationFinished)
		{
			_vector vecTranslation;
			_float fRotationRadian;
			m_pOwner->Get_Model()->Get_MoveTransformationMatrix("TransN", &vecTranslation, &fRotationRadian);
			m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.01f), fRotationRadian, m_pOwner->Get_Navigation());
			//m_pOwner->Get_Transform()->Go_Straight(fTimeDelta, m_pOwner->Get_Navigation());
		}

	}
	else
	{
		m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN");
		if (nullptr == m_pTarget)
		{
			m_pTarget = CBattleManager::Get_Instance()->Get_MinDistance_Monster(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
			m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
		}
		m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
		m_pOwner->Get_Transform()->Sliding_Straight(fTimeDelta*2.f, m_pOwner->Get_Navigation());
	}


	m_pOwner->Check_Navigation();


	return nullptr;
}

CAIState * CAI_ChaseState::LateTick(_float fTimeDelta)
{
	if (m_eCurrentPlayerID == CPlayer::ALPHEN || m_eCurrentPlayerID == CPlayer::LAW)
	{
		if (CheckTarget() == false)
			return nullptr;
	}
	else
	{
		if (CBattleManager::Get_Instance()->IsAllMonsterDead())
			return nullptr;

		CBattleManager* pBattleManager = GET_INSTANCE(CBattleManager);
		CBaseObj* pLockOn = pBattleManager->Get_LackonMonster();
		if (nullptr != pLockOn)
			m_pTarget = pLockOn;
		else
			m_pTarget = pBattleManager->Get_MinDistance_Monster(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

		RELEASE_INSTANCE(CBattleManager);

		if (m_pTarget == nullptr)
			return nullptr;
	}

	if (!m_bStopRunning)
	{
		switch (m_eCurrentPlayerID)
		{
		case CPlayer::ALPHEN:
			if (Get_Target_Distance() <= 6.5f || m_fCuttimer > 3.f)
			{
				__super::Exit();
				m_iCurrentAnimIndex = CAlphen::ANIM::ANIM_BATTLE_MOVE_BREAK;
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);
				m_bStopRunning = true;
			}
			
			break;

		case CPlayer::SION:
			if (Get_Target_Distance() <= 11.f || m_fCuttimer > 3.f)
			{
				__super::Exit();
				m_iCurrentAnimIndex = CSion::ANIM::BTL_MOVE_BRAKE;
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);
				m_bStopRunning = true;
			}
			break;

		case CPlayer::RINWELL:
			if (Get_Target_Distance() <= 11.f || m_fCuttimer > 3.f)
			{
				__super::Exit();
				m_iCurrentAnimIndex = CRinwell::ANIM::DASH_BRAKE_000;
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);
				m_bStopRunning = true;
			}
			break;

		case CPlayer::LAW:
			if (Get_Target_Distance() <= 6.5f || m_fCuttimer > 3.f)
			{
				__super::Exit();
				m_iCurrentAnimIndex = CLaw::ANIM::BTL_MOVE_BRAKE;
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);
				m_bStopRunning = true;
			}

			break;




		}



	}

	if (m_fbreaktimer > 0.5f && m_bStopRunning)
	{
		if (m_eCurrentPlayerID == CPlayer::ALPHEN)
		{
			if (nullptr == m_pTarget)
			{
				m_pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
				(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
			}

			if (m_pOwner->Get_Info().fCurrentMp > 1.f)
			{
				switch (rand() % 4)
				{


				case 0:
					return new CAI_Alphen_SkillAttackState(m_pOwner, STATE_ATTACK, m_pTarget, CAlphen::ANIM::ANIM_ATTACK_HIENZIN);

				case 1:
					return new CAI_Alphen_SkillAttackState(m_pOwner, STATE_ATTACK, m_pTarget, CAlphen::ANIM::ANIM_ATTACK_AKIZAME);

				case 2:
					return new CAI_Alphen_SkillAttackState(m_pOwner, STATE_ATTACK, m_pTarget, CAlphen::ANIM::ANIM_ATTACK_HOUSYUTIGAKUZIN);

				case 3:
					return new CAI_JumpState(m_pOwner, STATETYPE_START, true);

				}
			}
			else
				return new CAI_Alphen_NormalAttackState(m_pOwner, STATE_ATTACK, m_pTarget);
		}

		else if (m_eCurrentPlayerID == CPlayer::SION)
		{
			if (m_pOwner->Get_Info().fCurrentMp < 1)
			{
				switch (rand() % 2)
				{
				case 0:
					return new CAI_DodgeState(m_pOwner, m_pTarget);	//return new CAIAttackNormalState(m_pOwner, STATE_ATTACK, m_pTarget);
					break;
				case 1:
					return new CAICheckState(m_pOwner, m_eStateId);
					break;
					//	case 2:


				}
			}
			else
			{
				//switch (rand() % 7)
				//{

				//case 0:
				//	//return new CAIAttackNormalState(m_pOwner, STATE_ATTACK, m_pTarget);
				//	break;
				//case 1:
				//	//return new CAI_Sion_SkillState(m_pOwner, STATE_ATTACK, m_pTarget, CSion::ANIM::BTL_ATTACK_GRAVITY_FORCE);
				//	break;
				//case 2:
				//	//return new CAI_Sion_SkillState(m_pOwner, STATE_ATTACK, m_pTarget, CSion::ANIM::BTL_ATTACK_MAGNARAY);
				//	break;
				//case 3:
				//	//return new CAI_Sion_SkillState(m_pOwner, STATE_ATTACK, m_pTarget, CSion::ANIM::BTL_ATTACK_BRAVE);
				//	break;
				//case 4:
				//	//return new CAI_Sion_SkillState(m_pOwner, STATE_ATTACK, m_pTarget, CSion::ANIM::BTL_ATTACK_CRESCENT_BULLET);
				//	break;
				//case 5:
				//	//return new CAI_Sion_SkillState(m_pOwner, STATE_ATTACK, m_pTarget, CSion::ANIM::BTL_ATTACK_THUNDER_BOLT);
				//	break;
				//case 6:
				return new CAI_JumpState(m_pOwner, STATETYPE_START, true);

			}
		}


	}

	else if (m_eCurrentPlayerID == CPlayer::RINWELL)
	{
		if (m_pOwner->Get_Info().fCurrentMp < 1.f)
		{
			/*switch (rand() % 2)
			{
			case 0:*/
			return new CAI_DodgeState(m_pOwner, m_pTarget, true);
			//case 1:
			//	//return new CAIAttackNormalState(m_pOwner, STATE_ATTACK, m_pTarget);
			//	break;
			//}


		}
		else
		{
			switch (rand() % 2)
			{


			case 0:
				return new CAI_DodgeState(m_pOwner, m_pTarget);

			case 1:
				return new CAI_JumpState(m_pOwner, STATETYPE_START, true);



			}
			return nullptr;
		}

	}

	else if (m_eCurrentPlayerID == CPlayer::LAW)
	{
		if (CheckTarget() == false)
			return nullptr;

		if (m_pOwner->Get_Info().fCurrentMp < 1.f)
		{
			switch (rand() % 2)
			{
			case 0:
				return new CAI_DodgeState(m_pOwner, m_pTarget);
				break;
			case  1:
				return new AI_LAW_NomalAttack_State(m_pOwner, STATE_NORMAL_ATTACK1, m_pTarget);
				break;
			}

		}


		switch (rand() % 6)
		{

		case 0:
			return new CAI_LAW_SkillAttack_State(m_pOwner, STATE_SKILL_ATTACK_R);

		case 1:
			return new CAI_LAW_SkillAttack_State(m_pOwner, STATE_SKILL_ATTACK_E);

		case 2:
			return new CAI_LAW_SkillAttack_State(m_pOwner, STATE_SKILL_ATTACK_F);

		case 3:
			return new AI_LAW_NomalAttack_State(m_pOwner, STATE_NORMAL_ATTACK1, m_pTarget);
		case 4:
			return new CAI_DodgeState(m_pOwner, m_pTarget);
		case 5:
			return new CAI_JumpState(m_pOwner, STATETYPE_START, true);


		}
	}
	else
	{
		vector<ANIMEVENT> pEvents = m_pOwner->Get_Model()->Get_Events();
		if (m_bStopRunning)
		{
			for (auto& pEvent : pEvents)
			{
				if (pEvent.isPlay)
				{
					if (ANIMEVENT::EVENTTYPE::EVENT_STATE == pEvent.eType)
					{

						if (m_eCurrentPlayerID == CPlayer::ALPHEN)
						{
							if (nullptr == m_pTarget)
							{
								m_pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
								(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
							}

							if (m_pOwner->Get_Info().fCurrentMp > 1.f)
							{
								switch (rand() % 4)
								{


								case 0:
									return new CAI_Alphen_SkillAttackState(m_pOwner, STATE_ATTACK, m_pTarget, CAlphen::ANIM::ANIM_ATTACK_HIENZIN);

								case 1:
									return new CAI_Alphen_SkillAttackState(m_pOwner, STATE_ATTACK, m_pTarget, CAlphen::ANIM::ANIM_ATTACK_AKIZAME);

								case 2:
									return new CAI_Alphen_SkillAttackState(m_pOwner, STATE_ATTACK, m_pTarget, CAlphen::ANIM::ANIM_ATTACK_HOUSYUTIGAKUZIN);

								case 3:
									return new CAI_JumpState(m_pOwner, STATETYPE_START, true);

								}
							}
							else
								return new CAI_Alphen_NormalAttackState(m_pOwner, STATE_ATTACK, m_pTarget);
						}

						else if (m_eCurrentPlayerID == CPlayer::SION)
						{
							if (m_pOwner->Get_Info().fCurrentMp < 1)
							{
								switch (rand() % 2)
								{
								case 0:
									return new CAI_DodgeState(m_pOwner, m_pTarget);	//return new CAIAttackNormalState(m_pOwner, STATE_ATTACK, m_pTarget);
									break;
								case 1:
									return new CAICheckState(m_pOwner, m_eStateId);
									break;
									//	case 2:


								}
							}
							else
							{
								//switch (rand() % 7)
								//{

								//case 0:
								//	//return new CAIAttackNormalState(m_pOwner, STATE_ATTACK, m_pTarget);
								//	break;
								//case 1:
								//	//return new CAI_Sion_SkillState(m_pOwner, STATE_ATTACK, m_pTarget, CSion::ANIM::BTL_ATTACK_GRAVITY_FORCE);
								//	break;
								//case 2:
								//	//return new CAI_Sion_SkillState(m_pOwner, STATE_ATTACK, m_pTarget, CSion::ANIM::BTL_ATTACK_MAGNARAY);
								//	break;
								//case 3:
								//	//return new CAI_Sion_SkillState(m_pOwner, STATE_ATTACK, m_pTarget, CSion::ANIM::BTL_ATTACK_BRAVE);
								//	break;
								//case 4:
								//	//return new CAI_Sion_SkillState(m_pOwner, STATE_ATTACK, m_pTarget, CSion::ANIM::BTL_ATTACK_CRESCENT_BULLET);
								//	break;
								//case 5:
								//	//return new CAI_Sion_SkillState(m_pOwner, STATE_ATTACK, m_pTarget, CSion::ANIM::BTL_ATTACK_THUNDER_BOLT);
								//	break;
								//case 6:
								return new CAI_JumpState(m_pOwner, STATETYPE_START, true);

							}
						}


					}

					else if (m_eCurrentPlayerID == CPlayer::RINWELL)
					{
						if (m_pOwner->Get_Info().fCurrentMp < 1.f)
						{
							/*switch (rand() % 2)
							{
							case 0:*/
							return new CAI_DodgeState(m_pOwner, m_pTarget, true);
							//case 1:
							//	//return new CAIAttackNormalState(m_pOwner, STATE_ATTACK, m_pTarget);
							//	break;
							//}


						}
						else
						{
							switch (rand() % 2)
							{


							case 0:
								return new CAI_DodgeState(m_pOwner, m_pTarget);

							case 1:
								return new CAI_JumpState(m_pOwner, STATETYPE_START, true);



							}
							return nullptr;
						}

					}

					else if (m_eCurrentPlayerID == CPlayer::LAW)
					{
						if (CheckTarget() == false)
							return nullptr;

						if (m_pOwner->Get_Info().fCurrentMp < 1.f)
						{
							switch (rand() % 2)
							{
							case 0:
								return new CAI_DodgeState(m_pOwner, m_pTarget);
								break;
							case  1:
								return new AI_LAW_NomalAttack_State(m_pOwner, STATE_NORMAL_ATTACK1, m_pTarget);
								break;
							}

						}


						switch (rand() % 6)
						{

						case 0:
							return new CAI_LAW_SkillAttack_State(m_pOwner, STATE_SKILL_ATTACK_R);

						case 1:
							return new CAI_LAW_SkillAttack_State(m_pOwner, STATE_SKILL_ATTACK_E);

						case 2:
							return new CAI_LAW_SkillAttack_State(m_pOwner, STATE_SKILL_ATTACK_F);

						case 3:
							return new AI_LAW_NomalAttack_State(m_pOwner, STATE_NORMAL_ATTACK1, m_pTarget);
						case 4:
							return new CAI_DodgeState(m_pOwner, m_pTarget);
						case 5:
							return new CAI_JumpState(m_pOwner, STATETYPE_START, true);


						}
					}

				}


			}


		}
	}
		
	
		
		
		
	


	
	


	return nullptr;
}

void CAI_ChaseState::Enter()
{
	m_eStateId = STATE_ID::STATE_RUN;

	switch (m_eCurrentPlayerID)
	{
	case CPlayer::ALPHEN:
		m_iCurrentAnimIndex = CAlphen::ANIM::ANIM_BATTLE_MOVE_RUN;
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);
		break;

	case CPlayer::SION:
		m_iCurrentAnimIndex = CSion::ANIM::BTL_MOVE_RUN;
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);
		break;

	case CPlayer::RINWELL:
		m_iCurrentAnimIndex = CRinwell::ANIM::BTL_MOVE_RUN;
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);
		break;

	case CPlayer::LAW:
		m_iCurrentAnimIndex = CLaw::ANIM::BTL_MOVE_RUN;
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);
		break;



	}

	m_pOwner->Set_Manarecover(true);

	
}









//RinwellTransform->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), 1.f)


void CAI_ChaseState::Exit()
{
	__super::Exit();
}

void CAI_ChaseState::ChaseTarget(_float fTimeDelta)
{
	m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);
	m_pOwner->Get_Transform()->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), 2.f);

	_vector vTargetDir = XMVector3Normalize(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION)
		- m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	_vector vLook = XMVector3Normalize(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_LOOK));

	vLook = XMVectorSetY(vLook, 0.f);
	vTargetDir = XMVectorSetY(vTargetDir, 0.f);

	_float fDot = XMVectorGetX(XMVector3Dot(vTargetDir, vLook));

	if (fDot < 0.7f)
		m_pOwner->Get_Transform()->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), 0.05f);

	m_pOwner->Get_Transform()->Sliding_Straight(fTimeDelta * 2, m_pOwner->Get_Navigation());
}
