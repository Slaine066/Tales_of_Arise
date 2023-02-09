#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform final : public CComponent
{
public:
	enum STATE { STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_TRANSLATION, STATE_END };

public:
	typedef struct tagTransformDesc
	{
		float		fSpeedPerSec;
		float		fRotationPerSec;
	}TRANSFORMDESC;

private:
	CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTransform(const CTransform& rhs);
	virtual ~CTransform() = default;

public: /*Getter Setter */
	_vector Get_State(STATE eState) const {return XMLoadFloat4((_float4*)&m_WorldMatrix.m[eState][0]);}	
	/* ���Ϲ��� ����� �̿��� ������ �ؾ��� ��. */
	_matrix Get_WorldMatrix() const {return XMLoadFloat4x4(&m_WorldMatrix);}
	/* ���Ϲ��� ��ĺ����ؾ��� ��  */
	_float4x4 Get_World4x4() const {return m_WorldMatrix;}
	const _float4x4* Get_World4x4Ptr() const {return &m_WorldMatrix;}

	/* ���Ϲ��� ����� ���̴��� ����������.  */
	_float4x4 Get_World4x4_TP() const {
		_float4x4	TransposeMatrix;
		XMStoreFloat4x4(&TransposeMatrix, XMMatrixTranspose(Get_WorldMatrix()));
		return TransposeMatrix;
	}
	_float Get_Scale(STATE eState) {return XMVectorGetX(XMVector3Length(XMLoadFloat4x4(&m_WorldMatrix).r[eState]));}
	_matrix Get_WorldMatrixInverse() const { return XMMatrixInverse(nullptr, Get_WorldMatrix()); }

	void Set_State(STATE eState, _fvector vState) {
		_matrix		WorldMatrix = XMLoadFloat4x4(&m_WorldMatrix);
		WorldMatrix.r[eState] = vState;
		XMStoreFloat4x4(&m_WorldMatrix, WorldMatrix);	
	}

	void Set_WorldMatrix(_matrix mWorldMatrix) { XMStoreFloat4x4(&m_WorldMatrix, mWorldMatrix); }
	
	void Set_Scale(STATE eState, _float fScale);
	void Set_TransformDesc(const TRANSFORMDESC& TransformDesc ) {m_TransformDesc = TransformDesc;}
	void Change_Speed(_float fSpeed) { m_TransformDesc.fSpeedPerSec = fSpeed; }
	void Change_RotationPerSec(_float fRotationPerSec) { m_TransformDesc.fRotationPerSec = fRotationPerSec; }

	_float Get_CurrentRotationX() { return m_fCurrentRotationX; }
	_float Get_CurrentRotationY() { return m_fCurrentRotationY; }
	_float Get_CurrentRotationZ() { return m_fCurrentRotationZ; }

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);

public: /* Not Use Sliding */
	bool Go_Straight(_float fTimeDelta, class CNavigation* pNavigation = nullptr, _float fRadius = 1.f);
	bool Go_Backward(_float fTimeDelta, class CNavigation* pNavigation = nullptr, _float fRadius = 1.f);
	bool Go_Left(_float fTimeDelta, class CNavigation* pNavigation = nullptr, _float fRadius = 1.f);
	bool Go_Right(_float fTimeDelta, class CNavigation* pNavigation = nullptr, _float fRadius = 1.f);
	bool Go_Up(_float fTimeDelta, class CNavigation* pNavigation = nullptr, _float fRadius = 1.f);

public: /* Use Sliding */
	bool Sliding_Straight(_float fTimeDelta, class CNavigation* pNavigation = nullptr, _float fRadius = 1.f);
	bool Sliding_Backward(_float fTimeDelta, class CNavigation* pNavigation = nullptr, _float fRadius = 1.f);
	bool Sliding_Left(_float fTimeDelta, class CNavigation* pNavigation = nullptr, _float fRadius = 1.f);
	bool Sliding_Right(_float fTimeDelta, class CNavigation* pNavigation = nullptr, _float fRadius = 1.f);

public: /* Use Sliding while Animation */
	bool Sliding_Anim(_vector vecMove, _float fRotation, class CNavigation* pNavigation = nullptr);

public:
	void Jump(_float fTimeDelta, _float fVelocity, _float fGravity, _float fStartiHeight, _float fEndiHeight);
	bool Go_PosTarget(_float fTimeDelta, _vector TargetPos, class CNavigation* pNavigation = nullptr);
	bool Go_PosDir(_float fTimeDelta, _vector vecDir, class CNavigation* pNavigation = nullptr);
	bool Go_PosLerp(_float fTimeDelta, _vector TargetPos, _float fVecOffset);

public:/* Rotations */
	void Turn(_fvector vAxis, _float fTimeDelta);
	void Rotation(_fvector vAxis, _float fRadian);
	void Set_Rotation(_float3 fAngle);
	void Set_RotationY(_float fAngleY);

public:
	void LookAt(_fvector vAt);
	void LookDir(_fvector vDir);
	void LookAtExceptY(_fvector vAt);

public:
	void RoamingTurn(_matrix WorldMatrix, _float fRadian);

private:			
	_float4x4 m_WorldMatrix;
	TRANSFORMDESC m_TransformDesc;

	_float m_fCurrentRotationX = 0.f;
	_float m_fCurrentRotationY = 0.f;
	_float m_fCurrentRotationZ = 0.f;

public:
	static CTransform* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END