//-----------------------------------------------------
// © Copyright 2024 Case Engine. All Rights Reserved. 
//-----------------------------------------------------


// Includes
#include "Paths.h"


// Namespace Case_Engine
namespace Case_Engine
{
	std::string paths::MainDir()
	{
		return "..//..//source/";
	}

	std::string paths::ConfigDir()
	{
		return MainDir();
	}
	std::string paths::ResourcesDir()
	{
		return MainDir() + "Resources/";
	}
	std::string paths::SavedDir()
	{
		return MainDir() + "Saved/";
	}
	std::string paths::ToolsDir()
	{
		return MainDir() + "Tools/";
	}

	std::string paths::FontsDir()
	{
		return ResourcesDir() + "Fonts/";
	}
	std::string paths::IconsDir()
	{
		return ResourcesDir() + "Icons/";
	}
	std::string paths::ShaderDir()
	{
		return MainDir() + "Shaders/";
	}
	std::string paths::TexturesDir()
	{
		return ResourcesDir() + "Textures/";
	}

	std::string paths::ModelsDir()
	{
		return ResourcesDir() + "Models/";
	}

	std::string paths::ScreenshotsDir()
	{
		return SavedDir() + "Screenshots/";
	}
	std::string paths::LogDir()
	{
		return SavedDir() + "Log/";
	}

	std::string paths::ShaderCacheDir()
	{
		return SavedDir() + "ShaderCache/";
	}
	std::string paths::IniDir()
	{
		return SavedDir() + "Ini/";
	}

}

