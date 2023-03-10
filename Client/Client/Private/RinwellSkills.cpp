#include "stdafx.h"
#include "..\Public\RinwellSkills.h"
#include "Monster.h"
#include "Player.h"
#include "EffectMesh.h"

CRinwellSkills::CRinwellSkills(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CBullet(pDevice, pContext)
{
}

CRinwellSkills::CRinwellSkills(const CRinwellSkills & rhs)
	: CBullet(rhs)
{
}

HRESULT CRinwellSkills::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CRinwellSkills::Initialize(void * pArg)
{
	__super::Initialize(pArg);

	_vector vOffset = { 0.f,0.f,0.f,0.f };
	_vector vLocation = { 0.f,0.f,0.f,0.f };
	_matrix mWorldMatrix = XMMatrixIdentity();

	switch (m_BulletDesc.eBulletType)
	{
	case PHOTON_FLASH:
		vOffset = m_BulletDesc.pOwner->Get_TransformState(CTransform::STATE_LOOK);
		vLocation = m_BulletDesc.pOwner->Get_Transform()->Get_State(CTransform::STATE::STATE_TRANSLATION) + vOffset*2.f;
		mWorldMatrix = m_BulletDesc.pOwner->Get_Transform()->Get_WorldMatrix();
		mWorldMatrix.r[3] = vLocation;
		m_pEffects = CEffect::PlayEffectAtLocation(TEXT("PhotonFlashBall.dat"), mWorldMatrix);
		break;
	case GALE_FORCE:
		vOffset = XMVectorSet(0.f, m_fRadius, 0.f, 0.f);
		vLocation = m_pTransformCom->Get_State(CTransform::STATE::STATE_TRANSLATION) + vOffset;
		mWorldMatrix = m_BulletDesc.pOwner->Get_Transform()->Get_WorldMatrix();
		mWorldMatrix.r[3] = vLocation;
		m_pEffects = CEffect::PlayEffectAtLocation(TEXT("GaleForce.dat"), mWorldMatrix);

		vOffset = XMVectorSet(0.f, m_fRadius, 0.f, 0.f);
		vLocation = m_pTransformCom->Get_State(CTransform::STATE::STATE_TRANSLATION) + vOffset;
		mWorldMatrix = m_BulletDesc.pOwner->Get_Transform()->Get_WorldMatrix();
		mWorldMatrix.r[3] = vLocation;
		m_pBlastEffects = CEffect::PlayEffectAtLocation(TEXT("GaleForceParticle.dat"), mWorldMatrix);

		vOffset = XMVectorSet(0.f, m_fRadius, 0.f, 0.f);
		vLocation = m_pTransformCom->Get_State(CTransform::STATE::STATE_TRANSLATION) + vOffset;
		mWorldMatrix = m_BulletDesc.pOwner->Get_Transform()->Get_WorldMatrix();
		mWorldMatrix.r[3] = vLocation;
		m_pBlast2Effects = CEffect::PlayEffectAtLocation(TEXT("GaleForceParticle2.dat"), mWorldMatrix);
		break;
	case METEOR:
		vLocation = m_BulletDesc.vInitPositon;
		m_pTransformCom->Set_State(CTransform::STATE::STATE_TRANSLATION, vLocation);
		m_pTransformCom->LookDir(m_BulletDesc.vTargetDir);
		mWorldMatrix = m_pTransformCom->Get_WorldMatrix();
		mWorldMatrix.r[3] = vLocation;
		m_pEffects = CEffect::PlayEffectAtLocation(TEXT("MeteorBall.dat"), mWorldMatrix);

		break;
	case DIVINE_SABER:
		vOffset = XMVectorSet(0.f, m_fRadius + 1.f, 0.f, 0.f);
		vLocation = m_BulletDesc.vTargetPosition + vOffset;
		m_pTransformCom->Set_State(CTransform::STATE::STATE_TRANSLATION, vLocation);
		mWorldMatrix = m_pTransformCom->Get_WorldMatrix();
		mWorldMatrix.r[3] = vLocation;
		m_pEffects = CEffect::PlayEffectAtLocation(TEXT("DivineSaberRing.dat"), mWorldMatrix);

		vOffset = XMVectorSet(0.f, m_fRadius + 0.5f, 0.f, 0.f);
		vLocation = m_BulletDesc.vTargetPosition + vOffset;
		m_pTransformCom->Set_State(CTransform::STATE::STATE_TRANSLATION, vLocation);
		mWorldMatrix = m_pTransformCom->Get_WorldMatrix();
		mWorldMatrix.r[3] = vLocation;
		//m_pBlastEffects = CEffect::PlayEffectAtLocation(TEXT("DivineFloor.dat"), mWorldMatrix);

		memcpy(&OriginLightDesc, CGameInstance::Get_Instance()->Get_LightDesc(0), sizeof(LIGHTDESC));
		memcpy(&DevineLightDesc, &OriginLightDesc, sizeof(LIGHTDESC));

		//CGameInstance::Get_Instance()->PlaySounds(TEXT("Rinwell_Ctrl_R_SkillSound_Start.wav"), SOUND_EFFECT, 1.0f);
		break;
	case DIVINE_SABER_BULLET:
	{
		vLocation = m_BulletDesc.vTargetPosition;
		m_pTransformCom->Set_State(CTransform::STATE::STATE_TRANSLATION, vLocation);
		m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(_float(rand() % 180)));

		mWorldMatrix = m_pTransformCom->Get_WorldMatrix();
		mWorldMatrix.r[3] = vLocation;

		_int iRandNum = rand() % 4;
		if (iRandNum == 0)
		{
			m_pEffects = CEffect::PlayEffectAtLocation(TEXT("DivineSaberBullet0.dat"), mWorldMatrix);
			
			/*if (m_bDivineBullet0_Sound == false)
			{
				CGameInstance::Get_Instance()->PlaySounds(TEXT("Rinwell_Ctrl_R_SkillSound_Bullet0.wav"), SOUND_EFFECT, 1.0f);
				m_bDivineBullet0_Sound = false;
			}*/
		}
		if (iRandNum == 1)
		{
			m_pEffects = CEffect::PlayEffectAtLocation(TEXT("DivineSaberBullet1.dat"), mWorldMatrix);
			
			/*if (m_bDivineBullet1_Sound == false)
			{
				CGameInstance::Get_Instance()->PlaySounds(TEXT("Rinwell_Ctrl_R_SkillSound_Bullet1.wav"), SOUND_EFFECT, 1.0f);
				m_bDivineBullet1_Sound = false;
			}
		*/}
		if (iRandNum == 2)
		{
			m_pEffects = CEffect::PlayEffectAtLocation(TEXT("DivineSaberBullet2.dat"), mWorldMatrix);
			
			/*if (m_bDivineBullet2_Sound == false)
			{
				CGameInstance::Get_Instance()->PlaySounds(TEXT("Rinwell_Ctrl_R_SkillSound_Bullet2.wav"), SOUND_EFFECT, 1.0f);
				m_bDivineBullet2_Sound = false;
			}*/
		}
		if (iRandNum == 3)
		{
			m_pEffects = CEffect::PlayEffectAtLocation(TEXT("DivineSaberBullet3.dat"), mWorldMatrix);
			
			/*if (m_bDivineBullet3_Sound == false)
			{
				CGameInstance::Get_Instance()->PlaySounds(TEXT("Rinwell_Ctrl_R_SkillSound_Bullet3.wav"), SOUND_EFFECT, 1.0f);
				m_bDivineBullet3_Sound = false;
			}*/
		}


		//m_pBlastEffect = CEffect::PlayEffectAtLocation(TEXT("AquaImpact.dat"), mWorldMatrix);
		break;
	}
	case HOLY_RANCE:
		vOffset = XMVectorSet(0.f, 1.f, 0.f, 0.f);
		vLocation = XMVectorSetY(m_BulletDesc.vTargetPosition, 1.f) ;
		mWorldMatrix = m_pTransformCom->Get_WorldMatrix();
		mWorldMatrix.r[3] = vLocation;
		m_pBlastEffects = CEffect::PlayEffectAtLocation(TEXT("HolyLanceGround.dat"), mWorldMatrix);
		
		/*if (m_bHoly_RanceGround_Sound == false)
		{
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Rinwell_Ctrl_F_SkillSound_Ground.wav"), SOUND_EFFECT, 1.0f);
			m_bHoly_RanceGround_Sound = true;
		}*/
		/*vLocation = m_BulletDesc.vInitPositon;
		m_pTransformCom->Set_State(CTransform::STATE::STATE_TRANSLATION, vLocation);*/

		break;
	case HOLY_RANCE_BULLET:
	{
		vLocation = m_BulletDesc.vInitPositon;
		m_pTransformCom->Set_State(CTransform::STATE::STATE_TRANSLATION, vLocation);
		m_pTransformCom->LookAt(m_BulletDesc.vTargetPosition);
		mWorldMatrix = m_pTransformCom->Get_WorldMatrix();
		m_pEffects = CEffect::PlayEffectAtLocation(TEXT("HolyLanceBullet.dat"), mWorldMatrix);

		_int iRandNumber = rand() % 3;

		if (iRandNumber = 0)
		{
			/*if (m_bHoly_RanceBullet0_Sound == false)
			{
				CGameInstance::Get_Instance()->PlaySounds(TEXT("Rinwell_Ctrl_F_SkillSound_Bullet0.wav"), SOUND_EFFECT, 1.0f);
				m_bHoly_RanceBullet0_Sound = true;
			}*/
		}

		if (iRandNumber = 1)
		{
			/*if (m_bHoly_RanceBullet1_Sound == false)
			{
				CGameInstance::Get_Instance()->PlaySounds(TEXT("Rinwell_Ctrl_F_SkillSound_Bullet1.wav"), SOUND_EFFECT, 1.0f);
				m_bHoly_RanceBullet1_Sound = true;
			}*/
		}

		if (iRandNumber = 2)
		{
			/*if (m_bHoly_RanceBullet2_Sound == false)
			{
				CGameInstance::Get_Instance()->PlaySounds(TEXT("Rinwell_Ctrl_F_SkillSound_Bullet2.wav"), SOUND_EFFECT, 1.0f);
				m_bHoly_RanceBullet2_Sound = true;
			}*/
		}
		break;
	}
	case HOLY_RANCE_LASTBULLET:
		vLocation = m_BulletDesc.vInitPositon;
		m_pTransformCom->Set_State(CTransform::STATE::STATE_TRANSLATION, vLocation);

		mWorldMatrix = m_pTransformCom->Get_WorldMatrix();
		mWorldMatrix.r[3] = vLocation;
		m_pEffects = CEffect::PlayEffectAtLocation(TEXT("HolyLanceBigBullet.dat"), mWorldMatrix);
		
		/*if (m_bHoly_RanceLastBullet_Sound == false)
		{
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Rinwell_Ctrl_F_SkillSound_LastBullet.wav"), SOUND_EFFECT, 1.0f);
			m_bHoly_RanceLastBullet_Sound = true;
		}*/
		break;
	case HOLY_RANCE_FISRTBULLET:
		vLocation = m_BulletDesc.vInitPositon;
		m_pTransformCom->Set_State(CTransform::STATE::STATE_TRANSLATION, vLocation);
		
		mWorldMatrix = m_pTransformCom->Get_WorldMatrix();
		mWorldMatrix.r[3] = vLocation;
		m_pEffects = CEffect::PlayEffectAtLocation(TEXT("HolyLanceUpBullet.dat"), mWorldMatrix);
		
		/*if (m_bHoly_RanceFirstBullet_Sound == false)
		{
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Rinwell_Ctrl_F_SkillSound_FirstBullet.wav"), SOUND_EFFECT, 1.0f);
			m_bHoly_RanceFirstBullet_Sound = true;
		}*/
		break;
	case THUNDER_FIELD:
		vOffset = XMVectorSet(0.f, m_fRadius + 5.f, 0.f, 0.f);
		vLocation = m_pTransformCom->Get_State(CTransform::STATE::STATE_TRANSLATION) + vOffset;
		m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(_float(rand() % 180)));
		mWorldMatrix = m_pTransformCom->Get_WorldMatrix();
		mWorldMatrix.r[3] = vLocation;
		m_pEffects = CEffect::PlayEffectAtLocation(TEXT("ThunderFieldBullet.dat"), mWorldMatrix);

		vOffset = XMVectorSet(0.f, m_fRadius + 4, 0.f, 0.f);
		vLocation = m_pTransformCom->Get_State(CTransform::STATE::STATE_TRANSLATION) + vOffset;
		mWorldMatrix.r[3] = vLocation;
		m_pBlastEffects = CEffect::PlayEffectAtLocation(TEXT("ThunderFieldAura.dat"), mWorldMatrix);
		vOffset = XMVectorSet(0.f, m_fRadius + 2.f, 0.f, 0.f);
		vLocation = m_pTransformCom->Get_State(CTransform::STATE::STATE_TRANSLATION) + vOffset;
		mWorldMatrix.r[3] = vLocation;
		m_pBlast2Effects = CEffect::PlayEffectAtLocation(TEXT("ThunderFieldFloor.dat"), mWorldMatrix);
		break;
	case METEORDEAD:
		vLocation = m_pTransformCom->Get_State(CTransform::STATE::STATE_TRANSLATION);
		mWorldMatrix = m_pTransformCom->Get_WorldMatrix();
		mWorldMatrix.r[3] = vLocation;
		m_pEffects = CEffect::PlayEffectAtLocation(TEXT("MeteorDeadFlash.dat"), mWorldMatrix);


		break;

	case BANGJEON:
	{
		_float4 vCamView = (_float4)CGameInstance::Get_Instance()->Get_CamWorldMatrix().m[2];
		m_pTransformCom->LookDir(XMLoadFloat4(&vCamView));
		mWorldMatrix = m_BulletDesc.pOwner->Get_Transform()->Get_WorldMatrix();
		m_pEffects = CEffect::PlayEffectAtLocation(TEXT("ElecDischargeBall.dat"), mWorldMatrix);
		m_pBlastEffects = CEffect::PlayEffectAtLocation(TEXT("ElecDischargeThunder2.dat"), mWorldMatrix);
		break;
	}
	case BANGEONDEAD:
		vLocation = m_BulletDesc.pOwner->Get_Transform()->Get_State(CTransform::STATE::STATE_TRANSLATION);
		vOffset = XMVectorSet(0.f, 5.f, 0.f, 0.f);
		mWorldMatrix = m_BulletDesc.pOwner->Get_Transform()->Get_WorldMatrix();
		mWorldMatrix.r[3] = vLocation + vOffset;
		m_pEffects = CEffect::PlayEffectAtLocation(TEXT("ElecDischargeEnd.dat"), mWorldMatrix);
		break;
	}

	
	return S_OK;
}

int CRinwellSkills::Tick(_float fTimeDelta)
{
	_int iSuperTick = __super::Tick(fTimeDelta);

	if (iSuperTick == OBJ_DEAD)
	{
		Dead_Effect();
		return OBJ_DEAD;
	}

	if (iSuperTick == OBJ_NOSHOW)
		return OBJ_NOSHOW;

	_float fTime = CGameInstance::Get_Instance()->Get_TimeDelta(TEXT("Timer_60"));

	switch (m_BulletDesc.eBulletType)
	{
	case PHOTON_FLASH:
		Tick_PhotonFlash(fTime);
		break;
	case METEORDEAD:
	case BANGEONDEAD:
	case GALE_FORCE:
		Tick_GaleForce(fTime);
		break;
	case THUNDER_FIELD:
		Tick_ThunderField(fTime);
		break;
	case METEOR:
		Tick_Meteor(fTime);
		break;
	case DIVINE_SABER:
		Tick_DivineSaber(fTime);
		break;
	case DIVINE_SABER_BULLET:
		Tick_DivineSaberBullet(fTime);
		break;

	case HOLY_RANCE:
		Tick_HolyRance(fTime);
		break;
	case HOLY_RANCE_FISRTBULLET:
	case HOLY_RANCE_LASTBULLET:
		Tick_HolyRanceBullet2(fTime);
		break;
	case HOLY_RANCE_BULLET:
		Tick_HolyRanceBullet(fTime);
		break;

	case BANGJEON:
		Tick_BangJeon(fTime);
		break;

	}


	if(m_pAABBCom != nullptr)
		m_pAABBCom->Update(m_pTransformCom->Get_WorldMatrix());
	if (m_pSPHERECom != nullptr)
		m_pSPHERECom->Update(m_pTransformCom->Get_WorldMatrix());
	if (m_pOBBCom != nullptr)
		m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());

	return OBJ_NOEVENT;
}

void CRinwellSkills::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	switch (m_BulletDesc.eBulletType)
	{
	case METEOR:
		if (XMVectorGetY(Get_TransformState(CTransform::STATE_TRANSLATION)) <= 2.f)
			m_bDead = true;
		break;
	case HOLY_RANCE_FISRTBULLET:
		if (m_fTime >= m_BulletDesc.fDeadTime)
			m_bDead = true;
		break;
	case HOLY_RANCE_LASTBULLET:
		if(XMVectorGetY(Get_TransformState(CTransform::STATE_TRANSLATION)) <= - 0.3f)
			m_bBullet = true;
		if (m_bBullet == true && !m_bFirst)
		{
			_matrix mWorldMatrix = m_pTransformCom->Get_WorldMatrix();
			vector<CEffect*> Blast = CEffect::PlayEffectAtLocation(TEXT("holyLanceBlast.dat"), mWorldMatrix);

			for (auto& pEffect : Blast)
			{
				if (!pEffect)
					continue;

				CEffectMesh* pNotMesh = dynamic_cast<CEffectMesh*>(pEffect);
				if (!pNotMesh)
				{
					_vector vOffset = XMVectorSet(0.f, 3.f, 0.f, 0.f);
					_vector vLocation = m_pTransformCom->Get_State(CTransform::STATE::STATE_TRANSLATION) + vOffset;
					pEffect->Get_Transform()->Set_State(CTransform::STATE::STATE_TRANSLATION, vLocation);
				}
			}
			
			m_bFirst = true;
		}
		if (XMVectorGetY(Get_TransformState(CTransform::STATE_TRANSLATION)) <= -10.f)
			m_bDead = true;
		break;
	default:
		if (XMVectorGetY(Get_TransformState(CTransform::STATE_TRANSLATION)) <= -0.5f)
			m_bDead = true;
		break;
	}

	for (auto& iter : m_pEffects)
	{
		if (iter != nullptr && iter->Get_PreDead())
			iter = nullptr;
	}

	for (auto& iter : m_pBlastEffects)
	{
		if (iter != nullptr && iter->Get_PreDead())
			iter = nullptr;

	}

	for (auto& iter : m_pBlast2Effects)
	{
		if (iter != nullptr && iter->Get_PreDead())
			iter = nullptr;
	}
	
}

void CRinwellSkills::Collision_Check()
{

	if (Check_Exception() == false)
		return;

	CBaseObj* pCollisionTarget = nullptr;
	switch (m_BulletDesc.eBulletType)
	{
	case PHOTON_FLASH:
		__super::Collision_Check();
		if (m_bDead)
		{
			if (m_bNormalHit == false)
			{
				CGameInstance::Get_Instance()->PlaySounds(TEXT("Rinwell_Normal_Attack_Hit.wav"), SOUND_WEAPON, 0.6f);
				m_bNormalHit = true;
			}
		}
		break;
	case HOLY_RANCE_BULLET:
		m_HitLagDesc.bLockOnChange = false;
		m_HitLagDesc.bHitLag = true;
		m_HitLagDesc.fHitLagTimer = 0.1f;
		m_HitLagDesc.bShaking = true;
		m_HitLagDesc.fShakingPower = 1.f;
		m_HitLagDesc.fShakingMinusPower = 0.2f;

		if (m_BulletDesc.eCollisionGroup == PLAYER)
		{
			if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_MONSTER, m_pSPHERECom, &pCollisionTarget))
				dynamic_cast<CMonster*>(pCollisionTarget)->Take_Damage(m_BulletDesc.iDamage, m_BulletDesc.pOwner, m_HitLagDesc);
		}
		else
		{
			if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_PLAYER, m_pSPHERECom, &pCollisionTarget))
				dynamic_cast<CPlayer*>(pCollisionTarget)->Take_Damage(m_BulletDesc.iDamage, m_BulletDesc.pOwner);
		}
		break;
	case HOLY_RANCE_FISRTBULLET:
		m_HitLagDesc.bLockOnChange = false;
		m_HitLagDesc.bHitLag = true;
		m_HitLagDesc.fHitLagTimer = 0.3f;
		m_HitLagDesc.bShaking = true;
		m_HitLagDesc.fShakingPower = 2.f;
		m_HitLagDesc.fShakingMinusPower = 0.2f;

		if (m_BulletDesc.eCollisionGroup == PLAYER)
		{
			if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_MONSTER, m_pSPHERECom, &pCollisionTarget))
				dynamic_cast<CMonster*>(pCollisionTarget)->Take_Damage(m_BulletDesc.iDamage, m_BulletDesc.pOwner, m_HitLagDesc);
				
		}
		else
		{
			if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_PLAYER, m_pSPHERECom, &pCollisionTarget))
				dynamic_cast<CPlayer*>(pCollisionTarget)->Take_Damage(m_BulletDesc.iDamage, m_BulletDesc.pOwner);
				
		}
		break;
	case HOLY_RANCE_LASTBULLET:
		m_HitLagDesc.bLockOnChange = false;
		m_HitLagDesc.bHitLag = true;
		m_HitLagDesc.fHitLagTimer = 0.3f;
		m_HitLagDesc.bShaking = true;
		m_HitLagDesc.fShakingPower = 2.f;
		m_HitLagDesc.fShakingMinusPower = 0.2f;

		if (m_BulletDesc.eCollisionGroup == PLAYER)
		{
			if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_MONSTER, m_pSPHERECom, &pCollisionTarget))
			{
				dynamic_cast<CMonster*>(pCollisionTarget)->Take_Damage(m_BulletDesc.iDamage, m_BulletDesc.pOwner, m_HitLagDesc);
				
			}
		}
		else
		{
			if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_PLAYER, m_pSPHERECom, &pCollisionTarget))
			{
				dynamic_cast<CPlayer*>(pCollisionTarget)->Take_Damage(m_BulletDesc.iDamage, m_BulletDesc.pOwner);
			

			}
		}
		break;
	case METEOR:
		m_HitLagDesc.bLockOnChange = false;
		m_HitLagDesc.bHitLag = false;
		m_HitLagDesc.bShaking = false;

		if (m_BulletDesc.eCollisionGroup == PLAYER)
		{
			if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_MONSTER, m_pSPHERECom, &pCollisionTarget))
				dynamic_cast<CMonster*>(pCollisionTarget)->Take_Damage(m_BulletDesc.iDamage, m_BulletDesc.pOwner, m_HitLagDesc);
		}
		else
		{
			if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_PLAYER, m_pSPHERECom, &pCollisionTarget))
				dynamic_cast<CPlayer*>(pCollisionTarget)->Take_Damage(m_BulletDesc.iDamage, m_BulletDesc.pOwner, 4.f, HIT_DOWN);
		}
		break;
	case THUNDER_FIELD:
		m_HitLagDesc.bLockOnChange = false;
		m_HitLagDesc.bHitLag = false;
		m_HitLagDesc.bShaking = false;

		if (m_BulletDesc.eCollisionGroup == PLAYER)
		{
			if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_MONSTER, m_pAABBCom, &pCollisionTarget))
				dynamic_cast<CMonster*>(pCollisionTarget)->Take_Damage(m_BulletDesc.iDamage, m_BulletDesc.pOwner, m_HitLagDesc);
		}
		else
		{
			if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_PLAYER, m_pAABBCom, &pCollisionTarget))
				dynamic_cast<CPlayer*>(pCollisionTarget)->Take_Damage(m_BulletDesc.iDamage, m_BulletDesc.pOwner, 4.f, HIT_DOWN);
		}
		break;
	case GALE_FORCE:
		m_HitLagDesc.bLockOnChange = false;
		m_HitLagDesc.bHitLag = false;
		m_HitLagDesc.bShaking = false;

		if (m_BulletDesc.eCollisionGroup == PLAYER)
		{
			if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_MONSTER, m_pSPHERECom, &pCollisionTarget))
				dynamic_cast<CMonster*>(pCollisionTarget)->Take_Damage(m_BulletDesc.iDamage, m_BulletDesc.pOwner, m_HitLagDesc);
		}
		else
		{
			if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_PLAYER, m_pSPHERECom, &pCollisionTarget))
				dynamic_cast<CPlayer*>(pCollisionTarget)->Take_Damage(m_BulletDesc.iDamage, m_BulletDesc.pOwner, 4.f, HIT_DOWN);
		}
		break;
	case BANGJEON:
		m_HitLagDesc.bLockOnChange = false;
		m_HitLagDesc.bHitLag = false;
		m_HitLagDesc.bShaking = false;

		if (m_BulletDesc.eCollisionGroup == PLAYER)
		{
			if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_MONSTER, m_pSPHERECom, &pCollisionTarget))
				dynamic_cast<CMonster*>(pCollisionTarget)->Take_Damage(m_BulletDesc.iDamage, m_BulletDesc.pOwner, m_HitLagDesc);
		}
		else
		{
			if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_PLAYER, m_pSPHERECom, &pCollisionTarget))
				dynamic_cast<CPlayer*>(pCollisionTarget)->Take_Damage(m_BulletDesc.iDamage, m_BulletDesc.pOwner, 4.f, HIT_DOWN);
		}
		break;

	case DIVINE_SABER_BULLET:
		m_HitLagDesc.bLockOnChange = false;
		m_HitLagDesc.bHitLag = false;
		m_HitLagDesc.bShaking = false;


		if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_MONSTER, m_pAABBCom, &pCollisionTarget))
		{
			dynamic_cast<CMonster*>(pCollisionTarget)->Take_Damage(m_BulletDesc.iDamage, m_BulletDesc.pOwner, m_HitLagDesc);
			m_bDead = true;
		}
		break;
	}
}

void CRinwellSkills::Dead_Effect()
{
	for (auto& iter : m_pEffects)
	{
		if (iter != nullptr && iter->Get_PreDead())
			iter = nullptr;

	}
	switch (m_BulletDesc.eBulletType)
	{
	case PHOTON_FLASH:
	{
		_vector vOffset = XMVectorSet(0.f, m_fRadius, 0.f, 0.f);
		_vector vLocation = m_pTransformCom->Get_State(CTransform::STATE::STATE_TRANSLATION) + vOffset;
		_matrix mWorldMatrix = m_pTransformCom->Get_WorldMatrix();
		mWorldMatrix.r[3] = vLocation;
		m_pDeadEffects = CEffect::PlayEffectAtLocation(TEXT("PhotonFlashDead.dat"), mWorldMatrix);
		break;
	}
	case METEOR:
	{
		CBullet::BULLETDESC BulletDesc;
		BulletDesc.eCollisionGroup = PLAYER;
		BulletDesc.fVelocity = 1.f;
		BulletDesc.eBulletType = METEORDEAD;
		BulletDesc.iDamage = 300;
		BulletDesc.fDeadTime = 10.f;
		BulletDesc.vInitPositon = Get_TransformState(CTransform::STATE_TRANSLATION);
		BulletDesc.pOwner = m_BulletDesc.pOwner;

		if (CCameraManager::Get_Instance()->Get_CamState() == CCameraManager::CAM_DYNAMIC && Check_IsinFrustum(3.f) == true)
			dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera())->Set_ShakingMode(true, 1.4f, 0.1f);

		if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_RinwellSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
			return;
		break;
	}
	case THUNDER_FIELD:
	{
		if (!m_pBlast2Effects.empty())
		{
			for (auto& iter : m_pBlast2Effects)
			{
				if (iter != nullptr)
				{
					iter->Set_Dead(true);
				}
			}
		}
		break;
	}
	case BANGJEON:
	{
		if (!m_pBlastEffects.empty())
		{
			for (auto& iter : m_pBlast2Effects)
			{
				if (iter != nullptr)
				{
					iter->Set_Dead(true);
				}
			}
		}

		CBullet::BULLETDESC BulletDesc;
		BulletDesc.eCollisionGroup = PLAYER;
		BulletDesc.fVelocity = 1.f;
		BulletDesc.eBulletType = BANGEONDEAD;
		BulletDesc.iDamage = 300;
		BulletDesc.fDeadTime = 1.f;
		BulletDesc.vInitPositon = Get_TransformState(CTransform::STATE_TRANSLATION);
		BulletDesc.pOwner = m_BulletDesc.pOwner;

		if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_RinwellSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
			return;
		break;
	}
	case HOLY_RANCE:
	{
		if (!m_pBlastEffects.empty())
		{
			for (auto& iter : m_pBlastEffects)
			{
				if (iter != nullptr)
				{
					CParticleSystem* pParticleSystem = dynamic_cast<CParticleSystem*>(iter);
					if (pParticleSystem != nullptr)
						pParticleSystem->Set_Stop(true);
				}
			}
		}
		m_pBlastEffects.clear();

		break;
	}
	}
}


HRESULT CRinwellSkills::Ready_Components(void * pArg)
{
	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	/* For.Com_SPHERE */
	CCollider::COLLIDERDESC ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	switch (m_BulletDesc.eBulletType)
	{
	case PHOTON_FLASH:
		ColliderDesc.vScale = _float3(2.f, 2.f, 2.f);
		ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
		ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
		break;
	case GALE_FORCE:
		ColliderDesc.vScale = _float3(5.f, 5.f, 5.f);
		ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
		ColliderDesc.vPosition = _float3(0.f, 0.f, 1.f);
		break;

	case METEOR:
		ColliderDesc.vScale = _float3(15.f, 5.f, 15.f);
		ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
		ColliderDesc.vPosition = _float3(0.f, 0.f, 1.f);
		break;
	
	case THUNDER_FIELD:
		ColliderDesc.vScale = _float3(1.f, 10.f, 1.f);
		ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
		ColliderDesc.vPosition = _float3(0.f, 0.f, 1.f);
		break;

	case DIVINE_SABER:
		ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
		ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
		ColliderDesc.vPosition = _float3(0.f, 0.f, 1.f);
		break;

	case DIVINE_SABER_BULLET:
		ColliderDesc.vScale = _float3(0.5f, 7.f, 0.5f);
		ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
		ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
		break;

	case HOLY_RANCE:
		ColliderDesc.vScale = _float3(0.5f, 7.f, 0.5f);
		ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
		ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
		break;

	case HOLY_RANCE_BULLET:
		ColliderDesc.vScale = _float3(0.5f, 7.f, 0.5f);
		ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
		ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
		
		break;

	case BANGJEON:
		ColliderDesc.vScale = _float3(12.5f, 12.5f, 12.5f);
		ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
		ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
	}

	if (m_BulletDesc.eBulletType == THUNDER_FIELD || m_BulletDesc.eBulletType == DIVINE_SABER || m_BulletDesc.eBulletType == DIVINE_SABER_BULLET)
	{
		if (FAILED(__super::Add_Components(TEXT("Com_AABB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"), (CComponent**)&m_pAABBCom, &ColliderDesc)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(__super::Add_Components(TEXT("Com_SPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSPHERECom, &ColliderDesc)))
			return E_FAIL;
	}
	

	return S_OK;
}



void CRinwellSkills::Tick_PhotonFlash(_float fTimeDelta)
{
	if (m_bDeadEffect)
		m_bDead = true;

	m_pTransformCom->Go_PosDir(fTimeDelta, m_BulletDesc.vTargetDir);

	for (auto& iter : m_pEffects)
	{
		if (iter != nullptr && iter->Get_PreDead())
			iter = nullptr;

		if (iter != nullptr)
			iter->Set_State(CTransform::STATE_TRANSLATION, Get_TransformState(CTransform::STATE_TRANSLATION));
	}
	
	return;
}

void CRinwellSkills::Tick_GaleForce(_float fTimeDelta)
{
	m_fTime += fTimeDelta;

	if (m_fTime >= m_BulletDesc.fDeadTime)
		m_bDead = true;
}

void CRinwellSkills::Tick_Meteor(_float fTimeDelta)
{
	if (m_bDeadEffect)
		m_bDead = true;
	 
	//_vector vDir =  m_BulletDesc.vTargetDir;

	m_pTransformCom->Go_PosDir(fTimeDelta, m_BulletDesc.vTargetDir);

	for (auto& iter : m_pEffects)
	{
		if (iter != nullptr && iter->Get_PreDead())
			iter = nullptr;

		if (iter != nullptr)
			iter->Set_State(CTransform::STATE_TRANSLATION, Get_TransformState(CTransform::STATE_TRANSLATION));
	}

}

void CRinwellSkills::Tick_ThunderField(_float fTimeDelta)
{
	if (m_bDeadEffect)
		m_bDead = true;

	//_vector vDir =  m_BulletDesc.vTargetDir;

	//m_pTransformCom->LookAt(m_BulletDesc.vTargetPosition);
	m_fThunderStopTimer += fTimeDelta;

	if(m_fThunderStopTimer < 4.f)
	m_pTransformCom->Go_PosDir(fTimeDelta, m_BulletDesc.vTargetDir);

	for (auto& iter : m_pEffects)
	{
		if (iter != nullptr)
		{
			_vector vOffset = XMVectorSet(0.f, m_fRadius + 6.f, 0.f, 0.f);
			iter->Set_State(CTransform::STATE_TRANSLATION, Get_TransformState(CTransform::STATE_TRANSLATION) + vOffset);
		}
			
	}

	for (auto& iter : m_pBlast2Effects)
	{
		_vector vOffset = XMVectorSet(0.f, m_fRadius + 0.5f, 0.f, 0.f);
		if (iter != nullptr)
			iter->Set_State(CTransform::STATE_TRANSLATION, Get_TransformState(CTransform::STATE_TRANSLATION) + vOffset);
	}

}

void CRinwellSkills::Tick_DivineSaber(_float fTimeDelta)
{
	if (m_bDead == true)
		return;

	if (bulletcount > 30)
	{
		LightOffset.x += 0.001f;
		LightOffset.y -= 0.001f;
		LightOffset.z += 0.001f;

		if (LightOffset.x >= 0.1f)
			LightOffset.x = 0.1f;
		if (LightOffset.y <= -0.1f)
			LightOffset.y = -0.1f;
		if (LightOffset.z >= 0.1f)
			LightOffset.z = 0.1f;

		DevineLightDesc.vDiffuse.x = OriginLightDesc.vDiffuse.x + LightOffset.x;
		DevineLightDesc.vDiffuse.y = OriginLightDesc.vDiffuse.y + LightOffset.y;
		DevineLightDesc.vDiffuse.z = OriginLightDesc.vDiffuse.z + LightOffset.z;
		
		CGameInstance::Get_Instance()->Set_LightDesc(0, &DevineLightDesc);
	}


	
	m_fDivineTimer += fTimeDelta;

	if (m_fDivineTimer > 0.1f)
	{
		float offsetx = (_float)(rand() % m_fDivineOffset)*(rand() % 2 == 0 ? 1.f : -1.f) / 10;
		float offsetz = (_float)(rand() % m_fDivineOffset)*(rand() % 2 == 0 ? 1.f : -1.f) / 10;
		CBullet::BULLETDESC BulletDesc;
		_vector vLocation = { 0.f,0.f,0.f,0.f };
		BulletDesc.iDamage = rand() % 150 + 1;
		BulletDesc.vTargetPosition = m_BulletDesc.vTargetPosition;
		BulletDesc.vTargetPosition.m128_f32[0] += offsetx;
		BulletDesc.vTargetPosition.m128_f32[2] += offsetz;
		BulletDesc.eCollisionGroup = PLAYER;
		BulletDesc.pOwner = m_BulletDesc.pOwner;
		BulletDesc.eBulletType = CRinwellSkills::DIVINE_SABER_BULLET;
		BulletDesc.fDeadTime = 0.5f;

		if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_RinwellSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
			return;

			bulletcount -= 1;

		BulletDesc.iDamage = rand() % 150 + 1;
		BulletDesc.vTargetPosition = m_BulletDesc.vTargetPosition;
		BulletDesc.vTargetPosition.m128_f32[0] += offsetx;
		BulletDesc.vTargetPosition.m128_f32[2] += offsetz;
		BulletDesc.eCollisionGroup = PLAYER;
		BulletDesc.pOwner = m_BulletDesc.pOwner;
		BulletDesc.eBulletType = CRinwellSkills::DIVINE_SABER_BULLET;
		BulletDesc.fDeadTime = 0.5f;

		if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_RinwellSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
			return;

		bulletcount -= 1;
		m_fDivineOffset -= 1;
		m_fDivineTimer = 0.f;
	}

	if (bulletcount < 30)
	{
		
		LightOffset.x -= 0.01f;
		LightOffset.y += 0.01f;
		LightOffset.z -= 0.01f;

		if (LightOffset.x <= 0.f)
			LightOffset.x = 0.0f;
		if (LightOffset.y >= 0.f)
			LightOffset.y = 0.f;
		if (LightOffset.z <= 0.f)
			LightOffset.z = 0.f;

		DevineLightDesc.vDiffuse.x = OriginLightDesc.vDiffuse.x + LightOffset.x;
		DevineLightDesc.vDiffuse.y = OriginLightDesc.vDiffuse.y + LightOffset.y;
		DevineLightDesc.vDiffuse.z = OriginLightDesc.vDiffuse.z + LightOffset.z;

		CGameInstance::Get_Instance()->Set_LightDesc(0, &DevineLightDesc);
	}

	if (bulletcount <= 0)
	{
		m_bDead = true;
		CGameInstance::Get_Instance()->Set_LightDesc(0, &OriginLightDesc);
	}
		
}

void CRinwellSkills::Tick_DivineSaberBullet(_float fTimeDelta)
{
	m_fDivineTimer += fTimeDelta;
	if (m_fDivineTimer> 0.5f)
		m_bDead = true;

	for (auto& iter : m_pEffects)
	{
		if (iter != nullptr && iter->Get_PreDead())
			iter = nullptr;

		if (iter != nullptr)
			iter->Set_State(CTransform::STATE_TRANSLATION, Get_TransformState(CTransform::STATE_TRANSLATION));
	}

}

void CRinwellSkills::Tick_HolyRance(_float fTimeDelta)
{
	m_fholytimer += fTimeDelta;
    
	if (m_fholytimer > 0.5f && m_bholybullet[0])
	{
		m_bholybullet[0] = false;
		CBullet::BULLETDESC BulletDesc;
		BulletDesc.eCollisionGroup = m_BulletDesc.eCollisionGroup;
		BulletDesc.fVelocity = 5.f;
		BulletDesc.fDeadTime = 10.f;
		BulletDesc.eBulletType = CRinwellSkills::HOLY_RANCE_FISRTBULLET;
		BulletDesc.vInitPositon = m_BulletDesc.vTargetPosition;
		BulletDesc.vInitPositon.m128_f32[1] -= 4.f;
		BulletDesc.vTargetDir = XMVectorSet(0.f, 1.f, 0.f, 0.f);
		BulletDesc.vTargetPosition = m_BulletDesc.vTargetPosition;
		BulletDesc.pOwner = m_BulletDesc.pOwner;
		if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_RinwellSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
			return;

	}


	if (m_fholytimer > 0.8f && m_bholybullet[1])
	{
		m_bholybullet[1] = false;
		CBullet::BULLETDESC BulletDesc;
		BulletDesc.eCollisionGroup = m_BulletDesc.eCollisionGroup;
		BulletDesc.fVelocity = 5.f;
		BulletDesc.eBulletType = CRinwellSkills::HOLY_RANCE_BULLET;
		BulletDesc.vInitPositon = m_BulletDesc.vInitPositon;
		BulletDesc.vInitPositon.m128_f32[0] += 4.f;
		BulletDesc.vTargetPosition = m_BulletDesc.vTargetPosition;
		BulletDesc.vTargetDir = m_BulletDesc.vTargetPosition - BulletDesc.vInitPositon;
		BulletDesc.pOwner = m_BulletDesc.pOwner;
		if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_RinwellSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
			return;
		
	}

	if (m_fholytimer > 1.1f && m_bholybullet[2])
	{
		m_bholybullet[2] = false;
		CBullet::BULLETDESC BulletDesc;
		BulletDesc.eCollisionGroup = m_BulletDesc.eCollisionGroup;
		BulletDesc.fVelocity = 5.f;
		BulletDesc.eBulletType = CRinwellSkills::HOLY_RANCE_BULLET;
		BulletDesc.vInitPositon = m_BulletDesc.vInitPositon;
		BulletDesc.vInitPositon.m128_f32[0] += 4.f;
		BulletDesc.vTargetPosition = m_BulletDesc.vTargetPosition;
		BulletDesc.vTargetDir = m_BulletDesc.vTargetPosition - BulletDesc.vInitPositon;
		BulletDesc.pOwner = m_BulletDesc.pOwner;
		if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_RinwellSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
			return;
	}

	if (m_fholytimer > 1.4f && m_bholybullet[3])
	{
		m_bholybullet[3] = false;
		CBullet::BULLETDESC BulletDesc;
		BulletDesc.eCollisionGroup = m_BulletDesc.eCollisionGroup;
		BulletDesc.fVelocity = 5.f;
		BulletDesc.eBulletType = CRinwellSkills::HOLY_RANCE_BULLET;
		BulletDesc.vInitPositon = m_BulletDesc.vInitPositon;
		BulletDesc.vInitPositon.m128_f32[0] -= 8.f;

		BulletDesc.vTargetPosition = m_BulletDesc.vTargetPosition;
		BulletDesc.pOwner = m_BulletDesc.pOwner;
		if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_RinwellSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
			return;
	}

	if (m_fholytimer > 2.0f && m_bholybullet[4])
	{
		m_bholybullet[4] = false;
		CBullet::BULLETDESC BulletDesc;
		BulletDesc.eCollisionGroup = m_BulletDesc.eCollisionGroup;
		BulletDesc.fVelocity = 5.f;
		BulletDesc.eBulletType = CRinwellSkills::HOLY_RANCE_BULLET;
		BulletDesc.vInitPositon = m_BulletDesc.vInitPositon;
		BulletDesc.vInitPositon.m128_f32[0] += 6.f;

		BulletDesc.vTargetPosition = m_BulletDesc.vTargetPosition;
		BulletDesc.vTargetDir = m_BulletDesc.vTargetPosition - BulletDesc.vInitPositon;
		BulletDesc.pOwner = m_BulletDesc.pOwner;
		if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_RinwellSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
			return;
	}

	if (m_fholytimer > 2.3f && m_bholybullet[5])
	{
		m_bholybullet[5] = false;
		CBullet::BULLETDESC BulletDesc;
		BulletDesc.eCollisionGroup = m_BulletDesc.eCollisionGroup;
		BulletDesc.fVelocity = 5.f;
		BulletDesc.eBulletType = CRinwellSkills::HOLY_RANCE_BULLET;
		BulletDesc.vInitPositon = m_BulletDesc.vInitPositon;
		BulletDesc.vInitPositon.m128_f32[0] -= 6.f;

		BulletDesc.vTargetPosition = m_BulletDesc.vTargetPosition;
		BulletDesc.vTargetDir = m_BulletDesc.vTargetPosition - BulletDesc.vInitPositon;
		BulletDesc.pOwner = m_BulletDesc.pOwner;
		if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_RinwellSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
			return;
	}

	if (m_fholytimer > 3.5f && m_bholybullet[6])
	{
		m_bholybullet[6] = false;
		CBullet::BULLETDESC BulletDesc;
		BulletDesc.eCollisionGroup = m_BulletDesc.eCollisionGroup;
		BulletDesc.fVelocity = 5.f;
		BulletDesc.eBulletType = CRinwellSkills::HOLY_RANCE_LASTBULLET;
		BulletDesc.vInitPositon = m_BulletDesc.vInitPositon;
		BulletDesc.vInitPositon.m128_f32[0] += 2.f;
		BulletDesc.vTargetDir = XMVectorSet(0.f, -1.f, 0.f, 0.f);
		BulletDesc.vTargetPosition = m_BulletDesc.vTargetPosition;
		BulletDesc.pOwner = m_BulletDesc.pOwner;
		if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_RinwellSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
			return;
	}


	if (m_fholytimer > 3.5f)
	{

		m_bDead = true;
	}


	for (auto& iter : m_pBlastEffects)
	{
		if (iter != nullptr && iter->Get_PreDead())
			iter = nullptr;
	}

}

void CRinwellSkills::Tick_HolyRanceBullet(_float fTimeDelta)
{
	_vector vDir = XMVector3Normalize(m_BulletDesc.vTargetPosition - m_BulletDesc.vInitPositon);

	m_pTransformCom->Go_PosDir(fTimeDelta, vDir);

	for (auto& iter : m_pEffects)
	{
		if (iter != nullptr && iter->Get_PreDead())
			iter = nullptr;

		if (iter != nullptr)
		{
			iter->Set_State(CTransform::STATE_TRANSLATION, Get_TransformState(CTransform::STATE_TRANSLATION));
		}
			
	}
}

void CRinwellSkills::Tick_HolyRanceBullet2(_float fTimeDelta)
{
	m_pTransformCom->LookDir(m_BulletDesc.vTargetDir);
	m_pTransformCom->Go_PosDir(fTimeDelta, m_BulletDesc.vTargetDir);

	for (auto& iter : m_pEffects)
	{
		if (iter != nullptr && iter->Get_PreDead())
			iter = nullptr;

		if (iter != nullptr)
			iter->Set_State(CTransform::STATE_TRANSLATION, Get_TransformState(CTransform::STATE_TRANSLATION));
	}
}

void CRinwellSkills::Tick_BangJeon(_float fTimeDelta)
{
	if (m_bDeadEffect)
		m_bDead = true;

	for (auto& iter : m_pEffects)
	{
		if (iter != nullptr)
		{
			_vector vOffset = XMVectorSet(0.f, 4.f, 0.f, 0.f);
			iter->Set_State(CTransform::STATE_TRANSLATION, m_BulletDesc.pOwner->Get_TransformState(CTransform::STATE_TRANSLATION) + vOffset);
		}
			
	}

	for (auto& iter : m_pBlastEffects)
	{
		if (iter != nullptr)
		{
			_vector vOffset = XMVectorSet(0.f, 4.f, 0.f, 0.f);
			iter->Set_State(CTransform::STATE_TRANSLATION, m_BulletDesc.pOwner->Get_TransformState(CTransform::STATE_TRANSLATION) + vOffset);
		}
			
	}
}

CRinwellSkills * CRinwellSkills::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CRinwellSkills* pInstance = new CRinwellSkills(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CRinwellSkills"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CRinwellSkills::Clone(void * pArg)
{
	CRinwellSkills* pInstance = new CRinwellSkills(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CRinwellSkills"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRinwellSkills::Free()
{
	__super::Free();
	m_bMeteorSound = false;
	
}
