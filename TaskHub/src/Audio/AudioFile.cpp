#include "AudioFile.h"
#include "Core/Assert.h"

namespace taskhub {

	taskhub::AudioFile::AudioFile(const std::string& filepath)
		: m_Filepath(filepath), m_Decoder(std::make_unique<ma_decoder>())
	{
		HUB_CORE_ASSERT(std::filesystem::exists(m_Filepath), "Filepath does not exist");
		SetFileExtension();
		InitDecoder();
	}

	AudioFile::~AudioFile() {
		ma_decoder_uninit(m_Decoder.get());
	}

	void AudioFile::InitDecoder() {
		
		ma_result result = ma_decoder_init_file(m_Filepath.c_str(), nullptr, m_Decoder.get());
		HUB_CORE_ASSERT(result == MA_SUCCESS, "Filed to initialize decoder");
	}

	void AudioFile::SetFileExtension() {

		std::filesystem::path path(m_Filepath);
		std::string extension = path.extension().string();

		if (extension == ".wav") {
			m_FileExtension = FileExtension::WAV;
			return;
		}
		else if (extension == ".mp3") {
			m_FileExtension = FileExtension::MP3;
			return;
		}
		else if (extension == ".flac") {
			m_FileExtension = FileExtension::FLAC;
			return;
		}
	}
}
