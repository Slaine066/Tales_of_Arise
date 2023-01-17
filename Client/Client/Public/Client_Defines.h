#pragma once

namespace Client
{
	const unsigned int g_iWinSizeX = 1280;
	const unsigned int g_iWinSizeY = 720;

	enum DIRID { DIR_STRAIGHT, DIR_BACKWARD, DIR_LEFT, DIR_RIGHT, DIR_STRAIGHT_LEFT, DIR_STRAIGHT_RIGHT, DIR_BACKWARD_LEFT, DIR_BACKWARD_RIGHT, DIR_END };
	enum LEVEL { LEVEL_STATIC, LEVEL_LOADING, LEVEL_LOGO, LEVEL_GAMEPLAY, LEVEL_END };
	enum SOUND_ID { SOUND_BGM, SOUND_SYSTEM, SOUND_VOICE, SOUND_FOOT, SOUND_WEAPON, SOUND_EFFECT, SOUND_OBJECT, SOUND_END };
	enum SHADER_ANIMID { SHADER_ANIMDEFAULT, SHADER_ANIMSHADOW };
	enum SHADER_NONANIMID { SHADER_NONANIMDEFAULT, SHADER_NONANIM_SHADOW };


	typedef struct ObjectInfo
	{
		int iMaxHp;
		int iCurrentHp;
		int iDamage;
		int iCoin = 100;

	}OBJINFO;

	/*For Anim State*/
	enum STATETYPE
	{
		STATETYPE_START,
		STATETYPE_MAIN,
		STATETYPE_END,
		STATETYPE_DEFAULT
	};
}

#include <process.h>
#include <time.h>

extern HWND			g_hWnd;
extern HINSTANCE	g_hInst;

using namespace Client;