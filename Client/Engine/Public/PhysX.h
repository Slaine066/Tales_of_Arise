#pragma once
#include "Base.h"

BEGIN(Engine)

class CGameObject;

class ENGINE_DLL CPhysX final : public CBase
{
	DECLARE_SINGLETON(CPhysX)
public:
	enum GEOMETRY { GEOMETRY_SPHERE, GEOMETRY_BOX, GEOMETRY_CAPSULE, GEOMETRY_END };

private:
	CPhysX();
	virtual ~CPhysX() = default;
	
public:
	HRESULT Initialize(HWND hWnd, _uint iWinCX, _uint iWinCY, ID3D11Device*	pDevice, ID3D11DeviceContext* pContext);
	HRESULT CreateScene();
	HRESULT CleanScene();
	void Tick(_float TimeDelta);

public:
	void Add_Actor(PxActor* actor);

public:
	PxControllerManager* Get_ControllerManager() { return m_ControllerManager; }
	PxMaterial* Get_Material() { return m_Material; }
	PxScene* Get_Scene() { return m_Scene; }
	PxPhysics* Get_Physics() { return m_Physics; }
	PxCooking* Get_Cooking() { return m_Cooking; }

private:
	/* heightField�� �ʿ��� ��*/
	PxPhysics* customCreatePhysics(PxU32 version, PxFoundation& foundation, const PxTolerancesScale& scale, bool trackOutstandingAllocations, PxPvd* pvd);

private:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;
	HWND				m_hWnd;
	_uint				m_iWinCX, m_iWinCY;

	/* 1. Initialize�� Foundation ������ ���� �ʿ��� ����*/
	PxDefaultErrorCallback  m_ErrorCallback;
	PxDefaultAllocator  m_AllocatorCallback;
	PxFoundation*		m_Foundation = nullptr;
	// PxFoundation�� �־�� gPhysics�� ������ �� �ִ�

	/* 2. �ֻ��� PxPhysics �� ����� ���� �ʿ��� ���� */
	PxPhysics*			m_Physics = nullptr;
	PxPvd*				m_Pvd = nullptr;

	/* 3. PxCooking �� ����� ���� ����*/
	PxCooking*			m_Cooking = nullptr;

	/* 4. CPU���ҽ��� ȿ�������� ������ �� �ֵ��� �ϱ� ���� �����ϴ� ���� ��õ */
	PxDefaultCpuDispatcher* m_CpuDispatcher = nullptr;		
	PxControllerManager*	m_ControllerManager = nullptr;
	PxScene*				m_Scene = nullptr;

	// gPhysics�� ����� createMaterial�ؼ� ���
	// �浹�� ������, Dynamic ������, ź������ �����Ͽ� ���
	PxMaterial* m_Material = NULL;

private: /* test */
	PxRigidStatic* m_groundPlane = nullptr;

public:
	virtual void Free() override;
};

END