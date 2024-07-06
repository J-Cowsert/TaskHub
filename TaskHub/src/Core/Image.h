#pragma once
#include <glad/glad.h>
#include <string>
#include <filesystem>

namespace taskhub {

	enum class ImageFormat {
		None = 0, 
		RGBA,
		RGBA32F
	};

	class Image {
	public:
		Image(std::string_view path);
		Image(uint32_t width, uint32_t height, ImageFormat format, const void* data = nullptr);
		~Image();

		void SetData(const void* data);
		void Resize(uint32_t width, uint32_t height);

		GLuint GetTextureID() const { return m_TextureID; }
		uint32_t GetWidth() const { return m_Width; }
		uint32_t GetHeight() const { return m_Height; }

		static void* DecodeFromMemory(const void* buffer, uint64_t length, uint32_t& outWidth, uint32_t& outHeight);
		
	private:
		void ReleaseMemory();

	private:
		uint32_t m_Width = 0, m_Height = 0;
		GLuint m_TextureID = 0;
		std::string m_FilePath;
		ImageFormat m_ImageFormat = ImageFormat::None;
	};
}