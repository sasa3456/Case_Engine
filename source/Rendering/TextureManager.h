//-----------------------------------------------------
// © Copyright 2024 Case Engine. All Rights Reserved. 
//-----------------------------------------------------


// Includes
#pragma once
#include <string>
#include <array>
#include <unordered_map>
#include "Graphics/GfxDevice.h"
#include "Graphics/GfxView.h"
#include "Utilities/Singleton.h"


// Namespace Case_Engine
namespace Case_Engine
{
	using TextureHandle = uint64_t;
	inline constexpr TextureHandle const INVALID_TEXTURE_HANDLE = uint64_t(-1);

	class TextureManager : public Singleton<TextureManager>
	{
		friend class Singleton<TextureManager>;

	public:
		void Initialize(GfxDevice* gfx);
		void Destroy();

		CASE_ENGINE_NODISCARD TextureHandle LoadTexture(std::wstring const& name);
		CASE_ENGINE_NODISCARD TextureHandle LoadTexture(std::string const& name);
		CASE_ENGINE_NODISCARD TextureHandle LoadCubeMap(std::wstring const& name);
		CASE_ENGINE_NODISCARD TextureHandle LoadCubeMap(std::array<std::string, 6> const& cubemap_textures);

		GfxShaderResourceRO GetTextureView(TextureHandle tex_handle) const;
		void SetMipMaps(bool mipmaps);

	private:
		GfxDevice* gfx;
		bool mipmaps = true;
		TextureHandle handle = INVALID_TEXTURE_HANDLE;
		std::unordered_map<TextureHandle, GfxArcShaderResourceRO> texture_map{};
		std::unordered_map<std::wstring, TextureHandle> loaded_textures{};

	private:
		TextureManager() = default;
		TextureManager(TextureManager const&) = delete;
		TextureManager& operator=(TextureManager const&) = delete;
		~TextureManager() = default;

		TextureHandle LoadDDSTexture(std::wstring const& name);
		TextureHandle LoadWICTexture(std::wstring const& name);
		TextureHandle LoadTexture_HDR_TGA_PIC(std::string const& name);
	};
	#define g_TextureManager TextureManager::Get()
}