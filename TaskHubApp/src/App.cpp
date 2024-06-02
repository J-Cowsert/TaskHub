#include "Core/Application.h"
#include "Core/EntryPoint.h"
#include "Input/Input.h"
#include "Time/Clock.h"
#include "Time/Stopwatch.h"
#include "Time/Timer.h"
#include "Gui/Roboto-Regular.embed"
#include <iostream>
#include <sstream>

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

static bool show_clock_overlay = true;
static bool show_stopwatch = true;
static bool show_timer = true;

class TimeUtilTest : public taskhub::Layer {
public:

    void OnUIRender() override {
        ShowClockOverlay(&show_clock_overlay);
        ShowStopwatch(&show_stopwatch);
        ShowTimer(&show_timer);
    }

    void ShowClockOverlay(bool* p_open)
    {
        static int location = -2; //Center
        ImGuiIO& io = ImGui::GetIO();
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
        if (location >= 0)
        {
            const float PAD = 10.0f;
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
            ImVec2 work_size = viewport->WorkSize;
            ImVec2 window_pos, window_pos_pivot;
            window_pos.x = (location & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
            window_pos.y = (location & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
            window_pos_pivot.x = (location & 1) ? 1.0f : 0.0f;
            window_pos_pivot.y = (location & 2) ? 1.0f : 0.0f;
            ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
            ImGui::SetNextWindowViewport(viewport->ID);
            window_flags |= ImGuiWindowFlags_NoMove;
        }
        else if (location == -2)
        {
            // Center window
            ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
            window_flags |= ImGuiWindowFlags_NoMove;
        }
        ImGui::SetNextWindowBgAlpha(0.55f); // Transparent background

        if (ImGui::Begin("Clock overlay", p_open, window_flags))
        {

            ImGui::Text("Clock\n" "(right-click to change position)");
            ImGui::Separator();

            DisplayClockTime(m_Clock);
            DisplayClockDate(m_Clock);

            if (ImGui::BeginPopupContextWindow())
            {
                if (ImGui::MenuItem("Custom", NULL, location == -1)) location = -1;
                if (ImGui::MenuItem("Center", NULL, location == -2)) location = -2;
                if (ImGui::MenuItem("Top-left", NULL, location == 0)) location = 0;
                if (ImGui::MenuItem("Top-right", NULL, location == 1)) location = 1;
                if (ImGui::MenuItem("Bottom-left", NULL, location == 2)) location = 2;
                if (ImGui::MenuItem("Bottom-right", NULL, location == 3)) location = 3;
                if (p_open && ImGui::MenuItem("Close")) *p_open = false;
                ImGui::EndPopup();
            }

        }

        ImGui::End();
    }

    void ShowStopwatch(bool* p_open) {

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10)); // Increase button padding
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 10)); // Increase button spacing

        if (ImGui::Begin("Stopwatch", p_open)) {
            if (ImGui::Button("Start"))
                m_Stopwatch.Start();
            ImGui::SameLine();
            if (ImGui::Button("Stop"))
                m_Stopwatch.Stop();
            ImGui::SameLine();
            if (ImGui::Button("Reset"))
                m_Stopwatch.Reset();
        }
        ImGui::PopStyleVar(2);

        float elapsedTime = m_Stopwatch.GetElapsedTime();
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2) << "Elapsed Time: " << elapsedTime << " seconds";
        ImGui::Text("%s", oss.str().c_str());

        ImGui::End();
    }

    void ShowTimer(bool* p_open) {

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10)); // Increase button padding
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 10)); // Increase button spacing

        static float time;
        if (ImGui::Begin("Timer", p_open)) {
            ImGui::InputFloat("##Time", &time, 0.1f, 1.0f);
            if (ImGui::Button("Set Time")) {
                m_Timer.SetTimer(time);
            }  
            ImGui::SameLine();
            if (ImGui::Button("Reset Time")) {
                m_Timer.Reset();
            }
            if (ImGui::Button("Start")) {
                m_Timer.Start();
            }
            if (ImGui::Button("Pause")) {
                m_Timer.Stop();
            }
        }

        ImGui::PopStyleVar(2);

        float remainingTime = m_Timer.GetRemainingTime();
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2) << "Remaining Time: " << remainingTime << " seconds";
        ImGui::Text("%s", oss.str().c_str());

        ImGui::End();
    }

private:
    void DisplayClockTime(taskhub::Clock& clock) {
      
        std::string time = clock.GetTheTime();
        ImGui::Text("Time: %s", time.c_str());
    }
    void DisplayClockDate(taskhub::Clock& clock) {
        
        std::string date = clock.GetTheDate();
        ImGui::Text("Date: %s", date.c_str());
    }

private:
    taskhub::Clock m_Clock = taskhub::Clock("America/Chicago");  
    taskhub::Stopwatch m_Stopwatch = taskhub::Stopwatch();
    taskhub::Timer m_Timer = taskhub::Timer();
};

taskhub::Application* taskhub::CreateApplication() {

	taskhub::ApplicationProvision provision;

	taskhub::Application* app = new taskhub::Application(provision);

	app->PushLayer<ImGuiTools>();
    app->PushLayer<ToDoList>();
    app->PushLayer<TimeUtilTest>();

	return app;
}