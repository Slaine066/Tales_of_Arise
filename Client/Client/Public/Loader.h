#pragma once

#include "Client_Defines.h"
#include "Base.h"

/* 다음레벨을 위한 데이터를 준비한다. */

BEGIN(Client)

class CLoader final : public CBase
{
private:
	CLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLoader() = default;

public:
	LEVEL Get_NextLevelID() const {
		return m_eNextLevel;
	}

	CRITICAL_SECTION Get_CriticalSection() {
		return m_CriticalSection;
	}

	_bool Get_Finished() const {
		return m_isFinished;
	}

	const _tchar* Get_LoadingText() const {
		return m_szLoadingText;
	}

	void Set_Finished(_bool tof) { m_isFinished = tof; }

public:
	HRESULT Initialize(LEVEL eNextLevel);


	HRESULT Loading_ForLogoLevel();
	HRESULT Loading_ForPrototype();
	HRESULT Loading_ForStaticLevel();
	HRESULT Loading_ForBattleLevel();
	HRESULT Loading_ForCityLevel();
	HRESULT Loading_ForSnowFieldLevel();
	HRESULT Loading_ForRestaurantLevel();
	HRESULT Loading_ForWorkToolLevel();
	HRESULT Loading_ForBossLevel();
	HRESULT Loading_ForLawBattleLevel();

	HRESULT Loading_ForUITexture();
	HRESULT Loading_ForEffect();
	HRESULT Loading_ForEffectTexture();
	HRESULT Loading_ForStaticMapObject();

	HRESULT Loading_ForInteract();



private:
	LEVEL					m_eNextLevel = LEVEL_END;
	_bool					m_isFinished = false;
	_tchar					m_szLoadingText[MAX_PATH] = TEXT("");
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pContext = nullptr;

private:
	HANDLE					m_hThread = 0;
	CRITICAL_SECTION		m_CriticalSection;


public:
	static CLoader* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eNextLevel);
	virtual void Free();
};

END