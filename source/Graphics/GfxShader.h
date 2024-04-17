//-----------------------------------------------------
// © Copyright 2024 Case Engine. All Rights Reserved. 
//-----------------------------------------------------


// Includes
#pragma once
#include <string>
#include <vector>


// Namespace Case_Engine
namespace Case_Engine 
{
	struct GfxShaderMacro
	{
		std::string name;
		std::string value;
	};
	enum class GfxShaderStage
	{
		VS,
		PS,
		HS,
		DS,
		GS,
		CS,
		StageCount
	};
	using GfxShaderBlob = std::vector<uint8_t>;

	struct GfxShaderBytecode
	{
		GfxShaderBlob bytecode;

		void SetBytecode(void* data, uint32_t data_size)
		{
			bytecode.resize(data_size);
			memcpy(bytecode.data(), data, data_size);
		}
		void* GetPointer() const
		{
			return (void*)bytecode.data();
		}
		uint32_t GetLength() const
		{
			return (uint32_t)bytecode.size();
		}
	};

	enum GfxShaderCompilerFlagBit
	{
		GfxShaderCompilerFlagBit_None = 0,
		GfxShaderCompilerFlagBit_Debug = 1 << 0,
		GfxShaderCompilerFlagBit_DisableOptimization = 1 << 1,
	};

	struct GfxShaderDesc
	{
		GfxShaderStage stage = GfxShaderStage::StageCount;
		std::string source_file = "";
		std::string entrypoint = "";
		std::vector<GfxShaderMacro> macros;
		uint64_t flags = GfxShaderCompilerFlagBit_None;
	};
}