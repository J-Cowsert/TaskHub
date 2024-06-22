#pragma once
#include "miniaudio.h"
#include <string>
#include <memory>
#include <filesystem>

namespace taskhub {

	enum class FileExtension {
		None = 0,
		WAV,
		MP3,
		FLAC
	};

	class AudioFile {
	public:
		AudioFile(const std::string& filepath);
		~AudioFile();

		const std::string& GetFilepath() const { return m_Filepath; }
		const FileExtension& GetFileExtension() const { return m_FileExtension; }
		ma_decoder* GetDecoder() const { return m_Decoder.get(); } // Should this be exposed? Intended for use when loading a ma_sound.

	private:
		void InitDecoder();
		void SetFileExtension();

	private:
		std::string m_Filepath;
		FileExtension m_FileExtension = FileExtension::None;

		std::unique_ptr<ma_decoder> m_Decoder;
	};
}
