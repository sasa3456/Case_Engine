//-----------------------------------------------------
// © Copyright 2024 Case Engine. All Rights Reserved. 
//-----------------------------------------------------


// Includes
#pragma once
#include "GfxShader.h"


// Case_Engine
namespace Case_Engine
{
	
	struct GfxShaderCompileOutput
	{
		GfxShaderBytecode shader_bytecode;
		std::vector<std::string> includes;
		uint64_t hash;
	};
	
	struct GfxInputLayoutDesc;
	namespace GfxShaderCompiler
	{
		bool CompileShader(GfxShaderDesc const& input, GfxShaderCompileOutput& output);
		void GetBytecodeFromCompiledShader(char const* filename, GfxShaderBytecode& blob);
		void FillInputLayoutDesc(GfxShaderBytecode const& blob, GfxInputLayoutDesc& input_desc);
	}
}