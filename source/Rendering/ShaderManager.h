//-----------------------------------------------------
// © Copyright 2024 Case Engine. All Rights Reserved. 
//-----------------------------------------------------


// Includes
#pragma once
#include "Enums.h"


// Namespace Case_Engine
namespace Case_Engine
{
	struct GfxShaderProgram;
	class GfxDevice;

	namespace ShaderManager
	{
		void Initialize(GfxDevice* device);
		void Destroy();
		GfxShaderProgram* GetShaderProgram(ShaderProgram shader_program);
		void CheckIfShadersHaveChanged();
	};
}