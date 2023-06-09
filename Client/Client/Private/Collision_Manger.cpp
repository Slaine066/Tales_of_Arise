#include "stdafx.h"
#include "..\Public\Collision_Manger.h"
#include "BaseObj.h"
#include "Player.h"
#include "Monster.h"
#include "Weapon.h"

IMPLEMENT_SINGLETON(CCollision_Manager);


CCollision_Manager::CCollision_Manager()
{
}

void CCollision_Manager::Add_CollisionGroup(COLLSIONGROUP CollisionGroup, CBaseObj * pGameObject)
{
	if (nullptr == pGameObject)
		return;

	auto& iter = m_GameObjects[CollisionGroup].begin();
	while (iter != m_GameObjects[CollisionGroup].end())
	{
		if (*iter == pGameObject)
			return;
		else
			++iter;
	}

	m_GameObjects[CollisionGroup].push_back(pGameObject);
}

void CCollision_Manager::Add_CollisionGroupCollider(COLLSIONGROUP CollisionGroup, CCollider * pCollider, CBaseObj * pGameObject)
{
	if (nullptr == pCollider || nullptr == pGameObject)
		return;
	
	m_Colliders[CollisionGroup].push_back(pCollider);
	m_ColliderGameObjects[CollisionGroup].push_back(pGameObject);
}

void CCollision_Manager::Out_CollisionGroup(COLLSIONGROUP CollisionGroup, CBaseObj * pGameObject)
{
	auto& iter = m_GameObjects[CollisionGroup].begin();
	while (iter != m_GameObjects[CollisionGroup].end())
	{
		if (*iter == pGameObject)
			iter = m_GameObjects[CollisionGroup].erase(iter);
		else
			++iter;
	}
}

void CCollision_Manager::Out_CollisionGroupCollider(COLLSIONGROUP CollisionGroup, CCollider * pCollider, CBaseObj * pGameObject)
{
	auto& iterObj = m_ColliderGameObjects[CollisionGroup].begin();
	auto& iterCol = m_Colliders[CollisionGroup].begin();

	while (iterObj != m_ColliderGameObjects[CollisionGroup].end())
	{
		if (*iterObj == pGameObject)
			iterObj = m_ColliderGameObjects[CollisionGroup].erase(iterObj);
		else
			++iterObj;
	}

	while (iterCol != m_Colliders[CollisionGroup].end())
	{
		if (*iterCol == pCollider)
			iterCol = m_Colliders[CollisionGroup].erase(iterCol);
		else
			++iterCol;
	}
}

void CCollision_Manager::Clear_CollisionGroup(COLLSIONGROUP CollisionGroup)
{
	m_GameObjects[CollisionGroup].clear();
}

void CCollision_Manager::Clear_CollisionGroupExpect(COLLSIONGROUP CollisionGroup)
{
	for (_uint i = 0; i < COLLISION_END; ++i)
	{
		if (i == CollisionGroup)
			continue;

		m_GameObjects[i].clear();
	}
}


void CCollision_Manager::Clear_AllCollisionGroup()
{
	for (_uint i = 0; i < COLLISION_END; ++i)
	{
		if (i == COLLISION_PLAYER)
			continue;

		m_GameObjects[i].clear();
		m_ColliderGameObjects[i].clear();
		m_Colliders[i].clear();
	}
}


_bool CCollision_Manager::CollisionwithGroup(COLLSIONGROUP CollisionGroup, CCollider* pCollider, CBaseObj** pOut, COLLIDERTYPE eType)
{
	for (auto& iter : m_GameObjects[CollisionGroup])
	{
		if (iter == nullptr|| iter->Check_IsinFrustum() == false || iter->Get_Collider() == pCollider)
			continue;

		CCollider* pTargetCollider = nullptr;

		switch (eType)
		{
		case Client::CCollision_Manager::COLLIDER_AABB:
			pTargetCollider = iter->Get_ABBCollider();
			break;
		case Client::CCollision_Manager::COLLIDER_OBB:
			pTargetCollider = iter->Get_OBBCollider();
			break;
		case Client::CCollision_Manager::COLLIDER_SPHERE:
			pTargetCollider = iter->Get_SPHERECollider();
			break;
		case Client::CCollision_Manager::COLLIDER_END:
			pTargetCollider = iter->Get_Collider();
			break;
		}


		if (pCollider->Collision(pTargetCollider))
		{
			if (pOut != nullptr)
				*pOut = iter;
			return true;
		}
			
	}

	return false;
}

_bool CCollision_Manager::CollisionwithGroupCollider(COLLSIONGROUP CollisionGroup, CCollider * pCollider, CBaseObj ** pOut, COLLIDERTYPE eType)
{
	if ((0 >= m_ColliderGameObjects[CollisionGroup].size()) || (0 >= m_Colliders[CollisionGroup].size()))
		return false;

	auto& iterObj = m_ColliderGameObjects[CollisionGroup].begin();

	for (auto& iterCol : m_Colliders[CollisionGroup])
	{
		if (nullptr == iterCol)
		{
			++iterObj;
			continue;
		}
		
		if (pCollider->Collision(iterCol))
		{
			if (nullptr != pOut)
				*pOut = *iterObj;
			
			return true;
		}

		if (iterObj != m_ColliderGameObjects[CollisionGroup].end())
			++iterObj;
	}

	return false;
}

_bool CCollision_Manager::CollisionwithGroup(COLLSIONGROUP SourGroup, COLLSIONGROUP DestGroup, CBaseObj** outSour, CBaseObj** outDest)
{
	for (auto& Sour : m_GameObjects[SourGroup])
	{
		CCollider* pSourCollider = Sour->Get_Collider();
		if (pSourCollider == nullptr)
			continue;

		for (auto& Dest : m_GameObjects[DestGroup])
		{
			CCollider* pDestCollider = Dest->Get_Collider();
			if (pDestCollider == nullptr || Dest == Sour)
				continue;

		

			if (pDestCollider->Collision(pSourCollider))
			{
				*outSour = Sour;
				*outDest = Dest;
				return true;
			}	
		}
	}

	*outSour = nullptr;
	*outDest = nullptr;
	return false;

}

void CCollision_Manager::CollisionwithBullet()
{
	///* Player가 Monster Bullet에 충돌 했을 때*/
	//CBaseObj* pPlayer = nullptr;
	//CBaseObj* pMonsterBullet = nullptr;

	//if (CollisionwithGroup(COLLISION_PLAYER, COLLISION_MBULLET, &pPlayer, &pMonsterBullet))
	//{
	//	pMonsterBullet->Set_Dead(true);

	//	CPlayer* Player = dynamic_cast<CPlayer*>(pPlayer);
	//	if (Player->Get_AnimState() == CPlayer::SHIELD || Player->Get_AnimState() == CPlayer::SHIELD_HOLD_LP
	//		|| Player->Get_AnimState() == CPlayer::SHIELD_HOLD_B || Player->Get_AnimState() == CPlayer::SHIELD_HOLD_F
	//		|| Player->Get_AnimState() == CPlayer::SHIELD_HOLD_L || Player->Get_AnimState() == CPlayer::SHIELD_HOLD_R )
	//	{
	//		Player->Set_AnimState(CPlayer::SHIELD_HIT);
	//		Player->Make_GuardEffect(pMonsterBullet);
	//	}
	//	else
	//	{

	//		CMonsterBullet::BULLETDESC BulletDesc = dynamic_cast<CMonsterBullet*>(pMonsterBullet)->Get_BulletDesc(); 
	//		dynamic_cast<CPlayer*>(Player)->Take_Damage(1.f, &BulletDesc, pMonsterBullet);

	//	}
	//	
	//}

	/* Monster가 Player Bullet에 충돌 했을 때*/
	/*CBaseObj* pMonster = nullptr;
	CBaseObj* pPlayerBullet = nullptr;
	if (CollisionwithGroup(COLLISION_MONSTER, COLLISION_PBULLET, &pMonster, &pPlayerBullet))
	{
		CBaseObj* pPlayer =  dynamic_cast<CWeapon*>(pPlayerBullet)->Get_Owner();
		dynamic_cast<CMonster*>(pMonster)->Take_Damage(rand()%500 + 50, pPlayer);
	}*/
}


void CCollision_Manager::Update_Collider()
{
	for (_uint i = 0; i < COLLISION_END; ++i)
	{
		for (auto& iter : m_GameObjects[i])
		{
			if(iter->Get_IsSocket() == false)
				iter->Update_Collider();
		}
	}
}

void CCollision_Manager::Update_ColliderGroup(COLLSIONGROUP SourGroup)
{
}

CCollider * CCollision_Manager::Reuse_Collider(CCollider::TYPE eType, _uint iLevelIndex, const _tchar* pPrototypeTag, void* pArg)
{
	CCollider* pCollider = nullptr;

	if (m_ColliderPool[eType].empty())
	{
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

		pCollider = (CCollider*)pGameInstance->Clone_Component(iLevelIndex, pPrototypeTag, pArg);
		
		RELEASE_INSTANCE(CGameInstance);
	}
	else
	{
		pCollider = m_ColliderPool[eType].front();
		m_ColliderPool[eType].pop_front();
		pCollider->Set_ColliderDesc(pArg);
	}

	if (nullptr == pCollider)
		return nullptr;

	return pCollider;
}

void CCollision_Manager::Collect_Collider(CCollider::TYPE eType, CCollider * pCollider)
{
	if (nullptr == pCollider)
		return;
	
	m_ColliderPool[eType].push_back(pCollider);
}

_float CCollision_Manager::Calculate_DmgDirection(CBaseObj* Sour, CBaseObj* Dest, _float* fCross)
{
	_vector BulletLook = Sour->Get_TransformState(CTransform::STATE_LOOK);
	_vector PlayerLook = Dest->Get_TransformState(CTransform::STATE_LOOK);
	_vector fDot = XMVector3Dot(BulletLook, PlayerLook);
	_float fAngleRadian = acos(XMVectorGetX(fDot));
	_float fAngleDegree = XMConvertToDegrees(fAngleRadian);
	_vector vCross = XMVector3Cross(BulletLook, PlayerLook);

	if(fCross != nullptr)
		*fCross = XMVectorGetY(vCross);

	return fAngleDegree;
}

void CCollision_Manager::Free()
{
	for (_uint i = 0; i < COLLISION_END; ++i)
	{
		m_GameObjects[i].clear();
		m_ColliderGameObjects[i].clear();
		m_Colliders[i].clear();
	}

	for (_uint i = 0; i < COLLIDER_END; ++i)
	{
		for (auto& iter : m_ColliderPool[i])
			Safe_Release(iter);
		
		m_ColliderPool[i].clear();
	}
}
