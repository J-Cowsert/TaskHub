#include "AudioFile.h"
#include "Core/Assert.h"
#include <filesystem>

namespace taskhub {

	AudioFile::AudioFile(const std::string& filepath)
		: m_Filepath(filepath), m_DecoderHandle(std::make_unique<ma_decoder>())
	{
		HUB_CORE_ASSERT(std::filesystem::exists(m_Filepath), "Filepath does not exist");
		
		ma_result result = ma_decoder_init_file(m_Filepath.c_str(), nullptr, m_DecoderHandle.get()); // ma_decoder is used to calculate the duration of the AudioFile
		HUB_CORE_ASSERT(result == MA_SUCCESS, "Filed to initialize decoder");
		
		SetDuration();
		SetFileExtension();
		SetName();

		ma_decoder_uninit(m_DecoderHandle.get());
	}

	bool AudioFile::IsFileValid(const std::string& filepath) {

		std::filesystem::path path(filepath);
		std::string extension = path.extension().string();
		if (extension == ".wav" || extension == ".mp3" || extension == ".flac") {
			return true;
		}

		return false;
	}

	void AudioFile::SetName() {

		std::filesystem::path path(m_Filepath);
		m_Name = path.stem().string();
	}

	void AudioFile::SetDuration() {

		ma_result result;
		ma_uint64 frameCount;
		ma_uint32 sampleRate;

		result = ma_decoder_get_available_frames(m_DecoderHandle.get(), &frameCount);
		HUB_CORE_ASSERT(result == MA_SUCCESS, "Failed to get frame count");

		result = ma_data_source_get_data_format(m_DecoderHandle.get(), nullptr, nullptr, &sampleRate, nullptr, 0);
		HUB_CORE_ASSERT(result == MA_SUCCESS, "Failed to get sample rate");

		m_Duration = std::chrono::duration<float>((ma_uint64)frameCount / (float)sampleRate);
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
		else {
			HUB_CORE_ASSERT(false, "File type not supported. Be sure to check if the file is valid with the IsFileValid method.");
		}
	}
}
