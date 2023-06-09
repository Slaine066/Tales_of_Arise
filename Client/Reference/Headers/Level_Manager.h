#pragma once

/*  현재 화면에 보여줘야할 레벨의 주소를 보관한다. 현재레벨의 인덱스도 보관.  */
/* 레벨의 업데이트 처리를 한다. */
/* 레벨의 렌더처리.(x) */
/* 이전레벨에서 사용했던 리소스들을 정리한다. */

#include "Base.h"

BEGIN(Engine)

class CLevel_Manager final : public CBase
{
	DECLARE_SINGLETON(CLevel_Manager)
public:
	CLevel_Manager();
	virtual ~CLevel_Manager() = default;

public:
	HRESULT Open_Level(unsigned int iLevelIndex, class CLevel* pNewLevel);
	void Tick(_float fTimeDelta);
	void Late_Tick(_float fTimeDelta);
	

public:
	class CLevel* Get_CurrentLevel() { return m_pCurrentLevel; }
	_uint Get_CurrentLevelIndex() { return m_iLevelIndex; }
	_uint Get_DestinationLevelIndex() { return m_iDestinationIndex; }
	_uint Get_PastLevelIndex() { return m_iPastLevelIndex; }
	void  Set_DestinationLevel(_uint LevelIndex) { m_iDestinationIndex = LevelIndex; }

private:
	class CLevel*				m_pCurrentLevel = nullptr;
	_uint						m_iLevelIndex;
	_uint						m_iDestinationIndex;
	_uint						m_iPastLevelIndex;

public:
	virtual void Free() override;
};

END

