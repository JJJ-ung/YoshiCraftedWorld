#pragma once

#include <io.h>
#include "fmod.h"
#include "fmod.hpp"
#include "fmod.hpp"
#include "fmod_dsp.h"
#include "fmod_errors.h"
#pragma comment (lib, "fmodex_vc.lib")

#include "SoundMgr.h"

const unsigned short	g_nWinCX = 1280;
const unsigned short	g_nWinCY = 720;

enum SCENEID { SCENE_STATIC, SCENE_LOGO, SCENE_TITLE, SCENE_LOAD, SCENE_WORLDMAP, SCENE_TRAIN, SCENE_END };

extern HINSTANCE g_hInst;
extern HWND g_hWnd;

extern bool g_bRenderRay;

#define  NOEVENT 0
#define  OBJDEAD 1 