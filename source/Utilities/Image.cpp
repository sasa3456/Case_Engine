//-----------------------------------------------------
// © Copyright 2024 Case Engine. All Rights Reserved. 
//-----------------------------------------------------


// Includes
#include "Image.h"
#define STB_IMAGE_IMPLEMENTATION  
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#include "Core/Logger.h"


// Namespace Case_Engine
namespace Case_Engine
{

	Image::Image(std::string_view image_file, uint32_t desired_channels)
	{
		int32_t width, height, channels;

		if (is_hdr = static_cast<bool>(stbi_is_hdr(image_file.data())); is_hdr)
		{
			float* pixels = stbi_loadf(image_file.data(), &width, &height, &channels, desired_channels);
			if (!pixels) 
			{ 
				CASE_ENGINE_LOG(ERROR, "Loading image file %s unsuccessful", image_file.data()); 
			}
			else  _pixels.reset(reinterpret_cast<uint8_t*>(pixels));
		}
		else
		{
			stbi_uc* pixels = stbi_load(image_file.data(), &width, &height, &channels, desired_channels);
			if (!pixels)
			{
				CASE_ENGINE_LOG(ERROR, "Loading image file %s unsuccessful", image_file.data());
			}
			else _pixels.reset(pixels);
		}

		_width = static_cast<uint32_t>(width);
		_height = static_cast<uint32_t>(height);
		_channels = static_cast<uint32_t>(desired_channels);
	}

	uint32_t Image::Width() const
	{
		return _width;
	}

	uint32_t Image::Height() const
	{
		return _height;
	}

	uint32_t Image::Channels() const
	{
		return _channels;
	}

	uint32_t Image::BytesPerPixel() const
	{
		return _channels * (is_hdr ? sizeof(float) : sizeof(uint8_t));
	}

	uint32_t Image::Pitch() const
	{
		return _width * BytesPerPixel();
	}

	bool Image::IsHDR() const
	{
		return is_hdr;
	}

	void WriteImageTGA(char const* name, std::vector<uint8_t> const& data, int width, int height)
	{
		stbi_write_tga(name, width, height, STBI_rgb_alpha, (void*)data.data());
	}

	void WriteImagePNG(char const* name, std::vector<uint8_t> const& data, int width, int height)
	{
		stbi_write_png(name, width, height, STBI_rgb_alpha, (void*)data.data(), width * 4 * sizeof(stbi_uc));
	}

	void WriteImageJPG(char const* name, std::vector<uint8_t> const& data, int width, int height)
	{
		stbi_write_jpg(name, width, height, STBI_rgb_alpha, (void*)data.data(), 100);
	}

}

