#pragma once

namespace Client
{
	const unsigned int g_iWinSizeX = 1280;
	const unsigned int g_iWinSizeY = 720;

	enum DIRID { DIR_STRAIGHT, DIR_BACKWARD, DIR_LEFT, DIR_RIGHT, DIR_STRAIGHT_LEFT, DIR_STRAIGHT_RIGHT, DIR_BACKWARD_LEFT, DIR_BACKWARD_RIGHT, DIR_END };
	enum LEVEL { LEVEL_STATIC, LEVEL_LOADING, LEVEL_LOGO, LEVEL_SNOWFIELD, LEVEL_BATTLE, LEVEL_END };
	enum SOUND_ID { SOUND_BGM, SOUND_SYSTEM, SOUND_VOICE, SOUND_FOOT, SOUND_WEAPON, SOUND_EFFECT, SOUND_OBJECT, SOUND_END };
	
	/* Shader Enums */
	enum SHADER_ANIMID { SHADER_ANIMDEFAULT, SHADER_ANIMSHADOW, SHADER_ANIM_DISSLOVE};
	enum SHADER_NONANIMID { SHADER_NONANIMDEFAULT, SHADER_NONANIM_SHADOW, SHADER_NONANIM_WATER, SHADER_NONANIM_DISSOLVE	};
	enum SHADER_EFFECT { SHADER_DEFAULT, SHADER_ALPHAMASK, SHADER_DISTORTION, SHADER_END };


	enum PLAYER_MODE { ACTIVE, AI_MODE, UNVISIBLE };

	enum ITEM_NAME { ITEMNAME_LEMONJELLY, ITEMNAME_HWANGJELLY , ITEMNAME_GRAPEJELLY, ITEMNAME_PINEAPPLEJELLY, ITEMNAME_OMEGAELIXIR,
		ITEMNAME_PANASIABOTTLE , ITEMNAME_LIFEBOTTLE, ITEMNAME_REDSAGE , ITEMNAME_POTATO, ITEMNAME_TOMATO,ITEMNAME_APPLE,
		ITEMNAME_LETTUCE,ITEMNAME_END };
	enum ITEM_TYPE { ITEMTYPE_JELLY , ITEMTYPE_MATERIAL ,ITEMTYPE_SWORD, ITEMTYPE_ARMOR, ITEMTYPE_ACCESSORIES, ITEMTYPE_ACCMATERIAL,
		ITEMTYPE_GRAIN , ITEMTYPE_LEAVES, ITEMTYPE_VEGITABLE, ITEMTYPE_MEAT, ITEMTYPE_FRUIT, ITEMTYPE_ELSE,
		ITEMTYPE_CONDIMENT, ITEMTYPE_FISH, ITEMTYPE_RECIPE, ITEMTYPE_GUN, ITEMTYPE_BOOK, ITEMTYPE_GAUNTLET,
		ITEMTYPE_KEY , ITEMTYPE_POTION , ITEMTYPE_END };

	enum MONSTER_ID { ICE_WOLF, HAWK, BERSERKER, SLIME, MONSTER_END };

	typedef struct ObjectInfo
	{
		float fMaxHp;
		float fCurrentHp;
		float fMaxMp;
		float fCurrentMp;
		int iDamage;

	}OBJINFO;

	typedef struct iteminfo
	{
		ITEM_NAME eitemname;
		ITEM_TYPE eitemtype;
		int icount = 0;
		

	}ITEMINFO;

	/*For Anim State*/
	enum STATETYPE
	{
		STATETYPE_START,
		STATETYPE_MAIN,
		STATETYPE_END,
		STATETYPE_DEFAULT
	};

#define MAXDISTANCE 999999.f

}

#include <process.h>
#include <time.h>

extern HWND			g_hWnd;
extern HINSTANCE	g_hInst;
extern bool		g_bUIMade;

using namespace Client;