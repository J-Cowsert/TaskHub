#pragma once
#include "HubAudioEngine.h"
#include "AudioFile.h"
#include "Core/Assert.h"
#include "miniaudio.h"
#include <functional>

/*
	AudioSource offers a straightforward and easy-to-use interface for handling audio playback. 
	It is designed to simplify the process of managing audio files, allowing you to load, play, pause, and control tracks with minimal effort.
*/
namespace taskhub {

	class AudioSource {
	public:
		AudioSource();
		~AudioSource();

		AudioSource(const AudioSource&) = delete;
		AudioSource& operator=(const AudioSource&) = delete;
		
		void Load(AudioFile& audioFile);    // Loads the audio file data into memory, preparing it for playback.
		void Unload();  // Unloads the audio file data from memory, releasing resources.

		void Play();
		void Pause();
		void Seek(float seconds);  // Seeks to the specified position in the audio track.
		void SetLooping(bool isLooping);
		float GetCursorPosition() const;

		bool IsPlaying() const;
		bool IsFinished() const;
		bool IsLooping() const;
		bool IsLoaded() const { return m_IsLoaded; }  // Checks if the audio track is loaded into memory.

	private:
		std::unique_ptr<ma_sound> m_SoundHandle;

		bool m_IsLoaded = false;
	};
}