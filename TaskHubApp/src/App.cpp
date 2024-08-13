#include "Core/Application.h"
#include "Core/EntryPoint.h"

///////////////////////////////

#include "Image.h"
#include "Audio/AudioSource.h"
#include "Audio/AudioFile.h"
#include "Time/Timer.h"
#include "GUI/CustomWidgets.h"
#include "GUI/UIUtils.h"
#include "FileDialog.h"
#include "res/Images.embed"
#include <chrono>
#include <format>


class AudioPlayer : public taskhub::Layer {
public:
    void OnAttach() override {

        m_AudioPlayer = std::make_unique<taskhub::AudioSource>();
        m_FileDialog = taskhub::FileDialog::Create();
        m_DisplayFlag = std::make_unique<bool>(false);

        // The following code decodes images from byte arrays, eliminating the need to include 
        // PNG files directly in the repository. Below I've provided an example of loading an 
        // image directly from a file.
        // m_PlayImageButton = std::make_shared<taskhub::Image>("C:/Dev/Resources/Images/Audio/Play.png");
        {            
            uint32_t width, height;
            void* data;

            data = taskhub::Image::DecodeFromMemory(playImage, sizeof(playImage), width, height);
            m_PlayImageButton = std::make_shared<taskhub::Image>(width, height, taskhub::ImageFormat::RGBA, data);
            free(data);

            data = taskhub::Image::DecodeFromMemory(pauseImage, sizeof(pauseImage), width, height);
            m_PauseImageButton = std::make_shared<taskhub::Image>(width, height, taskhub::ImageFormat::RGBA, data);
            free(data);

            data = taskhub::Image::DecodeFromMemory(forwardImage, sizeof(forwardImage), width, height);
            m_ForwardImageButton = std::make_shared<taskhub::Image>(width, height, taskhub::ImageFormat::RGBA, data);
            free(data);

            data = taskhub::Image::DecodeFromMemory(backwardImage, sizeof(backwardImage), width, height);
            m_BackwardImageButton = std::make_shared<taskhub::Image>(width, height, taskhub::ImageFormat::RGBA, data);
            free(data);

            data = taskhub::Image::DecodeFromMemory(volumeImage, sizeof(volumeImage), width, height);
            m_VolumeImageButton = std::make_shared<taskhub::Image>(width, height, taskhub::ImageFormat::RGBA, data);
            free(data);

            data = taskhub::Image::DecodeFromMemory(muteImage, sizeof(muteImage), width, height);
            m_VolumeMuteImageButton = std::make_shared<taskhub::Image>(width, height, taskhub::ImageFormat::RGBA, data);
            free(data);

            data = taskhub::Image::DecodeFromMemory(loopOffImage, sizeof(loopOffImage), width, height);
            m_LoopingOffImageButton = std::make_shared<taskhub::Image>(width, height, taskhub::ImageFormat::RGBA, data);
            free(data);

            data = taskhub::Image::DecodeFromMemory(loopOnImage, sizeof(loopOnImage), width, height);
            m_LoopingOnImageButton = std::make_shared<taskhub::Image>(width, height, taskhub::ImageFormat::RGBA, data);
            free(data);
        }

        m_LoopingOffImageButton->Resize((uint32_t)(m_LoopingOffImageButton->GetWidth() * 0.5f), (uint32_t)(m_LoopingOffImageButton->GetHeight() * 0.5f));
        m_LoopingOnImageButton->Resize((uint32_t)(m_LoopingOnImageButton->GetWidth() * 0.5f), (uint32_t)(m_LoopingOnImageButton->GetHeight() * 0.5f));
    }

    void AddAudio() {
        
        // If adding into an empty playlist we must initalize the AudioPlayer's state for DrawControls() to function
        bool isEmptyFlag = m_Playlist.empty();

        std::string filePath = m_FileDialog->OpenFile();
        if (taskhub::AudioFile::IsFileValid(filePath)) {

            m_Playlist.emplace_back(std::make_shared<taskhub::AudioFile>(filePath));

            if (isEmptyFlag) {
                m_AudioPlayer->Load(*m_Playlist[0]);
                m_CurrentTrack = 0;
            }
        }
        else {
            HUB_INFO("File invalid");
        }
    }

    void AddAudioFolder() {

        // If adding into an empty playlist we must initalize the AudioPlayer's state for DrawControls() to function
        bool isEmptyFlag = m_Playlist.empty();

        std::string folderPath = m_FileDialog->OpenFolder();
        if (!folderPath.empty()) {

            for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {

                std::string filePath = entry.path().string();

                if (taskhub::AudioFile::IsFileValid(filePath)) {

                    m_Playlist.emplace_back(std::make_shared<taskhub::AudioFile>(filePath));

                    if (isEmptyFlag) {
                        m_AudioPlayer->Load(*m_Playlist[0]);
                        m_CurrentTrack = 0;
                    }
                }
                
            }
        }
    }

    void OnUIRender() override {

        if (!*m_DisplayFlag)
            return;

        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar;

        ImGui::Begin("Audio Player", nullptr, windowFlags);

        ImGui::BeginMenuBar();

        if (ImGui::MenuItem("Add Audio File"))
            AddAudio();
        
        if (ImGui::MenuItem("Add Audio Folder")) 
            AddAudioFolder();
        
        ImGui::EndMenuBar();

        DrawPlaylist();
        DrawControls();

        ImGui::End();
    }

    void OnUpdate() override {

        if (!*m_DisplayFlag)
            return;

        // Check to see if song is finished and play next song
        if (!m_IsLooping && m_AudioPlayer->IsFinished() && m_CurrentTrack < m_Playlist.size() - 1) {

            m_CurrentTrack++;
            m_AudioPlayer->Load(*m_Playlist[m_CurrentTrack]);
            m_AudioPlayer->Play();
        }
    }

    void DrawPlaylist() {

        ImGuiChildFlags childFlags = ImGuiChildFlags_Border | ImGuiChildFlags_AlwaysUseWindowPadding | ImGuiChildFlags_AutoResizeX;
        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_None;
        ImGuiSelectableFlags selectableFlags = ImGuiSelectableFlags_AllowItemOverlap;

        ImGui::BeginChild("PlaylistView", ImVec2(ImGui::GetContentRegionAvail().x, 300), childFlags, windowFlags);

        for (size_t i = 0; i < m_Playlist.size(); i++) {

            std::string songID = std::format("{}##{}", m_Playlist[i]->GetName(), i);
            
            if (ImGui::Selectable(songID.c_str(), m_CurrentTrack == i)) {

                m_CurrentTrack = static_cast<int>(i);

                m_AudioPlayer->Load(*m_Playlist[i]);
                m_AudioPlayer->Play();
                m_IsPlaying = true;
                m_IsLooping = false;
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

    void DrawControls() {

        if (m_Playlist.empty())
            return;

        static constexpr uint32_t s_Tint        = IM_COL32(230, 230, 230, 200);
        static constexpr uint32_t s_TintHovered = IM_COL32(230, 230, 230, 255);
        static constexpr uint32_t s_TintPressed = IM_COL32(230, 230, 230, 150);


        taskhub::UI::AlignCursorForHeight(100, 0.9f);
        ImGui::BeginChild("AudioControls");
        
        // Play|Pause control buttons
        {
            taskhub::UI::AlignCursorForWidth(static_cast<float>(m_PauseImageButton->GetWidth()), 0.5f);
            if (m_IsPlaying) {
                if (ImGui::InvisibleButton("PauseButton", ImVec2((float)m_PauseImageButton->GetWidth(), (float)m_PauseImageButton->GetHeight()))) {
                    m_AudioPlayer->Pause();
                    m_IsPlaying = false;
                }
                taskhub::UI::RenderImageButton(m_PauseImageButton, s_Tint, s_TintHovered, s_TintPressed);
            }
            else {
                if (ImGui::InvisibleButton("PlayButton", ImVec2((float)m_PlayImageButton->GetWidth(), (float)m_PlayImageButton->GetHeight()))) {
                    m_AudioPlayer->Play();
                    m_IsPlaying = true;
                }
                taskhub::UI::RenderImageButton(m_PlayImageButton, s_Tint, s_TintHovered, s_TintPressed);
            }
        }

        ImGui::SameLine();

        // Forward|Backward control buttons
        {
            taskhub::UI::ShiftCursorY(static_cast<float>(m_PlayImageButton->GetHeight()) * 0.15f);
            if (ImGui::InvisibleButton("Forward", ImVec2((float)m_ForwardImageButton->GetWidth(), (float)m_ForwardImageButton->GetHeight()))) {

                if (m_CurrentTrack < m_Playlist.size() - 1) {
                    m_CurrentTrack++;
                    m_AudioPlayer->Load(*m_Playlist[m_CurrentTrack]);

                    if (m_IsPlaying)
                        m_AudioPlayer->Play();
                    
                    m_IsLooping = false;
                }
            }
            taskhub::UI::RenderImageButton(m_ForwardImageButton, s_Tint, s_TintHovered, s_TintPressed);
            ImGui::SameLine();

            const ImVec2 cursor = ImGui::GetCursorPos();
            ImGui::SetCursorPos(ImVec2(cursor.x - (float)(m_ForwardImageButton->GetWidth() + m_PlayImageButton->GetWidth() + m_BackwardImageButton->GetWidth() + 18) , cursor.y + (float)(m_PlayImageButton->GetHeight() * 0.15f)));

            if (ImGui::InvisibleButton("Backward", ImVec2((float)m_BackwardImageButton->GetWidth(), (float)m_BackwardImageButton->GetHeight()))) {

                if (m_CurrentTrack > 0) {
                    m_CurrentTrack--;
                    m_AudioPlayer->Load(*m_Playlist[m_CurrentTrack]);

                    if (m_IsPlaying)
                        m_AudioPlayer->Play();
                    
                    m_IsLooping = false;
                }
            }
            taskhub::UI::RenderImageButton(m_BackwardImageButton, s_Tint, s_TintHovered, s_TintPressed);
        }

        ImGui::SameLine();

        // Volume control
        {
            static float s_VolumeBeforeMute = 0;

            ImGui::SetCursorPosX(0.0f);
            taskhub::UI::ShiftCursorY(10);

            if (m_MusicVolume > 0) {
                if (ImGui::InvisibleButton("Volume", ImVec2((float)m_VolumeImageButton->GetWidth(), (float)m_VolumeImageButton->GetHeight()))) {
                    s_VolumeBeforeMute = m_MusicVolume;
                    m_MusicVolume = 0;
                }
                taskhub::UI::RenderImageButton(m_VolumeImageButton, s_Tint, s_TintHovered, s_TintPressed);
            }
            else {
                if (ImGui::InvisibleButton("VolumeMute", ImVec2((float)m_VolumeMuteImageButton->GetWidth(), (float)m_VolumeMuteImageButton->GetHeight()))) {
                    m_MusicVolume = s_VolumeBeforeMute;
                   
                }
                taskhub::UI::RenderImageButton(m_VolumeMuteImageButton, s_Tint, s_TintHovered, s_TintPressed);
            }

            ImGui::SameLine();
            taskhub::UI::ShiftCursorY(15);
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.1f);

            ImGui::SliderFloat("##Volume", &m_MusicVolume, 0.0f, 1.0f, "", ImGuiSliderFlags_NoInput);
            m_AudioPlayer->SetVolume(m_MusicVolume);
        }

        ImGui::SameLine();

        // Looping control buttons
        {
            taskhub::UI::ShiftCursor(25, 7);
            
            if (!m_IsLooping) {
                if (ImGui::InvisibleButton("LoopingOff", ImVec2((float)m_LoopingOffImageButton->GetWidth(), (float)m_LoopingOffImageButton->GetHeight()))) {

                    m_AudioPlayer->SetLooping(true);
                    m_IsLooping = true;
                }
                taskhub::UI::RenderImageButton(m_LoopingOffImageButton, s_Tint, s_TintHovered, s_TintPressed);
            }
            else {
                if (ImGui::InvisibleButton("LoopingOn", ImVec2((float)m_LoopingOnImageButton->GetWidth(), (float)m_LoopingOnImageButton->GetHeight()))) {

                    m_AudioPlayer->SetLooping(false);
                    m_IsLooping = false;
                }
                taskhub::UI::RenderImageButton(m_LoopingOnImageButton, s_Tint, s_TintHovered, s_TintPressed);
            }

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

            taskhub::UI::AlignCursorForWidth(ImGui::GetContentRegionAvail().x, 0.5f);
            taskhub::UI::AlignCursorForHeight(30, 0.8f);
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);

            char sliderText[64];
            snprintf(sliderText, sizeof(sliderText), "%02d:%02d / %02d:%02d", currentMinutes, currentSeconds, totalMinutes, totalSeconds);

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

    bool* GetDisplayFlag() { return m_DisplayFlag.get(); }

private:
    int m_CurrentTrack = -1;
    float m_MusicVolume = 0.5f;
    bool m_IsPlaying = false;
    bool m_IsLooping = false;
    bool m_IsMuted = false; 

private:
    std::unique_ptr<taskhub::AudioSource> m_AudioPlayer;
    std::vector<std::shared_ptr<taskhub::AudioFile>> m_Playlist;
    std::unique_ptr<taskhub::FileDialog> m_FileDialog;
    std::unique_ptr<bool> m_DisplayFlag;

private:
    std::shared_ptr<taskhub::Image> m_PlayImageButton;
    std::shared_ptr<taskhub::Image> m_PauseImageButton;
    std::shared_ptr<taskhub::Image> m_ForwardImageButton;
    std::shared_ptr<taskhub::Image> m_BackwardImageButton;
    std::shared_ptr<taskhub::Image> m_VolumeImageButton;
    std::shared_ptr<taskhub::Image> m_VolumeMuteImageButton;
    std::shared_ptr<taskhub::Image> m_LoopingOffImageButton;
    std::shared_ptr<taskhub::Image> m_LoopingOnImageButton;
};

class FocusTimer : public taskhub::Layer {
public:
    void OnAttach() override {

        {
            uint32_t width, height;
            void* data;

            data = taskhub::Image::DecodeFromMemory(timerLogo, sizeof(timerLogo), width, height);
            m_MenuBarIcon = std::make_shared<taskhub::Image>(width, height, taskhub::ImageFormat::RGBA, data);
            free(data);
        }

        m_TimerAudioSource = std::make_unique<taskhub::AudioSource>();

        taskhub::AudioFile sound("../TaskHubApp/res/simple-notification.mp3");
        m_TimerAudioSource->Load(sound);

        m_Timer = std::make_unique<taskhub::Timer>([&]{ 
            HUB_INFO("Timer complete!"); 
            m_TimerAudioSource->Play();
            m_Timer->Reset();
        });

        m_Timer->SetTimer(m_SetTime);
    }

    void OnUIRender() override {

        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoResize;
        ImGuiPopupFlags popupFlags = ImGuiPopupFlags_MouseButtonLeft | ImGuiPopupFlags_AnyPopup;

        ImGui::OpenPopupOnItemClick("Focus Timer Popup", popupFlags);

        if (ImGui::BeginPopup("Focus Timer Popup", windowFlags)) {
         

            float remainingTime = m_Timer->GetRemainingTime();
            std::string formattedRemTime = std::format("{:02}:{:02}", static_cast<int>(remainingTime) / 60, static_cast<int>(remainingTime) % 60);
            std::string formattedSetTime = std::format("{:02}:{:02}", static_cast<int>(m_SetTime.count()) / 60, static_cast<int>(m_SetTime.count()) % 60);

            float progress = m_SetTime.count() != 0.0f ? remainingTime / m_SetTime.count() : 0.0f;
            progress = ImClamp(progress, 0.0f, 1.0f);

            ImVec4 startColor(0.9f, 0.5f, 0.4f, 1.0f);
            ImVec4 endColor(0.9f, 0.3f, 0.2f, 1.0f);
 
            // Interpolate from startColor to endColor based on progress
            float r = (1.0f - progress) * startColor.x + progress * endColor.x;
            float g = (1.0f - progress) * startColor.y + progress * endColor.y;
            float b = (1.0f - progress) * startColor.z + progress * endColor.z;

            uint32_t progressColor = IM_COL32((int)(r * 255), (int)(g * 255), (int)(b * 255), 255);

            if (m_Timer->IsRunning())
                taskhub::UI::CircularProgressBar(100, 10, progress, formattedRemTime, progressColor);
            else
                taskhub::UI::CircularProgressBar(100, 10, 0, formattedSetTime, progressColor);

            // Controls 
            {
                if (ImGui::Button("#")) {
                    m_Timer->Reset();
                }
                ImGui::SameLine();
                if (ImGui::Button(">")) {

                    if (!m_Timer->IsRunning())
                        m_Timer->SetTimer(m_SetTime);
                    
                    m_Timer->Start();
                }
                if (!m_Timer->IsRunning()) {
                    ImGui::SameLine();
                    if (ImGui::Button("+")) {
                        m_SetTime += 1min;
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("-")) {
                        m_SetTime -= 1min;
                    }
                }
            }


            ImGui::EndPopup();
        }
    }

    void RenderMenu() {
        ImGui::ImageButton("TimerIcon", (void*)(intptr_t)m_MenuBarIcon->GetTextureID(), ImVec2(25, 25));
    }

    bool* GetDisplayFlag() const { return m_DisplayFlag.get(); }

private:
    std::chrono::duration<float> m_SetTime = 0s;

private:
    std::unique_ptr<taskhub::Timer> m_Timer;
    std::unique_ptr<taskhub::AudioSource> m_TimerAudioSource;
    std::shared_ptr<taskhub::Image> m_MenuBarIcon;

    std::unique_ptr<bool> m_DisplayFlag;
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

    //provision.AppIconPath = "C:/Dev/Resources/Images/TaskHub/TaskHub.png";
    provision.Name = "TaskHub Demo";

    taskhub::Application* app = new taskhub::Application(provision);

    std::shared_ptr<AudioPlayer> audioPlayer = std::make_shared<AudioPlayer>();
    std::shared_ptr<FocusTimer> focusTimer = std::make_shared<FocusTimer>();
    app->PushLayer(audioPlayer);
    app->PushLayer(focusTimer);

    app->SetMenubarCallback([app, audioPlayer, focusTimer]() {

        ShowToolMenu();
        
        if (ImGui::BeginMenu("Mini Apps")) {
            ImGui::MenuItem("AudioPlayer", nullptr, audioPlayer->GetDisplayFlag());
            ImGui::EndMenu();
        }

        focusTimer->RenderMenu();        
    });

    return app;
}