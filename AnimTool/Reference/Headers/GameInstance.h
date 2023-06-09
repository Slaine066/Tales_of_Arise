#pragma once

#include "Graphic_Device.h"
#include "Input_Device.h"
#include "Timer_Manager.h"
#include "Level_Manager.h"
#include "Object_Manager.h"
#include "Component_Manager.h"
#include "PipeLine.h"
#include "Light_Manager.h"
#include "Font_Manager.h"
#include "Picking.h"
#include "Texture_Manager.h"

BEGIN(Engine)

class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance)
private:
	CGameInstance();
	virtual ~CGameInstance() = default;

public: /* For.Engine */
	HRESULT Initialize_Engine(HINSTANCE hInst, _uint iNumLevels, const GRAPHIC_DESC& GraphicDesc, ID3D11Device** ppDevice, ID3D11DeviceContext** ppContext);
	void Tick_Engine(_float fTimeDelta);

	void Clear(_uint iLevelIndex);

public: /* For.Graphic_Device */
	HRESULT Clear_BackBuffer_View(_float4 vClearColor);
	HRESULT Clear_DepthStencil_View(void);
	HRESULT Present();

public: /* For.Input_Device */
	_char Get_DIMKeyState(DIMK eMouseKeyID);
	_long Get_DIMMoveState(DIMM eMouseMoveID);

public: /* For.Timer_Manager */
	HRESULT Add_Timer(const _tchar* pTimerTag);
	_float Get_TimeDelta(const _tchar* pTimerTag);
	void Tick(const _tchar* pTimerTag);

public: /* For.Level_Manager */
	HRESULT Open_Level(unsigned int iLevelIndex, class CLevel* pNewLevel);

public: /* For.Object_Manager */
	HRESULT Add_Prototype(const _tchar* pPrototypeTag, class CGameObject* pPrototype);
	HRESULT Add_GameObject(const _tchar* pPrototypeTag, _uint iLevelIndex, const _tchar* pLayerTag, void* pArg = nullptr);
	class CComponent* Get_Component(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pComponentTag, _uint iIndex = 0);
	_uint Get_LayerCount(_uint iLevelIndex, const _tchar* pLayerTag);
	list<class CGameObject*> Get_GameObjects(_uint iLevelIndex, const _tchar* pLayerTag);
	class CGameObject* Clone_GameObject(const _tchar* pPrototypeTag, void* pArg = nullptr);

public: /* For.Component_Manager */
	HRESULT Add_Prototype(_uint iLevelIndex, const _tchar* pPrototypeTag, class CComponent* pPrototype);
	class CComponent* Clone_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, void* pArg = nullptr);
	
public: /* For.PipeLine */
	void Set_Transform(CPipeLine::TRANSFORMSTATE eState, _fmatrix TransformMatrix);
	_matrix Get_TransformMatrix(CPipeLine::TRANSFORMSTATE eState);
	_float4x4 Get_TransformFloat4x4(CPipeLine::TRANSFORMSTATE eState);
	_float4x4 Get_TransformFloat4x4_TP(CPipeLine::TRANSFORMSTATE eState);
	_float4 Get_CamPosition(void);

public: /* For.Light_Manager */
	const LIGHTDESC* Get_LightDesc(_uint iIndex);
	void Set_LightDesc(_uint iIndex, LIGHTDESC _tLightDesc);
	HRESULT Add_Light(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const LIGHTDESC& LightDesc);
	list<class CLight*> Get_Lights(void);
	void Erase_Light(_uint iIndex);

public:	/* For.Font_Manager */
	HRESULT Add_Fonts(ID3D11Device*	pDevice, ID3D11DeviceContext* pContext, const _tchar* pFontTag, const _tchar* pFontFilePath);
	HRESULT Render_Font(const _tchar* pFontTag, const _tchar* pText, _fvector vPosition, _fvector vColor);

public: /* For.Texture_Manager */
	HRESULT Add_Texture(const _tchar* pTextureTag, class CTexture* pTexture);
	class CTexture* Find_Texture(const _tchar* pTextureTag);

public:
	static void Release_Engine(void);

private:
	CGraphic_Device* m_pGraphic_Device = nullptr;
	CInput_Device* m_pInput_Device = nullptr;
	CTimer_Manager* m_pTimer_Manager = nullptr;
	CLevel_Manager* m_pLevel_Manager = nullptr;
	CObject_Manager* m_pObject_Manager = nullptr;
	CComponent_Manager* m_pComponent_Manager = nullptr;
	CPipeLine* m_pPipeLine = nullptr;
	CLight_Manager* m_pLight_Manager = nullptr;
	CFont_Manager* m_pFont_Manager = nullptr;
	CPicking* m_pPicking = nullptr;
	CTexture_Manager* m_pTexture_Manager = nullptr;

public:
	virtual void Free(void) override;
};

END