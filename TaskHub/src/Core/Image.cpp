#include "Image.h"
#include "Assert.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace taskhub {

	namespace Utils {

		static uint32_t BytesPerPixel(ImageFormat format) {

			switch (format) 
			{
				case ImageFormat::RGBA:      return 4;
				case ImageFormat::RGBA32F:   return 16;
			}

			return 0;
		}

		static GLint TaskHubFormatToOpenGLFormat(ImageFormat format) {

			switch (format) 
			{
				case ImageFormat::RGBA:      return GL_RGBA8;
				case ImageFormat::RGBA32F:   return GL_RGBA32F;
			}

			return 0;
		}
	}

	Image::Image(std::string_view path)
		: m_FilePath(path)
	{
		HUB_CORE_ASSERT(std::filesystem::exists(m_FilePath), "Filepath does not exist");

		int width, height, channels;
		uint8_t* data = nullptr;

		if (stbi_is_hdr(m_FilePath.c_str())) {
			
			data = (uint8_t*)stbi_loadf(m_FilePath.c_str(), &width, &height, &channels, 4);
			m_ImageFormat = ImageFormat::RGBA32F;
		}
		else {
			data = stbi_load(m_FilePath.c_str(), &width, &height, &channels, 4);
			m_ImageFormat = ImageFormat::RGBA;
		}

		if (!data)
			HUB_CORE_CRITICAL("Failed to load image data");

		m_Width = width;
		m_Height = height;

		SetData(data);
		stbi_image_free(data);
	}

	Image::Image(uint32_t width, uint32_t height, ImageFormat format, const void* data)
		: m_Width(width), m_Height(height), m_ImageFormat(format)
	{
		if (data)
			SetData(data);
	}

	Image::~Image() {
		ReleaseMemory();
	}

	void Image::SetData(const void* data) {

		glGenTextures(1, &m_TextureID);
		glBindTexture(GL_TEXTURE_2D, m_TextureID);

		GLint internalFormat = Utils::TaskHubFormatToOpenGLFormat(m_ImageFormat);
		GLenum type = (m_ImageFormat == ImageFormat::RGBA32F) ? GL_FLOAT : GL_UNSIGNED_BYTE;

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_Width, m_Height, 0, GL_RGBA, type, data);

		glBindTexture(GL_TEXTURE_2D, 0); // unbind the texture
	}

	void Image::Resize(uint32_t width, uint32_t height) {

		if (m_TextureID && m_Width == width && m_Height == height)
			return;

		m_Width = width;
		m_Height = height;
	}

	void* Image::DecodeFromMemory(const void* buffer, uint64_t length, uint32_t& outWidth, uint32_t& outHeight) {

		int width, height, channels;
		uint8_t* data = nullptr;
		uint64_t size = 0;

		data = stbi_load_from_memory((const stbi_uc*)buffer, static_cast<int>(length), &width, &height, &channels, 4);
		size = width * height * 4;

		outWidth = width;
		outHeight = height;

		return data;
	}

	void Image::ReleaseMemory() {

		if (m_TextureID) {
			glDeleteTextures(1, &m_TextureID);
			m_TextureID = 0;
		}
	}
}