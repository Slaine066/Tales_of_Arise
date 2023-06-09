#pragma once

#include "Player.h"
#include "Ice_Wolf.h"
#include "Client_Defines.h"
#include "PlayerManager.h"

BEGIN(Client)
class CIceWolfState
{
public:

	enum FIELD_STATE_ID
	{
		FIELD_STATE_IDLE,
		STATE_WALK,
		STATE_TURN_L,
		STATE_TURN,
		STATE_TRIGGER_TURN,
		STATE_HOWLING,
		FIELD_STATE_END,
	};



	enum STATE_ID
	{
		START_BATTLE,
		STATE_BATTLEING,
		STATE_IDLE,
		STATE_RUN,
		STATE_NORMAL_ATK,
		STATE_MOVE,
		STATE_DISCOVER,
		STATE_ELEMENTAL_CHARGE,
		STATE_SOMESAULT,
		STATE_BITE,
		STATE_BACKSTEP,
		STATE_CHARGE_START,
		STATE_CHARGE_LOOP,
		STATE_CHARGE_END,
		STATE_BE_DAMAGED,
		STATE_DOWN,
		STATE_ARISE,
		STATE_DEAD,
		STATE_END
	};



	virtual ~CIceWolfState() {};
	virtual CIceWolfState* AI_Behaviour(_float fTimeDelta) { return nullptr; };
	virtual CIceWolfState* Tick(_float fTimeDelta) PURE;
	virtual CIceWolfState* LateTick(_float fTimeDelta) PURE;

	virtual void Enter() PURE;
	virtual void Exit() PURE;

	CIceWolfState* ChangeState(CIceWolfState* pCurrentState, CIceWolfState* pNewState)
	{
		if (pCurrentState)
		{
			pCurrentState->Exit();
			Safe_Delete(pCurrentState);
		}

		pCurrentState = pNewState;
		pCurrentState->Enter();

		return pCurrentState;
	}

public:
	STATE_ID Get_StateId() { return m_eStateId; }
	_bool Has_Aggro() { return m_pOwner->Get_Aggro(); }
	void Reset_Target() { m_pTarget = nullptr; }
	void Set_BattleMode() { m_bBattleMode = true; }

protected:
	void Find_Target_InField(void)
	{
		CPlayer* pPlayer = CPlayerManager::Get_Instance()->Get_ActivePlayer();
		if (!pPlayer)
			return;

		_vector vPlayerPosition = pPlayer->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);
		_vector vPosition = m_pOwner->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);

		_float fDistance = XMVectorGetX(XMVector3Length(vPlayerPosition - vPosition));
		
		if (fDistance < m_pOwner->Get_AggroRadius())
		{
			m_pTarget = pPlayer;

			m_pOwner->Get_Transform()->Change_Speed(m_pOwner->Get_Stats().m_fRunSpeed);

			if (5 > fDistance)
				m_bBattleMode = true;
		}

		else
			m_pTarget = nullptr;

		m_fTarget_Distance = fDistance;
	}



	virtual void Find_Target(_bool bHasAggro = false)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		CGameObject* pGameObject = pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player"));
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(pGameObject);
		if (!pPlayer)
			return;

		if (bHasAggro)
			m_pTarget = pPlayer;
		else
		{
			_vector vPlayerPosition = pPlayer->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);
			_vector vPosition = m_pOwner->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);

			_float fDistance = XMVectorGetX(XMVector3Length(vPlayerPosition - vPosition));
			if (fDistance < m_pOwner->Get_AggroRadius())
			{
				m_pTarget = pPlayer;

				m_pOwner->Get_Transform()->Change_Speed(m_pOwner->Get_Stats().m_fRunSpeed);

				if (5 > fDistance)
				{
					m_bBattleMode = true;

				}

			}
		}
	}

	virtual _float Find_BattleTarget()
	{
		CPlayer* pPlayer = CPlayerManager::Get_Instance()->Get_ActivePlayer();

		m_pTarget = pPlayer;
		_vector vPlayerPosition = pPlayer->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);
		_vector vPosition = m_pOwner->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);

		_float fDistance = XMVectorGetX(XMVector3Length(vPlayerPosition - vPosition));
		return fDistance;

		if (fDistance < m_pOwner->Get_Attack_BiteRadius())
		{
			m_pOwner->Get_Transform()->Change_Speed(m_pOwner->Get_Stats().m_fRunSpeed);

			m_bBitePossible = true;
		}
	}


	virtual _vector Get_PlayerPos()
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		CGameObject* pGameObject = pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player"));
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(pGameObject);
		/*if (!pPlayer)
			return ;*/

		_vector vPlayerPosition = pPlayer->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);
		return vPlayerPosition;
	}

	virtual _float RadianToTarget()
	{
		_float fWidth = 0.f, fHeight = 0.f, fDepth = 0.f;

		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		CGameObject* pGameObject = pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player"));
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(pGameObject);
		if (!pPlayer)
			return 0.f;

		_vector vPlayerPosition = pPlayer->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);
		_vector vWolfPosition = m_pOwner->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);
		_vector vWolfLook = m_pOwner->Get_TransformState(CTransform::STATE::STATE_LOOK);

		_vector vDistance = vPlayerPosition - vWolfPosition;
		vDistance = XMVector4Normalize(vDistance);

		/*	_float4 fPlayerPosition;
			_float4 fWolfPosition;


			XMStoreFloat4(&fPlayerPosition, vPlayerPosition);
			XMStoreFloat4(&fWolfPosition, vWolfPosition);
			*/
		_float DotValue = XMVectorGetX(XMVector3Dot(vDistance, vWolfLook));


		//fWidth  = fPlayerPosition.x - fWolfPosition.x;
		//fHeight = fPlayerPosition.y - fWolfPosition.y;
		//fDepth =  fPlayerPosition.z - fWolfPosition.z;

		//_float fDiagonal = sqrtf(fWidth * fWidth + fHeight * fHeight + fDepth * fDepth);

		_float fRadian = acosf(DotValue);
		_float fdegree = XMConvertToDegrees(fRadian);

		return fdegree;

	}

	_float Find_ToTargetRadian(_fvector vAt)
	{
		_vector		vMonPos = m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION);
		_vector		vMonLook = XMVector3Normalize(m_pOwner->Get_TransformState(CTransform::STATE_LOOK));


		_vector		vTargetDir = XMVector3Normalize(vAt - vMonPos);
		_vector		vAxisY = XMVectorSet(0.f, 1.f, 0.f, 0.f);

		_vector		vMonRight = XMVector3Cross(vAxisY, vMonLook);
		_vector		vMonUp = XMVector3Cross(vMonLook, vMonRight);


		_float fDot = XMVectorGetX(XMVector3Dot(vMonLook, vTargetDir));

		_float fRadian = acos(fDot);

		//앞쪽인지 뒤쪽인지 
		_float fLookDot = XMVectorGetX(XMVector3Dot(vMonLook, vTargetDir));

		if (fLookDot < 0)
			return fRadian;

		else
			return fRadian + 180.f;

		return fRadian;
	}

	_bool Find_ToTargetRightSide(_fvector vAt)
	{
		_vector		vMonPos = m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION);
		_vector		vMonLook = XMVector3Normalize(m_pOwner->Get_TransformState(CTransform::STATE_LOOK));
		_vector		vAxisY = XMVectorSet(0.f, 1.f, 0.f, 0.f);

		_vector		vRight = XMVector3Cross(vAxisY, vMonLook);
		_vector		vUp = XMVector3Cross(vMonLook, vRight);

		_vector		vTargetDir = XMVector3Normalize(vAt - vMonPos);

		//오른쪽인지 왼쪽인지 
		_float fRightDot = XMVectorGetX(XMVector3Dot(vMonLook, vTargetDir));

		if (fRightDot > 0)
			return true;	//오른쪽

		else
			return false;

		return false;
	}

	_bool Is_TargetInSight(_fvector vAt, _float vSightAngle)
	{
		_vector		vMonPos = m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION);
		_vector		vMonLook = XMVector3Normalize(m_pOwner->Get_TransformState(CTransform::STATE_LOOK));


		_vector		vTargetDir = XMVector3Normalize(vAt - vMonPos);
		_vector		vAxisY = XMVectorSet(0.f, 1.f, 0.f, 0.f);

		_vector		vMonRight = XMVector3Cross(vAxisY, vMonLook);
		_vector		vMonUp = XMVector3Cross(vMonLook, vMonRight);


		_float fDot = XMVectorGetX(XMVector3Dot(vMonLook, vTargetDir));
		
		_float fRadian = acos(fDot) * (180 / XM_PI);
		
		//앞쪽인지 뒤쪽인지 
		_float fLookDot = XMVectorGetX(XMVector3Dot(vMonLook, vTargetDir));
		

		_float fSightAngle = vSightAngle;

		//앞쪽에 있고 시야각 안에 있다
		if (fLookDot > 0)
		{
			if (fRadian <= fSightAngle)
				return true;

			else
				return false;
		}

		else
			return false;
		
		return false;
	}

	_bool Is_TargetInFront(_fvector vAt)
	{
		_vector		vMonPos = m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION);
		_vector		vMonLook = XMVector3Normalize(m_pOwner->Get_TransformState(CTransform::STATE_LOOK));


		_vector		vTargetDir = XMVector3Normalize(vAt - vMonPos);
		_vector		vAxisY = XMVectorSet(0.f, 1.f, 0.f, 0.f);

		_vector		vMonRight = XMVector3Cross(vAxisY, vMonLook);
		_vector		vMonUp = XMVector3Cross(vMonLook, vMonRight);


		_float fDot = XMVectorGetX(XMVector3Dot(vMonLook, vTargetDir));


		//앞쪽인지 뒤쪽인지 
		_float fLookDot = XMVectorGetX(XMVector3Dot(vMonLook, vTargetDir));

	
		if (fLookDot > 0)
		{
			return true;
		}

		else
			return false;


		return false;
	}

protected:
	STATETYPE m_eStateType = STATETYPE_DEFAULT;
	STATE_ID m_eStateId = STATE_END;

	_bool		m_bIsAnimationFinished = false;
	_bool		m_bHasSpottedTarget = false;
	_bool		m_bBattleMode = false;
	_bool		m_bBitePossible = false;
	_bool		m_bAnimFinish = false;
	_bool		m_b_IsTargetInFront = false;
	_float		m_fTimeDeltaAcc = 0.f;
	_float		m_fTarget_Distance;
	_float		m_fDegreeToTarget;
	CIce_Wolf* m_pOwner = nullptr;
	class CPlayer* m_pTarget = nullptr;		/* If TRUE, has Aggro. */

	CBaseObj*	m_pOriginTarget = nullptr;
	CBaseObj*	m_pCurTarget = nullptr;
	_vector		m_vCurTargetPos;

};
END