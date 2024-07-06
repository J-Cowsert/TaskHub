#include "Core/Application.h"
#include "Core/EntryPoint.h"
#include "Core/Image.h"
#include "Audio/AudioSource.h"
#include "Audio/AudioFile.h"
#include "GUI/UIUtils.h"
#include <chrono>
#include <format>

class AudioPlayerTest : public taskhub::Layer {
public:
    void OnAttach() override {

        m_PlayButton = std::make_shared<taskhub::Image>("C:/Dev/Resources/Images/Audio/Play.png");
        m_PauseButton = std::make_shared<taskhub::Image>("C:/Dev/Resources/Images/Audio/Pause.png");
        m_ForwardButton = std::make_shared<taskhub::Image>("C:/Dev/Resources/Images/Audio/Forward.png");
        m_BackwardButton = std::make_shared<taskhub::Image>("C:/Dev/Resources/Images/Audio/Backward.png");
        m_VolumeImage = std::make_shared<taskhub::Image>("C:/Dev/Resources/Images/Audio/Volume.png");
        m_VolumeImageMute = std::make_shared<taskhub::Image>("C:/Dev/Resources/Images/Audio/mute.png");
        m_LoopingOff = std::make_shared<taskhub::Image>("C:/Dev/Resources/Images/Audio/loopOff.png");
        m_LoopingOn = std::make_shared<taskhub::Image>("C:/Dev/Resources/Images/Audio/loopOn.png");

        m_AudioPlayer = std::make_unique<taskhub::AudioSource>();

        m_Playlist.reserve(10);

        m_Playlist.emplace_back(std::make_shared<taskhub::AudioFile>("C:/Dev/Music/Mo Beats - Righteous (pepe lore song).mp3"));
        m_Playlist.emplace_back(std::make_shared<taskhub::AudioFile>("C:/Dev/Music/Shift (Instrumental).mp3"));
        m_Playlist.emplace_back(std::make_shared<taskhub::AudioFile>("C:/Dev/Music/1 HOUR OF CHILL RAP BEATS.mp3"));
        m_Playlist.emplace_back(std::make_shared<taskhub::AudioFile>("C:/Dev/Music/[dream].mp3"));
        m_Playlist.emplace_back(std::make_shared<taskhub::AudioFile>("C:/Dev/Music/Lofi Jazz Instrumental.mp3"));
        m_Playlist.emplace_back(std::make_shared<taskhub::AudioFile>("C:/Dev/Music/Nocturne.mp3"));
        m_Playlist.emplace_back(std::make_shared<taskhub::AudioFile>("C:/Dev/Music/Hope.mp3"));
        m_Playlist.emplace_back(std::make_shared<taskhub::AudioFile>("C:/Dev/Music/DVRST - Close Eyes.mp3"));
        m_Playlist.emplace_back(std::make_shared<taskhub::AudioFile>("C:/Dev/Music/Cmaj7 Backing Track Jam!!.mp3"));

        m_AudioPlayer->Load(*m_Playlist[0]);
        m_CurrentTrack = 0;
    }

    void OnUpdate() override {

        // Check to see if song is finished and play next song
        if (!m_IsLooping && m_AudioPlayer->IsFinished() && m_CurrentTrack < m_Playlist.size() - 1) {

            m_CurrentTrack++;
            m_AudioPlayer->Load(*m_Playlist[m_CurrentTrack]);
            m_AudioPlayer->Play();
        }
    }

    void OnUIRender() override {

        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar;

        ImGui::Begin("Music Player", nullptr, windowFlags);
        ShowPlaylist();
        ShowControls();
        ImGui::End();
    }

    void ShowPlaylist() {

        ImGuiChildFlags childFlags = ImGuiChildFlags_Border | ImGuiChildFlags_AlwaysUseWindowPadding | ImGuiChildFlags_AutoResizeX;
        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_None;

        ImGui::BeginChild("PlaylistView", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, ImGui::GetContentRegionAvail().y * 0.5f), childFlags, windowFlags);

        for (size_t i = 0; i < m_Playlist.size(); i++) {
            
            if (ImGui::Selectable(m_Playlist[i]->GetName().c_str(), m_CurrentTrack == i)) {

                m_AudioPlayer->Load(*m_Playlist[i]);

                if (m_IsPlaying)
                    m_AudioPlayer->Play();

                m_CurrentTrack = static_cast<int>(i);
            }

            auto duration = m_Playlist[i]->GetDuration();
            auto minutes = std::chrono::duration_cast<std::chrono::minutes>(duration);
            auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration - minutes);

            std::string formattedDuration = std::format("{:02}:{:02}", minutes.count(), seconds.count());

            ImVec2 textSize = ImGui::CalcTextSize(formattedDuration.c_str());
            ImGui::SameLine();
            // Display duration of song on right side of child window
            taskhub::UI::AlignCursorForWidth(textSize.x, 1.0f);
            ImGui::Text("%s", formattedDuration.c_str());

            ImGui::Separator();
        }

        ImGui::EndChild();
        ImGui::Separator();
    }

    void ShowControls() {

        taskhub::UI::AlignCursorForHeight(100, 0.9f);
        ImGui::BeginChild("MusicControls");
        
        // Play|Pause control buttons
        {
            taskhub::UI::AlignCursorForWidth(static_cast<float>(m_PauseButton->GetWidth()), 0.5f);
            if (m_IsPlaying) {
                if (ImGui::InvisibleButton("PauseButton", ImVec2((float)m_PauseButton->GetWidth(), (float)m_PauseButton->GetHeight()))) {
                    m_AudioPlayer->Pause();
                    m_IsPlaying = false;
                }
                taskhub::UI::RenderImageButton(m_PauseButton, IM_COL32(200, 200, 200, 200), IM_COL32(230, 230, 230, 255), IM_COL32(150, 150, 150, 150));
            }
            else {
                if (ImGui::InvisibleButton("PlayButton", ImVec2((float)m_PlayButton->GetWidth(), (float)m_PlayButton->GetHeight()))) {
                    m_AudioPlayer->Play();
                    m_IsPlaying = true;
                }
                taskhub::UI::RenderImageButton(m_PlayButton, IM_COL32(200, 200, 200, 200), IM_COL32(230, 230, 230, 255), IM_COL32(150, 150, 150, 150));
            }
        }

        ImGui::SameLine();

        // Forward|Backward control buttons
        {
            taskhub::UI::ShiftCursorY(static_cast<float>(m_PlayButton->GetHeight()) * 0.15f);
            if (ImGui::InvisibleButton("Forward", ImVec2((float)m_ForwardButton->GetWidth(), (float)m_ForwardButton->GetHeight()))) {

                if (m_CurrentTrack < m_Playlist.size() - 1) {
                    m_CurrentTrack++;
                    m_AudioPlayer->Load(*m_Playlist[m_CurrentTrack]);

                    if (m_IsPlaying)
                        m_AudioPlayer->Play();
                }
            }
            taskhub::UI::RenderImageButton(m_ForwardButton, IM_COL32(200, 200, 200, 200), IM_COL32(230, 230, 230, 255), IM_COL32(150, 150, 150, 150));

            ImGui::SameLine();

            const ImVec2 cursor = ImGui::GetCursorPos();
            ImGui::SetCursorPos(ImVec2(cursor.x - (float)(m_ForwardButton->GetWidth() + m_PlayButton->GetWidth() + m_BackwardButton->GetWidth() + 18) , cursor.y + (float)(m_PlayButton->GetHeight() * 0.15f)));

            if (ImGui::InvisibleButton("Backward", ImVec2((float)m_BackwardButton->GetWidth(), (float)m_BackwardButton->GetHeight()))) {

                if (m_CurrentTrack > 0) {
                    m_CurrentTrack--;
                    m_AudioPlayer->Load(*m_Playlist[m_CurrentTrack]);

                    if (m_IsPlaying)
                        m_AudioPlayer->Play();
                }
            }
            taskhub::UI::RenderImageButton(m_BackwardButton, IM_COL32(200, 200, 200, 200), IM_COL32(230, 230, 230, 255), IM_COL32(150, 150, 150, 150));
        }

        ImGui::SameLine();

        // Volume control
        {
            static float s_VolumeBeforeMute = 0;

            ImGui::SetCursorPosX(0.0f);
            taskhub::UI::ShiftCursorY(10);
            if (m_MusicVolume > 0) {
                if (ImGui::InvisibleButton("Volume", ImVec2((float)m_VolumeImage->GetWidth(), (float)m_VolumeImage->GetHeight()))) {
                    s_VolumeBeforeMute = m_MusicVolume;
                    m_MusicVolume = 0;
                }

                taskhub::UI::RenderImageButton(m_VolumeImage, IM_COL32(200, 200, 200, 200), IM_COL32(230, 230, 230, 255), IM_COL32(150, 150, 150, 150));
            }
            else {
                if (ImGui::InvisibleButton("VolumeMute", ImVec2((float)m_VolumeImageMute->GetWidth(), (float)m_VolumeImageMute->GetHeight()))) {
                    m_MusicVolume = s_VolumeBeforeMute;
                   
                }
                taskhub::UI::RenderImageButton(m_VolumeImageMute, IM_COL32(200, 200, 200, 200), IM_COL32(230, 230, 230, 255), IM_COL32(150, 150, 150, 150));
            }
            ImGui::SameLine();

            taskhub::UI::ShiftCursorY(15);
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.1f);

            ImGui::SliderFloat("##Volume", &m_MusicVolume, 0.0f, 1.0f, "", ImGuiSliderFlags_NoInput);

            m_AudioPlayer->SetVolume(m_MusicVolume);
        }

        // Looping control buttons ----- TODO
        {

        }

        ImGui::NewLine();

        // Cursor View|Control
        {
            static bool s_IsSeeking = false;
            static float s_PlaybackPosition = 0.0f;
            float duration = m_Playlist[m_CurrentTrack]->GetDuration().count();

            if (!s_IsSeeking)
                s_PlaybackPosition = m_AudioPlayer->GetCursorPosition();

            int currentMinutes = static_cast<int>(s_PlaybackPosition) / 60;
            int currentSeconds = static_cast<int>(s_PlaybackPosition) % 60;
            int totalMinutes = static_cast<int>(duration) / 60;
            int totalSeconds = static_cast<int>(duration) % 60;

            char sliderText[64];
            snprintf(sliderText, sizeof(sliderText), "%02d:%02d / %02d:%02d", currentMinutes, currentSeconds, totalMinutes, totalSeconds);
            
            taskhub::UI::AlignCursorForWidth(ImGui::GetContentRegionAvail().x, 0.5f);
            taskhub::UI::AlignCursorForHeight(30, 0.8f);
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);

            if (ImGui::SliderFloat("##Seek", &s_PlaybackPosition, 0.0f, duration, sliderText, ImGuiSliderFlags_NoInput)) {
                s_IsSeeking = true;
            }
            if (s_IsSeeking && !ImGui::IsItemActive()) {
                m_AudioPlayer->SetCursorPosition(s_PlaybackPosition);
                s_IsSeeking = false;
            }
        }

        ImGui::EndChild();
    }

private:
    int m_CurrentTrack = -1;
    float m_MusicVolume = 0.5f;
    bool m_IsPlaying = false;
    bool m_IsLooping = false;
    bool m_IsMuted = false;

private:
    std::unique_ptr<taskhub::AudioSource> m_AudioPlayer;
    std::vector<std::shared_ptr<taskhub::AudioFile>> m_Playlist;

    std::shared_ptr<taskhub::Image> m_PlayButton;
    std::shared_ptr<taskhub::Image> m_PauseButton;
    std::shared_ptr<taskhub::Image> m_ForwardButton;
    std::shared_ptr<taskhub::Image> m_BackwardButton;
    std::shared_ptr<taskhub::Image> m_VolumeImage;
    std::shared_ptr<taskhub::Image> m_VolumeImageMute;
    std::shared_ptr<taskhub::Image> m_LoopingOff;
    std::shared_ptr<taskhub::Image> m_LoopingOn;
};

void ShowToolMenu() {

    static bool s_ShowDemoWindow = false;
    static bool s_ShowMetricsDebugger = false;

    if (s_ShowDemoWindow)
        ImGui::ShowDemoWindow();
    if (s_ShowMetricsDebugger)
        ImGui::ShowMetricsWindow();

    if (ImGui::BeginMenu("Tools")) {
        ImGui::MenuItem("ImGuiDemo", nullptr, &s_ShowDemoWindow);
        ImGui::MenuItem("MetricsDebugger", nullptr, &s_ShowMetricsDebugger);
        ImGui::EndMenu();
    }
}

taskhub::Application* taskhub::CreateApplication() {

	taskhub::ApplicationProvision provision;

    provision.AppIconPath = "C:/Dev/Resources/Images/TaskHubIcon3.png";

	taskhub::Application* app = new taskhub::Application(provision);

    app->PushLayer<AudioPlayerTest>();

    app->SetMenubarCallback([app]() {
        ShowToolMenu();
	});

	return app;
}