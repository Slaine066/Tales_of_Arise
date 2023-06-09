#pragma once

namespace Client
{
	const unsigned int g_iWinSizeX = 1280;
	const unsigned int g_iWinSizeY = 720;

	enum LEVEL { LEVEL_STATIC, LEVEL_LOADING, LEVEL_GAMEPLAY, LEVEL_END };

	enum MODEL { MODEL_ALPHEN, MODEL_ICEWOLF, MODEL_SION, MODEL_BERSERKER, MODEL_HAWK, MODEL_ASTRAL_DOUBT, MODEL_RINWELL, MODEL_LAW, MODEL_ASU_005, /*MODEL_ORIGIN_ASU, MODEL_CURRENT_ASU,*/ MODEL_DUOHALEM, MODEL_KISARA, MODEL_END };

	enum NONOBJECT { NON_CHAIR, NON_DESK, NON_BLOODBOOK,
		NON_CABINET, NON_COIN, NON_DRIVER, NON_FIRE, NON_FUSE1,
		NON_FUSE2, NON_FUSE3, NON_MASTERKEY, NON_MAGIC, NON_PAPER,
		NON_POISON, NON_MAP, NON_SOYBEAN, NON_SPRAY, NON_TABLET,
		NON_FRIDAY,	NON_THURSDAY, NON_OBJECTS, NON_CDOOR_LEFT, NON_CDOOR_RIGHT,
		NON_TOILET_DOOR, NON_CABINET_BIG_LEFT, NON_CABINET_BIG_RIGHT, NON_EXIT_DOOR, 
		NON_IRON_DOOR, NON_KEYBOX_DOOR, NON_NEW_DOOR, NON_SECOND_DOOR, NON_SHOWCASE_DOOR_LEFT,
		NON_SHOWCASE_DOOR_RIGHT, NON_TOILET_IN_DOOR, NON_VENTILATION_DOOR, NON_REGISTER_DOOR, NON_END };

	enum ROOMTYPE {
		ROOM_GYM, ROOM_HOUSE, ROOM_MACHINE_HALLWAY, ROOM_MACHINE,
		ROOM_1F_HALL, ROOM_TEACHER, ROOM_2F_HALL, ROOM_MOUNTAIN, ROOM_STUDENT,
		ROOM_MAIN_HALLWAY, ROOM_MOUNTAIN_HALLWAY, ROOM_PHOTO, ROOM_2_1, ROOM_2_2,
		ROOM_2_3, ROOM_2_4, ROOM_2_5, ROOM_2_6, ROOM_2_7, ROOM_2_8, ROOM_2_9,
		ROOM_2_10, ROOM_2_11, ROOM_2_12, ROOM_TOILET_M_1F, ROOM_TOILET_M_2F,
		ROOM_TOILET_W_1F, ROOM_TOILET_W_2F, ROOM_2F_HALLWAY, ROOM_END };

	enum MAP { MAP_MAIN01, MAP_END };
}

#include <process.h>

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

extern HWND g_hWnd;
extern HINSTANCE g_hInst;

using namespace Client;