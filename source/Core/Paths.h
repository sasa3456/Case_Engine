//-----------------------------------------------------
// © Copyright 2024 Case Engine. All Rights Reserved. 
//-----------------------------------------------------


// Includes
#pragma once
#include <string>


// Namespace Case_Engine
namespace Case_Engine::paths
{
	std::string MainDir();
	std::string ResourcesDir();
	std::string SavedDir();
	std::string ConfigDir();
	std::string ToolsDir();

	std::string FontsDir();
	std::string IconsDir();
	std::string ShaderDir();
	std::string TexturesDir();
	std::string ModelsDir();

	std::string LogDir();
	std::string ScreenshotsDir();
	std::string ShaderCacheDir();
	std::string IniDir();
}