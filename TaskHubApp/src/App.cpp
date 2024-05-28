#include "Core/Application.h"
#include "Core/EntryPoint.h"
#include "Timer.h"
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
    void OnAttach() override {
        
    }

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

        if (ImGui::TreeNode("Todo List")) {
            ImGui::Text("Manage your tasks:");

            // Input for new todo item
            ImGui::InputText("New Task", new_todo_text, IM_ARRAYSIZE(new_todo_text));
            ImGui::SameLine();
            if (ImGui::Button("Add Task")) {
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
    }
};
static bool show_clock_overlay = true;
class ClockTime : public taskhub::Layer {
public:
    
    void OnUIRender() override {
        ShowClockOverlay(&show_clock_overlay);
    }

    void ShowClockOverlay(bool* p_open)
    {
        static int location = 0;
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
        ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
        if (ImGui::Begin("Clock overlay", p_open, window_flags))
        {
           
            ImGui::Text("Simple Clock overlay\n" "(right-click to change position)");
            ImGui::Separator();
            DisplayClock(m_Clock);

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

    std::string FormatTime(const std::chrono::hh_mm_ss<std::chrono::seconds>& time) {

        std::ostringstream oss;
        oss << std::setw(2) << std::setfill('0') << time.hours().count() << ":"
            << std::setw(2) << std::setfill('0') << time.minutes().count() << ":"
            << std::setw(2) << std::setfill('0') << time.seconds().count();
        return oss.str();
    }

    void DisplayClock(taskhub::Clock& clock) {
      
        auto time = clock.GetTime();
        std::string formattedTime = FormatTime(time);
        ImGui::Text("Current Time: %s", formattedTime.c_str());
    }

private:
    taskhub::Clock m_Clock = (taskhub::Clock("America/Chicago"));
   
};


taskhub::Application* taskhub::CreateApplication() {

	taskhub::ApplicationProvision provision;
	provision.Name = "Test";

	taskhub::Application* app = new taskhub::Application(provision);
	app->PushLayer<ImGuiTools>();
    app->PushLayer<ToDoList>();
    app->PushLayer<ClockTime>();

	return app;
}