#pragma once

#include "AiRinwell.h"
#include "Client_Defines.h"
#include "PlayerManager.h"
#include "GameInstance.h"
#include "Player.h"

BEGIN(Client)
class CRinwellState
{
public:
	enum STATE_ID
	{
		STATE_IDLE,
		STATE_MOVE,
	};

	virtual ~CRinwellState() {};
	virtual CRinwellState* AI_Behaviour(_float fTimeDelta) { return nullptr; };
	virtual CRinwellState* Tick(_float fTimeDelta) PURE;
	virtual CRinwellState* LateTick(_float fTimeDelta) PURE;

	virtual void Enter() PURE;
	virtual void Exit() PURE;

	CRinwellState* ChangeState(CRinwellState* pCurrentState, CRinwellState* pNewState)
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

protected:
	virtual _float Find_Target()
	{
		CPlayer* pPlayer = CPlayerManager::Get_Instance()->Get_ActivePlayer();

		m_pTarget = pPlayer;
		_vector vPlayerPosition = pPlayer->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);
		_vector vPosition = m_pOwner->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);

		_float fDistance = XMVectorGetX(XMVector3Length(vPlayerPosition - vPosition));
		return fDistance;
	}


	virtual _vector Get_PlayerPos()
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		CGameObject* pGameObject = pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player"));
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(pGameObject);

		_vector vPlayerPosition = pPlayer->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);
		return vPlayerPosition;
	}

	virtual _float RadianToTarget(_vector vPos)
	{
		_float fWidth = 0.f, fHeight = 0.f, fDepth = 0.f;

		_vector vPlayerPosition = vPos;
		_vector vWolfPosition = m_pOwner->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);
		_vector vWolfLook = m_pOwner->Get_TransformState(CTransform::STATE::STATE_LOOK);

		_vector vDistance = vPlayerPosition - vWolfPosition;
		vDistance = XMVector4Normalize(vDistance);

		_float DotValue = XMVectorGetX(XMVector3Dot(vDistance, vWolfLook));

		_float fRadian = acosf(DotValue);
		_float fdegree = XMConvertToDegrees(fRadian);

		return fdegree;

	}




protected:
	STATETYPE m_eStateType = STATETYPE_DEFAULT;
	STATE_ID m_eStateId = STATE_IDLE;

	_bool		m_bIsAnimationFinished = false;
	_float		m_fTarget_Distance;
	class CAiRinwell* m_pOwner = nullptr;
	class CPlayer* m_pTarget = nullptr;		/* If TRUE, has Aggro. */

};
END