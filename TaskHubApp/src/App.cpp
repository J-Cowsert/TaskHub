#include "Core/Application.h"
#include "Core/EntryPoint.h"
#include "UI/UIUtils.h"
#include "Input/Input.h"
#include "Audio/AudioFile.h"

class ImGuiTools : public taskhub::Layer {
public:
	void OnUIRender() override {
		ImGui::ShowDemoWindow();
		ImGui::ShowStyleEditor();
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

class AudioFileTest : public taskhub::Layer {
public:
    void OnAttach() override {

        m_CoreEngine = std::make_shared<taskhub::HubAudioEngine>();
      
        m_Playlist.emplace_back(std::make_shared<taskhub::AudioFile>(m_CoreEngine, "C:/Dev/Music/Shift (Instrumental).mp3"));
        m_Playlist.emplace_back(std::make_shared<taskhub::AudioFile>(m_CoreEngine, "C:/Dev/Music/TheEnd.mp3"));
        m_Playlist.emplace_back(std::make_shared<taskhub::AudioFile>(m_CoreEngine, "C:/Dev/Music/Nocturne.mp3"));
    }

    void OnUIRender() override {
        
        ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_FirstUseEver);
        ImGui::Begin("Audio Player", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

        // Define a consistent color scheme
        ImVec4 buttonColor = ImVec4(0.0f, 0.5f, 0.5f, 1.0f);
        ImVec4 buttonHoverColor = ImVec4(0.0f, 0.7f, 0.7f, 1.0f);
        ImVec4 buttonActiveColor = ImVec4(0.0f, 0.3f, 0.3f, 1.0f);
        ImVec4 sliderBgColor = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
        ImVec4 sliderGrabColor = ImVec4(0.0f, 0.5f, 0.5f, 1.0f);
        ImVec4 textColor = ImVec4(0.9f, 0.9f, 0.9f, 1.0f);
        ImVec4 headerColor = ImVec4(0.0f, 0.5f, 0.5f, 1.0f);

        ImGui::PushStyleColor(ImGuiCol_Button, buttonColor);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, buttonHoverColor);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, buttonActiveColor);
        ImGui::PushStyleColor(ImGuiCol_FrameBg, sliderBgColor);
        ImGui::PushStyleColor(ImGuiCol_SliderGrab, sliderGrabColor);
        ImGui::PushStyleColor(ImGuiCol_Text, textColor);

        // Set default font size
        ImGui::SetWindowFontScale(1.1f);

        // Add some vertical space
        ImGui::Dummy(ImVec2(0.0f, 10.0f));

        // Play/Pause Button
        static bool isPlaying = m_Playlist[m_CurrentTrack]->IsPlaying();
        if (ImGui::Button(isPlaying ? "||" : ">", ImVec2(40, 40))) {
            if (!isPlaying) {
                m_Playlist[m_CurrentTrack]->Play();
            }
            else {
                m_Playlist[m_CurrentTrack]->Pause();
            }
            isPlaying = !isPlaying;
        }
        if (ImGui::IsItemHovered())
            ImGui::SetTooltip("Play/Pause");

        ImGui::SameLine();

        // Loop Toggle Button
        static bool isLooping = false;
        if (ImGui::Button(isLooping ? "Looping: ON" : "Looping: OFF", ImVec2(120, 40))) {
            isLooping = !isLooping;
            m_Playlist[m_CurrentTrack]->SetLooping(isLooping);
        }
        if (ImGui::IsItemHovered())
            ImGui::SetTooltip("Toggle Loop");

        ImGui::SameLine();

        // Volume Slider
        static float s_MusicVolume = 0.5f;
        ImGui::SameLine();
        taskhub::UI::ShiftCursorX(100);
        ImGui::VSliderFloat("##Volume", ImVec2(30, 160), &s_MusicVolume, 0.0f, 1.0f, "");
        if (ImGui::IsItemActive() || ImGui::IsItemHovered()) {
            ImGui::SetTooltip("%.2f", s_MusicVolume);
        }
        m_CoreEngine->SetGlobalVolume(s_MusicVolume);
        if (ImGui::IsItemHovered())
            ImGui::SetTooltip("Volume");

        // Add some vertical space
        ImGui::Dummy(ImVec2(0.0f, 10.0f));

        // Seek Slider
        static bool s_IsSeeking = false;
        static float s_PlaybackPosition = 0.0f;
        float durationInSeconds = m_Playlist[m_CurrentTrack]->GetDuration().count();

        if (!s_IsSeeking)
            s_PlaybackPosition = m_Playlist[m_CurrentTrack]->GetCursorPosition();

        // Format cursor position and duration in minutes and seconds
        int currentMinutes = static_cast<int>(s_PlaybackPosition) / 60;
        int currentSeconds = static_cast<int>(s_PlaybackPosition) % 60;
        int totalMinutes = static_cast<int>(durationInSeconds) / 60;
        int totalSeconds = static_cast<int>(durationInSeconds) % 60;

        // Display the seek slider with current position and total duration
        ImGui::Text("%02d:%02d / %02d:%02d", currentMinutes, currentSeconds, totalMinutes, totalSeconds);
        if (ImGui::SliderFloat("##Seek", &s_PlaybackPosition, 0.0f, durationInSeconds, "")) {
            s_IsSeeking = true;
        }
        if (s_IsSeeking && !ImGui::IsItemActive()) {
            m_Playlist[m_CurrentTrack]->Seek(s_PlaybackPosition);
            s_IsSeeking = false;
        }
        if (ImGui::IsItemHovered())
            ImGui::SetTooltip("Seek");

        // Playlist Navigation
        if (ImGui::Button("<", ImVec2(30, 30)) && m_CurrentTrack > 0) {

            m_Playlist[m_CurrentTrack]->Pause();
            m_Playlist[m_CurrentTrack]->Seek(0);
            m_CurrentTrack--;

            if (isPlaying) {
                m_Playlist[m_CurrentTrack]->Play();
            }
        }
        ImGui::SameLine();
        if (ImGui::Button(">", ImVec2(30, 30)) && m_CurrentTrack < m_Playlist.size() - 1) {
            m_Playlist[m_CurrentTrack]->Pause();
            m_Playlist[m_CurrentTrack]->Seek(0);
            m_CurrentTrack++;

            if (isPlaying) {
                m_Playlist[m_CurrentTrack]->Play();
            }
        }

        if (m_Playlist[m_CurrentTrack]->IsFinished() && m_CurrentTrack < m_Playlist.size() - 1) {

            m_Playlist[m_CurrentTrack++]->Seek(0);
            if (isPlaying) {
                m_Playlist[m_CurrentTrack]->Play();
            }
        }

        ImGui::SameLine();
        ImGui::Text("Track %d / %d", m_CurrentTrack + 1, m_Playlist.size());
        ImGui::PopStyleColor(6);
        ImGui::End();
    }

private:
    std::shared_ptr<taskhub::HubAudioEngine> m_CoreEngine;

    std::vector<std::shared_ptr<taskhub::AudioFile>> m_Playlist;
    int m_CurrentTrack = 0;
};

taskhub::Application* taskhub::CreateApplication() {

	taskhub::ApplicationProvision provision;

	taskhub::Application* app = new taskhub::Application(provision);

	//app->PushLayer<ImGuiTools>();
    app->PushLayer<ToDoList>();
    app->PushLayer<AudioFileTest>();

	return app;
}