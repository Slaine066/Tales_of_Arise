#include "..\Public\PhysX.h"

#include "GameObject.h"
IMPLEMENT_SINGLETON(CPhysX)

CPhysX::CPhysX()
{
}


HRESULT CPhysX::Initialize(HWND hWnd, _uint iWinCX, _uint iWinCY, ID3D11Device*	pDevice, ID3D11DeviceContext* pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;
	Safe_AddRef(m_pDevice); Safe_AddRef(m_pContext);
	m_hWnd = hWnd;
	m_iWinCX = iWinCX;
	m_iWinCY = iWinCY;

	/* 1. Make PxFoundation */
	/* Every PhysX module requires a PxFoundation instance to be available */
	m_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_AllocatorCallback,
		m_ErrorCallback);
	if (!m_Foundation)
	{
		ERR_MSG(TEXT("PxCreateFoundation failed!"));
		return E_FAIL;
	}


	/* 2. �ֻ��� PxPhysics ��ü�� �����*/
	bool  recordMemoryAllocations = true; //�޸� �������ϸ��� �������� ���� ����
	m_Pvd = PxCreatePvd(*m_Foundation);
	PxPvdTransport*  transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	m_Pvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	/*PhysX�� �Է��� �ش� ������ �ϰ��ǰ� ����ϴ� �� ���� �Ǵ� ���� ������ ������� �ùٸ� ����� �����ϵ��� ����Ǿ����ϴ�.
	�׷��� ������ ���� �⺻���� �����ؾ� �ϴ� Ư�� ���� ���� �ֽ��ϴ�.
	�̷��� ��� ������ ������ ������ �⺻ �����ǵ��� �Ϸ��� PxPhysics �� PxCooking �������̽��� ������ ��
	PxTolerancesScale�� ���� �����Ͻʽÿ�. ��ü�� ���� ������ ���� �� �����Ǹ� ���� ���α׷����� �������� �� �ֽ��ϴ�.*/
	PxTolerancesScale scale = PxTolerancesScale();
	scale.length = 1.f;
	scale.speed = 10.f;
	m_Physics = customCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation,scale, recordMemoryAllocations, m_Pvd);
	if (!m_Physics)
	{
		ERR_MSG(TEXT("PxCreatePhysics failed!"));
		return E_FAIL;
	}

	/* 3. PxCreateCooking  */
	m_Cooking = PxCreateCooking(PX_PHYSICS_VERSION, *m_Foundation, PxCookingParams(scale));
	if (!m_Cooking)
	{
		ERR_MSG(TEXT("PxCreateCooking failed!"));
		return E_FAIL;
	}

	/* 4. Ȯ�� ���̺귯�� �ʱ�ȭ */
	if (!PxInitExtensions(*m_Physics, m_Pvd))
	{
		ERR_MSG(TEXT("PxInitExtensions failed!"));
		return E_FAIL;
	}
	

	return S_OK;
}

HRESULT CPhysX::CreateScene()
{
	// mNbThreads �۾��� �������� CPU ����ó ���� 
	SYSTEM_INFO info;
	GetSystemInfo(&info);
	m_CpuDispatcher = PxDefaultCpuDispatcherCreate(info.dwNumberOfProcessors); //CPU�� ������ ����
	if(!m_CpuDispatcher)
	{
		ERR_MSG(TEXT("PxDefaultCpuDispatcherCreate failed!"));
		return E_FAIL;
	}

	// Scene�� Set ���ݴϴ�.
	PxSceneDesc  sceneDesc(m_Physics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f); //default gravity
	sceneDesc.cpuDispatcher = m_CpuDispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	m_Scene = m_Physics->createScene(sceneDesc);

	PxPvdSceneClient* pvdClient = m_Scene->getScenePvdClient();
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}
	
	//default materials   //static friction, dynamic friction, restitution
	m_Material = m_Physics->createMaterial(0.5f, 0.5f, 0.5f);// �浹ü ������, dynamic ������, ź����

	//create a static plane (XZ)
	m_groundPlane = PxCreatePlane(*m_Physics, PxPlane(0, 1, 0, 0), *m_Material); // ������ ����� �ݴϴ�
	m_Scene->addActor(*m_groundPlane);

	m_ControllerManager = PxCreateControllerManager(*m_Scene);

	return S_OK;
}

HRESULT CPhysX::CleanScene()
{
	PX_RELEASE(m_ControllerManager);
	PX_RELEASE(m_Scene);
	PX_RELEASE(m_CpuDispatcher);
	return S_OK;
}

void CPhysX::Tick(_float TimeDelta)
{
	if (m_Scene)
	{
		m_Scene->simulate(TimeDelta);
		m_Scene->fetchResults(true);
	}
}

void CPhysX::Add_Actor(PxActor * actor)
{
	m_Scene->addActor(*actor);
}

PxPhysics * CPhysX::customCreatePhysics(PxU32 version, PxFoundation & foundation, const PxTolerancesScale & scale, bool trackOutstandingAllocations, PxPvd * pvd)
{
	PxPhysics* physics = PxCreateBasePhysics(version, foundation, scale,trackOutstandingAllocations, pvd);

	if (!physics)
		return nullptr;

	PxRegisterHeightFields(*physics);


	return physics;
}

void CPhysX::Free()
{
	PX_RELEASE(m_ControllerManager);
	PX_RELEASE(m_Scene);
	PX_RELEASE(m_CpuDispatcher);

	PxCloseExtensions();
	PX_RELEASE(m_Physics);
	PX_RELEASE(m_Cooking);

	if (m_Pvd)
	{
		PxPvdTransport* transport = m_Pvd->getTransport();
		m_Pvd->release();	m_Pvd = nullptr;
		PX_RELEASE(transport);

	}
	PX_RELEASE(m_Foundation);

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

}
