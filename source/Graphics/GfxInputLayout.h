//-----------------------------------------------------
// © Copyright 2024 Case Engine. All Rights Reserved. 
//-----------------------------------------------------


// Includes
#pragma once
#include <string>
#include <vector>
#include "GfxFormat.h"


// Namespace Case_Engine
namespace Case_Engine
{
	enum class GfxInputClassification
	{
		PerVertexData,
		PerInstanceData
	};
	struct GfxInputLayoutDesc
	{
		static constexpr uint32_t APPEND_ALIGNED_ELEMENT = ~0u;
		struct GfxInputElement
		{
			std::string semantic_name;
			uint32_t semantic_index = 0;
			GfxFormat format = GfxFormat::UNKNOWN;
			uint32_t input_slot = 0;
			uint32_t aligned_byte_offset = APPEND_ALIGNED_ELEMENT;
			GfxInputClassification input_slot_class = GfxInputClassification::PerVertexData;
		};
		std::vector<GfxInputElement> elements;
	};

	class GfxDevice;
	struct GfxShaderBytecode;
	class GfxInputLayout
	{
	public:
		GfxInputLayout(GfxDevice* gfx, GfxShaderBytecode const& vs_blob);
		GfxInputLayout(GfxDevice* gfx, GfxShaderBytecode const& vs_blob, GfxInputLayoutDesc const& desc);

		operator ID3D11InputLayout* () const
		{
			return input_layout.Get();
		}
	private:
		ArcPtr<ID3D11InputLayout> input_layout;
	};
}