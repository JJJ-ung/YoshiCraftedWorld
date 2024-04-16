#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <vector>
#include <list>
#include <map>
#include <queue>
#include <unordered_map>
#include <algorithm>
#include <process.h>
#include <string>
#include <fstream>
#include <chrono>
#include <iostream>

#pragma warning (disable : 4251)

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 
#endif  // _DEBUG  

#endif

#include "Engine_Macro.h"
#include "Engine_Typedef.h"
#include "Engine_Function.h"
#include "Engine_Struct.h"

using namespace std;
using namespace Engine;
using namespace chrono;

extern _bool		g_bRenderTarget;
extern _bool		g_bRenderCollider;
