#pragma once

#include "Client_Defines.h"
#include "BaseObj.h"


BEGIN(Engine)

class CNavigation;
class CModel;

END


BEGIN(Client)

class CPlayer final : public CBaseObj
{	
public:
	enum ANIM {
		ANIM_ADVENT, ANIM_ARISE_B, ANIM_ARISE_F, ANIM_ATTACK_AKIZAME, ANIM_ATTACK_BAKUSAIZAN, ANIM_ATTACK_DANKUUKEN, ANIM_ATTACK, ANIM_END
	};

	enum PARTS { PARTS_WEAPON, PARTS_END };
private:
	CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer(const CPlayer& rhs);
	virtual ~CPlayer() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual int Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT Render_ShadowDepth() override;

public: /*For Navigation*/
	void	Change_Navigation(LEVEL eLevel);
	void	Check_Navigation();
	void	Compute_CurrentIndex(LEVEL eLevel);

private:
	CModel*					m_pModelCom = nullptr;
	CNavigation*			m_pNavigationCom = nullptr;
	vector<CNavigation*>	m_vecNavigaitions;
	_uint					m_eState = ANIM_ADVENT;
	_uint					m_ePreState = ANIM_END;

private:
	vector<class CGameObject*>			m_Parts;

private:
	HRESULT Ready_Parts();
	HRESULT Ready_Components(void* pArg);
	HRESULT SetUp_ShaderResources(); /* ���̴� ���������� ���� �����Ѵ�. */


public:
	static CPlayer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END