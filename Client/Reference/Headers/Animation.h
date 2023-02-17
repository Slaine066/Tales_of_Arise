#pragma once

#include "Base.h"

BEGIN(Engine)

class CAnimation final : public CBase
{
private:
	CAnimation();
	virtual ~CAnimation() = default;

public:
	vector<ANIMEVENT> Get_Events(void) { return m_vecAnimEvent; }

public:
	HRESULT Initialize(HANDLE hFile, _ulong* pdwByte, class CModel* pModel, HANDLE hAddFile, _ulong* pdwAddByte);
	HRESULT Bin_Initialize(DATA_BINANIM* pAIAnimation, class CModel* pModel); // 추가

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

	/* 현재 애니메이션 하나를 재생하는데 걸리는 총 시간. */
	_float				m_fDuration = 0.f;

	/* 1초에 재생되어야할 애니메이션의 속도. */	
	_float				m_fTickPerSecond = 0.f;
	_float				m_fCurrentTime = 0.f;

	/* Channel : 뼈. */
	/* 이 애니메이션을 구동하는데 필요한 뼈의 갯수. */
	_uint							m_iNumChannels = 0;
	vector<class CChannel*>			m_Channels;

	_bool							m_isFinished = false;
	_bool							m_isLoop = true;

	/*For. Linear Time*/
	_float m_fTotal_Linear_Duration = 0.1f; //1더하면
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
	
//public: // For. Data 추가
//	void Get_AnimData(DATA_BINANIM* pAnimData);

public:
	//static CAnimation* Create(class CModel* pModel, aiAnimation* pAIAnimation);
	static CAnimation* Create(HANDLE hFile, _ulong* pdwByte, class CModel* pModel, HANDLE hAddFile, _ulong* pdwAddByte);
	static CAnimation* Bin_Create(DATA_BINANIM* pAIAnimation, class CModel* pModel); // 추가
	virtual void Free() override;
};

END