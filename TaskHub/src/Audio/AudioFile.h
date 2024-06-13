#pragma once

#include "HubAudioEngine.h"
#include "Core/Assert.h"
#include "miniaudio.h"
#include <functional>

/* TODO: Refactor error catching in Audio.cpp 
		 Implement callback for when sound is over
*/

namespace taskhub {

	class AudioFile {
	public:
		using Callback = std::function<void()>;

		AudioFile(std::shared_ptr<AudioEngine> engine, const std::string& filepath);
		~AudioFile();	
		
		void Play();
		void Pause();
		void Seek(float seconds);
		void SetLooping(bool isLooping);
		float GetCursorPosition() const;

		bool IsPlaying() const;
		bool IsFinished() const;

	public:
		inline std::string GetFilepath() const { return m_Filepath; }
		inline std::chrono::duration<float> GetDuration() const { return m_Duration; }

	private:
		void Load();

	private:
		std::shared_ptr<AudioEngine> m_Engine;
		std::unique_ptr<ma_sound> m_Sound;

		std::string m_Filepath;
		std::chrono::duration<float> m_Duration;
	};
}