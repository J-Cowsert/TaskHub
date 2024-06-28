#include "Core/Application.h"
#include "Core/EntryPoint.h"
#include "GUI/UIUtils.h"
#include "Input/Input.h"
#include "Audio/AudioTrack.h"
#include "Audio/AudioFile.h"
#include "Core/Image.h"

class ImGuiTools : public taskhub::Layer {
public:
	void OnUIRender() override {
		ImGui::ShowDemoWindow();
        //ImGui::ShowMetricsWindow();
        //ImGui::ShowIDStackToolWindow();
		//ImGui::ShowStyleEditor();
	}
};

class ToDoList : public taskhub::Layer {
public:
	void OnUIRender() override {
        ImGui::Begin("todo");
        ShowTodoListApp();
        ImGui::End();
	}

    struct TodoItem {
        std::string text;
        bool completed;
    };

    void ShowTodoListApp() {
        static std::vector<TodoItem> todo_items;
        static char new_todo_text[256] = "";
        ImGui::BeginChild("todo");
        if (ImGui::TreeNode("Todo List")) {
            ImGui::Text("Manage your tasks:");

            // Input for new todo item
            ImGui::InputText("##NewTask", new_todo_text, IM_ARRAYSIZE(new_todo_text));
            ImGui::SameLine();
            if (ImGui::Button("Add Task") || taskhub::Input::IsKeyPressed(taskhub::Key::Enter)) {
                if (strlen(new_todo_text) > 0) {
                    todo_items.push_back({ std::string(new_todo_text), false });
                    new_todo_text[0] = '\0'; // Clear input field
                }
            }

            // Display todo items
            for (size_t i = 0; i < todo_items.size(); ++i) {
                ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
                if (todo_items[i].completed) {
                    node_flags |= ImGuiTreeNodeFlags_Selected;
                }

                bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, todo_items[i].text.c_str());

                // Drag and drop functionality
                if (ImGui::BeginDragDropSource()) {
                    ImGui::SetDragDropPayload("_TODOITEM", &i, sizeof(size_t));
                    ImGui::Text("Move %s", todo_items[i].text.c_str());
                    ImGui::EndDragDropSource();
                }
                if (ImGui::BeginDragDropTarget()) {
                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("_TODOITEM")) {
                        size_t src_idx = *(const size_t*)payload->Data;
                        if (src_idx != i) {
                            std::swap(todo_items[src_idx], todo_items[i]);
                        }
                    }
                    ImGui::EndDragDropTarget();
                }

                // Button to remove todo item
                ImGui::SameLine();
                if (ImGui::SmallButton("X")) {
                    todo_items.erase(todo_items.begin() + i);
                    --i; // Adjust index after deletion
                }
            }

            ImGui::TreePop();
        }
        ImGui::EndChild();
    }
};

class PlaylistManager {
public:
    PlaylistManager(std::shared_ptr<taskhub::AudioEngine> engine)
        : m_CoreEngine(engine), m_CurrentTrack(0), m_IsPlaying(false), m_IsLooping(false), m_MusicVolume(0.5f) {
    }

    void AddAudioFile(const std::string& filepath) {
        m_Playlist.emplace_back(std::make_shared<taskhub::AudioTrack>(m_CoreEngine, filepath));
    }
    void Render() {
        ImVec2 windowSize = ImGui::GetWindowSize();
        ImVec2 contentRegion = ImGui::GetContentRegionAvail();

        ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_FirstUseEver);
        ImGui::Begin("AudioTest", nullptr, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse);

        ImVec4 buttonColor = ImVec4(0.0f, 0.5f, 0.5f, 1.0f);
        ImVec4 buttonHoverColor = ImVec4(0.0f, 0.7f, 0.7f, 1.0f);
        ImVec4 buttonActiveColor = ImVec4(0.0f, 0.3f, 0.3f, 1.0f);
        ImVec4 sliderBgColor = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
        ImVec4 sliderGrabColor = ImVec4(0.0f, 0.5f, 0.5f, 1.0f);
        ImVec4 textColor = ImVec4(0.9f, 0.9f, 0.9f, 1.0f);

        ImGui::PushStyleColor(ImGuiCol_Button, buttonColor);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, buttonHoverColor);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, buttonActiveColor);
        ImGui::PushStyleColor(ImGuiCol_FrameBg, sliderBgColor);
        ImGui::PushStyleColor(ImGuiCol_SliderGrab, sliderGrabColor);
        ImGui::PushStyleColor(ImGuiCol_Text, textColor);

        ImGui::SetWindowFontScale(1.1f);

        LoadCurrentTrack();

        static bool s_IsPlaying = IsPlaying();
        if (ImGui::Button(s_IsPlaying ? "||" : ">", ImVec2(40, 40))) {
            PlayPauseCurrentTrack();
            s_IsPlaying = !s_IsPlaying;
        }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Play/Pause");

        ImGui::SameLine();

        static bool s_IsLooping = IsLooping();
        if (ImGui::Button(s_IsLooping ? "Looping: ON" : "Looping: OFF", ImVec2(120, 40))) {
            ToggleLooping();
            s_IsLooping = !s_IsLooping;
        }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Toggle Loop");

        ImGui::SameLine();

        static float s_MusicVolume = GetVolume();
        taskhub::UI::AlignCursorForWidth(80, 0.9f);
        ImGui::VSliderFloat("##Volume", ImVec2(50, 160), &s_MusicVolume, 0.0f, 1.0f, "");
        if (ImGui::IsItemActive() || ImGui::IsItemHovered()) {
            ImGui::SetTooltip("%.2f", s_MusicVolume);
        }
        SetVolume(s_MusicVolume);
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Volume");

        ImGui::Dummy(ImVec2(0.0f, 10.0f));

        static bool s_IsSeeking = false;
        static float s_PlaybackPosition = 0.0f;
        float durationInSeconds = GetCurrentTrackDuration();

        if (!s_IsSeeking)
            s_PlaybackPosition = GetCurrentPlaybackPosition();

        int currentMinutes = static_cast<int>(s_PlaybackPosition) / 60;
        int currentSeconds = static_cast<int>(s_PlaybackPosition) % 60;
        int totalMinutes = static_cast<int>(durationInSeconds) / 60;
        int totalSeconds = static_cast<int>(durationInSeconds) % 60;

        ImGui::Text("%02d:%02d / %02d:%02d", currentMinutes, currentSeconds, totalMinutes, totalSeconds);
        if (ImGui::SliderFloat("##Seek", &s_PlaybackPosition, 0.0f, durationInSeconds, "")) {
            s_IsSeeking = true;
        }
        if (s_IsSeeking && !ImGui::IsItemActive()) {
            SeekCurrentTrack(s_PlaybackPosition);
            s_IsSeeking = false;
        }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Seek");

        if (ImGui::Button("<", ImVec2(30, 30))) {
            PreviousTrack();
        }
        ImGui::SameLine();
        if (ImGui::Button(">", ImVec2(30, 30))) {
            NextTrack();
        }

        HandleTrackCompletion();

        ImGui::SameLine();
        ImGui::Text("Track %d / %d", GetCurrentTrackIndex() + 1, GetTotalTracks());
        ImGui::PopStyleColor(6);
        ImGui::End();
    }

private:
    void LoadCurrentTrack() {
        if (m_CurrentTrack < 0 || m_CurrentTrack >= m_Playlist.size()) return;
        auto& currentTrack = m_Playlist[m_CurrentTrack];
        if (!currentTrack->IsLoaded()) {
            currentTrack->Load();
        }
    }
    void UnloadCurrentTrack() {
        if (m_CurrentTrack < 0 || m_CurrentTrack >= m_Playlist.size()) return;
        auto& currentTrack = m_Playlist[m_CurrentTrack];
        if (currentTrack->IsLoaded()) {
            currentTrack->Unload();
        }
    }
    void PlayPauseCurrentTrack() {
        if (m_CurrentTrack < 0 || m_CurrentTrack >= m_Playlist.size()) return;
        auto& currentTrack = m_Playlist[m_CurrentTrack];
        if (!m_IsPlaying) {
            currentTrack->Play();
        }
        else {
            currentTrack->Pause();
        }
        m_IsPlaying = !m_IsPlaying;
    }
    void ToggleLooping() {
        if (m_CurrentTrack < 0 || m_CurrentTrack >= m_Playlist.size()) return;
        auto& currentTrack = m_Playlist[m_CurrentTrack];
        m_IsLooping = !m_IsLooping;
        currentTrack->SetLooping(m_IsLooping);
    }
    void SetVolume(float volume) {
        m_MusicVolume = volume;
        m_CoreEngine->SetGlobalVolume(volume);
    }
    float GetVolume() const {
        return m_MusicVolume;
    }
    float GetCurrentPlaybackPosition() const {
        if (m_CurrentTrack < 0 || m_CurrentTrack >= m_Playlist.size()) return 0.0f;
        return m_Playlist[m_CurrentTrack]->GetCursorPosition();
    }
    float GetCurrentTrackDuration() const {
        if (m_CurrentTrack < 0 || m_CurrentTrack >= m_Playlist.size()) return 0.0f;
        return m_Playlist[m_CurrentTrack]->GetDuration().count();
    }
    int GetCurrentTrackIndex() const {
        return m_CurrentTrack;
    }
    int GetTotalTracks() const {
        return m_Playlist.size();
    }
    bool IsLooping() const {
        return m_IsLooping;
    }
    bool IsPlaying() const {
        return m_IsPlaying;
    }
    void SeekCurrentTrack(float position) {
        if (m_CurrentTrack < 0 || m_CurrentTrack >= m_Playlist.size()) return;
        m_Playlist[m_CurrentTrack]->Seek(position);
    }
    void PreviousTrack() {
        if (m_CurrentTrack > 0) {
            UnloadCurrentTrack();
            m_CurrentTrack--;
            LoadCurrentTrack();
            if (m_IsPlaying) {
                m_Playlist[m_CurrentTrack]->Play();
            }
        }
    }
    void NextTrack() {
        if (m_CurrentTrack < m_Playlist.size() - 1) {
            UnloadCurrentTrack();
            m_CurrentTrack++;
            LoadCurrentTrack();
            if (m_IsPlaying) {
                m_Playlist[m_CurrentTrack]->Play();
            }
        }
    }
    void HandleTrackCompletion() {
        if (m_CurrentTrack < 0 || m_CurrentTrack >= m_Playlist.size()) return;
        if (m_Playlist[m_CurrentTrack]->IsFinished() && m_CurrentTrack < m_Playlist.size() - 1) {
            m_Playlist[m_CurrentTrack]->Seek(0);
            m_CurrentTrack++;
            if (m_IsPlaying) {
                m_Playlist[m_CurrentTrack]->Play();
            }
        }
    }

private:
    std::shared_ptr<taskhub::AudioEngine> m_CoreEngine;
    std::vector<std::shared_ptr<taskhub::AudioTrack>> m_Playlist;
    int m_CurrentTrack;
    bool m_IsPlaying;
    bool m_IsLooping;
    float m_MusicVolume;
};

class AudioFileTest : public taskhub::Layer {
public:
    void OnAttach() override {
        m_CoreEngine = std::make_shared<taskhub::HubAudioEngine>();

        m_PlaylistManager = std::make_unique<PlaylistManager>(m_CoreEngine);

        m_PlaylistManager->AddAudioFile("C:/Dev/Music/Shift (Instrumental).mp3");
        m_PlaylistManager->AddAudioFile("C:/Dev/Music/[dream].mp3");
        m_PlaylistManager->AddAudioFile("C:/Dev/Music/1 HOUR OF CHILL RAP BEATS.mp3");
        m_PlaylistManager->AddAudioFile("C:/Dev/Music/Lofi Jazz Instrumental.mp3");
        m_PlaylistManager->AddAudioFile("C:/Dev/Music/Nocturne.mp3");
       
    }

    void OnUIRender() override {
        m_PlaylistManager->Render();
    }

private:
    std::shared_ptr<taskhub::HubAudioEngine> m_CoreEngine;
    std::unique_ptr<PlaylistManager> m_PlaylistManager;
   
};

class ImageTest : public taskhub::Layer {
public:
    void OnAttach() override {
        m_TestImage = std::make_shared<taskhub::Image>("C:/Dev/Resources/Images/TestImage.jpg");
        m_HDRTestImage = std::make_shared<taskhub::Image>("C:/Dev/Resources/Images/HDRTestImage.jpg");
    }

    void OnUIRender() override {
        ImGui::Begin("ImageTest");
        ImGui::Text("pointer = %x", m_TestImage->GetTextureID());
        ImGui::Text("size = %d x %d", m_TestImage->GetWidth(), m_TestImage->GetHeight());
        ImGui::Image((void*)(intptr_t)m_TestImage->GetTextureID(), ImVec2(m_TestImage->GetWidth(), m_TestImage->GetHeight()));
        ImGui::End();

        ImGui::Begin("HDRImageTest");
        ImGui::Text("pointer = %x", m_HDRTestImage->GetTextureID());
        ImGui::Text("size = %d x %d", m_HDRTestImage->GetWidth(), m_HDRTestImage->GetHeight());
        ImGui::Image((void*)(intptr_t)m_HDRTestImage->GetTextureID(), ImVec2(m_HDRTestImage->GetWidth() * 0.3, m_HDRTestImage->GetHeight() * 0.3));
        ImGui::End();

        ImGui::Begin("ImageButtonTest");
        ImGui::Text("pointer = %x", m_TestImage->GetTextureID());
        ImGui::Text("size = %d x %d", m_TestImage->GetWidth(), m_TestImage->GetHeight());
        if (ImGui::InvisibleButton("testbtn", ImVec2(100, 100))) {
            HUB_CORE_INFO("Image Button Pressed");
        }
        
        taskhub::UI::RenderImageButton(m_TestImage, IM_COL32(200, 200, 200, 255), IM_COL32(230, 230, 230, 255), IM_COL32(150, 150, 150, 255), ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
        ImGui::End();
        
    }

private:
    std::shared_ptr<taskhub::Image> m_TestImage;
    std::shared_ptr<taskhub::Image> m_HDRTestImage;
};

taskhub::Application* taskhub::CreateApplication() {

	taskhub::ApplicationProvision provision;

    provision.AppIconPath = "C:/Dev/Resources/Images/TaskHubIcon3.png";

	taskhub::Application* app = new taskhub::Application(provision);

	app->PushLayer<ImGuiTools>();
    //app->PushLayer<ToDoList>();
    app->PushLayer<AudioFileTest>();
    app->PushLayer<ImageTest>();

    app->SetMenubarCallback([app]() {

		if (ImGui::BeginMenu("File")) {

			if (ImGui::MenuItem("Exit")) {
				app->Close();
			}
			ImGui::EndMenu();
		}
	});

	return app;
}