#pragma once

#include "Base.h"

BEGIN(Engine)

class CAnimation final : public CBase
{
public:
	typedef struct tagAnimationEvent
	{
		_float fStartTime;
		_float fEndTime;
		EVENTTYPE eType;
	}ANIMEVENT;

private:
	CAnimation();
	virtual ~CAnimation() = default;

public:
	vector<EVENT> Get_Events(void) { return m_vecEvents; }

public:
	HRESULT Initialize(HANDLE hFile, _ulong* pdwByte, class CModel* pModel, HANDLE hAddFile, _ulong* pdwAddByte);
	HRESULT Bin_Initialize(DATA_BINANIM* pAIAnimation, class CModel* pModel); // �߰�
	_bool Invalidate_TransformationMatrix(_float fTimeDelta, _bool isLoop);
	_bool Animation_Linear_Interpolation(_float fTimeDelta, CAnimation* NextAnim);

public:
	vector<class CChannel*>	Get_Channels(void) { return m_Channels; }
	void	Set_TimeReset();
	void Reset_Events(void);

private:
	char				m_szName[MAX_PATH] = "";

	/* ���� �ִϸ��̼� �ϳ��� ����ϴµ� �ɸ��� �� �ð�. */
	_float				m_fDuration = 0.f;

	/* 1�ʿ� ����Ǿ���� �ִϸ��̼��� �ӵ�. */	
	_float				m_fTickPerSecond = 0.f;
	_float				m_fCurrentTime = 0.f;

	/* Channel : ��. */
	/* �� �ִϸ��̼��� �����ϴµ� �ʿ��� ���� ����. */
	_uint							m_iNumChannels = 0;
	vector<class CChannel*>			m_Channels;

	_bool							m_isFinished = false;
	_bool							m_isLoop = true;

	/*For. Linear Time*/
	_float m_fTotal_Linear_Duration = 2.2f;
	_float m_fLinear_CurrentTime = 0.f;
	_bool	m_bLinearFinished = false;

	/* For. Original TickPerSecond */
	_float m_fOriTickPerSecond = 0.f;
	/* For. Animation Playing Time */
	vector<_float> m_TickPerSeconds;
	vector<_float> m_ChangeTickTimes;
	_int m_iTickPerSecondIndex = 0;
	/* For. Animation Event */
	vector<ANIMEVENT> m_vecAnimEvent;
	vector<EVENT> m_vecEvents;

//public: // For. Data �߰�
//	void Get_AnimData(DATA_BINANIM* pAnimData);

public:
	//static CAnimation* Create(class CModel* pModel, aiAnimation* pAIAnimation);
	static CAnimation* Create(HANDLE hFile, _ulong* pdwByte, class CModel* pModel, HANDLE hAddFile, _ulong* pdwAddByte);
	static CAnimation* Bin_Create(DATA_BINANIM* pAIAnimation, class CModel* pModel); // �߰�
	virtual void Free() override;
};

END