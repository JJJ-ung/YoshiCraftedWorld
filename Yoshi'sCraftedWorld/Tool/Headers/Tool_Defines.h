#pragma once



#include <d3d9.h>
#include <d3dx9.h>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <process.h>
#include <string>
#include <fstream>
#include <chrono>

#define	DIRECTINPUT_VERSION		0x0800
#include <dinput.h>

#include "Tool_Macro.h"
#include "Tool_Typedef.h"
#include "Tool_Function.h"
#include "Tool_Struct.h"

const unsigned short	WINCX = 1280;
const unsigned short	WINCY = 800;

enum SCENEID { SCENE_STATIC, SCENE_LOGO, SCENE_STAGE, SCENE_END };

using namespace std;
using namespace chrono;