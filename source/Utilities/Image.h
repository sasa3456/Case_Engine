//-----------------------------------------------------
// © Copyright 2024 Case Engine. All Rights Reserved. 
//-----------------------------------------------------


// Includes
#pragma once
#include <string_view>
#include <memory>
#include <vector>


// Namespace Case_Engine
namespace Case_Engine
{
	class Image
	{
	public:
		Image(std::string_view image_file, uint32_t desired_channels = 4);

		Image(Image const&) = delete;
		Image(Image&&) = default;
		Image& operator=(Image const&) = delete;
		Image& operator=(Image&&) = default;
		~Image() = default;


		uint32_t Width() const;

		uint32_t Height() const;

		uint32_t Channels() const;

		uint32_t BytesPerPixel() const;

		uint32_t Pitch() const;

		bool IsHDR() const;

		template<typename T>
		T const* Data() const;

	private:
		uint32_t _width, _height;
		uint32_t _channels;
		std::unique_ptr<uint8_t[]> _pixels;
		bool is_hdr;
	};

	template<typename T>
	T const* Image::Data() const
	{
		return reinterpret_cast<T const*>(_pixels.get());
	}

	void WriteImageTGA(char const* name, std::vector<uint8_t> const& data, int width, int height);
	void WriteImagePNG(char const* name, std::vector<uint8_t> const& data, int width, int height);
	void WriteImageJPG(char const* name, std::vector<uint8_t> const& data, int width, int height);

}