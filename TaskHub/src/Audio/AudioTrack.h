#pragma once
#include "HubAudioEngine.h"
#include "AudioFile.h"
#include "Core/Assert.h"
#include "miniaudio.h"
#include <functional>

/*
	AudioTrack offers a straightforward and easy-to-use interface for handling audio playback. 
	It is designed to simplify the process of managing audio files, allowing you to load, play, pause, and control tracks with minimal effort.

	TODO:
		  Implement callback for when sound is over.
		  Implement a better way to manage load state.
		  Refactor to use AudioFile
*/

namespace taskhub {

	class AudioTrack {
	public:
		AudioTrack(std::shared_ptr<AudioEngine> engine, const std::string& filepath);
		~AudioTrack();	
		
		void Load();    // Loads the audio file into memory, preparing it for playback.
		void Unload();  // Unloads the audio file from memory, releasing resources.

		void Play();
		void Pause();
		void Seek(float seconds);  // Seeks to the specified position in the audio track.
		void SetLooping(bool isLooping);
		float GetCursorPosition() const;

		bool IsPlaying() const;
		bool IsFinished() const;
		bool IsLooping() const;
		
		bool IsLoaded() const { return m_IsLoaded; }  // Checks if the audio track is loaded into memory.
		std::string GetFilepath() const { return m_Filepath; }
		std::chrono::duration<float> GetDuration() const { return m_Duration; }

	private:
		std::shared_ptr<AudioEngine> m_EngineHandle;
		std::unique_ptr<ma_sound> m_Sound;

		std::string m_Filepath;
		std::chrono::duration<float> m_Duration;

		bool m_IsLoaded = false;
	};
}