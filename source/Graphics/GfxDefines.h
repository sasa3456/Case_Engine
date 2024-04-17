//-----------------------------------------------------
// © Copyright 2024 Case Engine. All Rights Reserved. 
//-----------------------------------------------------


// Includes
#pragma once
#include "Core/Defines.h"


// Defines
#define GFX_CHECK_HR(hr) if(FAILED(hr)) CASE_ENGINE_DEBUGBREAK();
#define GFX_BACKBUFFER_COUNT 3
#define GFX_PROFILING 1