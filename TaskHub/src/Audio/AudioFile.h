#pragma once
#include "miniaudio.h"
#include <string>
#include <memory>
#include <chrono>

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
		~AudioFile() = default;

		AudioFile(const AudioFile&) = delete;
		AudioFile& operator=(const AudioFile&) = delete;

		const std::string& GetFilepath() const { return m_Filepath; }
		const std::string& GetName() const { return m_Name; }
		const std::chrono::duration<float>& GetDuration() const { return m_Duration; }
		const FileExtension& GetFileExtension() const { return m_FileExtension; }

	private:
		void InitDecoder();

		void SetName(); 
		void SetDuration();
		void SetFileExtension();
		
	private:
		std::string m_Filepath;
		std::string m_Name;
		std::chrono::duration<float> m_Duration;
		FileExtension m_FileExtension = FileExtension::None;

		std::unique_ptr<ma_decoder> m_DecoderHandle;
	};
}
