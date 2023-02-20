#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CAnimation final : public CBase
{
private:
	CAnimation();
	virtual ~CAnimation() = default;

public:
	const char* Get_Name(void) const { return m_szName; }
	_bool Get_Finished(void) const { return m_isFinished; }
	_float Get_Duration(void) const { return m_fDuration; }
	_float Get_TickPerSecond(void) const { return m_fTickPerSecond; }
	vector<_float> Get_TickPerSeconds(void) const { return m_TickPerSeconds; }
	vector<_float> Get_ChangeTickTimes(void) const { return m_ChangeTickTimes; }
	_float Get_CurrentTime(void) const { return m_fCurrentTime; }
	vector<ANIMEVENT> Get_Events(void) const { return m_vecAnimEvent; }
	_float Get_OriginalTickPerSecond(void) const { return m_fOriTickPerSecond; }

	void Add_EventCall(ANIMEVENT eEvent) { m_vecAnimEvent.push_back(eEvent); }
	void Add_TickPerSecond(_float fTime, _float fTick);
	void Fix_TickPerSecond(_int iTimeChoice, _float fAnimationTickTime, _float fAnimationTick);
	void Delete_TickPerSecond(_int iTimeChoice);
	void Add_Event(ANIMEVENT tEvent) { m_vecAnimEvent.push_back(tEvent); }
	void Fix_Event(_int iIndex, ANIMEVENT tEvent) { memcpy(&m_vecAnimEvent[iIndex], &tEvent, sizeof(ANIMEVENT)); }
	void Delete_Event(_int iIndex);

public:
	void Change_Duration(_float fDuration);

	void Reset_NonLoop(void);

public:
	HRESULT Initialize(HANDLE hFile, _ulong* pdwByte, class CModel* pModel, HANDLE hAddFile, _ulong* pdwAddByte);
	HRESULT Bin_Initialize(DATA_BINANIM* pAIAnimation, class CModel* pModel); // �߰�

	_bool Invalidate_TransformationMatrix(_float fTimeDelta, _bool isLoop, const char* pBoneName);
	_bool Animation_Linear_Interpolation(_float fTimeDelta, CAnimation* NextAnim, const char* pBoneName);

	_bool Is_Keyframe(char* pChannelName, _uint iKeyframe);
	_bool Under_Keyframe(char* pChannelName, _uint iKeyframe);
	_bool Over_Keyframe(char* pChannelName, _uint iKeyframe);
	_bool Between_Keyframe(char * pChannelName, _uint iKeyframeLower, _uint iKeyframeUpper);

public:
	vector<class CChannel*>	Get_Channels(void) { return m_Channels; }
	void	Set_TimeReset();
	void Reset(void);
	void Reset_ChannelKeyFrame(void);
	void Set_CurrentTime(_float fTime) { m_fCurrentTime = fTime; }

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
	_float m_fTotal_Linear_Duration = 0.1f; //1���ϸ�
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
	
//public: // For. Data �߰�
//	void Get_AnimData(DATA_BINANIM* pAnimData);

public:
	//static CAnimation* Create(class CModel* pModel, aiAnimation* pAIAnimation);
	static CAnimation* Create(HANDLE hFile, _ulong* pdwByte, class CModel* pModel, HANDLE hAddFile, _ulong* pdwAddByte);
	static CAnimation* Bin_Create(DATA_BINANIM* pAIAnimation, class CModel* pModel); // �߰�
	virtual void Free() override;
};

END