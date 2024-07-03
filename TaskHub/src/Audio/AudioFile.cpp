#include "AudioFile.h"
#include "HubAudioEngine.h"
#include "Core/Assert.h"
#include <filesystem>

namespace taskhub {

	taskhub::AudioFile::AudioFile(const std::string& filepath)
		: m_Filepath(filepath), m_DecoderHandle(std::make_unique<ma_decoder>())
	{
		HUB_CORE_ASSERT(std::filesystem::exists(m_Filepath), "Filepath does not exist");

		InitDecoder();

		SetName();
		SetDuration();
		SetFileExtension();
	}

	AudioFile::~AudioFile() {
		ma_decoder_uninit(m_DecoderHandle.get());
	}

	void AudioFile::InitDecoder() {
		
		ma_result result = ma_decoder_init_file(m_Filepath.c_str(), nullptr, m_DecoderHandle.get());
		HUB_CORE_ASSERT(result == MA_SUCCESS, "Filed to initialize decoder");
	}

	void AudioFile::SetName() {

		std::filesystem::path path(m_Filepath);
		m_Name = path.filename().string();
	}

	void AudioFile::SetDuration() {
	
		ma_uint64 frameCount;
		ma_result result = ma_decoder_get_length_in_pcm_frames(m_DecoderHandle.get(), &frameCount);
		HUB_CORE_ASSERT(result == MA_SUCCESS, "Failed to get length of sound");

		ma_uint32 sampleRate = HubAudioEngine::GetInstance()->GetSampleRate();
		m_Duration = std::chrono::duration<float>(static_cast<float>(frameCount) / sampleRate);
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

		HUB_CORE_ERROR("File type not supported");
	}
}
